#include "compressor.h"
#include <iostream>
using namespace std;

int main()
{

    Compressor compressor("./template.tar");
    compressor.Compress();

    // Compressor compressor("./template.tar.cps");
    // compressor.Decompress();
    return 0;
}