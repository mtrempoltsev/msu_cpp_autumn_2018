#include"numbers.dat"
#include<iostream>
int isProst(int k){
    for(int i=2; i * i < k + 1; i++)
        if(k % i==0)
            return 0;
    if(k==1)
        return 0;
    return 1;
}
int atoi(char* c){
    int res=0;
    char digit;
    while(*c != '\0'){
        digit = *c++ - '0';
        if((digit>9)||(digit<0)){
            return -1;
        }
        res = res*10 + digit;
    }
    return res;
}
int binSearch(int k, int left, int right){
    int med = (left+right)/2;
    if(!(left < right))
        return left;
    if(Data[left] > k)
        return left;
    if(Data[med] >= k)
        return binSearch(k,left, med);
    return binSearch(k, med + 1, right);
}
int elementInData(int el){
    return el == Data[binSearch(el, 0, Size)];
}
int main(int argc,char* argv[]){
    if(((argc-1)%2!=0)||(argc==1))
        return -1;
    int *mas = new int[argc / 2];
    for(int i = 1; i < argc; i += 2){
        int counter = 0;
        int nijn = atoi(argv[i]);
        int verhn = atoi(argv[i+1]);
        if(!(elementInData(verhn) && elementInData(nijn))){
            delete []mas;
            return -1;
        }
        int elemind = binSearch(nijn,0,Size);
        while(Data[elemind]<=verhn)
            counter += isProst(Data[elemind++]);
        mas[i/2] = counter;
    }
    for(int i = 0; i < argc / 2; i++)
        std::cout<<mas[i]<<'\n';
    delete []mas;
}
