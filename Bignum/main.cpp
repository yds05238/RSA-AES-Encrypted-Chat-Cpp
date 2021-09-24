#include "bignum.hpp"

// const std::string rsa_n = "9616540267013058477253762977293425063379243458473593816900454019721117570003248808113992652836857529658675570356835067184715201230519907361653795328462699";
// const std::string rsa_e = "65537";
// const std::string rsa_d = "4802033916387221748426181350914821072434641827090144975386182740274856853318276518446521844642275539818092186650425384826827514552122318308590929813048801";

// std::string rsa_n, rsa_e, rsa_d;

const bool is_digit(const char value) noexcept { return std::isdigit(value); }
const bool is_numeric(const std::string& value) noexcept { return std::all_of(value.begin(), value.end(), is_digit); }

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

int main(int argc, char** argv) {
    std::ios_base::sync_with_stdio(false);

    // // only support encrypt/decrypt
    // if (argc != 5 || *argv[1] != 'e' || *argv[1] != 'd') {
    //     std::cout << "Bignum wrapper was updated to only support encryption/decryption. \nRun as bignum e/d (rsa_n) (rsa_e) (rsa_d)\n\n";
    //     return 0;
    // }

    if (argc != 4 && (argc != 5 || (*argv[1] != '^' && *argv[1] != 'e' && *argv[1] != 'd'))) {
        std::cout << "Run as bignum -op number1 number2, or bignum -help, where:\n	op is one of + - * / %% gcd encrypt decrypt\n	number1 and number2 are positive integers of arbitrary length\n"
                  << '\n';
        return 0;
    }

    const std::string op = std::string(argv[1]);
    std::string num1, num2, num3;

    num1 = std::string(argv[2]);
    if (!is_numeric(num1)) {
        std::cout << "Error: " << num1 << " is not an unsigned integer.\n\n";
        return 1;
    }
    num2 = std::string(argv[3]);
    if (!is_numeric(num2)) {
        std::cout << "Error: " << num2 << " is not an unsigned integer.\n\n";
        return 1;
    }

    if (argc == 5) {
        num3 = std::string(argv[4]);
        if (!is_numeric(num3)) {
            std::cout << "Error: " << num3 << " is not an unsigned integer.\n\n";
            return 1;
        }
    } else if (op[0] == '^' || op[0] == 'e' || op[0] == 'd') {
        std::cout << "Error: Modular exponentiation, encryption, decryption all require 3 positive integer values following the operator.\n"
                  << '\n';
        return 1;
    }

    // non-encrypt/decrypt operators -> bignum op number1 number2
    // encrypt/decrypt -> bignum e/d (rsa_n) (rsa_e) (rsa_d)

    // update to handle mainly encrypt + decrypt

    // bignum op (rsa_n) (rsa_e) (rsa_d) ... (rest same as before)

    // const std::string rsa_n = "9616540267013058477253762977293425063379243458473593816900454019721117570003248808113992652836857529658675570356835067184715201230519907361653795328462699";
    // const std::string rsa_e = "65537";
    // const std::string rsa_d = "4802033916387221748426181350914821072434641827090144975386182740274856853318276518446521844642275539818092186650425384826827514552122318308590929813048801";

    // if (argc != 4 && (argc != 5 || *argv[1] != '^') && (argc != 2 || (*argv[1] != 'e' && *argv[1] != 'd'))) {
    //     std::cout << "Run as bignum -op number1 number2, or bignum -help, where:\n	op is one of + - * / %% gcd encrypt decrypt\n	number1 and number2 are positive integers of arbitrary length\n"
    //               << '\n';
    //     return 0;
    // }
    // std::string num1, num2, num3;
    // const std::string op = std::string(argv[1]);
    // if (op[0] != 'd' && op[0] != 'e') {
    //     num1 = std::string(argv[2]);
    //     if (!is_numeric(num1)) {
    //         std::cout << "Error: " << num1 << " is not an unsigned integer.\n"
    //                   << '\n';
    //         return 1;
    //     }
    //     num2 = std::string(argv[3]);
    //     if (!is_numeric(num2)) {
    //         std::cout << "Error: " << num2 << " is not an unsigned integer.\n"
    //                   << '\n';
    //         return 1;
    //     }
    //     if (argc == 5) {
    //         num3 = std::string(argv[4]);
    //         if (!is_numeric(num3)) {
    //             std::cout << "Error: " << num3 << " is not an unsigned integer.\n"
    //                       << '\n';
    //             return 1;
    //         }
    //     } else if (op[0] == '^') {
    //         std::cout << "Error: Modular exponentiation requires an exponent and a modulus!\n"
    //                   << '\n';
    //         return 1;
    //     }
    // }

    int counter = 3;
    const reverse::Bignum Bbig("100000");
    switch (op[0]) {
    case '+':
        std::cout << (reverse::Bignum(num1) + reverse::Bignum(num2)).to_string() << '\n';
        return 0;
    case '-':
        std::cout << (reverse::Bignum(num1) - reverse::Bignum(num2)).to_string() << '\n';
        return 0;
    case '*':
        std::cout << (reverse::Bignum(num1) * reverse::Bignum(num2)).to_string() << '\n';
        return 0;
    case '/':
        std::cout << (reverse::Bignum(num1) / reverse::Bignum(num2)).to_string() << '\n';
        return 0;
    case '%':
        std::cout << (reverse::Bignum(num1) % reverse::Bignum(num2)).to_string() << '\n';
        return 0;
    case '<':
        std::cout << (reverse::Bignum(num1) < reverse::Bignum(num2)) << '\n';
        return 0;
    case '>':
        std::cout << (reverse::Bignum(num1) > reverse::Bignum(num2)) << '\n';
        return 0;
    case '=':
        std::cout << (reverse::Bignum(num1) == reverse::Bignum(num2)) << '\n';
        return 0;
    case '^':
        std::cout << reverse::Bignum(num1).expmod(num2, num3).to_string() << '\n';
        return 0;
    case 'g':
        std::cout << (reverse::Bignum(num1).gcd(reverse::Bignum(num2))).to_string() << '\n';
        return 0;
    case 'e': {
        const std::string rsa_n(num1);
        const std::string rsa_e(num2);

        while (!std::cin.eof()) {
            std::string next_line;
            std::getline(std::cin, next_line);

            // TODO: remove this character limitations
            if (next_line.size() * 3 > rsa_n.size()) {
                std::cout << reverse::Bignum(to_numeric(next_line.substr(0, rsa_n.size() / 3))).encrypt(rsa_n, rsa_e).to_string() << '\n';
                std::cout << reverse::Bignum(to_numeric(next_line.substr(rsa_n.size() / 3, rsa_n.size() / 3))).encrypt(rsa_n, rsa_e).to_string() << '\n';
            } else {
                std::cout << reverse::Bignum(to_numeric(next_line)).encrypt(rsa_n, rsa_e).to_string() << '\n';
                std::cout << reverse::Bignum(++counter).encrypt(rsa_n, rsa_e).to_string() << '\n';
            }
        }
        return 0;
    }
    case 'd': {
        const std::string rsa_n(num1);
        const std::string rsa_d(num3);

        //TODO: double check p2, Bbig comparison below
        while (!std::cin.eof()) {
            std::string part1, part2;
            std::getline(std::cin, part1);
            std::getline(std::cin, part2);
            std::cout << to_chars(reverse::Bignum(part1).decrypt(rsa_n, rsa_d).to_string());
            const reverse::Bignum p2 = reverse::Bignum(part2).decrypt(rsa_n, rsa_d);
            if (p2 > Bbig) {
                std::cout << to_chars(p2.to_string());
            }
            std::cout << '\n';
        }
        return 0;
    }
    default: {
        std::cout << "Error: " << op << " is not a supported operator\n"
                  << '\n';
        return 1;
    }
    }
}
