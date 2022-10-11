#include"packer.h"
using namespace std;

int main(){

    Filter filter;
    string root_path = "/home/leo/code/vscode/test_aes";
    string bak_path = "/home/leo/code/vscode/test_aes.bak";
    Packer packer(root_path, bak_path, filter);

    // packer.Pack();
    packer.Unpack();

    return 0;
}