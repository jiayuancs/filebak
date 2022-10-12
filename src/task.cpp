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

bool Task::CheckPath()
{
    // 判断路径是否存在
    if(!std::filesystem::exists(src_path))
    {
        std::cout<<"error: no such file or directory: "<<src_path.string()<<std::endl;
        return false;
    }

    // 判断文件名是否符合要求
    std::string name = bak_path.filename();
    std::regex reg("^[.]*[\\w]+[\\w.-]*$");
    return std::regex_match(name, reg);
}

void Task::SetComment(std::string comment_)
{
    memset(&(info.comment), 0, sizeof(info.comment));
    memcpy(info.comment, comment_.c_str(), std::min(comment_.length(), sizeof(info.comment)));
}

bool Task::Backup(std::string password)
{
    if (!CheckPath())
        return false;

    // 打包
    Packer packer(src_path, bak_path, filter);
    if (!packer.Pack())
        return false;
    bak_path += FILE_SUFFIX_PACK;

    if (info.mod & FILE_MOD_COMPRESS)
    {
        // 压缩
        Compressor compressor(bak_path);
        if (!compressor.Compress())
            return false;
        bak_path += FILE_SUFFIX_COMPRESS;
    }

    if (info.mod & FILE_MOD_ENCRYPT)
    {
        // 加密
        Aes aes(bak_path, password);
        if (!aes.Encrypt())
            return false;
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
    if (!CheckPath())
        return false;
    
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
            return false;
        bak_path.replace_extension("");
    }

    if (info.mod & FILE_MOD_COMPRESS)
    {
        // 解压
        Compressor compressor(bak_path);
        if (!compressor.Decompress())
            return false;
        bak_path.replace_extension("");
    }

    // 解包
    Packer packer(src_path, bak_path, filter);
    if (!packer.Unpack())
        return false;

    return true;
}