#ifndef INCLUDE_TASK_H_
#define INCLUDE_TASK_H_

#include "packer.h"
#include "compressor.h"
#include "aes.h"

class Task
{
private:
    std::filesystem::path src_path; // 待备份的目录
    std::filesystem::path bak_path; // 备份文件路径(存放位置)

    BackupInfo info; // 备份信息
    Filter filter;   // 用户自定义规则 过滤文件

public:
    Task(std::string src_path_, std::string bak_path_);
    ~Task();

    void SetComment(std::string comment_);
    void SetMod(unsigned char mod_);
    void SetFilter(const Filter &filter_);

    bool Backup(std::string password);
    bool Restore(std::string password, bool restore_metadata = false);
};

#endif // INCLUDE_TASK_H_