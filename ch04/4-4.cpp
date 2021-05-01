#include <iostream>
#include <cstdio>
#include <string>
#include <map>
using namespace std;

class UserProfile {
public:
    enum userLevel {
        Beginner, Intermediate, Advanced, Guru
    };
    UserProfile(string login, userLevel = Beginner);
    UserProfile();

    bool operator==(const UserProfile &rhs);
    bool operator!=(const UserProfile &rhs);

    // 读取数据
    string login() const { return _login; }
    int login_count() const { return _times_logged; }
    int guess_count() const { return _guesses; }
    int correct_guess_count() const { return _correct_guesses; }
    double guess_average() const;
    string level() const;

    // 读取数据
    void reset_login(const string &val) { _login = val; }
    void reset_level(const string &level);
    void reset_level(userLevel val) { _user_level = val; }
    void reset_login_count(int val) { _times_logged = val; }
    void reset_guess_count(int val) { _guesses = val; }
    void reset_correct_guess(int val) { _correct_guesses = val; }
    void update_login_count(int cnt = 1) { _times_logged += cnt; }
    void update_guess_count(int cnt = 1) { _guesses += cnt; }
    void update_correct_guess(int cnt = 1) { _correct_guesses += cnt; }

private:
    string _login;
    int _times_logged;
    int _guesses;
    int _correct_guesses;
    userLevel _user_level;
    // 以下static成员在类的外部定义
    static map<string, userLevel> _level_map;
    static void init_level_map();
};

inline UserProfile::UserProfile(string login, userLevel lvl):
_login(login), _user_level(lvl), _times_logged(1), _guesses(0), _correct_guesses(0)
{}

// 当不提供字符串login时，默认以访客身份登陆（guest），针对每个访客许生成独一无二的会话标志符
// 可以用哈希的方式每次随机生成一串字符，但这里采用的是静态变量的形式，保证每个访客的会话对应的静态变量数值不相同
inline UserProfile::UserProfile():
_login("guest"), _user_level(Beginner), _times_logged(1), _guesses(0), _correct_guesses(0) {
    static int session_id = 0;
    char buf[16];
    sprintf(buf, "%d", session_id++);
    _login += buf;
}

inline bool UserProfile::operator==(const UserProfile &rhs) {
    if (_login == rhs._login && _user_level == rhs._user_level) return true;
    return false;
}

inline bool UserProfile::operator!=(const UserProfile &rhs) {
    return !(*this == rhs);
}

inline double UserProfile::guess_average() const {
    return _guesses? double(_correct_guesses) / double(_guesses) * 100: 0.0;
}

inline string UserProfile::level() const {
    static string _level_table[] = {"Beginner", "Intermediate", "Advanced", "Guru"};
    return _level_table[_user_level];
}

// 静态成员的外部定义
map<string, UserProfile::userLevel> UserProfile::_level_map;
void UserProfile::init_level_map() {
    _level_map["Beginner"] = Beginner;
    _level_map["Intermediate"] = Intermediate;
    _level_map["Advanced"] = Advanced;
    _level_map["Guru"] = Guru;
}

// 如果设置的level不对，则置为beginner
inline void UserProfile::reset_level(const string &level) {
    map<string, userLevel>::iterator it;
    if (_level_map.empty()) init_level_map();
    _user_level = ((it = _level_map.find(level)) != _level_map.end())? it->second: Beginner;
}

// 重载流运算符
ostream& operator<<(ostream &os, const UserProfile &rhs) {
    os << rhs.login() << " " << rhs.level() << " "
    << rhs.login_count() << " " << rhs.guess_count() << " "
    << rhs.correct_guess_count() << " " << rhs.guess_average() << "%" << endl;
    return os;
}

istream& operator>>(istream &is, UserProfile &rhs) {
    // 并未对读取做任何检查
    string login, level;
    is >> login >> level;
    int l_cnt, g_cnt, g_correct;
    is >> l_cnt >> g_cnt >> g_correct;
    rhs.reset_login(login);
    rhs.reset_level(level);
    rhs.reset_login_count(l_cnt);
    rhs.reset_guess_count(g_cnt);
    rhs.reset_correct_guess(g_correct);
    return is;
}

int main() {
    UserProfile anon;
    cout << anon;
    UserProfile anon2;
    cout << anon2;

    UserProfile anna("AnnaL", UserProfile::Guru);
    cout << anna;
    anna.update_guess_count(25);
    anna.update_correct_guess(21);
    anna.update_login_count();
    cout << anna;

    cin >> anon;
    cout << anon;

    return 0;
}

