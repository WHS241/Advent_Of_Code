#ifndef BIG_INT
#define BIG_INT

#include <algorithm>
#include <compare>
#include <cstdint>
#include <iostream>
#include <limits>
#include <list>
#include <string>
#include <utility>

class big_int {
    private:
    std::list<uint32_t> parts;
    bool negative;

    static const uint64_t base = 0x100000000;

    public:
    big_int(long long x) : negative(x < 0) {
        if (x == std::numeric_limits<long long>::min()) {
            parts.push_back(0);
            x /= (int64_t)(base);
            x *= -1;
        } else {
            if (negative)
                x *= -1;
        }

        while (x != 0) {
            parts.push_back(x % base);
            x /= base;
        }
    }

    big_int(unsigned long long x) : negative(false) {
        while (x != 0) {
            parts.push_back(x % base);
            x /= base;
        }
    }

    big_int(long x) : big_int((long long) (x)){};
    big_int(unsigned long x) : big_int((unsigned long long) (x)){};
    big_int(int x) : big_int((long long) (x)){};
    big_int(unsigned int x) : big_int((unsigned long long) (x)){};
    big_int(short x) : big_int((long long) (x)){};
    big_int(unsigned short x) : big_int((unsigned long long) (x)){};

    big_int() : parts(), negative(false){};

    friend bool operator==(const big_int& lhs, const big_int& rhs) {
        return ((lhs.parts.empty() && rhs.parts.empty())
                || std::make_pair(lhs.negative, lhs.parts)
                     == std::make_pair(rhs.negative, rhs.parts));
    }

    friend std::strong_ordering operator<=>(const big_int& lhs, const big_int& rhs) {
        if (lhs == rhs)
            return std::strong_ordering::equal;
        if (lhs.negative != rhs.negative)
            return lhs.negative ? std::strong_ordering::less : std::strong_ordering::greater;
        if (lhs.negative) {
            if (lhs.parts.size() != rhs.parts.size())
                return rhs.parts.size() <=> lhs.parts.size();
            return std::lexicographical_compare_three_way(rhs.parts.rbegin(), rhs.parts.rend(),
                                                          lhs.parts.rbegin(), lhs.parts.rend());
        }
        if (lhs.parts.size() != rhs.parts.size())
            return lhs.parts.size() <=> rhs.parts.size();
        return std::lexicographical_compare_three_way(lhs.parts.rbegin(), lhs.parts.rend(),
                                                      rhs.parts.rbegin(), rhs.parts.rend());
    }

    big_int& operator+=(const big_int& rhs) {
        if (parts.empty())
            return *this = rhs;
        if (rhs.parts.empty())
            return *this;
        if (negative == rhs.negative) {
            bool carry = false;
            auto it = parts.begin();
            auto it2 = rhs.parts.begin();
            while (it != parts.end() || it2 != rhs.parts.end()) {
                if (it == parts.end()) {
                    parts.push_back(0);
                    --it;
                }
                uint64_t curr = uint64_t(*it);
                if (it2 != rhs.parts.end()) {
                    curr += uint64_t(*it2);
                    ++it2;
                }
                if (carry)
                    ++curr;
                carry = curr >= base;
                *(it++) = curr % base;
            }
        } else {
            negative = (*this < -rhs);
            bool borrow = false;
            auto it = parts.begin();
            auto it2 = rhs.parts.begin();
            while (it != parts.end() || it2 != rhs.parts.end()) {
                if (it == parts.end()) {
                    parts.push_back(0);
                    --it;
                }
                uint64_t v1 = *it;
                uint64_t v2 = it2 == rhs.parts.end() ? 0 : *(it2++);
                if (negative)
                    std::swap(v1, v2);
                if (borrow)
                    ++v2;
                borrow = v1 < v2;
                if (borrow)
                    v1 += base;
                *(it++) = v1 - v2;
            }

            trim();
        }
        return *this;
    }

    big_int& operator-=(const big_int& rhs) { return *this += -rhs; }

    big_int& operator*=(const big_int& rhs) { return *this = *this * rhs; }

    big_int& operator/=(const big_int& rhs) { return *this = *this / rhs; }

    big_int& operator%=(const big_int& rhs) { return *this = *this % rhs; }

