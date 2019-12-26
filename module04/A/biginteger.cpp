#include "biginteger.h"

void BigInteger::checkInvariant() {
    if (number == "") {
        number = "0";
    }
    if (number == "0") {
        sign = false;
    }
    return;
}

int BigInteger::toInt() const {
    return stoi(toString());
}

BigInteger &BigInteger::operator++() {
    (*this) = (*this) + 1;
    return (*this);
}

BigInteger BigInteger::operator++(int) {
    BigInteger before = (*this);

    (*this) = (*this) + 1;

    return before;
}

BigInteger &BigInteger::operator--() {
    (*this) = (*this) - 1;
    return (*this);

}

BigInteger BigInteger::operator--(int) {
    BigInteger before = (*this);

    (*this) = (*this) - 1;

    return before;
}

BigInteger &BigInteger::operator+=(const BigInteger &lhs) {
    (*this) = (*this) + lhs;
    return (*this);
}

BigInteger &BigInteger::operator-=(const BigInteger &lhs) {
    (*this) = (*this) - lhs;
    return (*this);
}

BigInteger &BigInteger::operator*=(const BigInteger &lhs) {
    (*this) = (*this) * lhs;
    return (*this);
}

BigInteger &BigInteger::operator/=(const BigInteger &lhs) {
    (*this) = (*this) / lhs;
    return (*this);
}

BigInteger &BigInteger::operator%=(const BigInteger &lhs) {
    (*this) = (*this) % lhs;
    return (*this);
}

bool operator<=(const BigInteger &rhs, const BigInteger &lhs) {
    return rhs == (lhs) || rhs < (lhs);
}

bool operator>=(const BigInteger &rhs, const BigInteger &lhs) {
    return rhs == (lhs) || rhs > (lhs);
}

BigInteger operator%(const BigInteger &rhs, const BigInteger &lhs) {
    return rhs - (lhs * (rhs / lhs));
}

BigInteger operator/(const BigInteger &rhs, const BigInteger &lhs) {
    BigInteger res;
    res.sign = rhs.sign xor lhs.sign;
    res.number = rhs.divide(rhs.number, lhs.number);
    res.checkInvariant();
    return res;
}

BigInteger::BigInteger(std::string bigint_str) {
    if (bigint_str[0] == '-') {
        sign = true;
        bigint_str.erase(bigint_str.begin());
    } else {
        sign = false;
    }
    number = bigint_str;
    checkInvariant();
}

bool operator!=(const BigInteger &rhs, const BigInteger &lhs) {
    return !(rhs == (lhs));
}

bool operator==(const BigInteger &rhs, const BigInteger &lhs) {
    if (rhs.sign xor lhs.sign) {
        return false;
    }
    return rhs.toString() == lhs.toString();
}

bool operator>(const BigInteger &rhs, const BigInteger &lhs) {
    return (rhs != (lhs) && !(rhs < (lhs)));
}

BigInteger BigInteger::operator/(int lhs) const {
    BigInteger result;
    if (lhs == 0) {
        return result;
    }
    bool rsign = (lhs < 0) ^sign;
    lhs = abs(lhs);
    int a = number[0] - '0';
    for (size_t i = 1; i <= number.size(); ++i) {
        result = result * 10 + a / lhs;
        a %= lhs;
        if (i == number.size())
            break;
        a = a * 10 + (number[i] - '0');
    }
    result.sign = rsign;
    result.checkInvariant();
    return result;
}

std::string BigInteger::divide(std::string bigint_str1, std::string bigint_str2) const {
    BigInteger bigint1(bigint_str1), bigint2(bigint_str2);
    if (bigint1 < bigint2) {
        return "0";
    }
    BigInteger l(0), r(bigint_str1);
    while (l < r) {
        BigInteger m = (l + r) / 2;
        if (bigint2 * m > bigint1)
            r = m;
        else if (bigint1 - bigint2 * m >= bigint2)
            l = m + 1;
        else
            return m.toString();
    }
    return l.toString();
}

BigInteger operator*(const BigInteger &a, const BigInteger &b) {
    BigInteger res;
    res.sign = a.sign xor b.sign;
    res.number = a.multiply(a.number, b.number);
    res.checkInvariant();
    return res;
}

std::vector<int> BigInteger::naiveMultiply(const std::vector<int> &bigint1, const std::vector<int> &bigint2) const {
    const size_t bigint_size = bigint1.size();
    std::vector<int> result(2 * bigint_size);

    for (size_t i = 0; i < bigint_size; ++i) {
        for (size_t j = 0; j < bigint_size; ++j) {
            result[i + j] += bigint1[i] * bigint2[j];
        }
    }
    return result;
}


