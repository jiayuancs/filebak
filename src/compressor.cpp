#include "compressor.h"

Compressor::Compressor(std::string file_path_)
{
    memset(freq, 0, sizeof(freq));
    this->file_path.assign(file_path_);
    huff_root = nullptr;
}

Compressor::~Compressor()
{
    CleanHuffmanTree(huff_root);
}

void Compressor::BuildHuffmanTree()
{
    // 小根堆
    std::priority_queue<HuffNode *, std::vector<HuffNode *>, HuffNodeCmp> freq_heap;
    for (int i = 0; i < 256; ++i)
    {
        HuffNode *huff_node = new HuffNode;
        huff_node->freq = freq[i];
        huff_node->uchar = (unsigned char)i;
        huff_node->left = nullptr;
        huff_node->right = nullptr;
        freq_heap.push(huff_node);
    }

    // 构建Huffman树
    while (freq_heap.size() > 1)
    {
        HuffNode *left = freq_heap.top();
        freq_heap.pop();
        HuffNode *right = freq_heap.top();
        freq_heap.pop();

        HuffNode *root = new HuffNode;
        root->left = left;
        root->right = right;
        root->freq = left->freq + right->freq;
        freq_heap.push(root);
    }
    huff_root = freq_heap.top();
    freq_heap.pop();
}

void Compressor::Encode(HuffNode *root, std::string code)
{
    root->code = code;
    // 叶子节点
    if (root->left == nullptr && root->right == nullptr)
    {
        code_map[root->uchar] = root->code;
        return;
    }
    if (root->left)
        Encode(root->left, code + "0");
    if (root->right)
        Encode(root->right, code + "1");
}

void Compressor::CleanHuffmanTree(HuffNode *root)
{
    if (!root)
        return;
    if (root->left)
        CleanHuffmanTree(root->left);
    if (root->right)
        CleanHuffmanTree(root->right);
    delete root;
    root = nullptr;
}

bool Compressor::Compress()
{
    // 待压缩的文件
    FileBase file_in(file_path);
    if (!file_in.OpenFile(std::ios::in | std::ios::binary))
        return false;
    BackupInfo info = file_in.ReadBackupInfo();

    // 压缩后的文件
    std::filesystem::path cps_path(file_path);
    cps_path += FILE_SUFFIX_COMPRESS;
    FileBase file_out(cps_path);
    if (!file_out.OpenFile(std::ios::out | std::ios::binary | std::ios::trunc))
        return false;
    file_out.WriteBackupInfo(info);

    // 统计词频
    memset(freq, 0, sizeof(freq));
    unsigned char buf_in[BLOCK_BUFFER_SIZE];
    while (file_in.read((char *)&buf_in, sizeof(buf_in)))
    {
        for (int i = 0; i < BLOCK_BUFFER_SIZE; ++i)
        {
            freq[buf_in[i]]++;
        }
    }

    for (int i = 0; i < file_in.gcount(); ++i)
    {
        freq[buf_in[i]]++;
    }

    // 构建Huffman树,得到Huffman编码
    BuildHuffmanTree();
    code_map.clear();
    Encode(huff_root, "");

    // 写入补0的个数(先填0，后续更改)
    unsigned char padding_size = 0;
    file_out.write((const char *)&padding_size, sizeof(padding_size));

    // 写入词频表
    file_out.write((const char *)freq, sizeof(freq));

    // 写入编码后的数据
    file_in.clear();
    file_in.seekg(sizeof(info), std::ios::beg); // 跳过备份信息头
    std::string buf = "";

    unsigned char buf_out[BLOCK_BUFFER_SIZE];
    unsigned char uchar = 0;
    while (file_in.read((char *)&buf_in, sizeof(buf_in)))
    {
        for (int i = 0; i < BLOCK_BUFFER_SIZE; ++i)
        {
            uchar  = buf_in[i];
            buf += code_map[uchar];
            while (buf.length() >= 8)
            {
                std::bitset<8> bs(buf.substr(0, 8));
                uchar = bs.to_ullong();
                file_out.write((const char *)&uchar, sizeof(uchar));
                buf = buf.substr(8);
            }
        }
    }

    for (int i = 0; i < file_in.gcount(); ++i)
    {
        uchar  = buf_in[i];
        buf += code_map[uchar];
        while (buf.length() >= 8)
        {
            std::bitset<8> bs(buf.substr(0, 8));
            uchar = bs.to_ulong();
            file_out.write((const char *)&uchar, sizeof(uchar));
            buf = buf.substr(8);
        }
    }

    // 文件末尾补0，使文件大小是字节的整数倍
    padding_size = (8 - buf.length()) % 8;
    if (padding_size)
    {
        for (int i = 0; i < padding_size; i++)
        {
            buf += "0";
        }
        std::bitset<8> bs(buf.substr(0, 8));
        uchar = bs.to_ulong();
        file_out.write((const char *)&uchar, sizeof(uchar));
    }

    // 记录补0的个数
    file_out.clear();
    file_out.seekp(sizeof(info), std::ios::beg); // 跳过备份信息头
    file_out.write((const char *)&padding_size, sizeof(padding_size));

    file_out.close();
    file_in.close();

    return true;
}

