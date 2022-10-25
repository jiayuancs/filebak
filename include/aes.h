#ifndef INCLUDE_AES_H_
#define INCLUDE_AES_H_

#include "filebase.h"
#include <openssl/aes.h>
#include <openssl/md5.h>

#define FILE_SUFFIX_ENCRYPT ".ept"

class Aes
{
private:
    unsigned char key[MD5_DIGEST_LENGTH];
    std::filesystem::path file_path;


public:
    Aes(std::string file_path_, std::string password_);
    ~Aes();

    bool Encrypt();

    /// @brief 解密
    /// @return 0成功 -1密码错误 -2文件错误
    int Decrypt();
};

#endif // INCLUDE_AES_H_