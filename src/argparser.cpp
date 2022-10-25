#include "argparser.h"

ArgParser::ArgParser() : options("filebak", "A data backup software for Linux")
{
    options.set_width(ARG_HELP_WIDTH).add_options()
    ("b,backup", "恢复", cxxopts::value(flag_backup))
    ("r,restore", "恢复", cxxopts::value(flag_restore))
    ("l,list", "查看指定备份文件的信息", cxxopts::value(str_list))
    ("v,verbose", "输出执行过程信息", cxxopts::value(flag_verbose))
    ("i,input", "备份时指出备份路径,恢复时指定备份文件名", cxxopts::value(str_input))
    ("o,output", "备份时指出备份文件名,恢复时指定输出路径", cxxopts::value(str_output))
    ("p,password", "密码", cxxopts::value(str_password))
    ("h,help", "Print usage", cxxopts::value(flag_help));

    options.set_width(ARG_HELP_WIDTH).add_options("Backup")
    ("c,compress", "压缩", cxxopts::value(flag_compress))
    ("e,encrypt", "加密", cxxopts::value(flag_encrypt))
    ("path", "过滤路径 正则表达式", cxxopts::value(str_path))
    ("type", "过滤文件类型: n普通文件,d目录文件,l符号链接,p管道文件", cxxopts::value(str_type))
    ("name", "过滤文件名 正则表达式", cxxopts::value(str_name))
    ("atime", "限制访问时间, 例\"2000-11-11 23:20:21 2022-10-11 20:10:51\"", cxxopts::value(str_atime))
    ("mtime", "限制修改时间, 格式同atime", cxxopts::value(str_mtime))
    ("ctime", "限制改变时间, 格式同atime", cxxopts::value(str_ctime))
    ("m,message", "备份说明", cxxopts::value(str_message));

    options.set_width(ARG_HELP_WIDTH).add_options("Restore")("a,metadata", "恢复元数据", cxxopts::value(flag_metadata));
}

ArgParser::~ArgParser()
{
}

bool ArgParser::Parse(int argc, char **argv)
{
    try
    {
        result = options.parse(argc, argv);
    }
    catch (cxxopts::OptionException e)
    {
        std::cout << e.what() << std::endl;
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
    bool format_correct = true;
    if (flag_backup + flag_restore + flag_list != 1)
    {
        std::cout << "error: invalid parameter" << std::endl;
        return false;
    }
    if (flag_backup)
        format_correct &= CheckArgsBackup();
    if (flag_restore)
        format_correct &= CheckArgsRestore();
    if (flag_list)
        format_correct &= CheckArgsList();
    if (flag_atime)
        format_correct &= ConvertTime(str_atime, atime_start, atime_end);
    if (flag_mtime)
        format_correct &= ConvertTime(str_mtime, mtime_start, mtime_end);
    if (flag_ctime)
        format_correct &= ConvertTime(str_ctime, ctime_start, ctime_end);
    if (flag_backup && flag_password)
        format_correct &= CheckPassword();
    format_correct &= CheckRegExp();
    if (!format_correct)
        return false;

    // 文件类型： n普通文件,d目录文件,l符号链接,p管道文件
    if (flag_type)
    {
        file_type = 0;
        if (str_type.find("n") != std::string::npos)
            file_type |= FILE_TYPE_NORMAL;
        if (str_type.find("d") != std::string::npos)
            file_type |= FILE_TYPE_DIRECTORY;
        if (str_type.find("l") != std::string::npos)
            file_type |= FILE_TYPE_SYMBOLIC_LINK;
        if (str_type.find("p") != std::string::npos)
            file_type |= FILE_TYPE_FIFO;
    }

    return true;
}

void ArgParser::PrintErrorInfo()
{
    std::cout << "Try 'filebak --help' for more information." << std::endl;
}

bool ArgParser::CheckPassword()
{
    if (str_password.length() < 6)
    {
        std::cout << "error: password must be at least 6 characters in length" << std::endl;
        return false;
    }
    return true;
}

bool ArgParser::CheckRegExp()
{
    try
    {
        if (flag_path)
            std::regex reg(str_path);
        if (flag_name)
            std::regex reg(str_name);
    }
    catch (const std::exception &e)
    {
        std::cout << "error: invalid regular expression: ";
        std::cout << e.what() << std::endl;
        return false;
    }
    return true;
}

bool ArgParser::CheckArgsBackup()
{
    if (!flag_input || !flag_output)
    {
        std::cout << "error: no input and output" << std::endl;
        return false;
    }
    if (!flag_encrypt && flag_password)
    {
        std::cout << "error: invalid parameter(using -e to encrypt)" << std::endl;
        return false;
    }
    return true;
}

bool ArgParser::CheckArgsRestore()
{
    if (!flag_input)
    {
        std::cout << "error: no input" << std::endl;
        return false;
    }
    if (flag_compress || flag_encrypt || flag_path || flag_type || flag_name || flag_atime || flag_ctime || flag_mtime || flag_message)
    {
        std::cout << "error: invalid parameter" << std::endl;
        return false;
    }
    return true;
}

bool ArgParser::CheckArgsList()
{
    if (flag_compress || flag_encrypt || flag_path || flag_type || flag_name || flag_atime || flag_ctime || flag_mtime || flag_message || flag_metadata || flag_input || flag_output || flag_password)
    {
        std::cout << "error: invalid parameter" << std::endl;
        return false;
    }
    return true;
}

bool ArgParser::ConvertTime(std::string str_time, time_t &time_start, time_t &time_end)
{
    // 格式: 2000-11-11 23:20:21 2022-10-11 20:10:51
    std::string time_format("(\\d{1,4}-(?:1[0-2]|0?[1-9])-(?:0?[1-9]|[1-2]\\d|30|31)) ((?:[01]\\d|2[0-3]):[0-5]\\d:[0-5]\\d)");
    std::regex reg("^" + time_format + " +" + time_format + "$");
    if (!std::regex_match(str_time, reg))
    {
        std::cout << "error: invalid time: " << str_time << std::endl;
        return false;
    }
    reg.assign("(\\d{1,4}-(?:1[0-2]|0?[1-9])-(?:0?[1-9]|[1-2]\\d|30|31)) ((?:[01]\\d|2[0-3]):[0-5]\\d:[0-5]\\d)");
    std::sregex_iterator it(str_time.begin(), str_time.end(), reg);
    time_start = StrToTime(it->str());
    ++it;
    time_end = StrToTime(it->str());
    return true;
}

time_t ArgParser::StrToTime(std::string str_time)
{
    char *cha = (char *)str_time.data();
    tm tm_;
    int year, month, day, hour, minute, second;
    sscanf(cha, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
    tm_.tm_year = year - 1900; // 年
    tm_.tm_mon = month - 1;    // 月
    tm_.tm_mday = day;         // 日
    tm_.tm_hour = hour;        // 时
    tm_.tm_min = minute;       // 分
    tm_.tm_sec = second;       // 秒
    tm_.tm_isdst = 0;          // 非夏令时
    time_t t_ = mktime(&tm_);  // 将tm结构体转换成time_t格式
    return t_;
}