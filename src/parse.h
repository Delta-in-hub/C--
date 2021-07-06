#ifndef PARSE_
#define PARSE_
#include "stdafx.h"
#include "token.h"

struct Env
{
    // Map* vars; //变量
    std::unordered_map<std::string, Var*> vars;
    // Map* typedefs; //
    std::unordered_map<std::string, Var*> typedefs;
    // Map* tags; //结构体
    std::unordered_map<std::string, Var*> structs;
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
    ND_NUM,       // Number literal
    ND_STRUCT,    // Struct
    ND_DECL,      // declaration
    ND_VARDEF,    // Variable definition
    ND_VARREF,    // Variable reference
    ND_IF,        // "if"
    ND_FOR,       // "for"
    ND_DO_WHILE,  // do ... while
    ND_ADDR,      // address-of operator ("&")
    ND_DEREF,     // pointer dereference ("*")  数组
    ND_DOT,       // Struct member access
    ND_EQ,        // ==
    ND_NE,        // !=
    ND_LE,        // <=
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
};

// Represents a variable.
struct Var
{
    Type* ty;
    char* name;
    bool is_local;
    // Local variables are compiled to offsets from RBP.
    int offset;

    // Global variables are compiled to labels with optional
    // initialized data.
    char* data;
};

// AST node
struct Node
{
    NodeType op; // Node type
    Type* ty;    // C type
    Node* lhs;   // left-hand side   一元操作用 lhs
    Node* rhs;   // right-hand side  二元操作 用 lhs , rhs
    int val;     // Number literal  or 数组下标
    double dval;
    Node* expr;               // "return" or expresson
    std::vector<Node*> stmts; // Compound statement

    std::string name;

    // For ND_VARREF  l_expression
    Var* var;

    // "if" ( cond ) then "else" els
    // "for" ( init; cond; inc ) body
    // "while" ( cond ) body
    Node* cond;
    Node* then;
    Node* els;
    Node* init;
    Node* inc;
    Node* body; //函数体,block块...

    // BB* bb;

    // Function definition  形参(parameter)
    std::vector<Var*> params;
    // Vector* params;

    // Function call  实参(argument)
    std::vector<Node*> args;
    // Vector* args;

    // For error reporting
    Token* token;
};

struct Function
{
    std::string name;
    Node* node;
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