#include"argparser.h"

ArgParser::ArgParser():options("filebak", "A data backup software for Linux")
{
    options.add_options()
    ("b,backup", "备份", cxxopts::value(flag_backup))
    ("r,restore", "恢复", cxxopts::value(flag_restore))
    ("l,list", "查看指定备份文件的信息", cxxopts::value(str_list))
    ("v,verbose", "输出执行过程信息", cxxopts::value(flag_verbose))
    ("i,input", "输入", cxxopts::value(str_input))
    ("o,output", "输出", cxxopts::value(str_output))
    ("p,password", "密码", cxxopts::value(str_password))
    ("h,help", "Print usage", cxxopts::value(flag_help));

    options.add_options("Backup")
    ("c,compress", "压缩", cxxopts::value(flag_compress))
    ("e,encrypt", "加密", cxxopts::value(flag_encrypt))
    ("path", "过滤路径", cxxopts::value(str_path))
    ("type", "过滤文件类型", cxxopts::value(str_type))
    ("name", "过滤文件名", cxxopts::value(str_name))
    ("atime", "访问时间", cxxopts::value(str_atime))
    ("mtime", "修改时间", cxxopts::value(str_mtime))
    ("ctime", "改变时间", cxxopts::value(str_ctime))
    ("m,message", "备份说明", cxxopts::value(str_message));

    options.add_options("Restore")
    ("a,metadata", "恢复元数据", cxxopts::value(flag_metadata));
}

ArgParser::~ArgParser()
{
    
}

bool ArgParser::Parse(int argc, char **argv)
{
    try{
        result = options.parse(argc, argv);
    }
    catch(cxxopts::OptionException e)
    {
        std::cout<<e.what()<<std::endl;
        return false;
    }

    flag_list = result.count("list");
    flag_input = result.count("input");
    flag_output = result.count("output");
    flag_password = result.count("password");
    flag_path = result.count("path");
    flag_type = result.count("type");
    flag_name = result.count("name");
    flag_atime = result.count("atime");
    flag_mtime = result.count("mtime");
    flag_ctime = result.count("ctime");
    flag_message = result.count("message");

    if (flag_help)
    {
        std::cout << options.help() << std::endl;
        return true;
    }

    // 判断参数格式是否合法
    bool flag_args = true;
    if(flag_backup + flag_restore + flag_list != 1)
    {
        std::cout<<"只能选择一个: backup restore list"<<std::endl;
        return false;
    }
    if(flag_backup) flag_args = CheckArgsBackup();
    if(flag_restore) flag_args = CheckArgsRestore();
    if(flag_list) flag_args = CheckArgsList();
    
    return flag_args;
}

void ArgParser::PrintErrorInfo()
{
    std::cout << "Try 'filebak --help' for more information." << std::endl;
}


bool ArgParser::CheckArgsBackup()
{
    if (!flag_input || !flag_output)
    {
        std::cout << "缺少输入输出: input output" << std::endl;
        return false;
    }
    if (!flag_encrypt && flag_password)
    {
        std::cout << "如果想加密, 请使用encrypt" << std::endl;
        return false;
    }
    return true;
}
bool ArgParser::CheckArgsRestore()
{
    if (!flag_input)
    {
        std::cout << "缺少输入: input" << std::endl;
        return false;
    }
    if (flag_compress || flag_encrypt || flag_path || flag_type || flag_name 
    || flag_atime || flag_ctime || flag_mtime || flag_message)
    {
        return false;
    }
    return true;
}
bool ArgParser::CheckArgsList()
{
    if (flag_compress || flag_encrypt || flag_path || flag_type || flag_name 
    || flag_atime || flag_ctime || flag_mtime || flag_message || flag_metadata 
    || flag_input || flag_output || flag_password)
    {
        std::cout<<"格式错误"<<std::endl;
        return false;
    }
    return true;
}
