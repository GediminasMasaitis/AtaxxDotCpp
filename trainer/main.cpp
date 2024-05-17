#include "../src/attacks.h"

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
    Square from;
    Square to;
};

#define POLICY 0

#if POLICY
static constexpr bool do_policy = false;
#else
static constexpr bool do_policy = false;
#endif


#define POLICYFORMAT 0
#if POLICYFORMAT
static constexpr bool policyformat = true;
#else
static constexpr bool policyformat = false;
#endif

constexpr int32_t input_size = 49;
constexpr int32_t input_sides = 2;
constexpr int32_t hidden_size = 256;
constexpr bool double_accumulator = true;

constexpr int32_t policy_hidden_size = 128;
constexpr int32_t policy_output_size = 610;

using InputData = std::array<data_type, input_size>;
using InputDatas = std::array<InputData, 2>;

using OutputData = std::array<data_type, 1>;

struct IDataProvider
{
    virtual ~IDataProvider() {}
    virtual DataEntry get(size_t index) = 0;
    virtual size_t size() const = 0;
};

struct Reader : IDataProvider
{
private:
#if POLICYFORMAT
    constexpr static size_t file_entry_size = sizeof(Bitboard) + sizeof(Bitboard) + sizeof(Color) + sizeof(Wdl) + sizeof(Score) + sizeof(Square) + sizeof(Square);
#else
    constexpr static size_t file_entry_size = sizeof(Bitboard) + sizeof(Bitboard) + sizeof(Color) + sizeof(Wdl) + sizeof(Score);
#endif;

    ifstream file;
    size_t entry_count;

public:
    Reader(const string& path, const size_t read_limit = 0)
    {
        cout << "Opening " << path << ": ";

        const auto fs_path = filesystem::path(path);
        if (!filesystem::exists(fs_path))
        {
            cerr << path << " doesn't exist";
            throw exception();
        }

        const auto size = filesystem::file_size(fs_path);
        entry_count = size / file_entry_size;

        file = ifstream(path, ios::binary);
        if (!file)
        {
            cerr << "Failed to open file: " << path << endl;
            throw exception();
        }

        cout << size << " bytes, ";
        cout << entry_count << " entries";
        if(read_limit > 0 && read_limit < entry_count)
        {
            cout << ", limit " << read_limit;
            entry_count = read_limit;
        }
        cout << endl;
    }

    DataEntry get(size_t index) override
    {
        const size_t offset = index * file_entry_size;
        file.seekg(offset);

        DataEntry entry;
        file.read(reinterpret_cast<char*>(&entry.white), sizeof(entry.white));
        file.read(reinterpret_cast<char*>(&entry.black), sizeof(entry.black));
        file.read(reinterpret_cast<char*>(&entry.turn), sizeof(entry.turn));
        file.read(reinterpret_cast<char*>(&entry.wdl), sizeof(entry.wdl));
        file.read(reinterpret_cast<char*>(&entry.score), sizeof(entry.score));
        if constexpr (policyformat)
        {
            file.read(reinterpret_cast<char*>(&entry.from), sizeof(entry.from));
            file.read(reinterpret_cast<char*>(&entry.to), sizeof(entry.to));
        }
        return entry;
    }

    size_t size() const override
    {
        return entry_count;
    }
};

struct CachingReader : Reader
{
private:
    vector<DataEntry> cache;

public:
    CachingReader(const string& path, const size_t read_limit = 0, size_t cache_limit = 0) : Reader(path, read_limit)
    {
        cache = vector<DataEntry>();
        if(cache_limit == 0)
        {
            cache_limit = read_limit;
        }
        cache_limit = min(size(), cache_limit);
        cout << "Caching " << cache_limit << " entries" << endl;
        cache.reserve(cache_limit);
        for(size_t index = 0; index < cache_limit; index++)
        {
            const auto entry = Reader::get(index);
            cache.push_back(entry);

            constexpr size_t print_every = 1000000;
            if (index % print_every == print_every - 1 || index == size() - 1)
            {
                cout << "Cached " << index + 1 << " entries" << endl;
            }
        }
    }

    DataEntry get(size_t index) override
    {
        if(index < cache.size())
        {
            return cache[index];
        }

        return Reader::get(index);
    }
};

