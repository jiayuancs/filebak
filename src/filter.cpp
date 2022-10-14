#include "filter.h"

Filter::Filter()
{
    type = 0;
}

Filter::~Filter()
{
}

void Filter::SetPathFilter(std::string reg_path_)
{
    reg_path = reg_path_;
    type |= FILTER_FILE_PATH;
}
void Filter::SetNameFilter(std::string reg_name_)
{
    reg_name = reg_name_;
    type |= FILTER_FILE_NAME;
}
void Filter::SetFileType(FileType file_type_)
{
    file_type = file_type_;
    type |= FILTER_FILE_TYPE;
}

void Filter::SetAccessTime(time_t atime_start_, time_t atime_end_)
{
    atime_start = atime_start_;
    atime_end = atime_end_;
    type |= FILTER_FILE_ACCESS_TIME;
}
void Filter::SetModifyTime(time_t mtime_start_, time_t mtime_end_)
{
    mtime_start = mtime_start_;
    mtime_end = mtime_end_;
    type |= FILTER_FILE_MODIFY_TIME;
}
void Filter::SetChangeTime(time_t ctime_start_, time_t ctime_end_)
{
    ctime_start = ctime_start_;
    ctime_end = ctime_end_;
    type |= FILTER_FILE_CHANGE_TIME;
}

bool Filter::check(const FileHeader &file_header_)
{
    FileType cur_file_type = FileBase::GetFileType(file_header_);
    // 路径匹配
    if (type & FILTER_FILE_PATH)
    {
        std::regex reg(reg_path);
        std::string dir_path = file_header_.name;
        if (cur_file_type == FILE_TYPE_DIRECTORY)
        {
            std::filesystem::path file_path(file_header_.name);
            dir_path = file_path.parent_path().string();
        }
        if (!std::regex_match(dir_path, reg))
        {
            return false;
        }
    }

    // 文件名匹配
    if ((type & FILTER_FILE_NAME) && (cur_file_type != FILE_TYPE_DIRECTORY))
    {
        std::regex reg(reg_name);
        std::filesystem::path file_path(file_header_.name);
        if (!std::regex_match(file_path.filename().string(), reg))
        {
            return false;
        }
    }

    // 类型匹配
    if ((type & FILTER_FILE_TYPE) && !(cur_file_type & file_type))
    {
        return false;
    }

    // 时间匹配
    if (type & FILTER_FILE_ACCESS_TIME)
    {
        time_t cur_file_sec = file_header_.metadata.st_atim.tv_sec;
        if (cur_file_sec < atime_start || cur_file_sec > atime_end)
        {
            return false;
        }
    }
    if (type & FILTER_FILE_MODIFY_TIME)
    {
        time_t cur_file_sec = file_header_.metadata.st_mtim.tv_sec;
        if (cur_file_sec < mtime_start || cur_file_sec > mtime_end)
        {
            return false;
        }
    }
    if (type & FILTER_FILE_CHANGE_TIME)
    {
        time_t cur_file_sec = file_header_.metadata.st_ctim.tv_sec;
        if (cur_file_sec < ctime_start || cur_file_sec > ctime_end)
        {
            return false;
        }
    }

    return true;
}