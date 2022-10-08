#ifndef INCLUDE_COMPRESSOR_H_
#define INCLUDE_COMPRESSOR_H_

#include "filebase.h"
#include <utility>
#include <unordered_map>

#define ull unsigned long long
#define FILE_SUFFIX_COMPRESS ".cps"

struct haffNode
{
    ull freq;
    unsigned char uchar;
    std::string code;
    struct haffNode *left;
    struct haffNode *right;
};

class Compressor
{
private:
    ull freq[256];                                           // 词频表
    std::unordered_map<unsigned char, std::string> code_map; // 编码表
    char buf[BLOCK_BUFFER_SIZE];                             // 读写文件缓冲区
    std::filesystem::path file_path;                         // 文件路径

    // 根据词频表计算Huffman编码
    void CalcHuffmanCode();

public:
    Compressor(std::string file_path_);
    ~Compressor();

    bool Compress();
    bool Decompress();
};

#endif // INCLUDE_COMPRESSOR_H_