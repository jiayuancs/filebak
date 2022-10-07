#ifndef INCLUDE_PACKER_H_
#define INCLUDE_PACKER_H_

#include"filebase.h"
#include <unordered_map>


#define BLOCK_BUFFER_SIZE 4096


class Packer
{
private:
    std::filesystem::path root_path;    // 需要备份或恢复的文件路径
    std::filesystem::path bak_path;     // 打包文件的路径
    std::unordered_map<ino_t, std::string> inode_table;

    void DfsFile(FileBase& bak_file, std::filesystem::path cur_path);

public:
    Packer(std::string root_path_, std::string pack_path_);
    ~Packer();

    // 打包
    bool Pack();
    // 解包到原有位置
    bool Unpack();
    // 解包到指定位置
    bool Unpack(std::string root_path_);
};

#endif // INCLUDE_PACKER_H_