#ifndef YADRO_TAPEINTERFACE_HPP
#define YADRO_TAPEINTERFACE_HPP

#include "pch.h"

class TapeInterface {
protected:
    int _cell_size;
    std::fstream _fs;
    std::string_view _path;

public:
    // Amount of written cells in last write session
    uint64_t cells_written{0};
    // Current cell
    uint64_t current_cell{0};

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

    virtual ~TapeInterface() {
        _fs.close();
    }

    virtual int32_t read() = 0;

    virtual void write(int32_t val) = 0;

    virtual void forward() = 0;

    virtual void backward() = 0;

    virtual void rewind(int32_t val) = 0;

    virtual void clear() {
        _fs.close();
        _fs.open(_path, std::fstream::out | std::fstream::trunc);
        _fs.close();
        _fs.open(_path);
        current_cell = 0;
        cells_written = 0;
    }

    virtual void rewindToBegin() {
        if (_fs.eof())
            _fs.clear();
        _fs.seekp(0);
        current_cell = 0;
    }

    virtual bool end() {
        return _fs.peek() == EOF;
    }

    virtual bool logicalEnd() {
        std::cout << "\tcurrent_cell=" << current_cell
                  << "; cells_written=" << cells_written << std::endl;
        return current_cell >= cells_written;
    }
};

#endif //YADRO_TAPEINTERFACE_HPP