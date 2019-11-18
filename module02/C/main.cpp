#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <cinttypes>

template<typename It>
std::vector<int> CountSort(const It cbegin, const It cend, size_t size,
                           typename std::enable_if<std::is_integral<
                                   typename std::iterator_traits<It>::value_type>::value
                           >::type * = nullptr) {
    // Функция прнимает итераторы контейнера.
    // Элементы контейнера должны иметь тип integer >= 0 и < size.
    std::vector<int> count(size, 0);
    for (auto it = cbegin; it < cend; ++it) {
        ++count[*it];
    }
    for (int i = 1; i < size; ++i) {
        count[i] += count[i - 1];
    }
    return count;
}

std::vector<int> BuildSuffixArray(std::string txt, std::vector<std::vector<int>> &classes) {
    static const char min_char = '#';
    static const int alphabet_size = 128;

    txt += min_char;
    const int txt_size = txt.size();
    std::vector<int> sort_suffs(txt_size, 0);
    auto count = CountSort(txt.cbegin(), txt.cend(), alphabet_size);

    for (int i = 0; i < txt_size; ++i) {
        sort_suffs[--count[txt[i]]] = i;
    }

    classes.emplace_back(std::vector<int>(txt_size));
    classes[0][sort_suffs[0]] = 0;

    int classesN = 1;
    for (int i = 1; i < txt_size; ++i) {
        if (txt[sort_suffs[i]] != txt[sort_suffs[i - 1]]) {
            ++classesN;
        }
        classes[0][sort_suffs[i]] = classesN - 1;
    }

    std::vector<int> new_suffs(txt_size);
    for (int k = 0; (1 << k) < txt_size; ++k) {
        const int two_pow_k = (1 << k);
        for (int i = 0; i < txt_size; ++i) {
            new_suffs[i] = sort_suffs[i] - two_pow_k;
            if (new_suffs[i] < 0) {
                new_suffs[i] += txt_size;
            }
        }

        count = CountSort(classes[k].cbegin(), classes[k].cend(), classesN);
        for (int i = txt_size - 1; i >= 0; --i) {
            sort_suffs[--count[classes[k][new_suffs[i]]]] = new_suffs[i];
        }

        classes.emplace_back(txt_size);
        classes[k + 1][sort_suffs[0]] = 0;
        classesN = 1;
        for (int i = 1; i < txt_size; i++) {
            const int mid1 = (sort_suffs[i] + two_pow_k) % txt_size;
            const int mid2 = (sort_suffs[i - 1] + two_pow_k) % txt_size;

            if (classes[k][sort_suffs[i]] != classes[k][sort_suffs[i - 1]]
                || classes[k][mid1] != classes[k][mid2]) {
                ++classesN;
            }
            classes[k + 1][sort_suffs[i]] = classesN - 1;
        }
    }
    return sort_suffs;
}

int GetLCPSize(int i, int j, const std::vector <std::vector<int>> &classes) {
    int result = 0;
    for (int k = classes.size() - 2; k >= 0; --k)
        if (classes[k][i] == classes[k][j]) {
            const int two_pow_k = 1 << k;
            i += two_pow_k;
            j += two_pow_k;
            result += two_pow_k;
        }
    return result;
}

std::string KthCommonSubsequence(const std::string &string1,
                                 const std::string &string2, const int64_t k) {
    const std::string txt = string1 + '$' + string2;
    const int string1_size = string1.size();
    const int txt_size = txt.size();

    std::vector<std::vector<int>> classes;
    const auto suffix_array = BuildSuffixArray(txt, classes);

    int iterator_1 = 1;
    int iterator_2 = 1;
    int previous_lcp = 0;
    int64_t result = 0;
    while (result < k) {
        if (iterator_1 < iterator_2) {
            do {
                ++iterator_1;
            } while (suffix_array[iterator_1] > string1_size);
        } else {
            do {
                ++iterator_2;
            } while (suffix_array[iterator_2] <= string1_size);
        }
        if (iterator_1 > txt_size || iterator_2 > txt_size) {
            return "-1";
        }
        const int current_lcp = GetLCPSize(suffix_array[iterator_1], suffix_array[iterator_2], classes);
        result = (current_lcp > previous_lcp) ? (result + current_lcp - previous_lcp) : result;
        previous_lcp = current_lcp;
    }
    const int found_string_size = (result == k) ? previous_lcp : (previous_lcp - result + k);
    return txt.substr(suffix_array[iterator_1], found_string_size);
}

int main() {
    int64_t k;
    std::string s, t;
    std::cin >> s >> t;
    while (std::cin >> k) {
        std::cout << KthCommonSubsequence(s, t, k) << std::endl;
    }
    return 0;
}
