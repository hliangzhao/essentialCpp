#include <string>
using std::string;

// 定义静态数据成员并使用Wrapper类包装它们
class GlobalWrapper {
public:
    static string program_name() {
        return _program_name;
    }
    static string version_name() {
        return _version_stamp;
    }
    static int version_number() {
        return _version_number;
    }
    static int tests_run() {
        return _tests_run;
    }
    static int tests_passed() {
        return _tests_passed;
    }
    
    static void program_name(const string &name) {
        _program_name = name;
    }
    static void version_stamp(const string &stamp) {
        _version_stamp = stamp;
    }
    static void version_number(int num) {
        _version_number = num;
    }
    static void tests_run(int run) {
        _tests_run = run;
    }
    static void tests_passed(int passed) {
        _tests_passed = passed;
    }
    
private:
    // 数据成员声明
    static string _program_name;
    static string _version_stamp;
    static int _version_number;
    static int _tests_run;
    static int _tests_passed;
};

// 数据成员定义
string GlobalWrapper::_program_name;
string GlobalWrapper::_version_stamp;
int GlobalWrapper::_version_number;
int GlobalWrapper::_tests_run;
int GlobalWrapper::_tests_passed;
