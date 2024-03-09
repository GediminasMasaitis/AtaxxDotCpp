#include <torch/torch.h>

#include <array>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

using Color = uint8_t;
using Bitboard = uint64_t;
using Wdl = int8_t;
using Score = int16_t;
using Rank = uint8_t;
using File = uint8_t;
using Square = uint8_t;

constexpr auto data_type_val = torch::kFloat32;
using data_type = float_t;
using quantized_type = int16_t;

struct DataEntry
{
    Bitboard white;
    Bitboard black;
    Color turn;
    Wdl wdl;
    Score score;
};

DataEntry read_entry(ifstream& file)
{
    DataEntry entry;
    file.read(reinterpret_cast<char*>(&entry.white), sizeof(entry.white));
    file.read(reinterpret_cast<char*>(&entry.black), sizeof(entry.black));
    file.read(reinterpret_cast<char*>(&entry.turn), sizeof(entry.turn));
    file.read(reinterpret_cast<char*>(&entry.wdl), sizeof(entry.wdl));
    file.read(reinterpret_cast<char*>(&entry.score), sizeof(entry.score));
    return entry;
}

static constexpr Square get_square(const File file, const Rank rank)
{
    return rank * 8 + file;
}

static constexpr Square get_index(const File file, const Rank rank)
{
    return rank * 7 + file;
}

constexpr int32_t input_size = 98;
constexpr int32_t hidden_size = 64;
constexpr bool double_accumulator = false;
using InputData = std::array<data_type, input_size>;
using InputDatas = std::array<InputData, 2>;
using OutputData = std::array<data_type, 1>;

constexpr Bitboard reverse_bits(Bitboard bitboard)
{
    const Bitboard h1 = 0x5555555555555555;
    const Bitboard h2 = 0x3333333333333333;
    const Bitboard h4 = 0x0F0F0F0F0F0F0F0F;
    const Bitboard v1 = 0x00FF00FF00FF00FF;
    const Bitboard v2 = 0x0000FFFF0000FFFF;
    bitboard = ((bitboard >> 1) & h1) | ((bitboard & h1) << 1);
    bitboard = ((bitboard >> 2) & h2) | ((bitboard & h2) << 2);
    bitboard = ((bitboard >> 4) & h4) | ((bitboard & h4) << 4);
    bitboard = ((bitboard >> 8) & v1) | ((bitboard & v1) << 8);
    bitboard = ((bitboard >> 16) & v2) | ((bitboard & v2) << 16);
    bitboard = (bitboard >> 32) | (bitboard << 32);
    bitboard >>= 9;
    return bitboard;
}

class MyDataset : public torch::data::Dataset<MyDataset>
{
public:
    vector<torch::Tensor> inputs;
    vector<torch::Tensor> targets;

    explicit MyDataset(const string& path, int limit = -1)
    {
        const auto size = filesystem::file_size(filesystem::path(path));
        constexpr auto entry_size = sizeof(Bitboard) + sizeof(Bitboard) + sizeof(Color) + sizeof(Wdl) + sizeof(Score);
        const auto entry_count = size / entry_size;

        cout << "Loading " << path << endl;
        cout << size << " bytes" << endl;
        cout << entry_count << " entries" << endl;

        //vector<InputDatas> inputs_data;
        //vector<OutputData> outputs_data;

        //inputs_data.resize(entry_count);
        //outputs_data.resize(entry_count);

        inputs.reserve(entry_count);
        targets.reserve(entry_count);

        auto stream = ifstream(path, ios::binary);
        if (!stream.is_open())
        {
            cerr << "Failed to open file: " << path << endl;
            throw exception();
        }

        const auto tensor_options = torch::TensorOptions().dtype(data_type_val);

        for (size_t entry_index = 0; entry_index < entry_count; entry_index++)
        {
            const auto entry = read_entry(stream);

            InputDatas input_data;
            OutputData output_data;

            const auto is_black = entry.turn == 1;

            const auto us_stm = is_black ? reverse_bits(entry.black) : entry.white;
            const auto them_stm = is_black ? reverse_bits(entry.white) : entry.black;

            const auto us_nstm = is_black ? entry.white : reverse_bits(entry.black);
            const auto them_nstm = is_black ? entry.black : reverse_bits(entry.white);

            for (Rank rank = 0; rank < 7; rank++)
            {
                for (File file = 0; file < 7; file++)
                {
                    const auto square = get_square(file, rank);
                    const auto index = get_index(file, rank);

                    input_data[0][index] = static_cast<data_type>((us_stm >> square) & 1);
                    input_data[0][49 + index] = static_cast<data_type>((them_stm >> square) & 1);

                    input_data[1][index] = static_cast<data_type>((us_nstm >> square) & 1);
                    input_data[1][49 + index] = static_cast<data_type>((them_nstm >> square) & 1);
                }
            }

            const auto wdl = is_black ? static_cast<data_type>(2 - entry.wdl) / 2 : static_cast<data_type>(entry.wdl) / 2;
            output_data[0] = wdl;
            auto input_stm = torch::from_blob(input_data[0].data(), { input_size }, tensor_options);
            auto input_nstm = torch::from_blob(input_data[1].data(), { input_size }, tensor_options);
            auto input = torch::stack({ input_stm, input_nstm }).clone();
            auto target = torch::from_blob(output_data.data(), { 1 }, tensor_options).clone();

            inputs.push_back(input);
            targets.push_back(target);

            if (entry_index % 1000000 == 0)
            {
                cout << "Loaded " << entry_index << " entries" << endl;
            }

            if (entry_index == limit)
            {
                break;
            }
        }
    }

