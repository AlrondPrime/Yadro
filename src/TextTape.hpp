#ifndef YADRO_TEXTTAPE_HPP
#define YADRO_TEXTTAPE_HPP

#include "TapeInterface.hpp"

class TextTape : public TapeInterface {
public:
    explicit TextTape(std::string_view path) : TapeInterface(12) {
        _fs.open(path);
    }

    ~TextTape() {
        _fs.close();
    }

    int32_t read() override {
        auto pos = _fs.tellp();
        if (!_fs.is_open()) {
            std::cout << "File is not opened" << std::endl;
            return {};
        }

        int32_t val;
        _fs >> val;

        _fs.seekp(pos);
        return val;
    }

    void write(int32_t val) override {
        auto pos = _fs.tellp();
        if (!_fs.is_open()) {
            std::cout << "File is not opened" << std::endl;
            return;
        }

        _fs << ((val < 0) ? '-' : '+')
            << std::setw(10) << std::setfill('0') << abs(val)
            << '\n';
        _fs.seekp(pos);
    }

    auto pos() {
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

#endif //YADRO_TEXTTAPE_HPP