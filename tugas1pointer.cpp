#include <iostream>
using namespace std;
int main() {
int nums[] = {5,10,15,20,25,30}; //jumlah array
int i = 6;
cout << nums << endl; //alamat array
cout << *nums << endl; //elemen pertama
cout << *(nums+i) << endl; //elemen kedua

int max = *nums;
int min = *nums;
int* ptr = nums;          
int* end = nums + i;      //pointer ke satu elemen setelah terakhir
 while(ptr < end) {        
   if(*ptr > max) max = *ptr;
    if(*ptr < min) min = *ptr;
     ptr++;               
    }
    cout << "Nilai maksimum: " << max << endl;
    cout << "Nilai minimum: " << min << endl;
 
int sum = 0;
int* ptr_sum = nums;
int* end_sum = nums + i;
		while(ptr_sum < end_sum) {
        sum += *ptr_sum;
        ptr_sum++;
    }

    int average = sum / i; // pembagian integer

    cout << "Jumlah semua elemen: " << sum << endl;
    cout << "Rata-rata (integer): " << average << endl;
}
