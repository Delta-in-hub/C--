#include "parse.h"

static int pos = 0; // tokenArr pos

//希望当前token类型是tokentype
//否的话,直接报错,结束程序
void expect(const std::string& tokentype, const std::string& error = "Universal Error")
{
    auto&& t = tokenArr[pos];
    if (t.type == tokenType.at(tokentype))
    {
        pos++;
        return;
    }
    errorParse(t, error);
}

//希望当前token类型是tokentype
//是,pos++,返回true
//否,返回false
//向前展望一个符号
bool consume(const std::string& tokentype)
{
    auto&& t = tokenArr[pos];
    if (t.type != tokenType.at(tokentype))
        return false;
    pos++;
    return true;
}

Token& nowToken()
{
    return tokenArr.at(pos);
}
/*
节点相关
*/

//进入新的作用域
void initScope(Env* prev)
{
    env       = new Env;
    env->prev = prev;
}

//离开当前作用域
void exitScope()
{
    env = env->prev;
}

Var* findVar(const std::string& name)
{
    auto t = env;
    while (t)
    {
        if (t->vars.find(name) != t->vars.end())
            return t->vars.at(name);
        else
            t = env->prev;
    }
    errorParse(nowToken(), "undefined variable");
    return nullptr;
}

Node* newNode()
{
    auto t   = new Node;
    t->name  = std::string(nowToken().start, nowToken().end);
    t->token = &nowToken();
}

void addGvar()
{
    ;
}
void addVar()
{
    ;
}
void addTypedef()
{
    ;
}
void addStruct()
{
    ;
}










/*
递归下降
*/

/*    
translation_unit

    | type_specifier fun_declarator compound_statement  translation_unit?
    | type_specifier declarator_list ';' translation_unit?
    ;
*/
Node* translation_unit()
{
    initScope(nullptr);
}

/*

type_specifier

    : VOID pointer?
    | INT pointer?
    | FLOAT pointer?
    | char pointer?
    | double pointer?
    | bool pointer?
    | struct-declaration pointer?
    ;
*/



/*                                          
fun_declarator
    : IDENTIFIER '(' parameter_list ')'
    | IDENTIFIER '(' ')'
    ;
*/
Node* fun_declarator()
{

}


/*
parameter_list                                                 
    : parameter_declaration
    | parameter_list ',' parameter_declaration
    ;
*/
Node* parameter_list()                   
{

}



/*
parameter_declaration                       
    : type_specifier declarator
    ;
*/

Node* parameter_declaration()
{
    type_specifier();
    declarator();
}
//
Type* type_specifier()
{
    auto t = new Type;
    if (consume("void"))
    {
        t->ty   = VarType::VOID;
        t->size = 1;
    }
    else if (consume("int"))
    {
        t->ty   = VarType::INT;
        t->size = 4;
    }
    else if (consume("float") or consume("double"))
    {
        t->ty   = VarType::DOUBLE;
        t->size = 8;
    }
    else if (consume("char"))
    {
        t->ty   = VarType::CHAR;
        t->size = 1;
    }
    else if (consume("bool"))
    {
        t->ty   = VarType::BOOL;
        t->size = 1;
    }
    else if (consume("struct"))
    {
        --pos;
        t->ty = VarType::STRUCT;
        struct_declaration(t);
    }
    else
        errorParse(nowToken(), "Expect type_specifier");

    if (consume("*"))
    {
        --pos;
        t = pointer(t);
    }
    return t;
}
/*
struct-declaration :
    'struct' IDENTIFIER? '{'  struct-declaration-list  '}'
*/
Type* struct_declaration(Type* base)
{
    expect("struct");
    base->ty = VarType::STRUCT;
    if (consume("id"))
    {
        --pos;
        auto pso = env->structs[std::string(nowToken().start, nowToken().end)];
        ++pos;
    }
    else
    {
        ;
    }
}
/*
struct-declaration-list :
    : declaration_list
    | struct-declaration-list declaration_list
    ;
*/
Node* struct_declaration_list()
{
    ;
}

/*
pointer :
    | '*'
    | '*' pointer
*/
Type* pointer(Type* base)
{
    expect("*");
    auto t    = new Type;
    t->size   = 8;
    t->ty     = VarType::PTR;
    t->ptr_to = base;
    while (consume("*"))
    {
        --pos;
        t = pointer(t);
    }
    return t;
}

/*
statement
    : compound_statement     //作用域嵌套 {{}}
    | selection_statement    // if
    | iteration_statement	 //while for
    | assignment_statement   //赋值,空语句
    | 'RETURN' expression ';'
    ;
*/
Node* statement()
{
    ;
}

Node* expression()
{
    ;
}

/*
l_expression
        : IDENTIFIER
        | IDENTIFIER '[' expression ']'
        ;
*/
Node* l_expression()
{
    Node* t = newNode();
    t->type = ND_VARREF;
    expect("id");
    if (consume("["))
    {
        t->expresson = expression();
        expect("]");
    }
    else
    {
        t->var = findVar(t->name);
    }
    return t;
}

