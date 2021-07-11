#include "token.h"
using namespace std;

std::vector<Token> tokenArr;
const char* path = nullptr;
const char* buferror;

static inline void sortSymbolArr()
{
    sort(symbols.begin(), symbols.end(), [](const string& a, const string& b) { return a.length() > b.length(); });
}

static inline bool startsWith(const string_view& s1, const string& s2)
{
    return s1.substr(0, s2.length()) == s2;
}


/*
<文件名>:Line 5:6: error: 错误信息
打印那一行
*/

void errorToken(const char* buf, const char* start, const char* end, const char* path, std::string msg)
{
    const char* s = buf;
    int line = 0, col = 0;
    for (const char* p = buf; p; p++)
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
        cout << '<' << path << ">:" << "Line " << line << ':' << col << ": error: " << msg << endl;
        //cout << "error\t\t\tfile\t\tline\tcol" << endl;
        //cout << msg << "\t" << path << "\t" << line << "\t" << col << endl;
        int linelen = strchr(p, '\n') - s;
        printf("%.*s\n", linelen, s);
#ifdef DEBUG__
        getchar();
#endif
        exit(1);
        // return;
    }
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

void scan(const char* buf)
{

    int i = 0, len, tlen = 0;
    string s = buf;
    len      = s.length();
    while (i < len)
    {
        //跳过空格,注释//  or  /*
        if (isspace(s[i]))
        {
            i++;
            continue;
        }
        if (startsWith(string_view(s.data() + i), "//")) //原来每次判断都把剩下的所有字符复制一遍生成一个新的std::string
        {
            i += 2;
            while (s[i++] != '\n')
                ;
            continue;
        }
        if (startsWith(string_view(s.data() + i), "/*"))
        {
            i += 2;
            while (!startsWith(string_view(s.data() + i), "*/"))
                i++;
            i += 2;
            continue;
        }
        //数字常量
        if (s[i] <= '9' and s[i] >= '0')
        {
            bool isint  = true;
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
                    isint = false;
                    temp += s[j++];
                    while (s[j] <= '9' and s[j] >= '0')
                    {
                        temp += s[j++];
                    }
                }
            }
            if (s[j - 1] == '.')
                errorToken(buf, buf + i, buf + j - 1, path, "number type error");
            if (isalpha(s[j]))
                errorToken(buf, buf + i, buf + j, path, "number type error");
            j--;
            double tempd;
            tempd = stof(temp);
            tlen  = temp.length();
            // ! int   这不合理吧 改了
            if (isint == true)
            {
                struct Token t = {"TK_INUM", buf + i, buf + j + 1, int(tempd), 0};
                tokenArr.push_back(move(t));
            }
            // double
            else
            {
                struct Token t = {"TK_DNUM", buf + i, buf + j + 1, 0, tempd};
                tokenArr.push_back(move(t));
            }
        }
        //字符常量
        else if (s[i] == '\'')
        {
            string temp = "";
            int j       = i;
            while (s[++j] != '\'')
            {
                if (s[j] == '\\' and s[j + 1] == '\'')
                {
                    temp += s[j++];
                }
                temp += s[j];
            }
            tlen = temp.length() + 2;
            if (tlen > 4 or tlen < 3) // ! 转义字符   like  len(`\n`) == 4
                errorToken(buf, buf + i, buf + j + 1, path, "字符常量格式错误");
            else
            {
                bool flag = true;
                if (tlen == 4)
                {
                    flag = false;
                    for (auto&& esc : escape)
                    {
                        if (esc == temp)
                        {
                            flag = true;
                            break;
                        }
                    }
                }
                if (flag)
                {
                    struct Token t = {"TK_CHARLITERAL", buf + i, buf + j + 1, s[j - 1], 0};
                    tokenArr.push_back(move(t));
                }
                else
                {
                    errorToken(buf, buf + i, buf + j + 1, path, "字符常量格式错误");
                }
            }
        }
        //字符串常量
        else if (s[i] == '\"') //为字符串
        {
            string temp = "";
            int j       = i;
            while (s[++j] != '\"') // ! 这里也有转义的问题 改了
            {
                if (s[j] == '\\' and s[j + 1] == '\"')
                {
                    temp += s[j++];
                }
                temp += s[j];
            }
            struct Token t = {"TK_STR", buf + i, buf + j + 1, 0, 0};
            tokenArr.push_back(move(t));
            tlen = temp.length() + 2;
        }
        else
        {
            int j = i;
            while (isdigit(s[j]) or isalpha(s[j]) or s[j] == '_' and j <= len - 1)
            {
                j++;
            }
            // if (j < len - 1)
            j--;
            tlen = j - i + 1;
            string temp;
            if (tlen > 0)
            {
                temp      = s.substr(i, tlen);
                auto iter = keywords.find(temp);
                //自定义标识符
                if (iter == keywords.end())
                {
                    struct Token t = {"TK_IDENT", buf + i, buf + j + 1, 0, 0};
                    tokenArr.push_back(move(t));
                }
                //关键字
                else
                {
                    struct Token t = {tokenType.at(temp), buf + i, buf + j + 1, 0, 0};
                    tokenArr.push_back(move(t));
                }
            }
            // symbols
            else
            {
                bool flag = false;
                for (auto&& sym : symbols)
                {
                    if (startsWith(string_view(s.data() + i), sym))
                    {
                        struct Token t = {tokenType.at(sym), buf + i, buf + i + sym.length(), 0, 0};
                        tlen           = sym.length();
                        tokenArr.push_back(move(t));
                        flag = true;
                        break;
                    }
                }
                if (not flag)
                {
                    errorToken(buf, buf + i, buf + j + 1, path, "标识符格式错误");
                }
            }
        }
        i += tlen;
    }
    tokenArr.push_back({"TK_EOF", buf + i, buf + i + 1, 0, 0});
}

char* openFile(const char* path)
{
    sortSymbolArr();
    FILE* file;
    char* buf1 = new char[100];
    char* buf  = new char[BUFSIZE];

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

void errorParse(const Token& tk, const std::string& error = "Universal Error")
{
    errorToken(buferror, tk.start, tk.end, path, error.data());
}