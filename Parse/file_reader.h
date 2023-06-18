#ifndef MAIN_CPP_FILE_READER_H
#define MAIN_CPP_FILE_READER_H

#include <iostream>
#include <fstream>

class file_reader {
private:
    std::string _file_path;
    std::ifstream _file;
    std::string current_string;

public:
    file_reader(std::string &file_path): _file_path(std::move(file_path)) {}
    bool check_file_openning()
    {
        _file.open(_file_path);
        return _file.is_open();
    }
    void read_file_by_line(std::ifstream &opened_file)
    {
        while(std::getline(_file, current_string)){
            std::cout << "current_string" << current_string << std::endl;
        }
    }

};


#endif //MAIN_CPP_FILE_READER_H
