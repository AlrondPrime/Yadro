#ifndef YADRO_TEXTTAPE_HPP
#define YADRO_TEXTTAPE_HPP

#include "TapeInterface.hpp"

class TextTape : public TapeInterface {
public:
    explicit TextTape(std::string_view path) :
            TapeInterface() {
        _cell_size = 13;
        _path = path;

        _fs.open(_path);
        if (!_fs.is_open()) {
            std::string msg;
            msg.append("File \'");
            msg.append(path);
            msg.append("\' is not opened");
            throw std::runtime_error(msg);
        }
    }

    int32_t read() override {
        if (_fs.eof())
            std::cerr << "EOF" << std::endl;
        auto pos = _fs.tellp();
        if (!_fs.is_open()) {
            throw std::runtime_error("File is not opened");
        }

        int32_t val{};
        _fs >> val;
        std::cout << "Reading value=" << val << std::endl;

        _fs.seekp(pos);
        return val;
    }

    void write(int32_t val) override {
        std::cout << "Writing value=" << val << std::endl;
        if (_fs.eof())
            _fs.clear();

        auto pos = _fs.tellp();
        if (!_fs.is_open()) {
            throw std::runtime_error("File is not opened");
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
        pos += _cell_size;
        _fs.seekp(pos);
    }

    void backward() override {
        auto pos = _fs.tellp();
        if (pos == 0)
            return;
        pos -= _cell_size;
        _fs.seekp(pos);
        --current_cell;
    }

    void rewind(int32_t val) override {
        auto pos = _fs.tellp();
        pos += _cell_size * val;
        if (pos < 0)
            return;
        _fs.seekp(pos);
    }
};

#endif //YADRO_TEXTTAPE_HPP