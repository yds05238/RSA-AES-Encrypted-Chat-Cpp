#include "RSA.h"

#include <stdio.h>
#include <stdlib.h>

#include <cstring>

// Helper function to read key from pem key file
const std::string readPemFile(const std::filesystem::path& fpath) {
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

    // return keyStr;

    std::string nlad("");
    for (const char c : keyStr) {
        const int d = ((int)c & 0xFF);
        nlad += (d / 100) % 10 + '0';
        nlad += (d / 10) % 10 + '0';
        nlad += d % 10 + '0';
    }

    std::cout << "key: " << keyStr << '\n'
              << '\n';
    std::cout << "10key: " << nlad << '\n'
              << '\n';

    return nlad;
}

void RSA::rsa_keys_generate() {
    // generate keys
    std::string scriptcall = std::string(std::filesystem::current_path());
    scriptcall += "/generate_keys.sh ";
    scriptcall += std::to_string(keyBits);
    char script_call[scriptcall.length() + 1];
    strcpy(script_call, scriptcall.c_str());

    std::cout << scriptcall << '\n';

    // system(scriptcall);
    system(script_call);

    // read keys
    // Public Pem file
    const std::filesystem::path fpath1(u8"keys/rsa_public.pem");
    rsa_n = readPemFile(fpath1);
    // Private Pem file
    const std::filesystem::path fpath2(u8"keys/rsa_private.pem");
    rsa_d = readPemFile(fpath2);

    // clean up key files
    // system("./cleanup_keys.sh");
    std::string scriptcall2 = std::string(std::filesystem::current_path());
    scriptcall2 += "/lib/RSA/src/cleanup_keys.sh";
    char script_call2[scriptcall2.length() + 1];
    strcpy(script_call2, scriptcall2.c_str());

    // system("cleanup_keys.sh");
    system(script_call2);
}

// const bool RSA::is_digit(const char value) noexcept { return std::isdigit(value); }
const bool is_digit(const char value) noexcept { return std::isdigit(value); }

const bool is_numeric(const std::string& value) noexcept { return std::all_of(value.begin(), value.end(), is_digit); }

// std::string RSA::to_numeric(std::string next_line) noexcept {
std::string to_numeric(std::string next_line) noexcept {
    std::string nlad("");
    for (const char c : next_line) {
        const int d = ((int)c & 0xFF);
        nlad += (d / 100) % 10 + '0';
        nlad += (d / 10) % 10 + '0';
        nlad += d % 10 + '0';
    }
    return nlad;
}

// std::string RSA::to_chars(reverse::Bignum to_numeric) noexcept {
std::string to_chars(reverse::Bignum to_numeric) noexcept {
    to_numeric.changeToBase10();
    std::string nlas("");
    auto d_to_go = to_numeric.num_digits() % 3;
    if (d_to_go == 0)
        d_to_go = 3;
    auto index = 0;
    auto next_d = 0;
    const int size = (to_numeric.num_digits() + 2) / 3;
    for (int n = 0; n < size; n++) {
        do {
            next_d = next_d * 10 + to_numeric[index++];
        } while (--d_to_go);
        nlas += (char)next_d;
        next_d = 0;
        d_to_go = 3;
    }
    return nlas;
}

