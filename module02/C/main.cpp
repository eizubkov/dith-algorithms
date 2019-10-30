#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cinttypes>


std::vector<int> buildSuffixArray(const std::string &txt,
                                  std::vector<std::vector<int>> &classes) {
    std::string txt_copy = txt;
    txt_copy += '#';
    const int txt_size = txt_copy.size();
    const int alphabet_size = 128;
    std::vector<int> suffs(txt_size, 0);
    std::vector<int> count(alphabet_size, 0);

    for (int i = 0; i < txt_size; ++i) {
        ++count[txt_copy[i]];
    }
    for (int i = 1; i < alphabet_size; ++i) {
        count[i] += count[i - 1];
    }

    for (int i = 0; i < txt_size; ++i) {
        suffs[--count[txt_copy[i]]] = i;
    }

    classes.emplace_back(std::vector<int>(txt_size));
    classes[0][suffs[0]] = 0;

    int classesN = 1;

    for (int i = 1; i < txt_size; ++i) {
        if (txt_copy[suffs[i]] != txt_copy[suffs[i - 1]]) {
            ++classesN;
        }
        classes[0][suffs[i]] = classesN - 1;
    }

    std::vector<int> new_suffs(txt_size);
    for (int k = 0; (1 << k) < txt_size; ++k) {
        for (int i = 0; i < txt_size; ++i) {
            new_suffs[i] = suffs[i] - (1 << k);
            if (new_suffs[i] < 0) {
                new_suffs[i] += txt_size;
            }
        }
        count.assign(classesN, 0);

        for (int i = 0; i < txt_size; ++i) {
            count[classes[k][new_suffs[i]]]++;
        }

        for (int i = 1; i < classesN; ++i) {
            count[i] += count[i - 1];
        }

        for (int i = txt_size - 1; i >= 0; --i) {
            suffs[--count[classes[k][new_suffs[i]]]] = new_suffs[i];
        }

        classes.emplace_back(txt_size);
        classes[k + 1][suffs[0]] = 0;
        classesN = 1;

        for (int i = 1; i < txt_size; i++) {
            int mid1 = (suffs[i] + (1 << k)) % txt_size;
            int mid2 = (suffs[i - 1] + (1 << k)) % txt_size;

            if (classes[k][suffs[i]] != classes[k][suffs[i - 1]]
                | classes[k][mid1] != classes[k][mid2]) {
                ++classesN;
            }
            classes[k + 1][suffs[i]] = classesN - 1;
        }
    }
    return suffs;
}

int lcp(int i, int j, const std::vector<std::vector<int >> &classes, int n) {
    int result = 0;
    for (int k = log2(n); k >= 0; --k)
        if (classes[k][i] == classes[k][j]) {
            i += 1 << k;
            j += 1 << k;
            result += 1 << k;
        }
    return result;
}

std::string kthCommonSubsequence(const std::string &string1,
                                 const std::string &string2, const uint64_t k) {
    uint64_t result = 0;
    int prev_lcp = 0;
    int diff = 0;
    std::string txt = string1 + '$' + string2;

    std::vector<std::vector<int >> classes;
    std::vector<int> suffs = buildSuffixArray(txt, classes);
    int iterator_1 = 1, iterator_2 = 1;
    std::string answer;

    while (result < k) {
        if (iterator_1 < iterator_2) {
            for (int i = iterator_1 + 1;; ++i) {
                if (i > txt.size()) {
                    return "-1";
                }
                if (suffs[i] <= string1.size()) {
                    iterator_1 = i;
                    break;
                }
            }
        } else {
            for (int i = iterator_2 + 1;; ++i) {
                if (i > txt.size()) {
                    return "-1";
                }
                if (suffs[i] > string1.size()) {
                    iterator_2 = i;
                    break;
                }
            }
        }
        int curr_lcp = lcp(suffs[iterator_1], suffs[iterator_2], classes, txt.size());
        diff = curr_lcp - prev_lcp;
        if (diff > 0) {
            result += diff;
        }
        prev_lcp = curr_lcp;
    }
    if (result == k) {
        for (int i = suffs[iterator_1]; i < suffs[iterator_1] + prev_lcp; ++i) {
            answer += txt[i];
        }
    } else if (result > k) {
        for (int i = suffs[iterator_1]; i < suffs[iterator_1] + prev_lcp - result + k; ++i) {
            answer += txt[i];
        }
    }
    return answer;
}


int main() {
    uint64_t k;
    std::string s, t;
    std::cin >> s >> t;
    while (std::cin >> k) {
        std::cout << kthCommonSubsequence(s, t, k);

    }
    return 0;
}