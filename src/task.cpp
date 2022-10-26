#include "task.h"

Task::Task(std::string src_path_, std::string bak_path_)
{
    if(src_path_.length() == 0)
        src_path_ = ".";
    src_path = std::filesystem::absolute(src_path_);
    bak_path = std::filesystem::absolute(bak_path_);
    memset(&info, 0, sizeof(info));
    info.mod = BACKUP_MOD_COMPRESS | BACKUP_MOD_ENCRYPT;
    info.timestamp = std::time(nullptr);
    memcpy(info.backup_path, src_path_.c_str(), std::min(src_path_.length(), sizeof(info.backup_path)));

    restore_metadata = false;
    verbose = true;
}

Task::~Task()
{
}

void Task::SetComment(std::string comment_)
{
    memset(&(info.comment), 0, sizeof(info.comment));
    memcpy(info.comment, comment_.c_str(), std::min(comment_.length(), sizeof(info.comment)));
}

void Task::SetMod(unsigned char mod_)
{
    info.mod = mod_;
}

void Task::SetFilter(const Filter &filter_)
{
    filter = filter_;
}

void Task::SetVerbose(bool verbose_)
{
    verbose = verbose_;
}

void Task::RestoreMetadata(bool restore_metadata_)
{
    restore_metadata = restore_metadata_;
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
    if (verbose)
        std::cout << "PACKING..." << std::endl;
    Packer packer(src_path, bak_path, filter, verbose);
    if (!packer.Pack())
    {
        std::cout << "error: failed to pack file" << std::endl;
        return false;
    }
    bak_path += FILE_SUFFIX_PACK;

    if (info.mod & BACKUP_MOD_COMPRESS)
    {
        if (verbose)
            std::cout << "COMPRESSING..." << std::endl;
        // 压缩
        Compressor compressor(bak_path);
        if (!compressor.Compress())
        {
            std::cout << "error: failed to compress file" << std::endl;
            return false;
        }
        std::filesystem::remove_all(bak_path);
        bak_path += FILE_SUFFIX_COMPRESS;
    }

    if (info.mod & BACKUP_MOD_ENCRYPT)
    {
        if (verbose)
            std::cout << "ENCRYPTING..." << std::endl;
        // 加密
        Aes aes(bak_path, password);
        if (!aes.Encrypt())
        {
            std::cout << "error: failed to encrpy file" << std::endl;
            return false;
        }
        std::filesystem::remove_all(bak_path);
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

    // 解密
    if (info.mod & BACKUP_MOD_ENCRYPT)
    {
        if (verbose)
            std::cout << "DECRYPTING..." << std::endl;
        
        Aes aes(bak_path, password);
        int status = aes.Decrypt();
        if (status == -2)
        {
            std::cout << "error: failed to decrypt file" << std::endl;
            return false;
        }
        else if (status == -1)
        {
            std::cout << "error: wrong password" << std::endl;
            return false;
        }
        bak_path.replace_extension("");
    }

    // 解压
    if (info.mod & BACKUP_MOD_COMPRESS)
    {
        if (verbose)
            std::cout << "DECOMPRESSING..." << std::endl;
        
        Compressor compressor(bak_path);
        if (!compressor.Decompress())
        {
            std::cout << "error: failed to decompress file" << std::endl;
            return false;
        }
        if (info.mod & BACKUP_MOD_ENCRYPT)
            std::filesystem::remove_all(bak_path);
        bak_path.replace_extension("");
    }

    // 解包
    if (verbose)
        std::cout << "UNPACKING..." << std::endl;
    Packer packer(src_path, bak_path, filter, verbose);
    if (!packer.Unpack(restore_metadata))
    {
        std::cout << "error: failed to unpack file" << std::endl;
        return false;
    }
    if (info.mod & BACKUP_MOD_COMPRESS)
        std::filesystem::remove_all(bak_path);

    return true;
}

bool Task::GetBackupInfo()
{
    return Task::GetBackupInfo(this->bak_path, this->info);
}

unsigned char Task::GetBackupMode()
{
    return info.mod;
}

bool Task::GetBackupInfo(std::string file_path_, BackupInfo &info_)
{
    FileBase file(file_path_);
    if (file.OpenFile(std::ios::in | std::ios::binary))
    {
        info_ = file.ReadBackupInfo();

        bool status = true;
        if(info_.CalcChecksum() != 0)   // 文件校验失败
        {
            std::cout << "error: invalid file: " << file_path_ << std::endl;
            status = false;
        }

        file.close();
        return status;
    }
    std::cout << "error: failed to open file: " << file_path_ << std::endl;
    return false;
}