std::string RSA::handle_bignum(int argc, std::vector<std::string> argv) {  // char** argv) {
    std::ios_base::sync_with_stdio(false);

    if (argc != 4 && (argc != 6 || (argv[1] != "e" && argv[1] != "d")) && (argc != 5 || (argv[1] != "^"))) {
        return "Run as bignum -op number1 number2, or bignum -help, where:\n	op is one of + - * / %% gcd encrypt decrypt\n	number1 and number2 are positive integers of arbitrary length\n\n";
    }

    const std::string op = std::string(argv[1]);
    std::string num1, num2, num3;

    num1 = std::string(argv[2]);
    if (!is_numeric(num1)) {
        std::cout << "Error: " << num1 << " is not an unsigned integer.\n\n";
        return "error";

        // std::cout << "Error: " << num1 << " is not an unsigned integer.\n\n";
        // return 1;
    }
    num2 = std::string(argv[3]);
    if (!is_numeric(num2)) {
        std::cout << "Error: " << num2 << " is not an unsigned integer.\n\n";
        // return 1;
        return "error";
    }

    if (argc == 5) {
        num3 = std::string(argv[4]);
        if (!is_numeric(num3)) {
            std::cout << "Error: " << num3 << " is not an unsigned integer.\n\n";
            // return 1;
            return "error";
        }
    } else if (op[0] == '^') {
        std::cout << "Error: Modular exponentiation require 3 positive integer values following the operator.\n"
                  << '\n';
        // return 1;
        return "error";
    }

    if (argc == 6 && op[0] != 'e' && op[0] != 'd') {
        std::cout << "Error: bignum e/d rsan, rsae rsad message\n\n";
        // return 1;
        return "error";
    }

    // int counter = 3;
    // const reverse::Bignum Bbig("100000");
    switch (op[0]) {
    case '+':
        std::cout << (reverse::Bignum(num1) + reverse::Bignum(num2)).to_string() << '\n';
        // return 0;
        return "";

    case '-':
        std::cout << (reverse::Bignum(num1) - reverse::Bignum(num2)).to_string() << '\n';
        // return 0;
        return "";

    case '*':
        std::cout << (reverse::Bignum(num1) * reverse::Bignum(num2)).to_string() << '\n';
        // return 0;
        return "";

    case '/':
        std::cout << (reverse::Bignum(num1) / reverse::Bignum(num2)).to_string() << '\n';
        // return 0;
        return "";

    case '%':
        std::cout << (reverse::Bignum(num1) % reverse::Bignum(num2)).to_string() << '\n';
        // return 0;
        return "";

    case '<':
        std::cout << (reverse::Bignum(num1) < reverse::Bignum(num2)) << '\n';
        // return 0;
        return "";

    case '>':
        std::cout << (reverse::Bignum(num1) > reverse::Bignum(num2)) << '\n';
        // return 0;
        return "";

    case '=':
        std::cout << (reverse::Bignum(num1) == reverse::Bignum(num2)) << '\n';
        // return 0;
        return "";

    case '^':
        std::cout << reverse::Bignum(num1).expmod(num2, num3).to_string() << '\n';
        // return 0;
        return "";

    case 'g':
        std::cout << (reverse::Bignum(num1).gcd(reverse::Bignum(num2))).to_string() << '\n';
        // return 0;
        return "";

    case 'e': {
        const std::string rsa_n(num1);
        const std::string rsa_e(num2);

        // mainly used for encrypting aes keys -> ignore potential size issue
        const std::string message = std::string(argv[5]);
        // std string res = reverse::Bignum(to_numeric(message)).encrypt(rsa_n, rsa_e).to_string();
        return reverse::Bignum(to_numeric(message)).encrypt(rsa_n, rsa_e).to_string();

        // while (!std::cin.eof()) {
        //     std::string next_line;
        //     std::getline(std::cin, next_line);

        //     // TODO: remove this character limitations
        //     if (next_line.size() * 3 > rsa_n.size()) {
        //         std::cout << reverse::Bignum(to_numeric(next_line.substr(0, rsa_n.size() / 3))).encrypt(rsa_n, rsa_e).to_string() << '\n';
        //         std::cout << reverse::Bignum(to_numeric(next_line.substr(rsa_n.size() / 3, rsa_n.size() / 3))).encrypt(rsa_n, rsa_e).to_string() << '\n';
        //     } else {
        //         std::cout << reverse::Bignum(to_numeric(next_line)).encrypt(rsa_n, rsa_e).to_string() << '\n';
        //         std::cout << reverse::Bignum(++counter).encrypt(rsa_n, rsa_e).to_string() << '\n';
        //     }
        // }
        // return 0;
    }
    case 'd': {
        const std::string rsa_n(num1);
        const std::string rsa_d(num3);
        const std::string message = std::string(argv[5]);
        // std::cout << to_chars(reverse::Bignum(message).decrypt(rsa_n, rsa_d).to_string()) << '\n';
        return to_chars(reverse::Bignum(message).decrypt(rsa_n, rsa_d).to_string());

        // //TODO: double check p2, Bbig comparison below
        // while (!std::cin.eof()) {
        //     std::string part1, part2;
        //     std::getline(std::cin, part1);
        //     std::getline(std::cin, part2);
        //     std::cout << to_chars(reverse::Bignum(part1).decrypt(rsa_n, rsa_d).to_string());
        //     const reverse::Bignum p2 = reverse::Bignum(part2).decrypt(rsa_n, rsa_d);
        //     if (p2 > Bbig) {
        //         std::cout << to_chars(p2.to_string());
        //     }
        //     std::cout << '\n';
        // }
        // return 0;
    }
    default: {
        std::cout << "Error: " << op << " is not a supported operator\n"
                  << '\n';
        // return 1;
        return "error";
    }
    }
}

// RSA::RSA(int keyBits = 2048) : keyBits(keyBits) {
//     rsa_keys_generate();
// }

RSA::RSA() {
    keyBits = 2048;
    rsa_keys_generate();
}

RSA::RSA(int keyBits) : keyBits(keyBits) {
    rsa_keys_generate();
}

const std::string RSA::get_public_key() noexcept {
    return rsa_n;
}

std::string RSA::rsa_encrypt(std::string receiver_public_key, std::string message) {
    // receiver_public_key can be either own or other's public key

    // build off of bignum input handler
    std::vector<std::string> argv;
    argv.emplace_back("bignum");
    argv.emplace_back("e");
    argv.emplace_back(receiver_public_key);
    argv.emplace_back(rsa_e);
    argv.emplace_back(rsa_d);
    argv.emplace_back(message);

    // std::string* argv = [ "bignum", "e", receiver_public_key, rsa_e, rsa_d, message ];
    int argc = 6;

    std::string res = handle_bignum(argc, argv);
    return res;
}

std::string RSA::rsa_decrypt(std::string message) {
    // build off of bignum input handler
    std::vector<std::string> argv;
    argv.emplace_back("bignum");
    argv.emplace_back("d");
    argv.emplace_back(rsa_n);
    argv.emplace_back(rsa_e);
    argv.emplace_back(rsa_d);
    argv.emplace_back(message);

    // std::string* argv = [ "bignum", "d", receiver_public_key, rsa_e, rsa_d, message ];
    int argc = 6;

    std::string res = handle_bignum(argc, argv);
    return res;
}