    torch::data::Example<> get(size_t index) override
    {
        return
        {
            inputs[index],
            targets[index]
        };
    }

    optional<size_t> size() const override
    {
        return inputs.size();
    }
};

struct Net : torch::nn::Module {
    torch::nn::Linear fc1{ nullptr };
    torch::nn::Linear fc2{ nullptr };

    Net() {
        fc1 = register_module("fc1", torch::nn::Linear(input_size, hidden_size));
        fc2 = register_module("fc2", torch::nn::Linear(hidden_size * (double_accumulator ? 2 : 1), 1));
    }

    torch::Tensor forward_inner(torch::Tensor x) {
        if constexpr (double_accumulator)
        {
            auto stm = x.select(1, 0);
            auto nstm = x.select(1, 1);

            stm = fc1->forward(stm);
            nstm = fc1->forward(nstm);

            auto combined = torch::cat({ stm, nstm }, 1);
            combined = torch::relu(combined);

            auto result = fc2->forward(combined);
            return result;
        }
        else
        {
            auto stm = x.select(1, 0);
            stm = fc1->forward(stm);
            stm = torch::relu(stm);
            auto result = fc2->forward(stm);
            return result;
        }
    }

    torch::Tensor forward(torch::Tensor x) {
        auto result = forward_inner(x);
        result = torch::sigmoid(result);
        return result;
    }

    torch::Tensor forward_no_sig(torch::Tensor x)
    {
        auto result = forward_inner(x);
        result *= 512;
        return result;
    }
};

void print_params(const Net& model, int epoch)
{
    const string float_path = "C:/shared/ataxx/nets/current/default-" + to_string(epoch) + ".nnue-floats";
    auto file_float = ofstream(float_path, ios::out | ios::binary);

    const string quantized_path = "C:/shared/ataxx/nets/current/default-" + to_string(epoch) + ".nnue";
    auto file_quantized = ofstream(quantized_path, ios::out | ios::binary);

    const string human_path = "C:/shared/ataxx/nets/current/default-" + to_string(epoch) + ".txt";
    auto file_human = ofstream(human_path, ios::out);

    stringstream ss;
    float max_val = 0;
    for (const auto& pair : model.named_parameters()) {
        auto& name = pair.key();
        auto param = pair.value().reshape(-1);
        //cout << name << ": " << pair.value().sizes() << endl;
        //cout << name << ": " << param.sizes() << endl;
        ss << name << endl;
        file_human << name << endl;
        for (auto i = 0; i < param.size(0); i++)
        {
            auto val = param[i].item<data_type>();
            file_float.write(reinterpret_cast<char*>(&val), sizeof(data_type));

            float rounded = round(val * 512);
            if (rounded < -32768)
            {
                cout << "TOO SMALL";
            }
            else if (rounded > 32767)
            {
                cout << "TOO BIG";
            }
            auto rounded_abs = abs(rounded);
            if (rounded_abs > max_val)
            {
                max_val = rounded_abs;
            }

            auto quantized = static_cast<quantized_type>(rounded);
            file_quantized.write(reinterpret_cast<char*>(&quantized), sizeof(quantized_type));

            const auto val_human = to_string(quantized);
            file_human << val_human << " ";
            ss << val_human << " ";
        }
        ss << endl;
        file_human << endl;
    }
    ss << endl;
    cout << "Max val: " << max_val << endl;
    const auto str = ss.str();
    //cout << str;
    file_float.flush();
    file_quantized.flush();
    file_human.flush();
}

