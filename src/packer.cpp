#include "packer.h"

Packer::Packer(std::string root_path_, std::string pack_path_, const Filter &filter_, bool verbose_)
{
    root_path.assign(root_path_);
    bak_path.assign(pack_path_);
    filter = filter_;
    verbose = verbose_;
}

Packer::~Packer()
{
}

void Packer::DfsFile(FileBase &bak_file, std::filesystem::path cur_path)
{
    FileBase file(cur_path);
    FileHeader fileheader = file.GetFileHeader();

    // 判断该文件是否满足用户自定义的过滤规则
    bool file_status = filter.Check(fileheader);

    // 输出遍历到的所有文件路径
    if (verbose)
    {
        std::cout << cur_path.string();
        if (file_status)
            std::cout << "  True" << std::endl;
        else
            std::cout << "  False" << std::endl;
    }

    if (!file_status)
    {
        if (file.GetFileType() == FILE_TYPE_DIRECTORY)
        {
            for (const auto &entry : std::filesystem::directory_iterator(cur_path))
            {
                DfsFile(bak_file, entry.path());
            }
        }
        return;
    }

    // 处理硬链接
    if (file.IsHardLink())
    {
        if (inode_table.count(fileheader.metadata.st_ino))
        {
            // 指向的inode已打包,记录文件路径
            strcpy(fileheader.linkname, inode_table[fileheader.metadata.st_ino].c_str());
            bak_file.write((const char *)&fileheader, sizeof(fileheader));
            return;
        }
        else
        {
            // 指向的inode未打包,作为常规文件处理
            fileheader.metadata.st_nlink = 1;
            inode_table[fileheader.metadata.st_ino] = cur_path.string();
        }
    }

    char buf[BLOCK_BUFFER_SIZE] = {0};
    switch (file.GetFileType())
    {
    case FILE_TYPE_DIRECTORY:
        bak_file.write((const char *)&fileheader, sizeof(fileheader));
        for (const auto &entry : std::filesystem::directory_iterator(cur_path))
        {
            DfsFile(bak_file, entry.path());
        }
        break;
    case FILE_TYPE_NORMAL:
        bak_file.write((const char *)&fileheader, sizeof(fileheader));

        file.OpenFile(std::ios::in | std::ios::binary);

        while (file.peek() != EOF)
        {
            file.read(buf, sizeof(buf));
            bak_file.write(buf, file.gcount());
        }
        file.close();
        break;
    case FILE_TYPE_SYMBOLIC_LINK:
        bak_file.write((const char *)&fileheader, sizeof(fileheader));
        break;
    case FILE_TYPE_FIFO:
        bak_file.write((const char *)&fileheader, sizeof(fileheader));
        break;
    default:
        break;
    }
}

bool Packer::Pack()
{
    bak_path += FILE_SUFFIX_PACK;
    FileHeader bak_file_header = {0};
    strcpy(bak_file_header.name, bak_path.string().c_str());
    // 普通文件
    bak_file_header.metadata.st_mode = S_IFREG;
    bak_file_header.metadata.st_nlink = 1;

    // 创建打包文件
    FileBase bak_file(bak_file_header);
    if (!bak_file.OpenFile(std::ios::out | std::ios::binary | std::ios::trunc))
        return false;

    bak_file.WriteBackupInfo();

    // 切换工作目录
    std::filesystem::current_path(root_path.parent_path());

    // 深度优先遍历目录树
    DfsFile(bak_file, std::filesystem::relative(root_path, root_path.parent_path()));

    bak_file.close();
    return true;
}

bool Packer::Unpack(bool restore_metadata)
{
    if (bak_path.extension() != FILE_SUFFIX_PACK)
        return false;

    FileBase bak_file(bak_path);
    bak_file.OpenFile(std::ios::in | std::ios::binary);

    BackupInfo info = bak_file.ReadBackupInfo();


    // 使用指定路径root_path
    std::filesystem::create_directories(root_path);
    std::filesystem::current_path(root_path);


    char buf[BLOCK_BUFFER_SIZE] = {0};
    FileHeader fileheader = {0};
    while (bak_file.peek() != EOF)
    {
        // 读文件头
        fileheader = bak_file.ReadFileHeader();

        if (verbose)
            std::cout << fileheader.name << std::endl;

        FileBase file(fileheader);

        // 只有普通文件需要复制文件内容到新文件中
        if (file.GetFileType() == FILE_TYPE_NORMAL && !file.IsHardLink())
        {
            file.OpenFile(std::ios::out | std::ios::binary | std::ios::trunc);
            size_t file_size = file.GetFileSize();
            while (file_size >= BLOCK_BUFFER_SIZE)
            {
                bak_file.read(buf, BLOCK_BUFFER_SIZE);
                file.write(buf, BLOCK_BUFFER_SIZE);
                file_size -= BLOCK_BUFFER_SIZE;
            }
            if (file_size)
            {
                bak_file.read(buf, file_size);
                file.write(buf, file_size);
            }
            file.close();
        }

        // 恢复文件时间戳
        if (restore_metadata)
        {
            file.ReatoreMetadata();
        }
    }

    bak_file.close();
    return true;
}