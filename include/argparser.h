#ifndef INCLUDE_ARGPARSER_H_
#define INCLUDE_ARGPARSER_H_
#include "cxxopts.hpp"

// 解析命令行参数
class ArgParser
{
private:
    cxxopts::Options options;
    cxxopts::ParseResult result;

public:
    bool flag_backup;
    bool flag_restore;
    bool flag_verbose;
    bool flag_compress;
    bool flag_encrypt;
    bool flag_metadata;
    bool flag_help;

    bool flag_list;
    bool flag_input;
    bool flag_output;
    bool flag_password;
    bool flag_path;
    bool flag_type;
    bool flag_name;
    bool flag_atime;
    bool flag_mtime;
    bool flag_ctime;
    bool flag_message;

    std::string str_list;
    std::string str_input;
    std::string str_output;
    std::string str_password;
    std::string str_path;
    std::string str_type;
    std::string str_name;
    std::string str_atime;
    std::string str_mtime;
    std::string str_ctime;
    std::string str_message;

public:
    ArgParser();
    ~ArgParser();

    bool Parse(int argc, char **argv);

    void PrintErrorInfo();

private:
    bool CheckArgsBackup();
    bool CheckArgsRestore();
    bool CheckArgsList();
};

#endif // INCLUDE_ARGPARSER_H_