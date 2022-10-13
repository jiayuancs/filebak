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

    bool verbose; // 输出执行过程信息

public:
    Aes(std::string file_path_, std::string password_, bool verbose_ = false);
    ~Aes();

    bool Encrypt();
    bool Decrypt();
};

#endif // INCLUDE_AES_H_