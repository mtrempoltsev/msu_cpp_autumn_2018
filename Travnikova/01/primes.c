    #include <iostream>
    #include "numbers.dat"
     
    using namespace std;
     
    enum { MAX_NUMBER = 100000 };
     
    char is_prime(int x) {
    	if (x == 2) {
    		return true;
    	}
    	for (int i = 2; i * i <= x; i++) {
    		if (x % i == 0) {
    			return 0;
    		}
    	}
    	if (x < 2) {
    		return false;
    	}
    	return true;
    }
     
    int main(int argc, char **argv)
    {
     
    	if ((argc & 1) == 0 || argc == 1) {
    		return -1;
    	}
    	int *pos_first = (int*)malloc((MAX_NUMBER + 1) * sizeof(*pos_first));
    	int *pos_last = (int*)malloc((MAX_NUMBER + 1) * sizeof(*pos_last));
    	char *primes = (char*)malloc(MAX_NUMBER + 1);
    	for (int i = 0; i < MAX_NUMBER + 1; i++) {
    		pos_first[i] = -1;
    		pos_last[i] = -1;
    	}
    	for (int i = 0; i < Size; i++) {
    		if (pos_first[Data[i]] == -1) {
    			pos_first[Data[i]] = i;
    		}
    		pos_last[Data[i]] = i;
    		primes[Data[i]] = is_prime(Data[i]);
    	}
    	for (int i = 1; i < argc; i += 2) {
    		int first = atoi(argv[i]);
    		int last = atoi(argv[i + 1]);
    		int ans = 0;
    		if ((pos_first[first] != -1) && (pos_last[last] != -1) && (pos_first[first] <= pos_last[last])) {
    			for (int j = pos_first[first]; j <= pos_last[last]; j++) {
    				if (primes[Data[j]]) {
    					ans++;
    				}
    			}
    		}
    		cout << ans << endl;
    	}
    	free(pos_first);
    	free(pos_last);
    	free(primes);
        return 0;
    }
