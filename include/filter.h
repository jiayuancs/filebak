#ifndef INCLUDE_FILTER_H_
#define INCLUDE_FILTER_H_

#include "filebase.h"
#include <string>
#include <regex>

// 根据路径/类型/名字/时间筛选备份的文件
#define FILTER_FILE_PATH 1
#define FILTER_FILE_NAME 2
#define FILTER_FILE_TYPE 4
#define FILTER_FILE_TIME 8

class Filter
{
private:
    unsigned char type;
    std::string reg_path; // 过滤文件路径的正则表达式
    std::string reg_name; // 过滤文件名的正则表达式
    FileType file_type;   // 过滤文件类型

    // 限定文件的时间
    std::string atim;
    std::string mtim;
    std::string ctim;

    bool verbose;           // 输出执行过程信息

public:
    Filter();
    Filter(const Filter &filter_);
    ~Filter();

    void SetPathFilter(std::string reg_path_);
    void SetNameFilter(std::string reg_name_);
    void SetFileType(FileType file_type_);
    void SetAccessTime(std::string atim_);
    void SetModifyTime(std::string mtim_);
    void SetChangeTime(std::string ctim_);

    // 判定文件是否满足给定的过滤规则
    bool check(const FileHeader &file_header_);
};

#endif // INCLUDE_FILTER_H_