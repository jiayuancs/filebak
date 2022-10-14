#include "filebase.h"

FileBase::FileBase(FileHeader fileheader_)
{
    this->fileheader = fileheader_;
    filepath.assign(fileheader.name);
    flag_no_fileheader = false;

    if (IsHardLink())
    {
        std::filesystem::remove_all(fileheader.name);
        std::filesystem::create_hard_link(fileheader.linkname, fileheader.name);
        return;
    }

    // 创建多级目录
    switch (GetFileType())
    {
    case FILE_TYPE_NORMAL:
        // 创建父目录
        std::filesystem::create_directories(filepath.parent_path());
        break;
    case FILE_TYPE_DIRECTORY:
        std::filesystem::create_directories(filepath.string());
        break;
    case FILE_TYPE_SYMBOLIC_LINK:
        // 需要先删除文件
        std::filesystem::remove_all(fileheader.name);
        std::filesystem::create_symlink(fileheader.linkname, fileheader.name);
        break;
    case FILE_TYPE_FIFO:
        // 需要先删除文件
        std::filesystem::remove_all(fileheader.name);
        mkfifo(fileheader.name, fileheader.metadata.st_mode);
        break;
    default:
        break;
    }
}

FileBase::FileBase(std::filesystem::path filepath_)
{
    filepath = filepath_;
    flag_no_fileheader = true;

    memset(&fileheader, 0, sizeof(fileheader));

    // 获取文件元信息
    lstat(filepath.string().c_str(), &(fileheader.metadata));
    strcpy(fileheader.name, filepath.string().c_str());

    if (GetFileType() == FILE_TYPE_SYMBOLIC_LINK)
    {
        char link_buf[MAX_PACK_PATH_LEN] = {0};
        readlink(filepath.string().c_str(), link_buf, MAX_PACK_PATH_LEN);
        strcpy(fileheader.linkname, link_buf);
    }
}

FileBase::~FileBase()
{
}

bool FileBase::OpenFile(std::ios_base::openmode mode_)
{
    this->open(filepath.string(), mode_);

    return is_open();
}

FileHeader FileBase::ReadFileHeader()
{
    this->read((char *)&fileheader, FILE_HEADER_SIZE);
    return fileheader;
}

BackupInfo FileBase::ReadBackupInfo()
{
    BackupInfo info;
    this->read((char *)&info, sizeof(info));
    return info;
}
void FileBase::WriteBackupInfo(BackupInfo info)
{
    this->write((char *)&info, sizeof(info));
}
void FileBase::WriteBackupInfo()
{
    BackupInfo info = {0};
    this->write((char *)&info, sizeof(info));
}
size_t FileBase::GetFileSize()
{
    return fileheader.metadata.st_size;
}

FileType FileBase::GetFileType()
{
    return FileBase::GetFileType(fileheader);
}

bool FileBase::IsHardLink()
{
    return (fileheader.metadata.st_nlink > 1);
}

FileHeader FileBase::GetFileHeader()
{
    return fileheader;
}

void FileBase::ReatoreMetadata()
{
    struct stat *metadata = &(fileheader.metadata);

    // 还原文件权限信息
    chmod(fileheader.name, metadata->st_mode);

    // 还原文件的用户和组
    lchown(fileheader.name, metadata->st_uid, metadata->st_gid);

    // 还原时间戳(访问时间 修改时间)
    timespec tim[2] = {metadata->st_atim, metadata->st_mtim};
    utimensat(AT_FDCWD, fileheader.name, tim, AT_SYMLINK_NOFOLLOW);
}

FileType FileBase::GetFileType(const FileHeader &file_header_)
{
    switch (file_header_.metadata.st_mode & S_IFMT)
    {
    case S_IFREG:
        return FILE_TYPE_NORMAL;
        break;
    case S_IFDIR:
        return FILE_TYPE_DIRECTORY;
        break;
    case S_IFLNK:
        return FILE_TYPE_SYMBOLIC_LINK;
        break;
    case S_IFIFO:
        return FILE_TYPE_FIFO;
        break;
    default:
        return FILE_TYPE_OTHER;
        break;
    }
}