static constexpr Square get_index(const File file, const Rank rank)
{
    return rank * 7 + file;
}

struct SquareToIndexClass
{
    std::array<Square, 64> values;

    static constexpr Square get_index(const File file, const Rank rank)
    {
        return rank * 7 + file;
    }

    static constexpr Square get_square_to_index(const Square sq)
    {
        return get_index(get_file(sq), get_rank(sq));
    }

    constexpr SquareToIndexClass()
    {
        values = {};
        for (Square sq = 0; sq < 64; sq++)
        {
            values[sq] = get_square_to_index(sq);
        }
        values[no_square] = 49;
    }
};

static constexpr SquareToIndexClass square_to_index = SquareToIndexClass();

struct PolicyIndexesClass
{
    std::array<std::array<int16_t, 64>, 64> values;

    constexpr PolicyIndexesClass()
    {
        values = {};
        for (Square from = 0; from < 64; from++)
        {
            for (Square to = 0; to < 64; to++)
            {
                values[from][to] = -1;
            }
        }

        uint16_t index = 0;
        for (Rank rank = 0; rank < 7; rank++)
        {
            for (File file = 0; file < 7; file++)
            {
                const auto sq = get_square(file, rank);
                values[no_square][sq] = index++;
            }
        }
        for (Rank rank = 0; rank < 7; rank++)
        {
            for (File file = 0; file < 7; file++)
            {
                const auto from = get_square(file, rank);
                auto bb = Attacks.far[from];
                while (bb)
                {
                    const auto to = lsb(bb);
                    bb &= bb - 1;
                    values[from][to] = index++;
                }
            }
        }
    }
};

static constexpr PolicyIndexesClass policy_indexes = PolicyIndexesClass();

struct TransientDataset : torch::data::Dataset<TransientDataset>
{
private:
    IDataProvider& _provider;

    const torch::TensorOptions tensor_options = torch::TensorOptions().dtype(data_type_val);
    const torch::TensorOptions tensor_options_index = torch::TensorOptions().dtype(torch::ScalarType::Long);
    
public:
    TransientDataset(IDataProvider& provider) : _provider(provider)
    {
    }

    torch::data::Example<> get(const size_t index) override
    {
        const auto entry = _provider.get(index);

        InputDatas input_data;

        const auto is_black = entry.turn == 1;

        //const auto us_stm = entry.white;
        //const auto them_stm = entry.black;

        const auto us_stm = is_black ? reverse_bits(entry.black) : entry.white;
        const auto them_stm = is_black ? reverse_bits(entry.white) : entry.black;

        //const auto us_nstm = is_black ? entry.white : reverse_bits(entry.black);
        //const auto them_nstm = is_black ? entry.black : reverse_bits(entry.white);

        for (Rank rank = 0; rank < 7; rank++)
        {
            for (File file = 0; file < 7; file++)
            {
                const auto square = get_square(file, rank);
                const auto index = get_index(file, rank);

                input_data[0][index] = static_cast<data_type>((us_stm >> square) & 1);
                input_data[1][index] = static_cast<data_type>((them_stm >> square) & 1);
                //input_data[2][index] = static_cast<data_type>((us_nstm >> square) & 1);
                //input_data[3][index] = static_cast<data_type>((them_nstm >> square) & 1);
            }
        }

        auto input_us_stm = torch::from_blob(input_data[0].data(), { input_size }, tensor_options);
        auto input_them_stm = torch::from_blob(input_data[1].data(), { input_size }, tensor_options);
        //auto input_us_nstm = torch::from_blob(input_data[2].data(), { input_size }, tensor_options);
        //auto input_them_nstm = torch::from_blob(input_data[3].data(), { input_size }, tensor_options);
        auto input = torch::stack({ input_us_stm, input_them_stm }).clone();

        torch::Tensor target;
        if constexpr (do_policy)
        {
            const auto target_index = policy_indexes.values[entry.from][entry.to];
            if(target_index == -1)
            {
                cout << "NEGATIVE INDEX";
                throw "NEGATIVE INDEX"; 
            }

            //target = torch::one_hot()

            target = torch::zeros(policy_output_size, tensor_options);
            target[target_index] = 1;

            //cout << target_index;
            //target = torch::one_hot(torch::tensor({ target_index }, tensor_options_index), 610).toType(data_type_val);

            //target = target.clone();

            //cout << (int)from_index << " " << (int)to_index << endl;
        }
        else
        {
            const auto wdl = is_black ? static_cast<data_type>(2 - entry.wdl) / 2 : static_cast<data_type>(entry.wdl) / 2;
            OutputData output_data;
            output_data[0] = wdl;
            target = torch::from_blob(output_data.data(), { 1 }, tensor_options).clone();
        }

        return
        {
            input,
            target
        };
    }

