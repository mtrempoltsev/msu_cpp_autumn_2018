#include <iostream>
#include <stdexcept>
#include "numbers.dat"


// flag = 0 - left-bounded search
//        1 - right-bounded search
int bsearch(int key, int flag) 
{
    int left = 0;
    int right = Size;
    while (left < right) {
        int mid = (left + right) / 2;
        if (flag ? Data[mid] <= key : Data[mid] < key) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    return right;
}

bool is_prime(int n)
{
    if (n == 1) {
        return false;
    }

    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }

    return true;
}

int main(int argc, char **argv)
{
    constexpr int MIN_BORDER = 0;
    constexpr int MAX_BORDER = 100000;
    
    if (argc < 3 || !(argc % 2)) {
        return -1;
    }

    for (int i = 1; i < argc; i += 2) {
        int count = 0;
        int left = atoi(argv[i]);
        int right = atoi(argv[i + 1]);

        if (left < MIN_BORDER || right < MIN_BORDER || left > MAX_BORDER || right > MAX_BORDER) {
            return -1;
        }

        std::pair<int, int> borders = std::make_pair(bsearch(left, 0), bsearch(right, 1));
        if (Data[borders.first] == left && Data[borders.second - 1] == right) {
            for (int index = borders.first; index < borders.second; index++) {
                if (is_prime(Data[index])) {
                    count++;
                }
            }
        }
        std::cout << count << std::endl;
    }    
    
    return 0;
}
