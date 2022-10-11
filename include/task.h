#ifndef INCLUDE_TASK_H_
#define INCLUDE_TASK_H_

#include"packer.h"
#include"compressor.h"
#include"aes.h"

class Task
{
private:
    std::filesystem::path src_path; // 待备份的目录
    std::filesystem::path bak_path; // 备份文件存放的位置
    std::string comment;            // 备份说明

    Filter filter;

public:
    Task(std::string src_path_, std::string bak_path_);
    ~Task();

    void SetComment(std::string comment_);

    bool Backup(std::string password);
    bool Restore(std::string password);
};


#endif // INCLUDE_TASK_H_