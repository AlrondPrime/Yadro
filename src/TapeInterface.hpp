#ifndef YADRO_TAPEINTERFACE_HPP
#define YADRO_TAPEINTERFACE_HPP

#include "pch.h"

class TapeInterface {
protected:
    const int _RECORD_SIZE;
    std::fstream _fs;

public:
    explicit TapeInterface(const int RECORD_SIZE) : _RECORD_SIZE(RECORD_SIZE) {}

    virtual int32_t read() = 0;

    virtual void write(int32_t val) = 0;

    virtual void forward() = 0;

    virtual void backward() = 0;

    virtual void rewind(int32_t val) = 0;

    virtual void rewindToBegin() {
        _fs.seekp(0);
    }
};

#endif //YADRO_TAPEINTERFACE_HPP