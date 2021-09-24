#include "bignum.hpp"

const unsigned int BASE = 1000000000;

normal::Bignum::Bignum() noexcept {}

normal::Bignum::Bignum(const normal::Bignum &b) noexcept : digits(b.digits) {}

normal::Bignum::Bignum(const std::string &s) noexcept : digits(std::stoi(s)) {}

normal::Bignum::Bignum(const unsigned int num_digits) noexcept : digits(num_digits) {}

normal::Bignum::Bignum(const std::vector<int_fast32_t> v) noexcept {
    digits = v;
    std::reverse(digits.begin(), digits.end());
}

const normal::Bignum BZero("0");
const normal::Bignum BOne("1");
const normal::Bignum BTwo("2");

std::string normal::Bignum::to_string() const noexcept {
    if (digits.empty())
        return "0";
    std::string s;
    auto vit = digits.begin();
    s += std::to_string(*vit);
    while (++vit != digits.end()) {
        std::string sub("000000000" + std::to_string(*vit));
        s += sub.substr(sub.size() - 9);
    }
    return s;
}

std::vector<int_fast32_t> normal::Bignum::as_vec() const noexcept {
    // only use for creating Bignum from normal Bignum
    return std::vector<int_fast32_t>(digits.rbegin(), digits.rend());
}

int_fast32_t &normal::Bignum::operator[](unsigned int idx) noexcept {
    return digits[idx];
}

int_fast32_t normal::Bignum::operator[](unsigned int idx) const noexcept {
    return digits[idx];
}

bool normal::Bignum::operator<(const normal::Bignum &b) const noexcept {
    if (digits.size() != b.digits.size()) {
        return digits.size() < b.digits.size();
    }
    return std::lexicographical_compare(digits.begin(), digits.end(),
                                        b.digits.begin(), b.digits.end());
}

bool normal::Bignum::operator>(const normal::Bignum &b) const noexcept {
    return b < *this;
}

bool normal::Bignum::operator!=(const normal::Bignum &b) const noexcept {
    return digits != b.digits;
}

bool normal::Bignum::operator==(const normal::Bignum &b) const noexcept {
    return digits == b.digits;
}

normal::Bignum normal::Bignum::operator-(const normal::Bignum &b) const noexcept {
    if (*this < b)
        return BZero;
    auto alen = digits.size();
    auto blen = b.digits.size();
    auto borrow = 0;
    auto zeros = 0;
    std::vector<int_fast32_t> rdigits;
    rdigits.reserve(alen);

    while (alen > 0 && blen > 0) {
        auto d = digits[--alen] - b.digits[--blen] - borrow;
        borrow = 0;
        if (d < 0) {
            d += BASE;
            borrow = 1;
        }
        if (d != 0) {
            while (zeros--) rdigits.emplace_back(0);
            zeros = 0;
            rdigits.emplace_back(d);
        } else {
            ++zeros;
        }
    }
    while (alen > 0) {
        auto d = digits[--alen] - borrow;
        borrow = 0;
        if (d < 0) {
            d += BASE;
            borrow = 1;
        }
        if (d != 0) {
            while (zeros--) rdigits.emplace_back(0);
            zeros = 0;
            rdigits.emplace_back(d);
        } else {
            ++zeros;
        }
    }

    return normal::Bignum(rdigits);
}

