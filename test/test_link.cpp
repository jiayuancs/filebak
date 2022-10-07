#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <iostream>
using namespace std::filesystem;
using namespace std;

void printTime(string dec, const struct timespec& tim){
    cout<<dec<<tim.tv_sec<<" "<<tim.tv_nsec<<endl;
}
void printStat(const struct stat& header){
    cout<<"文件类别: "<<header.st_mode<<endl;
    cout<<"inode: "<<header.st_ino<<endl;
    cout<<"设备号码: "<<header.st_dev<<endl;
    cout<<"特殊设备号: "<<header.st_rdev<<endl;
    cout<<"文件连接数: "<<header.st_nlink<<endl;
    cout<<"文件所有者: "<<header.st_uid<<endl;
    cout<<"文件所有者对应的组: "<<header.st_gid<<endl;
    cout<<"文件对应的文件字节数: "<<header.st_size<<endl;   // 软连接即连接路径字符所占用的空间
    cout<<"IO一次读入的块大小: "<<header.st_blksize<<endl;  // 可用于设置缓冲区大小
    cout<<"对应的512字节块数量: "<<header.st_blocks<<endl;  // 不需要
    printTime("上次访问:", header.st_atim);
    printTime("上次修改:", header.st_mtim);
    printTime("创建时间:", header.st_ctim);
}

int main(){

    std::string hardpath = "hard_link";
    std::string softpath = "soft_link";

    path hard(hardpath);
    path soft(softpath);

    cout<<is_symlink(hard)<<endl;   // 0
    cout<<is_symlink(soft)<<endl;   // 1

    char buf[1024];
    cout<<readlink(soft.string().c_str(), buf, 1024)<<endl;
    path allpath(buf);
    path base("/home/leo");
    cout<<"相对路径"<<relative(base, base.parent_path())<<endl;
    cout<<absolute(allpath)<<endl;  // 绝对路径


    struct stat header;
    lstat(hard.string().c_str(), &(header));
    printStat(header);
    cout<<endl;
    lstat(soft.string().c_str(), &(header));
    printStat(header);


    filesystem::current_path(base);
    string path_ = "..";
    for (const auto & entry : filesystem::directory_iterator(path_))
        // cout << relative(entry.path(), base) << std::endl;
        cout << entry.path() << std::endl;

    lstat("/home/leo/code/vscode/filebak/test/temple.tar", &(header));
    printStat(header);

    return 0;
}