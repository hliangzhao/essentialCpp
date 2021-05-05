#include <iostream>
#include <iomanip>
#include <cstring>
using namespace std;

// 使用字符数组读取用户名并判断长度。
int main() {
    const int name_size = 128;
    char username[name_size];
    // 用操纵符setw保证读入字符个数不会超过127
    cin >> setw(name_size) >> username;
    switch (strlen(username)) {
        case 0:
            cout << "No name entered" << endl;
            break;
        case 1:
            cout << "User with only 1-char length" << endl;
            break;
        case 127:
            cout << "Maybe the input has a length > 127!" << endl;
        default:
            cout << "Hello, " << username << endl;
    }

    return 0;
}
