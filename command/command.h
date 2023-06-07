#ifndef MAIN_CPP_COMMAND_H
#define MAIN_CPP_COMMAND_H

#include <iostream>
#include <vector>
#include <sstream>

class command
{

public:

    virtual ~command() = default;

protected:

    std::vector<std::string> split(std::string const & text, char delim) const
    {
        std::vector<std::string> result;
        std::string token;
        std::istringstream tokenStream(text);
        while (std::getline(tokenStream, token, delim))
        {
            result.push_back(token);
        }
        return result;
    }

public:

    virtual bool can_execute(std::string const &) noexcept = 0;

    virtual void execute(std::string const &) const noexcept = 0;
};


#endif //MAIN_CPP_COMMAND_H
