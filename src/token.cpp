#include "token.h"
#include "string"
using namespace std;
extern std::vector<Token> tokenArr{};
bool startswith(string s1, string s2) {
	string t = s1.substr(0, s2.length());
	return t==s2;
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
std::vector<Token> tokenArr{};

void scan(char* buf)
{
	int i=0, len,tlen=0;
    string s = buf;
	len = s.length();
	while (i < len)
	{
		string typ;
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
		string ts = s.substr(i);
		if (s[i] <= '9' and s[i] >= '0')//为常数
		{
			string temp = "";
			int j = i;
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
				errorToken(&s[0],&(s.substr(i))[0],&(s.substr(j))[0],"数字格式错误");
			if (isalpha(s[j]))
				errorToken(&s[0], &(s.substr(i))[0], &(s.substr(j))[0], "数字格式错误");
			double tempd;
			tempd = stof(temp);
			tlen = temp.length();
			if (tempd == int(tempd))
			{
				struct t={};
			}
			else
			{
				typ = "C2";
				t = strcom(temp, c2, lenc2);
				if (t > 0)
				{
					tlen = c2[t - 1].length();
				}
				else
				{
					c2[lenc2] = temp;
					lenc2++;
				}
				t = lenc2;
			}
		}
		/*
		else if (s[i] == '\'')//为字符
		{
			string temp = "";
			int j = i;
			while (s[++j] != '\'')
			{
				temp += s[j];
			}
			typ = "CT";
			t = strcom(temp, ct, lenct);
			if (t > 0)
			{
				tlen = ct[t - 1].length();
			}
			else
			{
				ct[lenct] = temp;
				lenct++;
			}
			tlen = temp.length() + 2;
			if (tlen != 3)
				error();
			t = lenct;
		}
		else if (s[i] == '\"')//为字符串
		{
			string temp = "";
			int j = i;
			while (s[++j] != '\"')
			{
				temp += s[j];
			}
			typ = "ST";
			t = strcom(temp, st, lenst);
			if (t > 0)
			{
				tlen = st[t - 1].length();
			}
			else
			{
				st[lenst] = temp;
				lenst++;
			}
			tlen = temp.length() + 2;
			t = lenst;
		}
		else
		{
			t = strcom(ts, p, lenp);//判断是否为界符
			if (t > 0)
			{
				tlen = p[t - 1].length();
				typ = 'P';
			}
			else
			{
				t = strcom(ts, k, lenk);//判断是否为关键字
				if (t > 0)
				{
					tlen = k[t - 1].length();
					typ = 'K';
				}
				else
				{
					t = strcom(ts, ti, leni);//判断是否为标识符
					typ = 'I';
					if (t > 0)
					{
						tlen = ti[t - 1].length();
					}
					else//若找不到，加入标识符表
					{
						string temp = "";
						int j = i;
						string ts = s.substr(j);
						bool bo = true;
						while (bo)
						{
							temp += s[j++];
							ts = s.substr(j);
							if (j >= len)break;
							if (strcom(ts, p, lenp) > 0 or strcom(ts, k, lenk) > 0
								or strcom(ts, ti, leni) > 0)
								bo = false;
							if (s[j] == ' ')
							{
								bo = false;
								i++;
							}
						}
						ti[leni] = temp;
						leni++;
						t = leni;
						tlen = temp.length();
					}
				}
			}

		}
		i += tlen;
		string st;
		st = to_string(t);
		token += "(" + typ + " " + st + ")";
	}
    while (*p)
    {
        //跳过空格,注释//  or  /*
		while(*p++==' ')

        //标识符
        //关键字

        // symbols
        //字符常量
        //数字常量 -> int/double
        //字符串常量
    }
}

*/

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