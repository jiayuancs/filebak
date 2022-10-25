#include "aes.h"

Aes::Aes(std::string file_path_, std::string password_)
{
    file_path.assign(file_path_);
    // 使用MD5将密码转换为128位密钥
    MD5((unsigned char *)(password_.c_str()), password_.length(), key);
}

Aes::~Aes()
{
}

bool Aes::Encrypt()
{
    AES_KEY aes_key;
    AES_set_encrypt_key(key, 128, &aes_key);

    // 待加密的文件
    FileBase file_in(file_path);
    if (!file_in.OpenFile(std::ios::in | std::ios::binary))
        return false;
    BackupInfo info = file_in.ReadBackupInfo();

    char buf_in[AES_BLOCK_SIZE];
    char buf_out[AES_BLOCK_SIZE];
    unsigned char ivec[AES_BLOCK_SIZE] = {0}; // 初始化向量全0

    // 加密后的文件
    std::filesystem::path ept_path(file_path);
    ept_path += FILE_SUFFIX_ENCRYPT;
    FileBase file_out(ept_path);
    if (!file_out.OpenFile(std::ios::out | std::ios::binary | std::ios::trunc))
        return false;
    file_out.WriteBackupInfo(info);

    // 将密钥加密后写入文件
    AES_cbc_encrypt(key, (unsigned char *)buf_out, AES_BLOCK_SIZE, &aes_key, ivec, AES_ENCRYPT);
    file_out.write((const char *)buf_out, sizeof(buf_out));

    // 加密
    while (file_in.read(buf_in, AES_BLOCK_SIZE))
    {
        AES_cbc_encrypt((unsigned char *)buf_in, (unsigned char *)buf_out, AES_BLOCK_SIZE, &aes_key, ivec, AES_ENCRYPT);
        file_out.write(buf_out, AES_BLOCK_SIZE);
    }
    // 处理最后一块 补足16字节
    char delta = AES_BLOCK_SIZE - file_in.gcount();
    memset(buf_in + file_in.gcount(), delta, delta);
    AES_cbc_encrypt((unsigned char *)buf_in, (unsigned char *)buf_out, AES_BLOCK_SIZE, &aes_key, ivec, AES_ENCRYPT);
    file_out.write(buf_out, AES_BLOCK_SIZE);

    file_in.close();
    file_out.close();
    return true;
}

// 0成功 -1密码错误 -2文件错误
int Aes::Decrypt()
{
    if (file_path.extension() != FILE_SUFFIX_ENCRYPT)
        return -2;

    AES_KEY aes_key;
    AES_set_decrypt_key(key, 128, &aes_key);

    // 待解密的文件
    FileBase file_in(file_path);
    if (!file_in.OpenFile(std::ios::in | std::ios::binary))
        return -2;
    BackupInfo info = file_in.ReadBackupInfo();

    char buf_in[AES_BLOCK_SIZE];
    char buf_out[AES_BLOCK_SIZE];
    unsigned char ivec[AES_BLOCK_SIZE] = {0}; // 初始化向量全0

    // 解密前16字节,验证密码是否正确
    file_in.read(buf_in, AES_BLOCK_SIZE);
    AES_cbc_encrypt((unsigned char *)buf_in, (unsigned char *)buf_out, AES_BLOCK_SIZE, &aes_key, ivec, AES_DECRYPT);
    if (memcmp(key, buf_out, sizeof(key)) != 0)
    {
        return -1;
    }

    // 解密后的文件
    std::filesystem::path dpt_path(file_path);
    dpt_path.replace_extension("");
    FileBase file_out(dpt_path);
    if (!file_out.OpenFile(std::ios::out | std::ios::binary | std::ios::trunc))
        return -2;
    file_out.WriteBackupInfo(info);

    // 解密
    while (file_in.read(buf_in, AES_BLOCK_SIZE))
    {
        AES_cbc_encrypt((unsigned char *)buf_in, (unsigned char *)buf_out, AES_BLOCK_SIZE, &aes_key, ivec, AES_DECRYPT);

        // 最后一块需要去掉加密时补的字节
        if (file_in.peek() == EOF)
        {
            file_out.write(buf_out, AES_BLOCK_SIZE - buf_out[AES_BLOCK_SIZE - 1]);
            break;
        }
        file_out.write(buf_out, AES_BLOCK_SIZE);
    }

    file_in.close();
    file_out.close();
    return 0;
}