#ifndef YADRO_BINARYTAPE_HPP
#define YADRO_BINARYTAPE_HPP

#include "TapeInterface.hpp"

class BinaryTape : public TapeInterface {
public:
    explicit BinaryTape(std::string_view path) : TapeInterface(4) {
        _fs.open(path);
    }

    ~BinaryTape() {
        _fs.close();
    }

    int32_t read() override {
        if (!_fs.is_open()) {
            std::cout << "File is not opened" << std::endl;
            return {};
        }

        int32_t val{};
        std::array<char, sizeof(int32_t)> buffer{};
        _fs.read(buffer.data(), buffer.size());
        val = *reinterpret_cast<const int32_t *>(buffer.data());

        backward();
        return val;
    }

    void write(int32_t val) override {
        if (!_fs.is_open()) {
            std::cout << "File is not opened" << std::endl;
            return;
        }

        std::array<char, sizeof(int32_t)> buffer{};
        std::copy(reinterpret_cast<char *>(&val),
                  reinterpret_cast<char *>(&val) + sizeof(int32_t),
                  buffer.begin());
        std::copy(buffer.begin(),
                  buffer.end(),
                  std::ostream_iterator<char>{_fs});

        backward();
    }

    auto pos(){
        return _fs.tellp();
    }

    void forward() override {
        auto pos = _fs.tellp();
        pos += _RECORD_SIZE;
        _fs.seekp(pos);
    }

    void backward() override {
        auto pos = _fs.tellp();
        if (pos == 0)
            return;
        pos -= _RECORD_SIZE;
        _fs.seekp(pos);
    }

    void rewind(int32_t val) override {
        auto pos = _fs.tellp();
        pos += _RECORD_SIZE * val;
        if (pos < 0)
            return;
        _fs.seekp(pos);
    }
};

#endif //YADRO_BINARYTAPE_HPP