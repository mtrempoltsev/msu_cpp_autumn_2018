#include <iostream>
#include <set>
#include "numbers.dat"
using namespace std;

bool is_prime(int a) {
    if (a < 2){
        return false;
    }
    if (a == 2) {
        return true;
    }
    if (a % 2 == 0) {
        return false;
    }
    for (int i = 3; i * i <= a; i += 2) {
        if (a % i == 0) {
            return false;
        }
    }
    return true;
}
enum { MAX_ELEM = 100000 };
int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    if ((argc & 1) == 0){
        return -1;
    }
    set <int> prime;
    for (int i = 0; i < MAX_ELEM; ++i) {
        if (is_prime(i)){
            prime.insert(i);
        }
    }
    if (argc == 1){
        return -1;
    }
    for (int i = 1; i < argc; i += 2) {
        int st, end;
        if (sscanf(argv[i], "%d", &st) != 1){
            return -1;
        }
        if (sscanf(argv[i + 1], "%d", &end) != 1){
            return -1;
        }
        int count = 0;
        bool flag = false;
        int position = 0;
        while (position < Size && Data[position] < st) {
            ++position;
        }
        if (position == Size || Data[position] != st) {
            cout << 0 << endl;
        } else {
            while (position < Size && Data[position] <= end){
                if (prime.find(Data[position]) != prime.end()){
                    ++count;
                }
                ++position;
            }
            if (position == Size && position > 0 && Data[position - 1] != end) {
                cout << 0 << endl;
            } else {
                cout << count << endl;
            }
        }
    }
    
    return 0;
}
