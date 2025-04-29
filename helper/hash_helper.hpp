// Copyright (c), buerjia.
// All rights reserved.

#ifndef __HASH_HELPER_HPP__
#define __HASH_HELPER_HPP__

#include <cstdint>
#include <string>
#include <sstream>
#include <memory>
#include <iostream>
#include <iomanip>

#include "openssl/evp.h"

namespace hash_helper
{

// 转换不可读字符串为16进制字串
static std::string to_hex(const std::string &msg)
{
    std::stringstream ss;
    for (int32_t i = 0; i < msg.size(); ++i)
    {
        auto val = static_cast<int32_t>(static_cast<uint8_t>(msg[i]));
        ss << std::hex << std::setw(2) << std::setfill('0') << val;
    }
    return ss.str();
}

// 使用openssl提供的hash算法对msg进行加密
// digest_name：md5,sha1,sha256,ripemd160
static std::string crypto(const std::string &digest_name,
                          const std::string &msg, bool is_hex = false)
{
    unsigned char digest[64];
    unsigned int md_len;

    //OpenSSL_add_all_digests();

    // openssl api reference.
    // https://www.openssl.org/docs/man1.1.1/man3/
    auto md = EVP_get_digestbyname(digest_name.c_str());
    if (!md)
    {
        throw std::runtime_error("not found digest: " + digest_name);
    }

    auto md_ctx = EVP_MD_CTX_new();

    EVP_DigestInit_ex(md_ctx, md, nullptr);
    EVP_DigestUpdate(md_ctx, msg.c_str(), msg.length());
    EVP_DigestFinal_ex(md_ctx, digest, &md_len);

    EVP_MD_CTX_free(md_ctx);

    auto res = std::string{ digest, digest + md_len };
    return is_hex ? to_hex(res) : res;
}

// 区块链hash算法：sha256+ripemd160
// 参考连接：https://www.liaoxuefeng.com/wiki/1207298049439968/1311929706479649
static std::string hash160(const std::string &msg)
{
    try
    {
        auto h1 = crypto("sha256", msg);
        return crypto("ripemd160", h1, true);
    }
    catch (std::exception &ex)
    {
        std::cerr << __FUNCTION__ << ": ex = " << ex.what() << std::endl;
        return "";
    }
}

// dhash: double sha256.
static std::string hash256(const std::string &msg)
{
    try
    {
        auto h1 = crypto("sha256", msg);
        return crypto("sha256", h1, true);
    }
    catch (std::exception &ex)
    {
        std::cerr << __FUNCTION__ << ": ex = " << ex.what() << std::endl;
        return "";
    }
}

} // namespace hash_helper

#endif // __HASH_HELPER_HPP__
