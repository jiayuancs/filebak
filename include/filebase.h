#ifndef INCLUDE_FILEBASE_H_
#define INCLUDE_FILEBASE_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <fcntl.h>
#include <iostream>

#define MAX_PACK_PATH_LEN 256   // 最大路径长度
#define BLOCK_BUFFER_SIZE 4096  // 读写文件的缓冲区大小
#define BACKUP_COMMENT_SIZE 255 // 备份时的备注信息长度

// 文件类型
#define FILE_TYPE_NORMAL 1
#define FILE_TYPE_HARD_LINK 2
#define FILE_TYPE_SYMBOLIC_LINK 4
#define FILE_TYPE_DIRECTORY 8
#define FILE_TYPE_FIFO 16
#define FILE_TYPE_OTHER 32

// 文件格式
#define BACKUP_MOD_COMPRESS 1 // 压缩
#define BACKUP_MOD_ENCRYPT 2  // 加密

typedef unsigned char FileType;

struct BackupInfo
{
    time_t timestamp;               // 时间戳
    uint32_t checksum;                   // 校验和
    char backup_path[MAX_PACK_PATH_LEN]; // 备份路径
    char comment[BACKUP_COMMENT_SIZE];   // 描述信息
    unsigned char mod;                   // 压缩、加密

    uint32_t CalcChecksum()
    {
        uint32_t *p = (uint32_t *)this;
        int cnt = sizeof(BackupInfo) / 4;
        uint32_t sum = 0;
        for(int i=0;i<cnt;++i)
        {
            sum += *p;
            p++;
        }
        return sum;
    }
}; // 528字节

struct FileHeader
{
    char name[MAX_PACK_PATH_LEN];
    struct stat metadata;

    // 对于硬链接，只有其链接目标在备份文件范围内该项才有效
    char linkname[MAX_PACK_PATH_LEN];
};

#define FILE_HEADER_SIZE (sizeof(FileHeader))

class FileBase : public std::fstream
{
private:
    FileHeader fileheader;          // 文件元数据
    std::filesystem::path filepath; // 文件路径

public:
    // 传入文件元数据 创建文件
    FileBase(FileHeader fileheader_);
    // 传入文件路径 获取文件元数据
    FileBase(std::filesystem::path filepath_);
    ~FileBase();

    bool OpenFile(std::ios_base::openmode mode_);

    // 从当前文件中读一个文件头信息
    FileHeader ReadFileHeader();
    // 读取备份信息
    BackupInfo ReadBackupInfo();
    void WriteBackupInfo();
    void WriteBackupInfo(BackupInfo info);

    size_t GetFileSize();
    FileType GetFileType();
    bool IsHardLink();
    FileHeader GetFileHeader();

    void ReatoreMetadata();

    static FileType GetFileType(const FileHeader &file_header_);

};

#endif // INCLUDE_FILEBASE_H_