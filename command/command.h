#ifndef MAIN_CPP_COMMAND_H
#define MAIN_CPP_COMMAND_H

template<typename trequest> class command
{
public:
    virtual ~command() = default;
public:
    virtual bool can_execute(trequest const &) noexcept = 0;
    virtual void execute(trequest const &) noexcept = 0;
};


#endif //MAIN_CPP_COMMAND_H
