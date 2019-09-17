#include <iostream>
#include <vector>
#include <string>
void Find(const std::string & haystack, const std::string & needle) {

    int const haystack_size = haystack.size();
    int const needle_size = needle.size();

    std::vector<int> z (needle_size, 0);

    for (int i = 1; i < needle_size; ++i) {
        int curr = 0;
        while (i + curr < needle_size && needle[curr + i] == needle[curr]) {
            ++curr;
        }
        z[i] = curr;
    }

    int left = 0;
    int right = 0;

    for (int i = 0; i < haystack_size; ++i) {
        int curr = i <= right ? std::min(z[i - left], right - i + 1) : 0;
        while (curr + i < haystack_size
                && curr < needle_size
                && haystack[i + curr] == needle[curr]) {
            ++curr;
        }
        if (curr == needle_size) {
            std::cout << i << " ";
        }
        if (curr + i - 1 > right) {
            left = i;
            right = curr + i - 1;
        }
    }
}
int main() {
    std::string needle, haystack;
    std::cin >> needle >> haystack;

    Find(haystack, needle);

    return 0;
}