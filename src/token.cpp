#include "token.h"
#include "string"
using namespace std;
bool startswith(string s1, string s2)
{
    string t = s1.substr(0, s2.length());
    return t == s2;
}
/*
struct Token
{
    std::string type;
    char *start, *end; // TokenName   start是首指针,end是尾后指针
    int val;           //数字,字符常量
    double dval;       //浮点数常量
};
void errorToken(char* buf, char* start, char* end, std::string msg);
{}
*/

using namespace std;
std::vector<Token> tokenArr;

void scan(char* buf)
{
    char* path;
    int i = 0, len, tlen = 0;
    string s = buf;
    len      = s.length();
    while (i < len)
    {
        //跳过空格,注释//  or  /*
        while (s[i] == ' ')
            i++;
        while (startswith(s.substr(i), "//"))
        {
            i += 2;
            while (s[i++] != '\n')
                ;
        }
        while (startswith(s.substr(i), "/*"))
        {
            i += 2;
            while (!startswith(s.substr(i), "*/"))
                i++;
            i += 2;
        }
        //数字常量
        if (s[i] <= '9' and s[i] >= '0')
        {
            string temp = "";
            int j       = i;
            while (s[j] <= '9' and s[j] >= '0')
            {
                temp += s[j++];
            }
            if (s[j] == '.')
            {
                temp += s[j++];
                while (s[j] <= '9' and s[j] >= '0')
                {
                    temp += s[j++];
                }
            }
            if (s[j] == 'e')
            {
                temp += s[j++];
                if (s[j] == '+' or s[j] == '-')
                    temp += s[j++];
                while (s[j] <= '9' and s[j] >= '0')
                {
                    temp += s[j++];
                }
                if (s[j] == '.')
                {
                    temp += s[j++];
                    while (s[j] <= '9' and s[j] >= '0')
                    {
                        temp += s[j++];
                    }
                }
            }
            if (s[j - 1] == '.')
                errorToken(buf, buf + i, buf + j - 1, path, "数字常量格式错误");
            if (isalpha(s[j]))
                errorToken(buf, buf + i, buf + j, path, "数字常量格式错误");
            j--;
            double tempd;
            tempd = stof(temp);
            tlen  = temp.length();
            // int
            if (tempd == int(tempd))
            {
                struct Token t = {"TK_INUM", buf + i, buf + j, int(tempd), 0};
                tokenArr.push_back(t);
            }
            // double
            else
            {
                struct Token t = {"TK_DNUM", buf + i, buf + j, 0, tempd};
                tokenArr.push_back(t);
            }
        }
        //字符常量
        else if (s[i] == '\'')
        {
            string temp = "";
            int j       = i;
            while (s[++j] != '\'')
            {
                temp += s[j];
            }
            tlen = temp.length() + 2;
            if (tlen != 3)
                errorToken(buf, buf + i, buf + j, path, "字符常量格式错误");
            else
            {
                struct Token t = {"TK_CHAR", buf + i, buf + j, s[j - 1], 0};
                tokenArr.push_back(t);
            }
        }
        //字符串常量
        else if (s[i] == '\"') //为字符串
        {
            string temp = "";
            int j       = i;
            while (s[++j] != '\"')
            {
                temp += s[j];
            }
            struct Token t = {"TK_STR", buf + i, buf + j, 0, 0};
            tokenArr.push_back(t);
            tlen = temp.length() + 2;
        }
        else
        {
            int j = i;
            while (s[j] != ' ' and s[j] != '\n' and s[j] != ';' and j < len)
            {
                j++;
            }
            if (s[j] == ' ' or s[j] == '\n' or s[j] == ';')
                j--;
            tlen        = j - i + 1;
            string temp = s.substr(i, tlen);
            auto iter   = tokenType.find(temp);
            //自定义标识符
            if (iter == tokenType.end())
            {
                struct Token t = {"TK_IDENT", buf + i, buf + j, 0, 0};
                tokenArr.push_back(t);
            }
            //关键字,symbols
            else
            {
                struct Token t = {iter->second, buf + i, buf + j, 0, 0};
                tokenArr.push_back(t);
            }
        }
        i += tlen;
    }
}

char* openFile(const char* path)
{
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

void errorToken(char* buf, char* start, char* end, char* path, std::string msg)
{
    char* s  = buf;
    int line = 0, col = 0;
    for (char* p = buf; p; p++)
    {
        if (*p == '\n')
        {
            s = p + 1;
            line++;
            col = 0;
            continue;
        }
        if (p != start)
        {
            col++;
            continue;
        }
        cout << "错误说明   文件    行号    列号" << endl;
        cout << msg << "   " << path << "   " << line << "   " << col << endl;
        int linelen = strchr(p, '\n') - s;
        printf("%.*s\n", linelen, s);
        // exit(1);
        return;
    }
}