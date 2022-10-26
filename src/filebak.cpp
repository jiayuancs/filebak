#include "task.h"
#include "argparser.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: filebak [OPTION...]" << std::endl;
        std::cout << "Try 'filebak --help' for more information." << std::endl;
        return -1;
    }

    ArgParser parser;
    if (!parser.Parse(argc, argv))
    {
        parser.PrintErrorInfo();
        return -1;
    }
    if (parser.flag_help)
        return 0;

    // 备份
    if (parser.flag_backup)
    {
        Task task(parser.str_input, parser.str_output);

        unsigned char mod = 0;
        if (parser.flag_compress)
            mod |= BACKUP_MOD_COMPRESS;
        if (parser.flag_encrypt)
        {
            mod |= BACKUP_MOD_ENCRYPT;
            if (!parser.flag_password)
            {
                std::string pwd1, pwd2;
                std::cout << "Input password: ";
                std::cin >> pwd1;
                std::cout << "Input password again: ";
                std::cin >> pwd2;
                if (pwd1 != pwd2)
                {
                    std::cout << "error: passwords do not match" << std::endl;
                    return -1;
                }

                parser.flag_password = true;
                parser.str_password = pwd2;

                if (!parser.CheckPassword())
                    return -1;
            }
        }

        task.SetMod(mod);
        task.SetVerbose(parser.flag_verbose);
        task.SetComment(parser.str_message);

        Filter filter;
        if (parser.flag_path)
            filter.SetPathFilter(parser.str_path);
        if (parser.flag_name)
            filter.SetNameFilter(parser.str_name);
        if (parser.flag_type)
            filter.SetFileType(parser.file_type);
        if (parser.flag_atime)
            filter.SetAccessTime(parser.atime_start, parser.atime_end);
        if (parser.flag_mtime)
            filter.SetModifyTime(parser.mtime_start, parser.mtime_end);
        if (parser.flag_ctime)
            filter.SetChangeTime(parser.ctime_start, parser.ctime_end);
        task.SetFilter(filter);

        if (!task.Backup(parser.str_password))
            return -1;
        else
            std::cout<<"Done"<<std::endl;
    }

    // 恢复
    if (parser.flag_restore)
    {
        Task task(parser.str_output, parser.str_input);
        if (!task.GetBackupInfo())
            return -1;
        if ((task.GetBackupMode() & BACKUP_MOD_ENCRYPT) && !parser.flag_password)
        {
            std::cout << "Input password: ";
            std::cin >> parser.str_password;
        }
        task.RestoreMetadata(parser.flag_metadata);
        task.SetVerbose(parser.flag_verbose);
        if (!task.Restore(parser.str_password))
            return -1;
        else
            std::cout<<"Done"<<std::endl;
    }

    // 显示备份文件信息
    if (parser.flag_list)
    {
        BackupInfo info;
        if (Task::GetBackupInfo(parser.str_list, info))
        {
            std::string backup_mode = "pack ";
            if (info.mod & BACKUP_MOD_COMPRESS)
                backup_mode += "compress ";
            if (info.mod & BACKUP_MOD_ENCRYPT)
                backup_mode += "encrypt";
            std::cout << "Backup path: " << info.backup_path << std::endl;
            char buf_time[100];
            strftime(buf_time, sizeof(buf_time), "%Y-%m-%d %H:%M:%S", localtime(&info.timestamp));
            std::cout << "Backup Time: " << buf_time << std::endl;
            std::cout << "Backup Mode: " << backup_mode << std::endl;
            std::cout << "Comment: " << info.comment << std::endl;
        }
        else
        {
            return -1;
        }
    }

    return 0;
}