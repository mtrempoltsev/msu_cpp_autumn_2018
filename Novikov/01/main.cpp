#include <iostream>
#include <cmath>
#include <set>
#include "numbers.dat"

using namespace std;

bool prostoe(int chislo, const int* Data, int Size) {
	if(chislo < 2) return false;
	for (int i = 2; i <= sqrt(chislo); i++)
		if(chislo % i == 0) return false;
	return true;
}

void obrabotka(multiset<int> chisla, int argc, char* argv[], const int* Data, int Size) {
	for(int n = 1; n <= argc - 1; n += 2) {
		int kolvo_vseh = 0;
		int nachalo = atoi(argv[n]), konec = atoi(argv[n + 1]);
		if(chisla.count(nachalo) && chisla.count(konec)) {
			for(int i = nachalo; i <= konec; i++) {
				int kolvo = chisla.count(i);
				if(kolvo && prostoe(i, Data, Size)) kolvo_vseh += kolvo;
			}
		}
		cout << kolvo_vseh << '\n';
	}
}

int main(int argc, char* argv[]) {
	if((argc == 1) || (argc % 2 == 0)) return -1;
	multiset<int> chisla;
	for(int i = 1; i < Size; i++) chisla.insert(Data[i]);
	obrabotka(chisla, argc, argv, Data, Size);
    return 0;
}