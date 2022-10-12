#ifndef INCLUDE_FILEBASE_H_
#define INCLUDE_FILEBASE_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <filesystem>

#include <iostream>
#define MAX_PACK_PATH_LEN 256  // 最大路径长度
#define BLOCK_BUFFER_SIZE 4096 // 读写文件的缓冲区大小
#define BACKUP_COMMENT_SIZE 255

// 文件类型
#define FILE_TYPE_NORMAL 1
#define FILE_TYPE_HARD_LINK 2
#define FILE_TYPE_SYMBOLIC_LINK 4
#define FILE_TYPE_DIRECTORY 8
#define FILE_TYPE_FIFO 16
#define FILE_TYPE_OTHER 32

// 文件格式
#define FILE_MOD_COMPRESS 1     // 压缩
#define FILE_MOD_ENCRYPT 2      // 加密

typedef unsigned char FileType;

struct BackupInfo{
    char comment[BACKUP_COMMENT_SIZE];
    unsigned char mod;
};

struct FileHeader
{
    char name[MAX_PACK_PATH_LEN];
    struct stat metadata;

    // 软链接路径 或 硬链接路径
    // 对于硬链接，只有其链接目标在备份文件范围内该项才有效
    // 反之，则备份时将其改为链接目标类型文件
    char linkname[MAX_PACK_PATH_LEN];
};

#define FILE_HEADER_SIZE (sizeof(FileHeader))

class FileBase : public std::fstream
{
private:
    FileHeader fileheader;          // 文件元数据
    std::filesystem::path filepath; // 文件路径

    bool flag_no_fileheader;

public:
    // 传入文件元数据 创建文件
    FileBase(FileHeader fileheader_);
    // 传入文件路径 打开文件
    FileBase(std::filesystem::path filepath_);
    ~FileBase();

    // void CreateAndOpenFile();
    // void CreateFile(string filepath_);
    bool OpenFile(std::ios_base::openmode mode_);

    // 从当前文件中读一个文件头信息
    FileHeader ReadFileHeader();
    // 读取备份信息
    BackupInfo ReadBackupInfo();
    void WriteBackupInfo();
    void WriteBackupInfo(BackupInfo info);

    size_t GetFileSize();
    FileType GetFileType();
    FileHeader GetFileHeader();
    void SetRelativePath(std::string filepath_);

    // 用于测试
    void printTime(std::string dec, const struct timespec &tim)
    {
        std::cout << dec << tim.tv_sec << " " << tim.tv_nsec << std::endl;
    }
    void printStat()
    {
        const struct stat &header = fileheader.metadata;
        std::cout << "文件类别: " << header.st_mode << std::endl;
        std::cout << "inode: " << header.st_ino << std::endl;
        std::cout << "设备号码: " << header.st_dev << std::endl;
        std::cout << "特殊设备号: " << header.st_rdev << std::endl;
        std::cout << "文件连接数: " << header.st_nlink << std::endl;
        std::cout << "文件所有者: " << header.st_uid << std::endl;
        std::cout << "文件所有者对应的组: " << header.st_gid << std::endl;
        std::cout << "文件对应的文件字节数: " << header.st_size << std::endl;  // 软连接即连接路径字符所占用的空间
        std::cout << "IO一次读入的块大小: " << header.st_blksize << std::endl; // 可用于设置缓冲区大小
        std::cout << "对应的512字节块数量: " << header.st_blocks << std::endl; // 不需要
        printTime("上次访问:", header.st_atim);
        printTime("上次修改:", header.st_mtim);
        printTime("创建时间:", header.st_ctim);
    }
};

#endif // INCLUDE_FILEBASE_H_