#include "task.h"
#include <iostream>

using namespace std;

int main()
{
    cout << "Test Task" << endl;

    // Task task("/home/leo/code/vscode/test", "/home/leo/code/vscode/filebak/bakfile/test");
    // task.SetComment("Test Task");
    // task.Backup("123456");

    Task task2("/home/leo/code/vscode/test55", "/home/leo/code/vscode/filebak/bakfile/test.pak.cps.ept");
    task2.RestoreMetadata(true);
    task2.UseOrignalPath(false);
    task2.SetVerbose(false);
    cout << task2.Restore("123456") << endl;

    return 0;
}