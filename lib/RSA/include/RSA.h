/**
 * RSA wrapper over Bignum
 */
#ifndef _RSA_H_
#define _RSA_H_

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "bignum.h"

class RSA {
private:
    int keyBits;  // 2048, 4096

    std::string rsa_n;  // public modulus
    std::string rsa_d;  // private exponent

    const std::string rsa_e = "65537";  // public exponent

private:
    // utility functions
    // const std::string readPemFile(const std::filesystem::path& fpath);

    void rsa_keys_generate();

    // const bool is_digit(const char value) noexcept;

    // const bool is_numeric(const std::string& value) noexcept;

    // std::string to_numeric(std::string next_line) noexcept;

    // std::string to_chars(reverse::Bignum to_numeric) noexcept;

    std::string handle_bignum(int argc, std::vector<std::string> argv);  //char** argv);

public:
    // RSA(int keyBits = 2048);
    RSA();

    RSA(int keyBits);

    const std::string get_public_key() noexcept;  // get rsa_n

    std::string rsa_encrypt(std::string receiver_public_key, std::string message);

    std::string rsa_decrypt(std::string message);  // decrypt using self private key
};

#endif /* _RSA_H_ */