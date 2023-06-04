#ifndef MAIN_CPP_FILE_PARSER_H
#define MAIN_CPP_FILE_PARSER_H
#include <iostream>
#include <fstream>


class file_parser
{
private :
    std::ifstream &file_stream;







public:
    file_parser(std::string &file_path);

};
#endif //MAIN_CPP_FILE_PARSER_H
