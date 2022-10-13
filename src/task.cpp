#include "task.h"

Task::Task(std::string src_path_, std::string bak_path_)
{
    src_path = std::filesystem::absolute(src_path_);
    bak_path = std::filesystem::absolute(bak_path_);
    memset(&info, 0, sizeof(info));
    info.mod = FILE_MOD_COMPRESS | FILE_MOD_ENCRYPT;
}

Task::~Task()
{
}

void Task::SetComment(std::string comment_)
{
    memset(&(info.comment), 0, sizeof(info.comment));
    memcpy(info.comment, comment_.c_str(), std::min(comment_.length(), sizeof(info.comment)));
}

bool Task::Backup(std::string password)
{
    // 判断路径是否存在
    if (!std::filesystem::exists(src_path))
    {
        std::cout << "error: no such file or directory: " << src_path.string() << std::endl;
        return false;
    }
    // 判断文件名是否符合要求
    std::string name = bak_path.filename();
    std::regex reg("^[.]*[\\w]+[\\w.-]*$");
    if (!std::regex_match(name, reg))
    {
        std::cout << "error: invalid file name: " << name << std::endl;
        return false;
    }

    // 打包
    Packer packer(src_path, bak_path, filter);
    if (!packer.Pack())
    {
        std::cout << "error: failed to pack file" << std::endl;
        return false;
    }
    bak_path += FILE_SUFFIX_PACK;

    if (info.mod & FILE_MOD_COMPRESS)
    {
        // 压缩
        Compressor compressor(bak_path);
        if (!compressor.Compress())
        {
            std::cout << "error: failed to compress file" << std::endl;
            return false;
        }
        bak_path += FILE_SUFFIX_COMPRESS;
    }

    if (info.mod & FILE_MOD_ENCRYPT)
    {
        // 加密
        Aes aes(bak_path, password);
        if (!aes.Encrypt())
        {
            std::cout << "error: failed to encrpy file" << std::endl;
            return false;
        }
        bak_path += FILE_SUFFIX_ENCRYPT;
    }

    // 写入备份信息
    FileBase file(bak_path);
    file.OpenFile(std::ios::in | std::ios::out | std::ios::binary);
    file.WriteBackupInfo(info);
    file.close();

    return true;
}

bool Task::Restore(std::string password)
{
    // 判断路径是否存在
    if (!std::filesystem::exists(bak_path))
    {
        std::cout << "error: no such file or directory: " << src_path.string() << std::endl;
        return false;
    }

    // 读取的备份信息
    FileBase file(bak_path);
    file.OpenFile(std::ios::in | std::ios::binary);
    info = file.ReadBackupInfo();
    file.close();

    if (info.mod & FILE_MOD_ENCRYPT)
    {
        // 解密
        Aes aes(bak_path, password);
        if (!aes.Decrypt())
        {
            std::cout << "error: failed to decrypt file" << std::endl;
            return false;
        }
        bak_path.replace_extension("");
    }

    if (info.mod & FILE_MOD_COMPRESS)
    {
        // 解压
        Compressor compressor(bak_path);
        if (!compressor.Decompress())
        {
            std::cout << "error: failed to decompress file" << std::endl;
            return false;
        }
        bak_path.replace_extension("");
    }

    // 解包
    Packer packer(src_path, bak_path, filter);
    if (!packer.Unpack())
    {
        std::cout << "error: failed to unpack file" << std::endl;
        return false;
    }

    return true;
}