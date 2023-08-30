#ifndef MAIN_CPP_FILE_READER_H
#define MAIN_CPP_FILE_READER_H

#include <iostream>
#include <fstream>
#include <sstream>

#include "kursach_puk_puk/trees_names.h"

class file_reader {
private:
    std::string _file_path;
    std::ifstream _file;
    std::string current_string;
    trees tree_type;
public:
    file_reader(std::string &file_path): _file_path(std::move(file_path)) {}
private:
    bool check_file_openning()
    {
        _file.open(_file_path);
        return _file.is_open();
    }
public:
    void read_file_by_line()
    {
        std::string tree_type_str;
        if (!(check_file_openning()))
        {
            throw std::invalid_argument("file_path is wrong " + _file_path);
        }
        std::getline(_file, tree_type_str);
        try
        {
            get_tree_type(tree_type_str, tree_type);
        }
        catch (const std::invalid_argument &ex)
        {
            throw ex;
        }

        while(std::getline(_file, current_string))
        {
            parseWords(current_string); // add try for invalid args
            std::cout << "current_string = " << current_string  << " --------- words_count = " << std::endl;
        }
    }
private:
    enum class types_commands
    {
        ADD,
        Delete,
        Read
        //TODO:: добавить виды комманд
    };
private:
    void parseWords(const std::string &text)
    {
        std::stringstream ss( text );
        std::string word;
        // types_commands typesCommands; вроде не нужно
        while( ss >> word ) {
            if (word == "ADD")
            {
                ss >> word;
                if (word == "POOL")
                {
                    //
                } // нужно отправить в паттерн команду ss оставшуюся строку, затем break
                else if (word == "SCHEME") {}
                else if (word == "COLLECTION") {}

                else throw std::invalid_argument(word);
            }
            if (word == "READ")
            {
                ss >> word;
                if (word == "POOL")
                {
                    //
                }
                else if (word == "SCHEME") {}
                else if (word == "COLLECTION") {}
                else throw std::invalid_argument(word);
            }
            //TODO:: more cases
            throw std::invalid_argument("wrong input");
        }
    }
    void get_tree_type(const std::string &input, trees &_trees)
    {
        if (input == "AVL tree")
        {
            _trees = trees::avl_tree;
        }
        else if (input == "SPLAY tree")
        {
            _trees = trees::splay_tree;
        }
        else
        {
            throw std::invalid_argument(input);
        }
        //TODO:: добавить типы деревьев
    }
};


#endif //MAIN_CPP_FILE_READER_H