std::string BigInteger::multiply(std::string bigint1, std::string bigint2) const {
    if (bigint1.length() > bigint2.length()) {
        bigint1.swap(bigint2);
    }

    std::string res = "0";
    for (int i = bigint1.length() - 1; i >= 0; --i) {
        std::string temp = bigint2;
        int currentDigit = bigint1[i] - '0';
        int carry = 0;

        for (int j = temp.length() - 1; j >= 0; --j) {
            temp[j] = ((temp[j] - '0') * currentDigit) + carry;

            if (temp[j] > 9) {
                carry = (temp[j] / 10);
                temp[j] -= (carry * 10);
            } else
                carry = 0;

            temp[j] += '0';
        }

        if (carry > 0) {
            temp.insert(0, 1, (carry + '0'));
        }

        temp.append((bigint1.length() - i - 1), '0');

        res = add(res, temp);
    }

    while (res[0] == '0' && res.length() != 1)
        res.erase(0, 1);

    return res;
}


BigInteger::BigInteger(int a) {
    if (a < 0) {
        sign = true;
        a = -a;
    } else
        sign = false;
    if (a == 0)
        number.push_back('0');
    else {
        while (a != 0) {
            number.insert(number.begin(), (a % 10) + 48);
            a /= 10;
        }
    }
}

std::istream &operator>>(std::istream &stream, BigInteger &lhs) {
    std::string s;
    stream >> s;
    lhs = BigInteger(s);
    return stream;
}

std::string BigInteger::toString() const {
    return (sign) ? "-" + number : number;
}

BigInteger BigInteger::operator-() const {
    BigInteger res;
    res.number = number;
    res.sign = !sign;
    res.checkInvariant();
    return res;
}

bool operator<(const BigInteger &rhs, const BigInteger &lhs) {
    if (!rhs.sign && lhs.sign)
        return false;
    else if (rhs.sign && !lhs.sign)
        return true;
    bool res = false;
    if (rhs.number.size() < lhs.number.size())
        res = true;
    else if (rhs.number.size() > lhs.number.size())
        res = false;
    else {
        for (size_t i = 0; i < rhs.number.size(); ++i) {
            if (rhs.number[i] < lhs.number[i]) {
                res = true;
                break;
            } else if (rhs.number[i] > lhs.number[i]) {
                res = false;
                break;
            }
            if (i == rhs.number.size() - 1)
                return false;
        }
    }
    return (rhs.sign) ? !res : res;
}

std::ostream &operator<<(std::ostream &stream, const BigInteger &lhs) {
    stream << lhs.toString();
    return stream;
}

std::string BigInteger::substract(std::string bigint_str1, std::string bigint_str2) const {
    int dif_in_len = abs((int) bigint_str1.size() - (int) bigint_str2.size());
    bigint_str2.insert(0, dif_in_len, '0');
    std::string res(bigint_str1.size(), '0');
    for (int i = bigint_str1.size() - 1; i >= 0; --i) {
        res[i] = (bigint_str1[i] - '0') - (bigint_str2[i] - '0') + '0';
        if (i != 0) {
            if (res[i] < '0') {
                res[i] += 10;
                --bigint_str1[i - 1];
            }
        }
    }
    while (res[0] == '0') {
        res.erase(res.begin());
    }
    return res;
}

std::string BigInteger::add(std::string bigint_str1, std::string bigint_str2) const {
    if (BigInteger(bigint_str1) < BigInteger(bigint_str2))
        swap(bigint_str1, bigint_str2);
    int dif_in_len = abs((int) bigint_str1.size() - (int) bigint_str2.size());
    bigint_str2.insert(0, dif_in_len, '0');
    char carry = 0;
    std::string res(bigint_str1.size(), '0');
    for (int i = bigint_str1.size() - 1; i >= 0; --i) {
        res[i] = carry + (bigint_str1[i] - '0') + (bigint_str2[i] - '0') + '0';
        if (i != 0) {
            if (res[i] > '9') {
                res[i] -= 10;
                carry = 1;
            } else {
                carry = 0;
            }
        }
    }
    if (res[0] > '9') {
        res[0] -= 10;
        res.insert(0, 1, '1');
    }
    return res;
}

BigInteger operator+(const BigInteger &lhs, const BigInteger &rhs) {
    if (abs(lhs) < abs(rhs)) {
        return operator+(rhs, lhs);
    }
    BigInteger res;
    if (!lhs.sign && !rhs.sign) {
        res.number = lhs.add(lhs.number, rhs.number);
        res.sign = false;
    } else if (!lhs.sign && rhs.sign) {
        res.sign = false;
        res.number = lhs.substract(lhs.number, rhs.number);
    } else if (lhs.sign && !rhs.sign) {
        res.sign = true;
        res.number = lhs.substract(lhs.number, rhs.number);
    } else {
        res.sign = true;
        res.number = lhs.add(lhs.number, rhs.number);
    }
    res.checkInvariant();
    return res;
}

BigInteger operator-(const BigInteger &a, const BigInteger &b) {
    BigInteger b1 = b;
    b1.sign = !b1.sign;
    return a + b1;
}