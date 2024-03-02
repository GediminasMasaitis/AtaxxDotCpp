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
using InputData = std::array<data_type, input_size>;
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
    vector<InputData> inputs_data;
    vector<OutputData> outputs_data;

    vector<torch::Tensor> inputs;
    vector<torch::Tensor> targets;

    explicit MyDataset(const string& path)
    {
        const auto size = filesystem::file_size(filesystem::path(path));
        constexpr auto entry_size = sizeof(Bitboard) + sizeof(Bitboard) + sizeof(Color) + sizeof(Wdl) + sizeof(Score);
        const auto entry_count = size / entry_size;

        cout << "Loading " << path << endl;
        cout << size << " bytes" << endl;
        cout << entry_count << " entries" << endl;

        inputs_data.resize(entry_count);
        outputs_data.resize(entry_count);

        inputs.reserve(entry_count);
        targets.reserve(entry_count);

        auto stream = ifstream(path, ios::binary);
        if (!stream.is_open())
        {
            cerr << "Failed to open file: " << path << endl;
            throw exception();
        }

        const auto tensor_options = torch::TensorOptions().dtype(data_type_val);

        for(size_t entry_index = 0; entry_index < entry_count; entry_index++)
        {
            const auto entry = read_entry(stream);
            
            auto& input_data = inputs_data[entry_index];
            auto& output_data = outputs_data[entry_index];

            const auto is_black = entry.turn == 1;
            const auto us = is_black ? reverse_bits(entry.black) : entry.white;
            const auto them = is_black ? reverse_bits(entry.white) : entry.black;

            for(Rank rank = 0; rank < 7; rank++)
            {
                for (File file = 0; file < 7; file++)
                {
                    const auto square = get_square(file, rank);
                    const auto index = get_index(file, rank);
                    input_data[index] = static_cast<data_type>((us >> square) & 1);
                    input_data[49 + index] = static_cast<data_type>((them >> square) & 1);
                }
            }

            const auto wdl = is_black ? static_cast<data_type>(2 - entry.wdl) / 2 : static_cast<data_type>(entry.wdl) / 2;
            output_data[0] = wdl;
            auto input = torch::from_blob(input_data.data(), { input_size }, tensor_options);
            auto target = torch::from_blob(output_data.data(), { 1 }, tensor_options);

            inputs.push_back(input);
            targets.push_back(target);
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

    Net() {
        fc1 = register_module("fc1", torch::nn::Linear(input_size, 1));
    }

    torch::Tensor forward(torch::Tensor x) {
        x = torch::sigmoid(fc1->forward(x.reshape({ x.size(0), input_size })));
        return x;
    }

    torch::Tensor forward_no_sig(torch::Tensor x)
    {
        x = fc1->forward(x.reshape({ x.size(0), input_size }));
        return x;
    }
};

void print_params(const Net& model)
{
    auto file_bin = ofstream("C:/shared/ataxx/nets/default.nnue", ios::out | ios::binary);
    auto file_human = ofstream("C:/shared/ataxx/nets/default.txt", ios::out);
    stringstream ss;
    for (const auto& pair : model.named_parameters()) {
        auto& name = pair.key();
        auto param = pair.value().reshape(-1);
        ss << name << endl;
        file_human << name << endl;
        cout << param.size(0);
        for(auto i = 0; i < param.size(0); i++)
        {
            auto val = param[i].item<data_type>();
            file_bin.write(reinterpret_cast<char*>(&val), sizeof(data_type));

            const auto val_str = to_string(val);
            file_human << val_str << " ";
            ss << val_str << " ";
            //ss << setprecision(5) << val << "f, ";
        }
        ss << endl;
        file_human << endl;
    }
    ss << endl;
    const auto str = ss.str();
    cout << str;
    file_bin.flush();
    file_human.flush();
}

int main()
{
    //constexpr int32_t batch_size = 1024 * 128;
    constexpr int32_t batch_size = 1024 * 32;
    //constexpr int32_t batch_size = 128;

    constexpr auto test_path = "C:/shared/ataxx/data/data_test.bin";
    const auto test_load_start = chrono::high_resolution_clock::now();
    auto test_set = MyDataset(test_path).map(torch::data::transforms::Stack<>());
    auto test_size = test_set.size().value();
    auto test_loader = torch::data::make_data_loader<torch::data::samplers::SequentialSampler>(std::move(test_set), torch::data::DataLoaderOptions(batch_size));
    const auto test_load_end = chrono::high_resolution_clock::now();
    const auto test_load_ms = chrono::duration_cast<chrono::milliseconds>(test_load_end - test_load_start);
    cout << "Loaded test set in " << test_load_ms.count() << "ms" << endl;

    constexpr auto train_path = "C:/shared/ataxx/data/data3M.bin";
    //constexpr auto train_path = "C:/shared/ataxx/data/data_train_small.bin";
    const auto train_load_start = chrono::high_resolution_clock::now();
    auto train_set = MyDataset(train_path).map(torch::data::transforms::Stack<>());
    auto train_size = train_set.size().value();
    auto train_loader = torch::data::make_data_loader<torch::data::samplers::RandomSampler>(std::move(train_set), torch::data::DataLoaderOptions(batch_size));
    const auto train_load_end = chrono::high_resolution_clock::now();
    const auto train_load_ms = chrono::duration_cast<chrono::milliseconds>(train_load_end - train_load_start);
    cout << "Loaded training set in " << train_load_ms.count() << "ms" << endl;

    auto net = Net();
    auto adam_options = torch::optim::AdamOptions(0.001);
    auto optimizer = torch::optim::Adam(net.parameters(), adam_options);
    auto criterion = torch::nn::MSELoss();

    data_type total_train_loss = 0.0;
    data_type total_test_loss = 0.0;
    for(auto epoch = 0; epoch < 20; epoch++)
    {
        for (auto& batch : *train_loader)
        {
            optimizer.zero_grad();

            torch::Tensor prediction = net.forward(batch.data);
            auto loss = criterion->forward(prediction, batch.target);

            loss.backward();
            optimizer.step();

            const auto this_batch_size = batch.data.size(0);
            const auto this_loss = loss.item<data_type>();
            total_train_loss += this_loss * this_batch_size;
        }

        torch::NoGradGuard no_grad;
        for (auto& batch : *test_loader)
        {
            torch::Tensor prediction = net.forward(batch.data);
            auto loss = criterion->forward(prediction, batch.target);

            const auto this_batch_size = batch.data.size(0);
            const auto this_loss = loss.item<data_type>();
            total_test_loss += this_loss * this_batch_size;
        }

        const auto average_train_loss = total_train_loss / train_size;
        const auto average_test_loss = total_test_loss / test_size;
        std::cout << "Epoch: " << epoch << " | Train loss: " << average_train_loss << " | Test loss: " << average_test_loss << std::endl;
        total_train_loss = 0;
        total_test_loss = 0;
    }

    torch::NoGradGuard no_grad;
    print_params(net);

    auto it = test_loader->begin();
    auto sample_data = it->data.clone();
    auto sample_target = it->target.reshape(-1);

    stringstream ss;
    ss << setprecision(5);

    ss << "Targets: " << endl;
    for(auto i = 0; i < sample_data.size(0); i++)
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