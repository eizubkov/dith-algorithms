#include <iostream>
#include <vector>
#include <string>

std::vector<int> buildSuffixArray(const std::string &txt) {

    std::string txt_copy = txt;
    txt_copy += '$';
    const int txt_size = txt_copy.size();

    const int alphabet_size = 128;
    std::vector<int> count(alphabet_size, 0);

    for (int i = 0; i < txt_size; ++i) {
        ++count[txt_copy[i]];
    }

    for (int i = 1; i < alphabet_size; ++i) {
        count[i] += count[i - 1];
    }

    std::vector<int> suffs(txt_size, 0);
    for (int i = 0; i < txt_size; ++i) {
        suffs[--count[txt_copy[i]]] = i;
    }

    std::vector<std::vector<int>> classes(1, std::vector<int>(txt_size));
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
                || classes[k][mid1] != classes[k][mid2]) {
                classesN++;
            }
            classes[k + 1][suffs[i]] = classesN - 1;
        }
    }

    suffs.erase(suffs.begin());
    return suffs;
}


std::vector<int> kasai(const std::string &txt,
                       const std::vector<int> &suffix_array) {

    int suffix_array_size = suffix_array.size();

    std::vector<int> lcp(suffix_array_size, 0);
    std::vector<int> inv_suffs(suffix_array_size, 0);

    for (int i = 0; i < suffix_array_size; i++) {
        inv_suffs[suffix_array[i]] = i;
    }

    int k = 0;
    for (int i = 0; i < suffix_array_size; i++) {
        if (inv_suffs[i] != suffix_array_size - 1) {
            int j = suffix_array[inv_suffs[i] + 1];
            while (i + k < suffix_array_size && j + k < suffix_array_size
                   && txt[i + k] == txt[j + k]) {
                ++k;
            }
            lcp[inv_suffs[i]] = k;
            k = (k > 0) ? k - 1 : k;
        } else {
            k = 0;
        }
    }

    return lcp;
}

int countDistinctSubstring(const std::string &txt) {
    int txt_size = txt.size();
    std::vector<int> suffix_array = buildSuffixArray(txt);
    std::vector<int> lcp = kasai(txt, suffix_array);

    int result = txt_size - suffix_array[0];

    for (int i = 1; i < lcp.size(); i++) {
        result += (txt_size - suffix_array[i]) - lcp[i - 1];
    }
    return result;
}

int main() {
    std::string txt;
    std::cin >> txt;
    std::cout << countDistinctSubstring(txt);
    return 0;
}