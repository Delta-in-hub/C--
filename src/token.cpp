#include "token.h"

std::vector<Token> tokenArr{};

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
    FILE* file;
    char* buf1 = new char[100];
    char* buf  = new char[100];

    file = fopen(path, "r");
    if (!file)
    {
        perror(path);
        exit(1);
    }
    fgets(buf, 100, file);
    while (fgets(buf1, 100, file))
    {
        strcat(buf, buf1);
    }
    // fgets(buf, 100, file);
    return buf;
}