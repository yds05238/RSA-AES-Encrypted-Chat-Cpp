/**
 * Bignum header 
 */

#ifndef _RSA_BIGNUM_H_
#define _RSA_BIGNUM_H_

#pragma once
#pragma GCC optimize("Ofast,unroll-loops,unroll-all-loops,fast-math,no-stack-protector,omit-frame-pointer,no-trapping-math,rename-registers,inline")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,avx2,fma")

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace reverse {
    class Bignum {
    public:
        Bignum() noexcept;
        Bignum(const Bignum&) noexcept;
        Bignum(const unsigned int num_digits) noexcept;
        Bignum(const std::string&) noexcept;
        Bignum(const int, const bool) noexcept;
        Bignum(const std::vector<int_fast32_t>) noexcept;

        std::string to_string() const noexcept;
        Bignum operator+(const Bignum&) const noexcept;
        Bignum operator-(const Bignum&) const noexcept;
        Bignum operator*(const Bignum&) const noexcept;
        Bignum operator/(const unsigned int) const noexcept;

        Bignum operator/(const Bignum&) const noexcept;
        Bignum operator%(const Bignum&) const noexcept;

        Bignum expmod(Bignum, const Bignum) const noexcept;
        Bignum gcd(const Bignum&) const noexcept;
        Bignum encrypt(const std::string&, const std::string&) const noexcept;
        Bignum decrypt(const std::string&, const std::string&) const noexcept;

        bool operator<(const Bignum&) const noexcept;
        bool operator==(const Bignum&) const noexcept;
        bool operator>(const Bignum&) const noexcept;
        bool operator!=(const Bignum&) const noexcept;
        bool operator<=(const Bignum&) const noexcept;
        bool operator>=(const Bignum&) const noexcept;

        int_fast32_t& operator[](unsigned int idx) noexcept;
        int_fast32_t operator[](unsigned int idx) const noexcept;
        unsigned int num_digits() const noexcept;
        std::vector<int_fast32_t> as_vec() const noexcept;

        void reverse_bignum() noexcept;
        Bignum reverse_copy() const noexcept;
        void changeToBase10() noexcept;

    private:
        unsigned int base;
        std::vector<int_fast32_t> digits;
        void purge() noexcept;
        bool isEmpty;
    };
}  // namespace reverse

namespace normal {
    class Bignum {
    public:
        Bignum() noexcept;
        Bignum(const std::vector<int_fast32_t>) noexcept;
        Bignum(const Bignum&) noexcept;
        Bignum(const unsigned int num_digits) noexcept;
        Bignum(const std::string&) noexcept;

        std::string to_string() const noexcept;
        Bignum operator-(const Bignum&) const noexcept;
        Bignum operator*(const unsigned int) const noexcept;
        std::pair<Bignum, Bignum> mult(const unsigned int) const noexcept;

        Bignum operator/(const Bignum&) const noexcept;
        Bignum operator%(const Bignum&) const noexcept;
        bool operator<(const Bignum&) const noexcept;
        bool operator>(const Bignum&) const noexcept;
        bool operator==(const Bignum&) const noexcept;
        bool operator!=(const Bignum&) const noexcept;
        int_fast32_t& operator[](unsigned int idx) noexcept;
        int_fast32_t operator[](unsigned int idx) const noexcept;
        std::vector<int_fast32_t> as_vec() const noexcept;
        unsigned int bin_search(Bignum&) const noexcept;

    private:
        std::vector<int_fast32_t> digits;
    };

}  // namespace normal

#endif /* _RSA_BIGNUM_H_ */