#ifndef YADRO_TAPEINTERFACE_HPP
#define YADRO_TAPEINTERFACE_HPP

#include "pch.h"

class TapeInterface {
protected:
    int _record_size;
    std::fstream _fs;
    std::string_view _path;
    uint64_t _processed_bytes;

public:
    void cur_sym() {
        std::cout << "Current symbol:"
                  << "\'" << (char)_fs.peek() << "\'"
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

    virtual ~TapeInterface(){
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
    }

    virtual void rewindToBegin() {
        if (_fs.eof())
            _fs.clear();
        _fs.seekp(0);
    }

    virtual bool isEnd() {
        return _fs.peek() == EOF;
    }
};

#endif //YADRO_TAPEINTERFACE_HPP