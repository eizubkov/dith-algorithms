#include <iostream>
#include <vector>
#include <string>
#include <cassert>

const int kAlphabetSize = 26;

std::vector<int> StringToZ(const std::string &txt) {
    const int txt_size = txt.size();
    std::vector<int> z(txt_size, 0);
    int left = 0;
    int right = 0;
    for (int i = 1; i < txt_size; ++i) {
        int shift = (i <= right) ? std::min(z[i - left], right - i + 1) : 0;
        while (shift + i < txt_size && txt[i + shift] == txt[shift]) {
            ++shift;
        }
        z[i] = shift;
        if (shift + i - 1 > right) {
            left = i;
            right = shift + i - 1;
        }
    }
    return z;
}

std::vector<int> StringToPrefix(const std::string &txt) {
    const int txt_size = txt.size();
    std::vector<int> prefix(txt_size, 0);
    for (int i = 1; i < txt_size; ++i) {
        int k = prefix[i - 1];
        while (k > 0 && txt[i] != txt[k]) {
            k = prefix[k - 1];
        }
        prefix[i] = (txt[i] == txt[k]) ? (++k) : k;
    }
    return prefix;
}

std::vector<int> PrefixToZ(const std::vector<int> &prefix) {
    const int prefix_size = prefix.size();
    std::vector<int> z(prefix_size, 0);
    for (int i = 1; i < prefix_size; i++) {
        if (prefix[i] > 0) {
            z[i - prefix[i] + 1] = prefix[i];
        }
    }
    int i = 1;
    while (i < prefix_size) {
        int z_block_begin = i;
        if (z[i] > 0) {
            for (int j = 1; j < z[i]; j++) {
                if (z[i + j] > z[j]) {
                    break;
                }
                z[i + j] = std::min(z[j], z[i] - j);
                z_block_begin = i + j;
            }
        }
        i = z_block_begin + 1;
    }
    return z;
}

std::string ZToString(const std::vector<int> &z) {
    const int z_size = z.size();

    if (z_size == 0) {
        return std::string();
    }

    std::string txt = "a";
    std::vector<int> used_indices;
    bool append_above_block = true;
    int txt_index = 1;
    while (txt_index < z_size) {
        if (z[txt_index] != 0) {
            used_indices.clear();
            int prefix_index = 0;
            int remaining_block_size = z[txt_index];
            while (remaining_block_size > 0) {
                if (z[txt_index] > remaining_block_size) {
                    remaining_block_size = z[txt_index];
                    used_indices.push_back(z[txt_index]);
                    prefix_index = 0;
                }
                if (z[txt_index] == remaining_block_size) {
                    used_indices.push_back(z[txt_index]);
                }
                txt += txt[prefix_index];
                ++prefix_index;
                ++txt_index;
                --remaining_block_size;
            }
            append_above_block = true;
        } else {
            if (append_above_block) {
                std::vector<bool> used_chars(kAlphabetSize, false);
                for (int n : used_indices) {
                    used_chars[txt[n] - 'a'] = true;
                }
                char c = 'b';
                while (used_chars[c - 'a']) {
                    ++c;
                }
                txt += c;
                append_above_block = false;
            } else {
                txt += 'b';
            }
            ++txt_index;
        }
    }
    return txt;
}

std::vector<int> ZToPrefix(const std::vector<int> &z) {
    const int z_size = z.size();
    std::vector<int> prefix(z_size, 0);
    for (int i = 1; i < z_size; ++i) {
        for (int j = z[i] - 1; j >= 0 && prefix[i + j] >= 0; --j) {
            prefix[i + j] = j + 1;
        }
    }
    return prefix;
}

char GetNewChar(const std::vector<int> &prefix, const std::string &txt, int pos) {
    std::vector<bool> used(kAlphabetSize, false);
    int symbol = prefix[pos - 1];
    while (symbol > 0) {
        used[txt[symbol] - 'a'] = true;
        symbol = prefix[symbol - 1];
    }
    char new_character = 'b';
    while (used[new_character - 'a']) {
        ++new_character;
    }
    return new_character;
}

std::string PrefixToString(const std::vector<int> &prefix) {
    const int prefix_size = prefix.size();
    if (prefix_size == 0) {
        return std::string();
    }
    std::string txt = "a";
    for (int i = 1; i < prefix_size; ++i) {
        if (prefix[i] == 0) {
            txt += GetNewChar(prefix, txt, i);
        } else {
            txt += txt[prefix[i] - 1];
        }
    }
    return txt;
}

void CheckResult() {
    {
        std::string min_string = "aabbabbaba";

        assert((ZToString(PrefixToZ(StringToPrefix(min_string)))) == min_string);
        assert(PrefixToString(ZToPrefix(StringToZ(min_string))) == min_string);
    }
    {
        std::string string_ = "hello";
        std::string min_string = "abbbb";
        assert((ZToString(PrefixToZ(StringToPrefix(string_)))) == min_string);
        assert(PrefixToString(ZToPrefix(StringToZ(string_))) == min_string);
    }

    {
        std::string string_ = "cuchcuc";
        std::string min_string = "abacaba";
        assert((ZToString(PrefixToZ(StringToPrefix(string_)))) == min_string);
        assert(PrefixToString(ZToPrefix(StringToZ(string_))) == min_string);
    }


    {
        std::string min_string = "abacaba";
        std::vector<int> prefix = {0, 0, 1, 0, 1, 2, 3};
        std::vector<int> z = {0, 0, 1, 0, 3, 0, 1};
        std::vector<int> min_prefix = {0, 0, 1, 0, 1, 2, 1};

        assert(StringToZ(min_string) == z);
        assert(StringToPrefix(min_string) == prefix);
        assert(PrefixToZ(prefix) == z);
        assert(ZToString(z) == min_string);
        assert(PrefixToString(prefix) == min_string);
        assert(ZToPrefix(z) == min_prefix);
        assert(PrefixToZ(prefix) == z);
    }

    std::cout << "OK" << std::endl;
}

int main() {
    CheckResult();
    return 0;
}
