#include "filebase.h"
using namespace std;

int main()
{

    FileBase file("/home/leo/code/vscode/filebak/test/hello.txt");
    file.OpenFile(ios::in | ios::binary);

    char buf[16];
    while (file.read(buf, 16))
    {
        cout << file.gcount() << endl;

        cout << buf << endl;
        // cout<<file.gcount()<<endl;
        // if(file.peek() == EOF){
        //     cout<<"EOF"<<endl;
        // }
    }
    cout << file.gcount() << endl;
    cout << buf << endl;

    file.close();
    return 0;
}