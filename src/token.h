#ifndef TOKEN_
#define TOKEN_

#include "stdafx.h"

/// token.c
static const std::unordered_map<std::string, std::string> tokenType{
    {"num", "TK_INUM"},
    {"dnum", "TK_DNUM"},
    {"id", "TK_IDENT"},
    {"lchar", "TK_CHARLITERAL"},
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
    {"float", "TK_DOUBLE"}, //flaot same to double
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
    {"EOF", "TK_EOF"},
};

//????????????
static const std::unordered_set<std::string> keywords{
    "typedef",  "extern",   "static", "auto",     "register", "void",     "char",       "short",  "int",   "long",
    "float",    "double",   "signed", "unsigned", "_Bool",    "_Complex", "_Imaginary", "struct", "union", "const",
    "restrict", "volatile", "sizeof", "enum",     "inline",   "case",     "default",    "if",     "else",  "switch",
    "while",    "do",       "for",    "goto",     "continue", "break",    "return"};
//?????????
static std::vector<std::string> symbols{"{",  "}",  ";",  ",",  ":",  "*",   "(",   ")",  "[",   "]",  "?",  "||",
                                        "&&", "|",  "^",  "&",  "==", "!=",  "<",   ">",  "<=",  ">=", "<<", ">>",
                                        "+",  "-",  "/",  "%",  "++", "--",  ".",   "->", "...", "=",  "~",  "!",
                                        "*=", "/=", "%=", "+=", "-=", "<<=", ">>=", "&=", "^=",  "|="};
static std::vector<std::string> escape{"\\n", "\\t", "\\\"", "\\\'", "\\\\"};
struct Token
{
    std::string type;
    const char *start, *end; // TokenName   start????????????,end???????????????
    int val;                 //??????,????????????
    double dval;             //???????????????
};

/*
???path????????????,???????????????
BUFSIZE??????????????????
*/
constexpr size_t BUFSIZE = 1e5; // 100 kb
char* openFile(const char* path);

/*
?????????
?????????token, push_back???tokenArr
*/

void scan(const char* buf);

/*
TOKEN ????????????
???????????????buf
token??????start,end
????????????msg
-----
???????????????,?????????????????????,????????????
*/
// void errorToken(const char* buf, const char* start, const char* end, const char* path, std::string msg);

void errorParse(const Token& tk, const std::string& error);

/*
????????????
*/
extern const char* buferror;
extern std::vector<Token> tokenArr;
extern const char* path;
#endif