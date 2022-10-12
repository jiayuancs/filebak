#include<iostream>
#include<filesystem>
#include<regex>
using namespace std;

int main()
{
    filesystem::path fp("/./--");
    cout<<fp.filename()<<endl;
    bool ok = fp.filename()=="";
    cout<<ok<<endl;
    
    cout<<"Parent Path: "<<fp.has_parent_path()<<endl;
    cout<<"Parent Path: "<<fp.parent_path()<<endl;
    cout<<"File Name: "<<fp.has_filename()<<endl;
    cout<<"File Name: "<<fp.filename()<<endl;

    cout<<std::filesystem::absolute(fp)<<endl;

    cout<<std::filesystem::absolute("/..")<<endl;

    string str = "234lkjsdfj";
    cout<<str.back()<<endl;

    std::regex reg("^[.]*[\\w]+[\\w.-]*$");
    cout<<std::regex_match(".sdf.dfs.sd-sdf", reg)<<endl;
    cout<<std::regex_match(".", reg)<<endl;
    cout<<std::regex_match("..", reg)<<endl;
    cout<<std::regex_match("-", reg)<<endl;
    cout<<std::regex_match("-sf", reg)<<endl;
    cout<<std::regex_match("-aw", reg)<<endl;
    cout<<std::regex_match("3msma-sd.", reg)<<endl;
    cout<<std::regex_match("", reg)<<endl;


    return 0;
}