/*
Function call  实参(argument)
std::vector<Node*>* args;

expression_list
        : expression
        | exprssion_list ',' expression
*/
std::vector<Node*>* expression_list()
{
    auto v = new std::vector<Node*>;
    auto t = expression();
    v->push_back(t);
    while (consume(","))
    {
        t = expression();
        v->push_back(t);
    }
    return v;
}

/*
unary_operator
    :
    "*"
    "-"
    "!"
    ;
*/

Node* unary_operator()
{
    auto n = newNode();
    if (consume("*"))
    {
        n->type = ND_DEREF;
    }
    else if (consume("-"))
    {
        n->type = ND_NEG;
    }
    else if (consume("!"))
    {
        n->type = ND_NOT;
    }
    else
    {
        errorParse(nowToken(), "Expect unary_operator");
    }
    return n;
}

/*
selection_statement
        : IF '(' expression ')' statement ELSE statement
    ;
*/
Node* selection_statement()
{
    auto n  = newNode();
    n->type = ND_IF;
    expect("if");
    expect("(");
    n->condition = expression();
    expect(")");
    n->then = statement();
    expect("else");
    n->els = statement();
    return n;
}

/*
iteration_statement
    : WHILE '(' expression ')' statement
    | FOR '(' expression ';' expression ';' expression ')' statement  //modified this production
    ;
*/
Node* iteration_statement()
{
    auto n = newNode();
    if (consume("while"))
    {
        n->type = ND_DO_WHILE;
        expect("(");
        n->condition = expression();
        expect(")");
        n->body = statement();
    }
    else if (consume("for"))
    {
        n->type = ND_FOR;
        expect("(");
        n->init = expression();
        expect(";");
        n->condition = expression();
        expect(";");
        n->inc = expression();
        expect(")");
        n->body = statement();
    }
    else
        errorParse(nowToken(), "Expect while or for");
    return n;
}

/*
declaration_list
    : declaration
    | declaration_list declaration
    ;
*/
Node* declaration_list()
{
    auto t = declaration();
    while (consume("void") or consume("char") or consume("bool") or consume("int") or consume("double") or
           consume("struct"))
    {
        --pos;
        t = declaration();
    }
}

/*
//变量
declaration
    : type_specifier declarator_list ';'
    ;
*/
Node* declaration()
{
    ;
}

/*
declarator_list
    : declarator
    | declarator_list ',' declarator
    ;
*/
Node* declarator_list()
{
    ;
}

//--------------wyd的分割线----------------

/*declarator
   : IDENTIFIER declaratorInit
   | IDENTIFIER '[' constant_expression ']'
   ;

declaratorInit
   : '=' expression
   | 空

constant_expression
       : INT_CONSTANT
       : FLOAT_CONSTANT
       ;

compound_statement
   : '{' '}'
   | '{' statement_list '}'
   | '{' declaration_list statement_list '}'
   ;
-
statement_list
   : statement
   | statement_list statement
   ;

statement
   : compound_statement     //作用域嵌套 {{}}
   | selection_statement    // if
   | iteration_statement	 //while for
   | assignment_statement   //赋值,空语句
   | 'RETURN' expression ';'
   ;

assignment_statement
   : ';'
   |  l_expression '=' expression ';'
   ;

expression
   : logical_and_expression
   | expression OR_OP logical_and_expression
   ;*/

//--------------wyd的分割线----------------
//--------------cc的分割线----------------
/*logical_and_expression
    : equality_expression
    | logical_and_expression AND_OP equality_expression
    ;

equality_expression   //等式
    : relational_expression
    | equality_expression EQ_OP relational_expression
    | equality_expression NE_OP relational_expression
    ;

relational_expression  //关系表达式
    : additive_expression
    | relational_expression '<' additive_expression
    | relational_expression '>' additive_expression
    | relational_expression LE_OP additive_expression
    | relational_expression GE_OP additive_expression
    ;

additive_expression   //加减
    : multiplicative_expression
    | additive_expression '+' multiplicative_expression
    | additive_expression '-' multiplicative_expression
    ;

multiplicative_expression  //乘除
    : unary_expression
    | multiplicative_expression '*' unary_expression
    | multiplicative_expression '/' unary_expression
    ;
unary_expression     //一元操作符
    : postfix_expression
    | unary_operator postfix_expression
    ;

postfix_expression
    : primary_expression
    | IDENTIFIER '(' ')'      //函数调用
    | IDENTIFIER '(' expression_list ')'   //函数调用
    | l_expression '++'/'--'
    ;

primary_expression
    : l_expression
    |  l_expression = expression       // added this production
    | INT_CONSTANT
    | FLOAT_CONSTANT
    | STRING_LITERAL
    | '(' expression ')'
    ;
*/
//--------------cc的分割线----------------

