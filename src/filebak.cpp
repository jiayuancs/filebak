#include "task.h"
#include "argparser.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "参数过少" << std::endl;
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
            mod |= BACKUP_MOD_ENCRYPT;
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

        task.Backup(parser.str_password);
    }

    // 恢复
    if (parser.flag_restore)
    {
        Task task(parser.str_output, parser.str_input);
        task.RestoreMetadata(parser.flag_metadata);
        task.UseOrignalPath(!parser.flag_output);
        task.SetVerbose(parser.flag_verbose);
        task.Restore(parser.str_password);
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
            std::cout << "Backup Time: " << info.timestamp << std::endl;
            std::cout << "Backup Mode: " << backup_mode << std::endl;
            std::cout << "Comment: " << info.comment << std::endl;
        }
        else
        {
            std::cout << "error: no such file or directory: " << parser.str_list << std::endl;
            return -1;
        }
    }

    return 0;
}