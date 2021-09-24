/**
 * Testing reading pem key files.
 */
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

const std::string readPemFile(const std::filesystem::path& fpath) {
    // read specified pem key file

    // read file lines
    std::ifstream infile(fpath);
    std::string line, keyStr;
    bool startKeyStr = false;

    const std::string s1 = "-----BEGIN RSA PUBLIC KEY-----";
    const std::string s2 = "-----BEGIN RSA PRIVATE KEY-----";
    const std::string s1last = "-----END RSA PUBLIC KEY-----";
    const std::string s2last = "-----END RSA PRIVATE KEY-----";

    while (std::getline(infile, line)) {
        if (!startKeyStr) {
            // handle start of the file
            if (line == s1 || line == s2) {
                startKeyStr = true;
            }
        } else {
            // handle key string
            if (line != s1last && line != s2last) {
                keyStr += line;
            } else {
                // handle last line of file
                break;
            }
        }
    }

    return keyStr;
}

int main() {
    // test reading pem files

    // Public Pem file
    const std::filesystem::path fpath1(u8"rsa_public.pem");
    auto res1 = readPemFile(fpath1);
    std::cout << res1 << '\n'
              << '\n';

    // Private Pem file
    const std::filesystem::path fpath2(u8"rsa_private.pem");
    auto res2 = readPemFile(fpath2);
    std::cout << res2 << '\n';
}