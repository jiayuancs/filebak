#include"packer.h"
using namespace std;

int main(){

    string root_path = "/home/leo/code/vscode/filebak/test";
    string bak_path = "/home/leo/code/vscode/filebak/a.bak";
    Packer packer(root_path, bak_path);

    packer.Pack();
    // packer.Unpack();

    return 0;
}