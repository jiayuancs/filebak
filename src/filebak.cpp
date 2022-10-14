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

    // 备份任务
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
        // //
        task.SetFilter(filter);

        task.Backup(parser.str_password);
    }
    // 恢复任务
    else if (parser.flag_restore)
    {
        Task task(parser.str_output, parser.str_input);
        task.RestoreMetadata(parser.flag_metadata);
        task.UseOrignalPath(!parser.flag_output);
        task.SetVerbose(parser.flag_verbose);
        task.Restore(parser.str_password);
    }
    // 显示备份文件信息
    else if (parser.flag_list)
    {
        BackupInfo info;
        if(Task::GetBackupInfo(parser.str_list, info)){
            // cout
        }
        else{
            // 
        }
    }
    else
    {
        parser.PrintErrorInfo();
        return -1;
    }

    return 0;
}