#include "aes.h"

using namespace std;

int main()
{
    cout << "Test AES" << endl;
    // Aes aes("hello.txt", "123456");
    // aes.Encrypt();

    Aes aes("hello.txt.ept", "123456");
    aes.Decrypt();
    return 0;
}