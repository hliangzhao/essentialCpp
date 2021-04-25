#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

inline int int_max(int t1, int t2) {
    return (t1 > t2)? t1: t2;
}

inline float float_max(float t1, float t2) {
    return (t1 > t2)? t1: t2;
}

template <typename Type> inline Type max(Type t1, Type t2) {
    return (t1 > t2)? t1: t2;
}

inline int max(const vector<int> &vec) {
    return *max_element(vec.begin(), vec.end());
}

inline string max(const vector<string> &vec) {
    return *max_element(vec.begin(), vec.end());
}

template <typename elemType> inline elemType max(const vector<elemType> &vec) {
    return *max_element(vec.begin(), vec.end());
} 

inline int max(const int *arr_ptr, int size) {
    return *max_element(arr_ptr, arr_ptr + size);
}

inline string max(const string *arr_ptr, int size) {
    return *max_element(arr_ptr, arr_ptr + size);
}

template <typename elemType> inline elemType max(const elemType *arr_ptr, int size) {
    return *max_element(arr_ptr, arr_ptr + size);
}

int main() {
    // some test code
}

