#ifndef INCLUDE_TASK_H_
#define INCLUDE_TASK_H_

#include"packer.h"
#include"compressor.h"
#include"aes.h"

// 默认将目录下的文件打包，可选对打包后的文件进行压缩或加密




class Task
{
private:
    std::filesystem::path src_path; // 待备份的目录
    std::filesystem::path bak_path; // 备份文件存放的位置

    BackupInfo info;
    Filter filter;
    
public:
    Task(std::string src_path_, std::string bak_path_);
    ~Task();

    void SetComment(std::string comment_);

    bool Backup(std::string password);
    bool Restore(std::string password);
};


#endif // INCLUDE_TASK_H_