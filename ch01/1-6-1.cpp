#include <iostream>
using namespace std;

int main() {
    const int max_size = 128;
    int arr[max_size];
    int v, i = 0;
    while(cin >> v && i < max_size) {
        arr[i++] = v;
    }
    int sum = 0;
    for (int idx = 0; idx < i; idx++) {
        sum += arr[idx];
    }
    cout << sum << " " << (double)sum / i << endl;

    return 0;
}