bool Compressor::Decompress()
{
    if (file_path.extension() != FILE_SUFFIX_COMPRESS)
        return false;

    // 待解压的文件
    FileBase file_in(file_path);
    if (!file_in.OpenFile(std::ios::in | std::ios::binary))
        return false;
    BackupInfo info = file_in.ReadBackupInfo();

    // 解压后的文件
    std::filesystem::path bak_path(file_path);
    bak_path.replace_extension("");
    FileBase file_out(bak_path);
    if (!file_out.OpenFile(std::ios::out | std::ios::binary | std::ios::trunc))
        return false;
    file_out.WriteBackupInfo(info);

    // 读取补0个数
    unsigned char padding_size = 0;
    file_in.read((char *)&padding_size, sizeof(padding_size));

    // 读取词频表
    file_in.read((char *)&freq, sizeof(freq));

    // 构建Huffman树
    BuildHuffmanTree();

    // 解码
    HuffNode *cur_node = huff_root;
    unsigned char uchar;
    std::string pre_buf, cur_buf; // 上一个字节 当前字节
    file_in.read((char *)&uchar, sizeof(uchar));
    std::bitset<8> bs = uchar;
    cur_buf = bs.to_string();
    while (file_in.read((char *)&uchar, sizeof(uchar)))
    {
        std::bitset<8> bs = uchar;
        pre_buf = cur_buf;
        cur_buf = bs.to_string();

        // 对上一个字节进行解码
        for (int i = 0; i < 8; ++i)
        {
            if (pre_buf[i] == '0')
            {
                cur_node = cur_node->left;
            }
            else if (pre_buf[i] == '1')
            {
                cur_node = cur_node->right;
            }
            // 到达叶子节点
            if (cur_node->left == nullptr && cur_node->right == nullptr)
            {
                file_out.write((const char *)&(cur_node->uchar), sizeof(cur_node->uchar));
                cur_node = huff_root;
            }
        }
    }
    // 对最后一个字节进行解码
    int valid_size = (8 - padding_size) % 8;
    for (int i = 0; i < valid_size; ++i)
    {
        if (cur_buf[i] == '0')
        {
            cur_node = cur_node->left;
        }
        else if (cur_buf[i] == '1')
        {
            cur_node = cur_node->right;
        }
        // 到达叶子节点
        if (cur_node->left == nullptr && cur_node->right == nullptr)
        {
            file_out.write((const char *)&(cur_node->uchar), sizeof(cur_node->uchar));
            cur_node = huff_root;
        }
    }

    file_in.close();
    file_out.close();

    return true;
}