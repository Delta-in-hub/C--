#ifndef TOEKN_
    #define TOKEN_
    #include "stdafx.h"

/// token.c



enum
{
    TK_NUM = 256, // Number literal
    TK_STR,       // String literal
    TK_IDENT,     // Identifier
    TK_ARROW,     // ->
    TK_EXTERN,    // "extern"
    TK_TYPEDEF,   // "typedef"
    TK_INT,       // "int"
    TK_CHAR,      // "char"
    TK_VOID,      // "void"
    TK_STRUCT,    // "struct"
    TK_BOOL,      // "_Bool"
    TK_IF,        // "if"
    TK_ELSE,      // "else"
    TK_FOR,       // "for"
    TK_DO,        // "do"
    TK_WHILE,     // "while"
    TK_SWITCH,    // "switch"
    TK_CASE,      // "case"
    TK_BREAK,     // "break"
    TK_CONTINUE,  // "continue"
    TK_EQ,        // ==
    TK_NE,        // !=
    TK_LE,        // <=
    TK_GE,        // >=
    TK_LOGOR,     // ||
    TK_LOGAND,    // &&
    TK_SHL,       // <<
    TK_SHR,       // >>
    TK_INC,       // ++
    TK_DEC,       // --
    TK_MUL_EQ,    // *=
    TK_DIV_EQ,    // /=
    TK_MOD_EQ,    // %=
    TK_ADD_EQ,    // +=
    TK_SUB_EQ,    // -=
    TK_SHL_EQ,    // <<=
    TK_SHR_EQ,    // >>=
    TK_AND_EQ,    // &=
    TK_XOR_EQ,    // ^=
    TK_OR_EQ,     // |=
    TK_RETURN,    // "return"
    TK_SIZEOF,    // "sizeof"
    TK_ALIGNOF,   // "_Alignof"
    TK_TYPEOF,    // "typeof"
    TK_PARAM,     // Function-like macro parameter
    TK_EOF,       // End marker
};

/*
 (?typedef?, ?extern?, ?static? 3 , ?auto?, ?register?,
?void?, ?char?, ?short?, ?int?, ?long?, ?float?, ?double?, ?signed?, ?unsigned?, ?_Bool?,
?_Complex?, ?_Imaginary?, ?struct?, ?union?, ?const?, ?restrict?, ?volatile?, ?sizeof? 2 ,
?enum? 3 , ?inline?, ?case?, ?default?, ?if? 2 , ?else?, ?switch?, ?while? 2 , ?do?, ?for? 2 , ?goto?,
?continue?, ?break?, ?return?), 0 letters (—), 0 numerics (—), 46 signs ("{" 8 , "}" 8 , ";" 11 , "," 12 ,
":" 5 , "*" 6 , "(" 18 , ")" 18 , "[" 8 , "]" 8 , "?", "||", "&&", "|", "^", "&" 2 , "==", "!=", "<", ">", "<=", ">=",
"<<", ">>", "+" 2 , "-" 2 , "/", "%", "++" 2 , "--" 2 , "." 2 , "->", "...", "=" 4 , "~", "!", "*=", "/=", "%=",
"+=", "-=", "<<=", ">>=", "&=", "^=", "|=")
*/

static inline const std::unordered_map<std::string, std::string> tokenType{
    {"num", "TK_NUM"},
    {"dnum", "TK_DNUM"},
    {"int", "TK_INT"},
    {"==", "TK_EQ"},
};

//关键字表
static inline const std::unordered_set<std::string> keywords{};
//符号表
static inline const std::unordered_set<std::string> symbols{};

struct Token
{
    std::string type;
    char *start, *end; // TokenName   start是首指针,end是尾后指针
    int val;           //数字,字符常量
    double dval;       //浮点数常量
};

extern std::vector<Token> tokenArr;

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
void errorToken(char* buf, char* start, char* end, std::string msg);

#endif