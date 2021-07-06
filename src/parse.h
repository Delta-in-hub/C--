#ifndef PARSE_
#define PARSE_
#include "stdafx.h"
#include "token.h"

struct Env
{
    // Map* vars; //变量
    std::unordered_map<std::string, Var*> vars;
    // Map* typedefs; //
    std::unordered_map<std::string, Type*> typedefs;
    // Map* tags; //结构体
    std::unordered_map<std::string, Type*> structs;
    struct Env* prev;
}* env = nullptr;

enum VarType
{
    VOID,
    BOOL,
    CHAR,
    INT,
    DOUBLE,
    PTR,
    ARY,
    STRUCT,
    FUNC,
};

struct Type
{
    VarType ty; // VarType
    int size;   // sizeof     bool/char 1     int 4  double 8   void* 8
    // int align; // alignof

    // Pointer
    Type* ptr_to;

    // Array
    Type* ary_of;
    int len;

    // Struct
    // Map* members;
    std::unordered_map<std::string, Type*> members;
    int offset;

    // Function
    Type* returning;
};

enum NodeType
{
    ND_NUM,       // Number literal 常量
    ND_DNUM,      // Double Number literal 浮点常量
    ND_STRUCT,    // Struct 结构体
                  // ND_DECL,      // declaration
    ND_VARDEF,    // Variable definition 变量定义
    ND_VARREF,    // Variable reference 变量引用
    ND_IF,        // "if"
    ND_FOR,       // "for"
    ND_DO_WHILE,  // do ... while
    ND_ADDR,      // address-of operator ("&")
    ND_DEREF,     // pointer dereference ("*")  数组
    ND_DOT,       // Struct member access
    ND_EQ,        // ==
    ND_NE,        // !=
    ND_LE,        // <=
    ND_GE,        // >=
    ND_LESS,      // <
    ND_GREAD,     // >
    ND_ADD,       // +
    ND_SUB,       // -
    ND_LOGAND,    // &&
    ND_LOGOR,     // ||
    ND_SHL,       // <<
    ND_SHR,       // >>
    ND_MOD,       // %
    ND_RETURN,    // "return"
    ND_CALL,      // Function call
    ND_FUNC,      // Function definition
    ND_COMP_STMT, // Compound statement
    ND_EXPR_STMT, // Expression statement
    ND_NEG,       // Negitive  -
    ND_NOT,       // Logic not !
    ND_ASSIGN,    // assign 赋值
    ND_MUL,       // *
    ND_DIV,       // /
    ND_INC,       // ++
    ND_DEC        // --
};

// Represents a variable.
struct Var
{
    Type* ty;
    std::string name;

    bool isArray;
    int arrLen;
    bool is_local;
    // Local variables are compiled to offsets from RBP.
    int offset;

    // Global variables are compiled to labels with optional
    // initialized data.
    Node* data;
};

// AST node
struct Node
{
    NodeType type; // Node type 类型
    Node* lhs;     // left-hand side   一元操作用 lhs
    Node* rhs;     // right-hand side  二元操作 用 lhs , rhs

    // For ND_VARREF  l_expression
    Var* var;
    Type* ctype; // C type
    std::string name;

    int val; // Number literal
    double dval;
    Node* expresson; // expresson  类似  1+2  3  fun(a) 见 https://zh.cppreference.com/w/c/language/expressions
    std::vector<Node*>* statement; // Compound statement

    // "if" ( cond ) then "else" els
    // "for" ( init; cond; inc ) body
    // "while" ( cond ) body
    Node* condition;
    // "if" ( cond ) __then__ "else" els
    Node* then;
    // "if" ( cond ) __then__ "else" __els__
    Node* els;
    // "for" ( __init__; cond; inc ) body
    Node* init;
    // "for" ( init; __cond__; inc ) body
    Node* inc;
    // "for" ( init; cond; inc ) __body__
    // "while" ( cond ) __body__
    Node* body; //函数体,block块...

    // BB* bb;

    // Function definition  形参(parameter)
    std::vector<Type*>* params;
    // Vector* params;

    // Function call  实参(argument)
    std::vector<Node*>* args;
    // Vector* args;

    // For error reporting
    Token* token;
};

struct Function
{
    Node* info;
    std::string name;
    Node* compound;
    // Vector* lvars;
    std::vector<Var*> lvars;
    // Vector* bbs;
};

// Represents toplevel constructs.
struct Program
{
    // Vector* gvars;
    std::vector<Var*> gvars;
    // Vector* funcs;
    std::vector<Function*> funcs;
} * prog;
#endif