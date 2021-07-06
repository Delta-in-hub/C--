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