    optional<size_t> size() const override
    {
        return _provider.size();
    }
};

class CachingDataset : public torch::data::Dataset<CachingDataset>
{
public:
    vector<torch::Tensor> inputs;
    vector<torch::Tensor> targets;

    explicit CachingDataset(TransientDataset& dataset)
    {
        assert(dataset.size().has_value());

        const auto size = dataset.size().value();
        inputs.reserve(size);
        targets.reserve(size);

        for (size_t entry_index = 0; entry_index < dataset.size(); entry_index++)
        {
            const auto entry = dataset.get(entry_index);

            inputs.push_back(entry.data);
            targets.push_back(entry.target);

            constexpr size_t print_every = 1000000;
            if (entry_index % print_every == print_every - 1 || entry_index == size - 1)
            {
                cout << "Loaded " << entry_index + 1 << " entries" << endl;
            }
        }
    }

    torch::data::Example<> get(const size_t index) override
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

struct MyNet : torch::nn::Module
{
    virtual torch::Tensor forward(const torch::Tensor& us_stm, const torch::Tensor& them_stm, const torch::Tensor& us_nstm, const torch::Tensor& them_nstm) = 0;
};

struct EvalNet : MyNet {
    torch::nn::Linear fc1{ nullptr };
    torch::nn::Linear fc2{ nullptr };

    EvalNet() {
        fc1 = register_module("fc1", torch::nn::Linear(input_size * input_sides, hidden_size));
        fc2 = register_module("fc2", torch::nn::Linear(hidden_size * (double_accumulator ? 2 : 1), 1));
    }

    torch::Tensor forward_inner(const torch::Tensor& us_stm, const torch::Tensor& them_stm, const torch::Tensor& us_nstm, const torch::Tensor& them_nstm) {
        if constexpr (double_accumulator)
        {
            auto stm = torch::cat({us_stm, them_stm}, 1);
            auto nstm = torch::cat({us_nstm, them_nstm}, 1);

            stm = fc1->forward(stm);
            nstm = fc1->forward(nstm);

            auto combined = torch::cat({ stm, nstm }, 1);
            //combined = torch::relu(combined);
            combined = torch::clamp(combined, 0, 1);

            auto result = fc2->forward(combined);
            return result;
        }
        else
        {
            auto stm = torch::cat({ us_stm, them_stm }, 1);
            stm = fc1->forward(stm);
            stm = torch::relu(stm);
            auto result = fc2->forward(stm);
            return result;
        }
    }

    torch::Tensor forward(const torch::Tensor& us_stm, const torch::Tensor& them_stm, const torch::Tensor& us_nstm, const torch::Tensor& them_nstm) override
    {
        auto result = forward_inner(us_stm, them_stm, us_nstm, them_nstm);
        result = torch::sigmoid(result);
        return result;
    }

    torch::Tensor forward_no_sig(const torch::Tensor& us_stm, const torch::Tensor& them_stm, const torch::Tensor& us_nstm, const torch::Tensor& them_nstm)
    {
        auto result = forward_inner(us_stm, them_stm, us_nstm, them_nstm);
        result *= 512;
        return result;
    }
};

struct PolicyNet : MyNet
{
    torch::nn::Linear fc1{ nullptr };
    torch::nn::Linear fc2{ nullptr };

    PolicyNet() {
        fc1 = register_module("fc1", torch::nn::Linear(input_size * input_sides, policy_hidden_size));
        fc2 = register_module("fc2", torch::nn::Linear(policy_hidden_size, policy_output_size));
    }

