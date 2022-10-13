#include "task.h"
#include <iostream>

using namespace std;

int main()
{
    cout << "Test Task" << endl;

    Task task("/home/leo/code/vscode/test", "/home/leo/code/vscode/filebak/bakfile/test");
    task.SetComment("Test Task");
    task.Backup("123456");

    Task task2("/home/leo/code/vscode/test", "/home/leo/code/vscode/filebak/bakfile/test.pak.cps.ept");
    cout << task2.Restore("123456", true) << endl;

    return 0;
}