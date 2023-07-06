#ifndef YADRO_TEXTTAPE_HPP
#define YADRO_TEXTTAPE_HPP

#include "TapeInterface.hpp"

class TextTape : public TapeInterface {
protected:
    int _cell_size{};
    std::fstream _fs{};
    std::string_view _path{};
    double _read_delay{}, _write_delay{}, _rewind_delay{}, _move_delay{};

    void cur_sym() {
        std::cout << "Current symbol:"
                  << "\'" << (char) _fs.peek() << "\'"
                  << std::endl;
    }

    void cur_state() {
        std::cout << "Current state:"
                  << "\tGood:" << _fs.good()
                  << "\tBad:" << _fs.bad()
                  << "\tFail:" << _fs.fail()
                  << "\tEOF:" << _fs.eof()
                  << "\tPos:" << _fs.tellp()
                  << std::endl;
    }

public:
    // Amount of written cells in last write session
    uint64_t cells_written{0};
    // Current cell
    uint64_t current_cell{0};

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

        TextTape::configure("../Config.txt");
    }

    ~TextTape() {
        _fs.close();
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

    void clear() override {
        _fs.close();
        _fs.open(_path, std::fstream::out | std::fstream::trunc);
        _fs.close();
        _fs.open(_path);
        current_cell = 0;
        cells_written = 0;
    }

    void rewindToBegin() override {
        if (_fs.eof())
            _fs.clear();
        _fs.seekp(0);
        current_cell = 0;
    }

    virtual bool end() override {
        return _fs.peek() == EOF;
    }

    bool logicalEnd() override {
        std::cout << "\tcurrent_cell=" << current_cell
                  << "; cells_written=" << cells_written << std::endl;
        return current_cell >= cells_written;
    }

    void configure(std::string_view path) override {
        bpo::options_description config("Config");
        config.add_options()
                ("readDelay", bpo::value<double>(&_read_delay))
                ("writeDelay", bpo::value<double>(&_write_delay))
                ("rewindDelay", bpo::value<double>(&_rewind_delay))
                ("moveDelay", bpo::value<double>(&_move_delay));
        bpo::variables_map variables_map;
        bpo::store(bpo::parse_config_file("../Config.txt", config), variables_map);
        variables_map.notify();
    }
};

#endif //YADRO_TEXTTAPE_HPP