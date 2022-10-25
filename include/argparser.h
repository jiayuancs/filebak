#ifndef INCLUDE_ARGPARSER_H_
#define INCLUDE_ARGPARSER_H_
#include "cxxopts.hpp"
#include "filebase.h"

// 帮助文档每行最大字符数
#define ARG_HELP_WIDTH 80

// 解析命令行参数
class ArgParser
{
private:
    cxxopts::Options options;
    cxxopts::ParseResult result;

public:
    // bool变量标识用户是否输入了该参数
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

    // 具体参数的值
    std::string str_list;
    std::string str_input;
    std::string str_output;
    std::string str_password;
    std::string str_path;
    std::string str_name;
    std::string str_message;
    FileType file_type;
    time_t atime_start;
    time_t atime_end;
    time_t mtime_start;
    time_t mtime_end;
    time_t ctime_start;
    time_t ctime_end;

private:
    std::string str_type;
    std::string str_atime;
    std::string str_mtime;
    std::string str_ctime;

public:
    ArgParser();
    ~ArgParser();

    // 解析命令行参数
    bool Parse(int argc, char **argv);

    // 输出错误提示信息
    void PrintErrorInfo();

    bool CheckPassword();

private:
    // 参数格式检查
    bool CheckRegExp();
    bool CheckArgsBackup();
    bool CheckArgsRestore();
    bool CheckArgsList();
    // 字符串日期转time_t
    bool ConvertTime(std::string str_time, time_t &time_start, time_t &time_end);
    time_t StrToTime(std::string str_time);
};

#endif // INCLUDE_ARGPARSER_H_