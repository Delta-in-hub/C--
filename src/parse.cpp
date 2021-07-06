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
    std::string s("Expect ");
    s += tokentype;
    errorParse(t, s);
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

Token& nowToken(int p = 0)
{
    return tokenArr.at(pos + p);
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
    auto t = env;
    env    = env->prev;
    if (env == nullptr)
        env = t;
    else
        delete t;
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

Type* findStruct(const std::string& name)
{
    auto t = env;
    while (t)
    {
        if (t->structs.find(name) != t->structs.end())
            return t->structs.at(name);
        else
            t = env->prev;
    }
    errorParse(nowToken(), "undefined struct");
    return nullptr;
}

Node* newNode()
{
    auto t   = new Node;
    t->name  = std::string(nowToken().start, nowToken().end);
    t->token = &nowToken();
}

void addVar(Var* v)
{
    if (env->vars.find(v->name) != env->vars.end())
    {
        errorParse(nowToken(), "Multi Definition");
    }
    else
    {
        env->vars[v->name] = v;
    }
}

void addStruct(Type* ty, const std::string& name)
{
    if (env->vars.find(name) != env->vars.end() or env->structs.find(name) != env->structs.end())
    {
        errorParse(nowToken(), "Multi Definition");
    }
    else
    {
        env->structs[name] = ty;
    }
}

Var* findVar(const std::string& name);
Type* struct_declaration(Type* base);

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
    prog = new Program;
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
        t     = struct_declaration(t);
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
    : 'struct' IDENTIFIER        //声明
    | 'struct' IDENTIFIER?  '{'  struct-declaration-list  '}'  //定义
*/
Type* struct_declaration(Type* base)
{
    expect("struct");
    base->ty = VarType::STRUCT;
    std::string name;
    if (consume("id"))
    {
        --pos;
        name = std::string(nowToken().start, nowToken().end);
        ++pos;
    }
    if (consume("{"))
    {
        struct_declaration_list(base);
        expect("}");
    }
    else
    {
        if (name.empty())
            errorParse(nowToken(), "Expect struct name");
        else
            return findStruct(name);
    }
    return base;
}
/*
struct-declaration-list :
    : declaration_list
    | struct-declaration-list declaration_list
    ;
*/
Node* struct_declaration_list(Type* base)
{
    base->members;
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
std::vector<Var*>* declaration_list()
{
    std::vector<Var*>* arr;
    arr    = new std::vector<Var*>;
    auto t = declaration(arr);
    while (consume("void") or consume("char") or consume("bool") or consume("int") or consume("double") or
           consume("struct"))
    {
        --pos;
        t = declaration(arr);
    }
    return arr;
}

/*
//变量
declaration
    : type_specifier declarator_list ';'
    ;
*/
Node* declaration(std::vector<Var*>* arr)
{
    auto ty = type_specifier();
    declarator_list(arr);
    expect(";");
}

/*
declarator_list
    : declarator
    | declarator_list ',' declarator
    ;
*/
Node* declarator_list(std::vector<Var*>* arr)
{
    auto t = new Var;
}

//--------------wyd的分割线----------------

/*
declarator
   : IDENTIFIER declaratorInit?
   | IDENTIFIER '[' constant_expression ']'
   ;
*/
Var* declarator()
{
    auto t     = new Var;
    t->name    = std::string(nowToken().start, nowToken().end);
    t->isArray = false;
    expect("id");
    if (consume("["))
    {
        t->isArray = true;
        auto tmp   = constant_expression();
        if (tmp->type == ND_DNUM)
        {
            errorParse(nowToken(), "Length of array can not be float number");
        }
        t->arrLen = tmp->val;
        expect("]");
    }
    else if (consume("="))
    {
        --pos;
        declaratorInit();
    }
}

/*
declaratorInit
   : '=' expression
*/
Node* declaratorInit()
{
    expect("=");
    return expression();
}

/*
constant_expression
       : INT_CONSTANT
       : DOUBLE_CONSTANT
       ;
*/
Node* constant_expression()
{
    auto t = newNode();
    if (consume("num"))
    {
        t->type = ND_NUM;
        t->val  = nowToken(-1).val;
    }
    else if (consume("dnum"))
    {
        t->type = ND_DNUM;
        t->dval = nowToken(-1).dval;
    }
    else
    {
        errorParse(nowToken(), "Expect unary_operator");
    }
    return t;
}
/*
compound_statement
   : '{' '}'
   | '{' statement_list '}'
   | '{' declaration_list statement_list '}'
   ;
*/
Node* compound_statement()
{
    auto t = newNode();
    expect("{");
    if (consume("return") || consume("{") || consume("if") || consume("while") || consume("for") || consume(";") || consume("id")) {
        statement_list();
        expect("}");
    }
    else if (consume("void") || consume("char") || consume("bool") || consume("int") || consume("double") || consume("struct")) {
        declaration_list();
        statement_list();
        expect("}");
    }
    else
    {
        expect("}");
    }
    return t;
}

/*
statement_list
   : statement
   | statement_list statement
   ;
*/

Node* statement_list()
{
    auto t = statement();
    while (consume("return")||consume("{") || consume("if") || consume("while") || consume("for") || consume(";") || consume("id")) {
        pos--;
        t = statement();
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
    auto t = newNode();
    if (consume("{"))
    {
        compound_statement();
    }
    else if (consume("if"))
    {
        selection_statement();
    }
    else if (consume("while")||consume("for")) {
        iteration_statement();
    }
    else if (consume(";")||consume("id")) {
        assignment_statement();
    }
    else if (consume("return")) {
        expression();
    }
    else {
        errorParse(nowToken(), "Expect statement");
    }
    return t;
}

/*
assignment_statement
   : ';'
   |  l_expression '=' expression ';'
   ;
*/
Node* assignment_statement()
{
    auto t = newNode();
    if (consume(";")) {

    }
    else if (consume("id")) {
        pos--;
        l_expression();
        expect("=");
        expression();
        expect(";");
    }
    else {
        errorParse(nowToken(), "Expect assignment");
    }
    return t;
}

/*
expression
   : logical_and_expression
   | expression OR_OP logical_and_expression
   ;
 */

Node* expression(){
    auto t = logical_and_expression();
    int flag = 0;
    while (consume("||")) {
        auto orll = newNode();
        orll->type = ND_LOGOR;
        if (flag == 0) {
            orll->rhs = logical_and_expression();
            orll->lhs = t;
            t = orll;
            flag = (flag + 1) % 2;
        }
        else if (flag == 1) {
            orll->lhs = logical_and_expression();
            orll->rhs = t;
            t = orll;
            flag = (flag + 1) % 2;
        } 
    }
    return t;
}
//--------------wyd的分割线----------------
//--------------cc的分割线----------------
/*logical_and_expression
    : equality_expression
    | logical_and_expression AND_OP equality_expression
    ;
*/
Node* logical_and_expression() {
    auto t = equality_expression();
    int flag = 0;
    while (consume("&&")) {
        auto orll = newNode();
        orll->type = ND_LOGAND;
        if (flag == 0) {
            orll->rhs = equality_expression();
            orll->lhs = t;
            t = orll;
            flag = (flag + 1) % 2;
        }
        else if (flag == 1) {
            orll->lhs = equality_expression();
            orll->rhs = t;
            t = orll;
            flag = (flag + 1) % 2;
        }
    }
    return t;
}
/*
equality_expression   //等式
    : relational_expression
    | equality_expression EQ_OP relational_expression
    | equality_expression NE_OP relational_expression
    ;
*/
Node* equality_expression() {
    auto t = relational_expression();
    int flag = 0;
    while (consume("==")||consume("!=")) {
        auto orll = newNode();
        pos--;
        if (consume("==")) {
            orll->type = ND_EQ;
        }
        else {
            orll->type = ND_NE;
        }
        if (flag == 0) {
            orll->rhs = relational_expression();
            orll->lhs = t;
            t = orll;
            flag = (flag + 1) % 2;
        }
        else if (flag == 1) {
            orll->lhs = relational_expression();
            orll->rhs = t;
            t = orll;
            flag = (flag + 1) % 2;
        }
    }
    return t;
}

/*
relational_expression  //关系表达式
    : additive_expression
    | relational_expression '<' additive_expression
    | relational_expression '>' additive_expression
    | relational_expression LE_OP additive_expression
    | relational_expression GE_OP additive_expression
    ;
*/

Node* relational_expression(){
    auto t = additive_expression();
    int flag = 0;
    while (consume("<") || consume(">")|| consume("<=") || consume(">=")) {
        auto orll = newNode();
        pos--;
        if (consume("<")) {
            orll->type = ND_LESS;
        }
        else if(consume(">")){
            orll->type = ND_GREAD;
        }
        else if (consume("<=")) {
            orll->type = ND_LE;
        }
        else if (consume(">=")) {
            orll->type = ND_GE;
        }
        if (flag == 0) {
            orll->rhs = additive_expression();
            orll->lhs = t;
            t = orll;
            flag = (flag + 1) % 2;
        }
        else if (flag == 1) {
            orll->lhs = additive_expression();
            orll->rhs = t;
            t = orll;
            flag = (flag + 1) % 2;
        }
    }
    return t;
}
/*
additive_expression   //加减
    : multiplicative_expression
    | additive_expression '+' multiplicative_expression
    | additive_expression '-' multiplicative_expression
    ;
*/
    
Node* additive_expression() {
    auto t = multiplicative_expression();
    int flag = 0;
    while (consume("+") || consume("-")) {
        auto orll = newNode();
        pos--;
        if (consume("+")) {
            orll->type = ND_ADD;
        }
        else {
            orll->type = ND_SUB;
        }
        if (flag == 0) {
            orll->rhs = multiplicative_expression();
            orll->lhs = t;
            t = orll;
            flag = (flag + 1) % 2;
        }
        else if (flag == 1) {
            orll->lhs = multiplicative_expression();
            orll->rhs = t;
            t = orll;
            flag = (flag + 1) % 2;
        }
    }
    return t;
}
    

/*
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
