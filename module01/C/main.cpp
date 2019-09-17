#include <iostream>
#include <vector>
#include <string>


void PrintEntries(const std::string& haystack, const std::string& needle) {

    const int haystack_size = haystack.size();
    const int needle_size = needle.size();

    std::vector<int> z(needle_size, 0);

    for (int i = 1; i < needle_size; ++i) {
        int shift = 0;
        while (i + shift < needle_size && needle[shift + i] == needle[shift]) {
            ++shift;
        }
        z[i] = shift;
    }

    int left = 0;
    int right = 0;

    for (int i = 0; i < haystack_size; ++i) {
        int shift = (i <= right) ? std::min(z[i - left], right - i + 1) : 0;
        while (shift + i < haystack_size && shift < needle_size
               && (haystack[i + shift] == needle[shift]
               || (needle[shift] == '?'))) {
            ++shift;
        }
        if (shift == needle_size) {
            std::cout << i << " ";
        }
        if (shift + i - 1 > right) {
            left = i;
            right = shift + i - 1;
        }
    }
    std::cout << std::endl;
}
int main() {
    std::string needle, haystack;
    std::cin >> needle >> haystack;

    PrintEntries(haystack, needle);

    return 0;
}