void print_time(chrono::time_point<chrono::high_resolution_clock> start)
{
    const auto end = chrono::high_resolution_clock::now();
    const auto milliseconds = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    if (milliseconds < 10000)
    {
        cout << "[" << milliseconds << "ms] ";
        return;
    }
    const auto seconds = chrono::duration_cast<chrono::seconds>(end - start).count();
    cout << "[" << seconds << "s] ";
}

int main()
{
    const auto start = chrono::high_resolution_clock::now();
    //auto device = torch::Device(torch::kCPU);
    auto device = torch::Device(torch::kCUDA);

    constexpr int32_t batch_size = 1024 * 128;
    //constexpr int32_t batch_size = 1024 * 32;
    //constexpr int32_t batch_size = 128;

    constexpr auto test_path = "C:/shared/ataxx/data/data_test.bin";
    auto test_set = MyDataset(test_path).map(torch::data::transforms::Stack<>());
    auto test_size = test_set.size().value();
    auto data_loader_options = torch::data::DataLoaderOptions();
    data_loader_options.batch_size(batch_size);
    data_loader_options.workers(10);
    //data_loader_options.enforce_ordering(false);

    auto test_loader = torch::data::make_data_loader<torch::data::samplers::SequentialSampler>(std::move(test_set), data_loader_options);
    print_time(start);
    cout << "Loaded test set" << endl;

    constexpr auto train_path = "C:/shared/ataxx/data/data22M-old.bin";
    //constexpr auto train_path = "C:/shared/ataxx/data/data3M.bin";
    //constexpr auto train_path = "C:/shared/ataxx/data/data_train_small.bin";
    constexpr auto limit = 22'000'000;
    //constexpr auto limit = -1;
    auto train_set = MyDataset(train_path, limit).map(torch::data::transforms::Stack<>());
    auto train_size = train_set.size().value();
    auto train_loader = torch::data::make_data_loader<torch::data::samplers::RandomSampler>(std::move(train_set), data_loader_options);
    print_time(start);
    cout << "Loaded training set" << endl;

    auto net = Net();
    net.to(device);

    auto adam_options = torch::optim::AdamOptions(0.001);
    auto optimizer = torch::optim::Adam(net.parameters(), adam_options);
    auto criterion = torch::nn::MSELoss();

    data_type total_train_loss = 0.0;
    data_type total_test_loss = 0.0;
    for (auto epoch = 0; epoch < 200; epoch++)
    {
        const auto epoch_start = chrono::high_resolution_clock::now();
        for (auto& batch : *train_loader)
        {
            auto data = batch.data.to(device);
            auto target = batch.target.to(device);

            optimizer.zero_grad();

            torch::Tensor prediction = net.forward(data);
            auto loss = criterion->forward(prediction, target);

            loss.backward();
            optimizer.step();

            const auto this_batch_size = batch.data.size(0);
            const auto this_loss = loss.item<data_type>();
            total_train_loss += this_loss * this_batch_size;
        }

        torch::NoGradGuard no_grad;
        for (auto& batch : *test_loader)
        {
            auto data = batch.data.to(device);
            auto target = batch.target.to(device);

            torch::Tensor prediction = net.forward(data);
            auto loss = criterion->forward(prediction, target);

            const auto this_batch_size = batch.data.size(0);
            const auto this_loss = loss.item<data_type>();
            total_test_loss += this_loss * this_batch_size;
        }
        const auto average_train_loss = total_train_loss / train_size;
        const auto average_test_loss = total_test_loss / test_size;
        print_time(epoch_start);
        cout << "Epoch: " << epoch << " | Train loss: " << average_train_loss << " | Test loss: " << average_test_loss << std::endl;
        total_train_loss = 0;
        total_test_loss = 0;
        print_params(net, epoch);
    }

    torch::NoGradGuard no_grad;

    auto it = test_loader->begin();
    auto sample_data = it->data.clone();
    auto sample_target = it->target.reshape(-1);

    stringstream ss;
    ss << setprecision(5);

    ss << "Targets: " << endl;
    for (auto i = 0; i < sample_data.size(0); i++)
    {
        ss << sample_target[i].item<data_type>() << " ";
    }
    ss << endl << endl;

    const auto predictions = net.forward(sample_data).reshape(-1);
    const auto predictions_no_sig = net.forward_no_sig(sample_data).reshape(-1);

    ss << "Predictions: " << endl;
    for (auto i = 0; i < sample_data.size(0); i++)
    {
        ss << predictions[i].item<data_type>() << " ";
    }
    ss << endl << endl;

    ss << "Predictions no sigmoid: ";
    for (auto i = 0; i < sample_data.size(0); i++)
    {
        ss << predictions_no_sig[i].item<data_type>() << " ";
    }
    ss << endl << endl;
    auto str = ss.str();
    cout << str;

    return 0;
}
