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


void errorToken(char* buf, char* start, char* end, char *path,std::string msg) {
    char* s = buf;
    int line = 0, col = 0;
    for (char* p = buf; p; p++) {
        if (*p == '\n') {
            s = p + 1;
            line++;
            col = 0;
            continue;
        }
        if (p != start) {
            col++;
            continue;
        }
        cout << "错误说明   文件    行号    列号" << endl;
        cout << msg << "   " << path << "   " << line << "   " << col << endl;
        int linelen = strchr(p, '\n') - s;
        printf("%.*s\n", linelen, s);
        //exit(1);
        return;
    }
}