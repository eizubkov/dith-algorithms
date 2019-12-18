//
// Created by Elisey Zubkov on 2019-12-04.
//

#ifndef A_BIGINTEGER_H
#define A_BIGINTEGER_H


#include <iostream>
#include <string>
#include <vector>


template<class T>
T abs(const T &a) {
    return (a < 0) ? -a : a;
}

class BigInteger {
private:
    std::string number;
    bool sign;

    std::string add(std::string bigint_str1, std::string bigint_str2) const;

    std::string substract(std::string bigint_str1, std::string bigint_str2) const;

    std::string multiply(std::string bigint1, std::string bigint2) const;

    std::string divide(std::string bigint_str1, std::string bigint_str2) const;

    std::vector<int> decomposeInger(const BigInteger& bigint) const;

    std::vector<int> karatsubaMultiply(const std::vector<int> &bigint1, const std::vector<int> &bigint2) const;

    std::vector<int> naiveMultiply(const std::vector<int>& bigint1, const std::vector<int>& bigint2) const;

    std::string subtract(std::string lhs, std::string rhs) const;

    void checkInvariant();

public:
    BigInteger(int bigint_str = 0);

    BigInteger(std::string bigint_str);

    std::string toString() const;

    int toInt() const;

    friend BigInteger operator+(const BigInteger &rhs, const BigInteger &lhs);

    friend BigInteger operator-(const BigInteger &rhs, const BigInteger &lhs);

    friend BigInteger operator*(const BigInteger &rhs, const BigInteger &lhs);

    friend BigInteger operator/(const BigInteger &rhs, const BigInteger &lhs);

    friend BigInteger operator%(const BigInteger &rhs, const BigInteger &lhs);

    BigInteger operator/(int lhs) const;

    explicit operator bool() const { return (*this) != 0; }

    BigInteger &operator+=(const BigInteger &lhs);

    BigInteger &operator-=(const BigInteger &lhs);

    BigInteger &operator*=(const BigInteger &lhs);

    BigInteger &operator/=(const BigInteger &lhs);

    BigInteger &operator%=(const BigInteger &lhs);

    BigInteger &operator++();

    BigInteger operator++(int);

    BigInteger &operator--();

    BigInteger operator--(int);

    friend bool operator<(const BigInteger &rhs, const BigInteger &lhs);

    friend bool operator<=(const BigInteger &rhs, const BigInteger &lhs);

    friend bool operator>(const BigInteger &rhs, const BigInteger &lhs);

    friend bool operator>=(const BigInteger &rhs, const BigInteger &lhs);

    friend bool operator==(const BigInteger &rhs, const BigInteger &lhs);

    friend bool operator!=(const BigInteger &rhs, const BigInteger &lhs);

    BigInteger operator-() const;

    friend std::ostream &operator<<(std::ostream &stream, const BigInteger &lhs);

    friend std::istream &operator>>(std::istream &stream, BigInteger &lhs);
};

#endif //A_BIGINTEGER_H
