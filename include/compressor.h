#ifndef INCLUDE_COMPRESSOR_H_
#define INCLUDE_COMPRESSOR_H_

#include "filebase.h"
#include <utility>
#include <unordered_map>
#include <bitset>
#include <queue>

#define ull unsigned long long
#define FILE_SUFFIX_COMPRESS ".cps"

struct HuffNode
{
    ull freq;
    unsigned char uchar;
    std::string code;
    struct HuffNode *left;
    struct HuffNode *right;
};

// priority_queue的比较函数 构造小根堆
struct HuffNodeCmp
{
    bool operator()(const HuffNode *a, const HuffNode *b)
    {
        return a->freq > b->freq;
    }
};

class Compressor
{
private:
    ull freq[256];                                           // 词频表
    std::unordered_map<unsigned char, std::string> code_map; // 编码表
    std::filesystem::path file_path;                         // 文件路径
    HuffNode *huff_root;

    // 根据词频表计算Huffman编码
    void BuildHuffmanTree();
    void Encode(HuffNode *root, std::string code);

    // 释放内存
    void CleanHuffmanTree(HuffNode *root);

public:
    Compressor(std::string file_path_);
    ~Compressor();

    bool Compress();
    bool Decompress();
};

#endif // INCLUDE_COMPRESSOR_H_