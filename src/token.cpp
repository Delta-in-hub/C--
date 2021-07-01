#include "token.h"
using namespace std;
extern std::vector<Token> tokenArr{};

void scan(char* buf)
{
    char* p = buf;

    while (*p)
    {
        //跳过空格,注释//  or  /*

        //标识符
        //关键字

        // symbols
        //字符常量
        //数字常量 -> int/double
        //字符串常量
    }
}



char* openFile(const char* path) {
    ifstream file(path);
    char* buf = new char[BUFSIZE];
    while (file.getline(buf,1000)) {

    }
    return buf;
}