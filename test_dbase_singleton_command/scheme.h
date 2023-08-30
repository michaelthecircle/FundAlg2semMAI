#ifndef MAIN_CPP_SCHEME_H
#define MAIN_CPP_SCHEME_H

#include <iostream>

class scheme
{
    private:

        associative_container<std::string, data_collection> * _scheme;

    public:

        scheme();

        ~scheme();

    public:

        void add(std::string const &key, data_collection && target);

        void remove(std::string const & key);

        data_collection const & find(std::string const & collection_name) const;

        bool find_in(std::string const & collection_name) const;

    public:

        scheme(scheme const & other);

        scheme & operator=(scheme const & other);

        scheme(scheme && other) noexcept;

        scheme & operator=(scheme && other) noexcept;
};


#endif //MAIN_CPP_SCHEME_H
