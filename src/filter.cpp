#include "filter.h"

Filter::Filter()
{
    type = 0;
}

Filter::Filter(const Filter &f) : type(f.type), reg_path(f.reg_path),
                                  reg_name(f.reg_name), file_type(f.file_type),
                                  atim(f.atim), mtim(f.mtim), ctim(f.ctim)
{
}

Filter::~Filter()
{
}

void Filter::SetPathFilter(std::string reg_path_)
{
    reg_path = reg_path_;
}
void Filter::SetNameFilter(std::string reg_name_)
{
    reg_name = reg_name_;
}
void Filter::SetFileType(FileType file_type_)
{
    file_type = file_type_;
}
void Filter::SetAccessTime(std::string atim_)
{
    atim = atim_;
}
void Filter::SetModifyTime(std::string mtim_)
{
    mtim = mtim_;
}
void Filter::SetChangeTime(std::string ctim_)
{
    ctim = ctim_;
}

bool Filter::check(const FileHeader &file_header_)
{
    return true;
}