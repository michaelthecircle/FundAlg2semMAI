#ifndef MAIN_CPP_PARSE_H
#define MAIN_CPP_PARSE_H
#include <iostream>
#include <vector>

std::vector<std::string> chop_string_to_vector(
        std::string::iterator const &begin,
        std::string::iterator const &end,
        const char delim)
{
    std::vector<std::string> out;
    auto it = begin;
    std::string lexem;
    size_t start;
    size_t finish = 0;

    while (it != end)
    {
        if (*it == delim && !lexem.empty())
        {
            out.push_back(std::move(lexem));
        }
        else
        {
            lexem += *it;
        }

        ++it;
    }

    if (!lexem.empty())
    {
        out.push_back(std::move(lexem));
    }

    return out;
}

bool prefix_validation(std::string const& prefix, std::string const& request)
{
    return (request.starts_with(prefix+'(') && request.ends_with(")"));
}

std::vector<std::string>validation(std::string prefix, std::string const& request, int arguments_count)
{
    std::string* non_const_request = const_cast<std::string*>(&request);

    std::vector<std::string> chopped_string = chop_string_to_vector(
            non_const_request->begin() + std::string(prefix).length(),
            non_const_request->end(),
            ' ');

    if(arguments_count != chopped_string.size())
    {
        chopped_string.clear();
        return chopped_string;
    }

    return chopped_string;
}

#endif //MAIN_CPP_PARSE_H
