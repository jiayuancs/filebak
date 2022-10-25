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

    bool restore_metadata;  // 是否恢复文件元信息
    bool use_original_path; // 是否将文件恢复到原来的位置
    bool verbose;           // 是否输出执行过程信息

public:
    Task(std::string src_path_, std::string bak_path_);
    ~Task();

    void SetVerbose(bool verbose_);

    // 备份
    bool Backup(std::string password);
    void SetComment(std::string comment_);
    void SetMod(unsigned char mod_);
    void SetFilter(const Filter &filter_);

    // 恢复
    bool Restore(std::string password);
    void RestoreMetadata(bool restore_metadata_);
    void UseOriginalPath(bool use_original_path_);
    bool GetBackupInfo();
    unsigned char GetBackupMode();

    // 获取备份文件的基本信息
    static bool GetBackupInfo(std::string file_path_, BackupInfo &info_);
};

#endif // INCLUDE_TASK_H_