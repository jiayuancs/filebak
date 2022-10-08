#include "compressor.h"

Compressor::Compressor(std::string file_path_)
{
    this->file_path.assign(file_path_);
}

Compressor::~Compressor()
{
}

void Compressor::CalcHuffmanCode()
{
}

bool Compressor::Compress()
{
    // 待压缩的文件
    FileBase file_bak(file_path);
    if (!file_bak.OpenFile(std::ios::in | std::ios::binary))
        return false;

    // 压缩后的文件
    std::filesystem::path cps_path(file_path);
    cps_path += FILE_SUFFIX_COMPRESS;
    FileBase file_cps(cps_path);
    if (!file_cps.OpenFile(std::ios::out | std::ios::binary | std::ios::trunc))
        return false;

    // 统计词频

    // 构建Huffman树,得到Huffman编码
    CalcHuffmanCode();

    // 写入补0的个数

    // 写入词频表

    // 写入编码后的数据

    file_cps.close();
    file_bak.close();

    return true;
}

bool Compressor::Decompress()
{
    if (file_path.extension() != FILE_SUFFIX_COMPRESS)
        return false;

    // 待解压的文件
    FileBase file_cps(file_path);
    if (!file_cps.OpenFile(std::ios::in | std::ios::binary))
        return false;

    // 解压后的文件
    std::filesystem::path bak_path(file_path);
    bak_path.replace_extension("");
    FileBase file_bak(bak_path);
    if (!file_bak.OpenFile(std::ios::out | std::ios::binary | std::ios::trunc))
        return false;

    // 读取补0个数

    // 读取词频表

    // 构建Huffman树,得到Huffman编码
    CalcHuffmanCode();

    // 写入解码后的数据

    file_cps.close();
    file_bak.close();

    return true;
}