    friend big_int operator+(const big_int& lhs, const big_int& rhs) {
        big_int temp(lhs);
        return temp += rhs;
    }
    friend big_int operator-(const big_int& lhs, const big_int& rhs) {
        big_int temp(lhs);
        return temp -= rhs;
    }
    friend big_int operator*(const big_int& lhs, const big_int& rhs) {
        if (lhs.parts.empty() || rhs.parts.empty())
            return big_int();
        if (lhs.negative != rhs.negative)
            return -(lhs * -rhs);
        big_int result;
        if (lhs.parts.size() < 10 || rhs.parts.size() < 10) {
            result.parts.push_back(0);
            auto ins_it = result.parts.begin();
            // long mult
            for (auto it = lhs.parts.begin(); it != lhs.parts.end(); ++it) {
                uint32_t carry = 0;
                big_int temp, temp_part;
                uint64_t part = *it;
                for (auto it = rhs.parts.begin(); it != rhs.parts.end(); ++it) {
                    uint64_t part2 = *it;
                    uint64_t prod = part * part2 + carry;
                    temp.parts.push_back(prod % base);
                    carry = prod / base;
                }
                if (carry)
                    temp.parts.push_back(carry);

                temp_part.parts.splice(temp_part.parts.end(), result.parts, ins_it--,
                                       result.parts.end());
                temp_part += temp;
                result.parts.splice(result.parts.end(), temp_part.parts);
                std::advance(ins_it, 2);
            }
        } else {
            // Karatsuba
            std::size_t break_size = std::min(lhs.parts.size(), rhs.parts.size()) / 2;
            big_int x1(lhs), x2, y1(rhs), y2;
            x1.negative = y1.negative = false;
            auto mid1 = x1.parts.begin();
            auto mid2 = y1.parts.begin();
            std::advance(mid1, break_size);
            std::advance(mid2, break_size);
            x2.parts.splice(x2.parts.end(), x1.parts, mid1, x1.parts.end());
            y2.parts.splice(y2.parts.end(), y1.parts, mid2, y1.parts.end());
            big_int z0 = x1 * y1;
            big_int z1 = (x1 + x2) * (y1 + y2);
            big_int z2 = x2 * y2;
            z1 -= (z0 + z2);
            for (std::size_t i = 0; i < break_size; ++i) {
                z1.parts.push_front(0);
                z2.parts.push_front(0);
                z2.parts.push_front(0);
            }
            result = z0 + z1 + z2;
        }
        // TODO: if part sizes exceed 1000, switch to Schonhage-Strassen
        return result;
    }

    friend big_int operator/(const big_int& lhs, const big_int& rhs) {
        return lhs.negative == rhs.negative ? div_r(lhs, rhs).first : -(-lhs / rhs);
    }
    friend big_int operator%(const big_int& lhs, const big_int& rhs) {
        if (rhs < 0)
            throw std::domain_error("Neg mod");
        return lhs >= 0 ? div_r(lhs, rhs).second : rhs - (-lhs % rhs);
    }

    big_int& operator++() { return *this += 1; }
    big_int& operator--() { return *this += -1; }
    big_int operator++(int) {
        auto temp(*this);
        ++*this;
        return temp;
    }
    big_int operator--(int) {
        auto temp(*this);
        --*this;
        return temp;
    }

    big_int operator+() const { return *this; }
    big_int operator-() const {
        auto temp(*this);
        temp.negative = !temp.negative;
        return temp;
    }

    std::string to_string(int base = 10) const {
        if (parts.empty())
            return "0";
        if (*this < 0)
            return "-" + ((-*this).to_string(base));

        std::string result;
        big_int temp(*this);
        while (!temp.parts.empty()) {
            int digit = (temp % base).parts.front();
            result.push_back(digit < 10 ? '0' + digit : 'A' + (digit - 10));
            temp /= base;
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

    private:
    void trim() {
        while (!parts.empty() && parts.back() == 0)
            parts.pop_back();
    }

    static std::pair<big_int, big_int> div_r(const big_int& lhs, const big_int& rhs) {
        if (rhs == 0)
            throw std::domain_error("除以零");
        std::pair<big_int, big_int> result;
        if (lhs < rhs) {
            result.second = lhs;
            return result;
        }
        std::copy_n(lhs.parts.rbegin(), rhs.parts.size() - 1,
                    std::front_inserter(result.second.parts));
        auto it = lhs.parts.rbegin();
        std::advance(it, rhs.parts.size() - 1);
        while (it != lhs.parts.rend()) {
            result.second.parts.push_front(*it);
            uint64_t lower = 0, upper = base - 1;
            while (upper != lower + 1) {
                uint64_t mid = (lower + upper) / 2;
                big_int res = rhs * mid;
                if (res < result.second) {
                    lower = mid;
                } else if (res > result.second) {
                    upper = mid;
                } else {
                    lower = mid;
                    upper = mid + 1;
                }
            }
            result.second -= rhs * lower;
            result.first.parts.push_front(lower);
            ++it;
        }
        result.first.trim();
        result.second.trim();
        return result;
    }
};

std::ostream& operator<<(std::ostream& os, const big_int& x) {
    auto flags = os.flags();
    if (flags & std::ios_base::dec)
        os << x.to_string(10);
    else if (flags & std::ios_base::oct)
        os << x.to_string(8);
    else if (flags & std::ios_base::hex)
        os << x.to_string(16);
    return os;
}

namespace std {
big_int pow(const big_int& b, const big_int& exp) {
    std::string exp_bin = exp.to_string(2);
    big_int r = 1;
    for (char c : exp_bin) {
        r *= r;
        if (c == '1')
            r *= b;
    }

    return r;
}
} // namespace std

#endif
