#ifndef YADRO_TAPE_HPP
#define YADRO_TAPE_HPP

#include "pch.h"
#include "TapeInterface.hpp"

class Tape : public TapeInterface {
protected:
    /// Amount of bytes using to represent single tape cell
    int _cell_size{};
    std::fstream _fs{};
    std::string_view _path{};
    double _read_delay{}, _write_delay{}, _rewind_delay{}, _move_delay{};

    /// Helper function
    [[maybe_unused]] void cursym() {
        std::cout << "Current symbol:"
                  << "\'" << (char) _fs.peek() << "\'"
                  << std::endl;
    }

    /// Helper function
    [[maybe_unused]] void curstate() {
        std::cout << "Current state:"
                  << "\tGood:" << _fs.good()
                  << "\tBad:" << _fs.bad()
                  << "\tFail:" << _fs.fail()
                  << "\tEOF:" << _fs.eof()
                  << "\tPos:" << _fs.tellp()
                  << std::endl;
    }

    static void _sleep(double milliseconds) {
        std::this_thread::sleep_for(std::chrono::duration<double>(milliseconds));
    }

public:
    /// Amount of written cells in last write session; The countdown starts from one
    uint64_t cells_written{0};
    /// Offset from the beginning; The countdown starts from zero
    uint64_t current_cell{0};

    explicit Tape(std::string_view path) :
            TapeInterface() {
        // 13 is the size of string "+2147483647\r\n" or "-0000000001\r\n"
        _cell_size = 13;
        _path = path;

        // Create a file if it does not exist
        if (!std::filesystem::exists(path)) {
            std::ofstream file_creator{path.data()};
            file_creator.close();
        }

        _fs.open(_path);
        if (!_fs.is_open()) {
            std::string msg;
            msg.append("File \'");
            msg.append(path);
            msg.append("\' is not opened");
            throw std::runtime_error(msg);
        }

        Tape::configure("../Config.txt");
    }

    ~Tape() {
        _fs.close();
    }

    int32_t read() override {
        _sleep(_read_delay);

        auto pos = _fs.tellp();
        if (!_fs.is_open()) {
            throw std::runtime_error("File is not opened");
        }

        int32_t val{};
        _fs >> val;

        _fs.seekp(pos);
        return val;
    }

    static std::string format(int32_t val) {
        std::stringstream ss{};
        ss << ((val < 0) ? '-' : '+')
           << std::setw(10) << std::setfill('0') << llabs(val)
           << '\n';
        return ss.str();
    }

    void write(int32_t val) override {
        _sleep(_write_delay);

        if (_fs.eof())
            _fs.clear();

        auto pos = _fs.tellp();
        if (!_fs.is_open()) {
            throw std::runtime_error("File is not opened");
        }

        _fs << format(val);

        _fs.seekp(pos);
    }

    void forward() override {
        _sleep(_move_delay);

        auto pos = _fs.tellp();
        pos += _cell_size;
        _fs.seekp(pos);
    }

    void backward() override {
        _sleep(_move_delay);

        auto pos = _fs.tellp();
        if (pos == 0)
            return;
        pos -= _cell_size;
        _fs.seekp(pos);
        --current_cell;
    }

    void rewind(int32_t val) override {
        _sleep(_rewind_delay);

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
        _sleep(_rewind_delay);

        if (_fs.eof())
            _fs.clear();
        _fs.seekp(0);
        current_cell = 0;
    }

    virtual bool end() override {
        return _fs.peek() == EOF;
    }

    bool logicalEnd() override {
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

#endif //YADRO_TAPE_HPP