#include"task.h"

Task::Task(std::string src_path_, std::string bak_path_)
{
    src_path.assign(src_path_);
    bak_path.assign(bak_path_);
}

Task::~Task()
{

}

void Task::SetComment(std::string comment_)
{
    comment = comment_;
}