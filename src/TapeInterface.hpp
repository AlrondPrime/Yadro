#ifndef YADRO_TAPEINTERFACE_HPP
#define YADRO_TAPEINTERFACE_HPP

#include "pch.h"

class TapeInterface {
public:
    virtual int32_t read() = 0;

    virtual void write(int32_t val) = 0;

    virtual void forward() = 0;

    virtual void backward() = 0;

    virtual void rewind(int32_t val) = 0;

    virtual void clear() = 0;

    virtual void rewindToBegin() = 0;

    virtual bool end() = 0;

    virtual bool logicalEnd() = 0;

    virtual void configure(std::string_view pathToConfigFile) = 0;
};

#endif //YADRO_TAPEINTERFACE_HPP