std::pair<normal::Bignum, normal::Bignum> normal::Bignum::mult(const unsigned int d) const noexcept {
    switch (d) {
    case 0:
        return std::pair<normal::Bignum, normal::Bignum>(BZero, *this);
    case 1:
        return std::pair<normal::Bignum, normal::Bignum>(*this, *this * 2);
    default:
        auto alen = digits.size();
        int_fast32_t carry = 0;
        int_fast32_t carry2 = 0;
        normal::Bignum res(*this);
        normal::Bignum res2(*this);
        const auto d1 = d + 1;
        while (alen--) {
            carry += res[alen] * d;
            carry2 += res2[alen] * d1;
            res[alen] = carry % BASE;
            res2[alen] = carry2 % BASE;
            carry /= BASE;
            carry2 /= BASE;
        }
        while (carry > 0) {
            res.digits.emplace(res.digits.begin(), carry % BASE);
            carry /= BASE;
        }
        while (carry2 > 0) {
            res2.digits.emplace(res2.digits.begin(), carry2 % BASE);
            carry2 /= BASE;
        }
        return std::pair<normal::Bignum, normal::Bignum>(res, res2);
    }
}

normal::Bignum normal::Bignum::operator*(const unsigned int d) const noexcept {
    switch (d) {
    case 0:
        return BZero;
    case 1:
        return *this;
    default:
        auto alen = digits.size();
        int_fast32_t carry = 0;
        normal::Bignum res(*this);
        while (alen--) {
            carry += res[alen] * d;
            res[alen] = carry % BASE;
            carry /= BASE;
        }
        while (carry > 0) {
            res.digits.emplace(res.digits.begin(), carry % BASE);
            carry /= BASE;
        }
        return res;
    }
}

unsigned int normal::Bignum::bin_search(normal::Bignum &target) const noexcept {
    unsigned int left = 0, right = 999999999;
    while (left <= right) {
        const unsigned int mid = (left + right) / 2;
        std::pair<normal::Bignum, normal::Bignum> pnum = this->mult(mid);
        if (pnum.second < target) {
            left = mid + 1;
        } else if (target < pnum.first) {
            right = mid - 1;
        } else if (target == pnum.second) {
            target = target - pnum.second;
            return mid + 1;
        } else {
            target = target - pnum.first;
            return mid;
        }
    }
    return left;
}

normal::Bignum normal::Bignum::operator/(const normal::Bignum &b) const noexcept {
    normal::Bignum res;
    normal::Bignum tmp;
    unsigned int aindex = 0;
    auto placed_first = false;
    while (aindex < b.digits.size()) {
        const auto d = digits[aindex++];
        if (placed_first || d != 0) {
            tmp.digits.emplace_back(d);
            placed_first = true;
        }
    }
    if (!placed_first) {
        tmp = BZero;
    }
    auto first = true;
    do {
        // unsigned int d = 0;
        if (tmp < b && aindex < digits.size()) {
            const auto d1 = digits[aindex++];
            if (tmp != BZero) {
                tmp.digits.emplace_back(d1);
            } else if (d1 != 0) {
                tmp[0] = d1;
            }
        }
        const auto d2 = b.bin_search(tmp);
        if (!first || d2 != 0) {
            first = false;
            res.digits.emplace_back(d2);
        }
    } while (aindex < digits.size());
    tmp.digits.clear();
    tmp.digits.shrink_to_fit();

    return (res.digits.empty()) ? (BZero) : (res);
}

normal::Bignum normal::Bignum::operator%(const normal::Bignum &b) const noexcept {
    // normal::Bignum tmp;
    normal::Bignum tmp;
    tmp.digits.reserve(digits.size());
    unsigned int aindex = 0;
    auto placed_first = false;
    while (aindex < b.digits.size()) {
        const auto d = digits[aindex++];
        if (placed_first || d != 0) {
            tmp.digits.emplace_back(d);
            placed_first = true;
        }
    }
    if (!placed_first)
        tmp = BZero;
    do {
        if (tmp < b && aindex < digits.size()) {
            const auto d = digits[aindex++];
            if (tmp != BZero) {
                tmp.digits.emplace_back(d);
            } else if (d != 0) {
                tmp[0] = d;
            }
        }
        if (b < tmp) {
            b.bin_search(tmp);
        } else if (b == tmp) {
            tmp = tmp - b;
        }
    } while (aindex < digits.size());
    return (tmp.digits.empty()) ? (BZero) : (tmp);
}