    torch::Tensor forward(const torch::Tensor& us_stm, const torch::Tensor& them_stm, const torch::Tensor& us_nstm, const torch::Tensor& them_nstm) override
    {
        auto stm = torch::cat({ us_stm, them_stm }, 1);
        stm = fc1->forward(stm);
        stm = torch::relu(stm);
        stm = fc2->forward(stm);
        return stm;
    }
};

void print_params(const MyNet& model, int epoch)
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
    //auto a = torch::one_hot(torch::tensor({ 5 }), 10).toType(data_type_val);
    //cout << a;
    //return 0;

    const auto start = chrono::high_resolution_clock::now();
    //auto device = torch::Device(torch::kCPU);
    auto device = torch::Device(torch::kCUDA);

    constexpr int32_t batch_size = 1024 * 128;
    //constexpr int32_t batch_size = 1024 * 32;
    //constexpr int32_t batch_size = 128;

    constexpr auto test_path = "C:/shared/ataxx/data/data3M.bin";
    constexpr auto limit_test = 100'000;
    auto test_reader = CachingReader(test_path, limit_test);
    auto test_transient_set = TransientDataset(test_reader);
    auto test_set = CachingDataset(test_transient_set).map(torch::data::transforms::Stack<>());
    //auto test_set = test_transient_set.map(torch::data::transforms::Stack<>());
    auto test_size = test_set.size().value();
    auto test_loader_options = torch::data::DataLoaderOptions();
    test_loader_options.batch_size(batch_size);
    test_loader_options.workers(2);
    //data_loader_options.enforce_ordering(false);

    auto test_loader = torch::data::make_data_loader<torch::data::samplers::SequentialSampler>(std::move(test_set), test_loader_options);
    print_time(start);
    cout << "Loaded test set" << endl;

    constexpr auto train_path = "C:/shared/ataxx/data/Zataxx-550M.bin";
    //constexpr auto train_path = "C:/shared/ataxx/data/data3M.bin";
    //constexpr auto train_path = "C:/shared/ataxx/data/data_train_small.bin";
    constexpr auto limit_train = 10'000'000;
    //constexpr auto limit = -1;
    auto train_reader = CachingReader(train_path, limit_train);
    auto train_transient_set = TransientDataset(train_reader);
    train_transient_set.get(0);
    //auto train_set = CachingDataset(train_transient_set).map(torch::data::transforms::Stack<>());
    auto train_set = train_transient_set.map(torch::data::transforms::Stack<>());
    auto train_size = train_set.size().value();
    auto train_loader_options = torch::data::DataLoaderOptions();
    train_loader_options.batch_size(batch_size);
    train_loader_options.workers(12);
    auto train_loader = torch::data::make_data_loader<torch::data::samplers::RandomSampler>(std::move(train_set), train_loader_options);
    print_time(start);
    cout << "Loaded training set" << endl;

#if POLICY
    auto net = PolicyNet();
#else
    auto net = EvalNet();
#endif
    
    net.to(device);

