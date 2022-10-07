#include "filebase.h"
#include<iostream>

using namespace std;

int main(){

    FileBase filebase("soft_link");


    filebase.OpenFile(ios::in | ios::binary);

    filebase.printStat();

    return 0;
}