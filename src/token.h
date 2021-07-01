#ifndef TOEKN_
    #define TOKEN_
    #include "stdafx.h"


/// token.c
static const std::unordered_map<std::string, std::string> tokenType{
    {"num", "TK_INUM"},
    {"dnum", "TK_DNUM"},
    {"id", "TK_IDENT"},
    {"char", "TK_CHAR"},
    {"str", "TK_STR"},
    {"typedef", "TK_TYPEDEF"},
    {"extern", "TK_EXTERN"},
    {"static", "TK_STATIC"},
    {"auto", "TK_AUTO"},
    {"register", "TK_REGISTER"},
    {"void", "TK_VOID"},
    {"char", "TK_CHAR"},
    {"short", "TK_SHORT"},
    {"int", "TK_INT"},
    {"long", "TK_LONG"},
    {"float", "TK_FLOAT"},
    {"double", "TK_DOUBLE"},
    {"signed", "TK_SIGNED"},
    {"unsigned", "TK_UNSIGNED"},
    {"_Bool", "TK_B00L"},
    {"_Complex", "TK_COMPLEX"},
    {"_Imaginary", "TK_IMAGINARY"},
    {"struct", "TK_STRUCT"},
    {"union", "TK_UNION"},
    {"const", "TK_CONST"},
    {"restrict", "TK_RESTRICT"},
    {"volatile", "TK_VOLATILE"},
    {"sizeof", "TK_SIZEOF"},
    {"enum", "TK_ENUM"},
    {"inline", "TK_INLINE"},
    {"case", "TK_CASE"},
    {"default", "TK_DEFAULT"},
    {"if", "TK_IF"},
    {"else", "TK_ELSE"},
    {"switch", "TK_SWITCH"},
    {"while", "TK_WHILE"},
    {"do", "TK_DO"},
    {"for", "TK_FOR"},
    {"goto", "TK_GOTO"},
    {"break", "TK_BREAK"},
    {"return", "TK_RETURN"},
    {"{", "TK_{"},
    {"}", "TK_}"},
    {";", "TK_;"},
    {",", "TK_,"},
    {":", "TK_:"},
    {"*", "TK_*"},
    {"(", "TK_("},
    {")", "TK_)"},
    {"[", "TK_["},
    {"]", "TK_]"},
    {"?", "TK_?"},
    {"||", "TK_||"},
    {"&&", "TK_&&"},
    {"|", "TK_|"},
    {"^", "TK_^"},
    {"&", "TK_&"},
    {"==", "TK_=="},
    {"!=", "TK_!="},
    {"<", "TK_<"},
    {">", "TK_>"},
    {"<=", "TK_<="},
    {">=", "TK_>="},
    {"<<", "TK_<<"},
    {">>", "TK_>>"},
    {"+", "TK_+"},
    {"-", "TK_-"},
    {"/", "TK_/"},
    {"%", "TK_%"},
    {"++", "TK_++"},
    {"--", "TK_--"},
    {".", "TK_."},
    {"->", "TK_->"},
    {"...", "TK_..."},
    {"=", "TK_="},
    {"~", "TK_~"},
    {"!", "TK_!"},
    {"*=", "TK_*="},
    {"/=", "TK_/="},
    {"%=", "TK_%="},
    {"+=", "TK_+="},
    {"-=", "TK_-="},
    {"<<=", "TK_<<="},
    {">>=", "TK_>>="},
    {"&=", "TK_&="},
    {"^=", "TK_^="},
    {"|=", "TK_|="},
};

//关键字表
static const std::unordered_set<std::string> keywords{
    "typedef",  "extern",   "static", "auto",     "register", "void",     "char",       "short",  "int",   "long",
    "float",    "double",   "signed", "unsigned", "_Bool",    "_Complex", "_Imaginary", "struct", "union", "const",
    "restrict", "volatile", "sizeof", "enum",     "inline",   "case",     "default",    "if",     "else",  "switch",
    "while",    "do",       "for",    "goto",     "continue", "break",    "return"};
//符号表
static const std::unordered_set<std::string> symbols{
    "{",   "}",  ";", ",", ":",  "*",  "(",  ")",  "[",  "]",   "?",   "||", "&&", "|",  "^", "&",
    "==",  "!=", "<", ">", "<=", ">=", "<<", ">>", "+",  "-",   "/",   "%",  "++", "--", ".", "->",
    "...", "=",  "~", "!", "*=", "/=", "%=", "+=", "-=", "<<=", ">>=", "&=", "^=", "|="};

struct Token
{
    std::string type;
    char *start, *end; // TokenName   start是首指针,end是尾后指针
    int val;           //数字,字符常量
    double dval;       //浮点数常量
};

extern std::vector<Token> tokenArr;
extern char* path;

/*
从path中读文件,返回字符串
BUFSIZE是字符串长度
*/
constexpr size_t BUFSIZE = 1e5; // 100 kb
char* openFile(const char* path);

/*
主过程
分割出token, push_back到tokenArr
*/

void scan(char* buf);

/*
TOKEN 错误处理
文件字符串buf
token起止start,end
错误信息msg
-----
要输出行数,以及那行的内容,指出位置
*/
void errorToken(char* buf, char* start, char* end, const char* path, std::string msg);

#endif