    auto adam_options = torch::optim::AdamOptions(0.001);
    auto optimizer = torch::optim::Adam(net.parameters(), adam_options);
#if POLICY
    auto criterion = torch::nn::CrossEntropyLoss();
#else
    auto criterion = torch::nn::MSELoss();
#endif
    data_type total_train_loss = 0.0;
    data_type total_test_loss = 0.0;
    for (auto epoch = 0; epoch < 300; epoch++)
    {
        const auto epoch_start = chrono::high_resolution_clock::now();
        for (auto& batch : *train_loader)
        {
            optimizer.zero_grad();

            auto data = batch.data.to(device);
            auto target = batch.target.to(device);

            torch::Tensor predictions;
            torch::Tensor targets;
            if(do_policy)
            {
                auto us_stm = data.select(1, 0);
                auto them_stm = data.select(1, 1);
                auto us_nstm = torch::flip(them_stm, 1);
                auto them_nstm = torch::flip(us_stm, 1);
                //cout << us_stm.sizes() << endl;
                //cout << target.sizes() << endl;
                auto prediction = net.forward(us_stm, them_stm, us_nstm, them_nstm);
                predictions = prediction;
                targets = target;
            }
            else
            {
                auto us_stm = data.select(1, 0);
                auto them_stm = data.select(1, 1);
                auto us_nstm = torch::flip(them_stm, 1);
                auto them_nstm = torch::flip(us_stm, 1);
                auto prediction = net.forward(us_stm, them_stm, us_nstm, them_nstm);
                auto prediction_flip = net.forward(them_nstm, us_nstm, them_stm, us_stm);

                us_stm = us_stm.view({ -1, 7, 7 }).transpose(1, 2).reshape({ -1, 49 });
                them_stm = them_stm.view({ -1, 7, 7 }).transpose(1, 2).reshape({ -1, 49 });
                us_nstm = torch::flip(them_stm, 1);
                them_nstm = torch::flip(us_stm, 1);
                auto prediction_diagonal = net.forward(us_stm, them_stm, us_nstm, them_nstm);
                auto prediction_diagonal_flip = net.forward(them_nstm, us_nstm, them_stm, us_stm);

                us_stm = us_stm.view({ -1, 7, 7 }).rot90(1, { 1, 2 }).reshape({ -1, 49 });
                them_stm = them_stm.view({ -1, 7, 7 }).rot90(1, { 1, 2 }).reshape({ -1, 49 });
                us_nstm = torch::flip(them_stm, 1);
                them_nstm = torch::flip(us_stm, 1);
                auto prediction_rot = net.forward(us_stm, them_stm, us_nstm, them_nstm);
                auto prediction_rot_flip = net.forward(them_nstm, us_nstm, them_stm, us_stm);

                us_stm = us_stm.view({ -1, 7, 7 }).transpose(1, 2).reshape({ -1, 49 });
                them_stm = them_stm.view({ -1, 7, 7 }).transpose(1, 2).reshape({ -1, 49 });
                us_nstm = torch::flip(them_stm, 1);
                them_nstm = torch::flip(us_stm, 1);
                auto prediction_rot_diagonal = net.forward(us_stm, them_stm, us_nstm, them_nstm);
                auto prediction_rot_diagonal_flip = net.forward(them_nstm, us_nstm, them_stm, us_stm);

                predictions = torch::stack({ prediction, prediction_flip, prediction_diagonal, prediction_diagonal_flip, prediction_rot, prediction_rot_flip, prediction_rot_diagonal, prediction_rot_diagonal_flip });
                targets = torch::stack({ target, target, target, target, target, target, target, target });
            }
            auto loss = criterion->forward(predictions, targets);

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

            auto us_stm = data.select(1, 0);
            auto them_stm = data.select(1, 1);
            auto us_nstm = torch::flip(them_stm, 1);
            auto them_nstm = torch::flip(us_stm, 1);

            torch::Tensor prediction = net.forward(us_stm, them_stm, us_nstm, them_nstm);
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

    net.to(torch::kCPU);

    auto it = test_loader->begin();
    cout << it->data.sizes() << endl;
    cout << it->target.sizes() << endl;
    auto sample_data = it->data.select(0, 0).reshape({ -1, 2, 49 });
    auto sample_target = it->target.select(0, 0).reshape({-1, 610});
    cout << sample_data.sizes() << endl;
    cout << sample_target.sizes() << endl;

    cout << setprecision(5);

    cout << "Targets: " << endl;
    for (auto i = 0; i < sample_target.size(1); i++)
    {
        cout << sample_target[0][i].item<data_type>() << " ";
    }
    cout << endl << endl;

    auto us_stm = sample_data.select(1, 0).clone();
    auto them_stm = sample_data.select(1, 1).clone();
    auto us_nstm = torch::flip(them_stm, 1).clone();
    auto them_nstm = torch::flip(us_stm, 1).clone();

    cout << us_stm.sizes() << endl;
    cout << them_stm.sizes() << endl;
    cout << us_nstm.sizes() << endl;
    cout << them_nstm.sizes() << endl;

    const auto predictions = net.forward(us_stm, them_stm, us_nstm, them_nstm);
    //const auto predictions_no_sig = net.forward_no_sig(sample_data).reshape(-1);
    cout << predictions.sizes() << endl;
    cout << "Predictions: " << endl;
    for (auto i = 0; i < predictions.size(1); i++)
    {
        cout << static_cast<int32_t>(round(predictions[0][i].item<data_type>() * 128)) << " ";
    }
    cout << endl << endl;

    //ss << "Predictions no sigmoid: ";
    //for (auto i = 0; i < sample_data.size(0); i++)
    //{
    //    ss << predictions_no_sig[i].item<data_type>() << " ";
    //}
    //ss << endl << endl;

    return 0;
}
