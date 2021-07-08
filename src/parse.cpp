#include "parse.h"

static int pos = 0; // tokenArr pos
Program* prog  = nullptr;
Env *env = nullptr, *global = nullptr;

void translation_unit();
Node* fun_declarator();
std::vector<Type*>* parameter_list();
void parameter_declaration(std::vector<Type*>* arr);
Type* type_specifier();
Type* struct_declaration(Type* base);
void struct_declaration_list(Type* base);
Type* pointer(Type* base);

Node* l_expression();
std::vector<Node*>* expression_list();
Node* unary_operator();
Node* selection_statement();
Node* iteration_statement();
std::vector<Var*>* declaration_list();
void declaration(std::vector<Var*>* arr);
void declarator_list(std::vector<Var*>* arr);

Var* declarator();
Node* declaratorInit();
Node* constant_expression();
Node* compound_statement();
std::vector<Node*>* statement_list();
Node* statement();
Node* assignment_statement();
Node* expression();

Node* logical_and_expression();
Node* equality_expression();
Node* relational_expression();
Node* additive_expression();
Node* multiplicative_expression();
Node* unary_expression();
Node* postfix_expression();
Node* primary_expression();
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
    s += tokenType.at(tokentype);
    errorParse(t, s);
}

//希望当前token类型是tokentype
//是,pos++,返回true
//否,返回false
//向前展望一个符号
bool consume(const std::string_view& tokentype)
{
    auto&& t = tokenArr[pos];
    if (t.type != tokenType.at(std::string(tokentype)))
        return false;
    pos++;
    return true;
}

Token& nowToken(int p = 0)
{
    return tokenArr.at(pos + p);
}

std::string nowName(int p = 0)
{
    return std::string(nowToken(p).start, nowToken(p).end);
}

/*
节点相关
*/

//进入新的作用域
void initScope(Env* prev)
{
    env       = new Env{};
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
    auto t   = new Node{};
    t->name  = std::string(nowToken().start, nowToken().end);
    t->token = &nowToken();
    return t;
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

// Var* findVar(const std::string& name);
// Type* struct_declaration(Type* base);

/*
递归下降
*/

void parse()
{
    initScope(nullptr);
    global = env;
    prog   = new Program{};
    while (nowToken().type != "TK_EOF")
    {
        translation_unit();
    }
}

/*
translation_unit
    | type_specifier fun_declarator compound_statement
    | type_specifier declarator_list ';'
    ;
*/
void translation_unit()
{
    auto t = type_specifier();
    if (nowToken().type != tokenType.at("id"))
    {
        errorParse(nowToken(), "Expect IDENTIFIER");
    }
    if (nowToken(1).type == tokenType.at("("))
    {
        Function* f = new Function{};
        auto fun    = fun_declarator();
        fun->ctype  = t;
        f->name     = fun->name;
        f->info     = fun;
        auto nv     = new Var{};
        nv->name    = fun->name;
        auto ny     = new Type{};
        ny->ty      = FUNC;
        nv->ty      = ny;
        addVar(nv);
        f->compound = compound_statement();
        prog->funcs.push_back(f);
    }
    else
    {
        std::vector<Var*>* arr = new std::vector<Var*>{};
        declarator_list(arr);
        expect(";");
        for (auto&& i : *arr)
        {
            if (i->isArray)
            {
                auto ty2    = new Type{};
                ty2->ty     = VarType::ARY;
                ty2->ary_of = t;
                ty2->len    = i->arrLen;
                ty2->size   = ty2->len * (ty2->ptr_to->size);
                i->ty       = ty2;
            }
            else
            {
                i->ty = t;
            }
            addVar(i);
            prog->gvars.push_back(i);
        }
    }
}

/*
fun_declarator
    : IDENTIFIER '(' parameter_list ')'
    | IDENTIFIER '(' ')'
    ;
*/
Node* fun_declarator()
{
    auto t = newNode();
    std::string name(nowToken().start, nowToken().end);
    t->name = name;
    expect("id");
    expect("(");
    if (consume(")"))
    {
        t->params = new std::vector<Type*>{};
    }
    else
    {
        t->params = parameter_list();
    }
    return t;
}

/*
parameter_list
    : parameter_declaration
    | parameter_list ',' parameter_declaration
    ;
*/
std::vector<Type*>* parameter_list()
{
    std::vector<Type*>* arr = new std::vector<Type*>{};
    parameter_declaration(arr);
    while (consume(","))
    {
        parameter_declaration(arr);
    }
    return arr;
}

/*
parameter_declaration
    : type_specifier declarator
    ;
*/
void parameter_declaration(std::vector<Type*>* arr)
{
    auto ty = type_specifier();
    auto t  = declarator();
    if (t->isArray)
    {
        auto nty    = new Type{};
        nty->size   = 8;
        nty->ty     = VarType::PTR;
        nty->ptr_to = ty;
        arr->push_back(nty);
        return;
    }
    arr->push_back(ty);
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
Type* type_specifier()
{
    auto t = new Type{};
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
    if (consume("{")) //定义
    {
        struct_declaration_list(base);
        addStruct(base, name);
        expect("}");
    }
    else //声明
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
    ;
*/
void struct_declaration_list(Type* base)
{
    base->members;
    auto arr = declaration_list();
    for (auto&& i : *arr)
    {
        base->members[i->name] = i->ty;
    }
}

/*
pointer :
    | '*'
    | '*' pointer
*/
Type* pointer(Type* base)
{
    expect("*");
    auto t    = new Type{};
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
        | IDENTIFIER '.' IDENTIFIER
        | * l_expression
        ;
*/
Node* l_expression()
{
    if (consume("*"))
    {
        auto t  = newNode();
        t->type = NodeType::ND_DEREF;
        t->lhs  = l_expression();
        return t;
    }
    Node* t = newNode();
    t->type = ND_VARREF;
    std::string name(nowToken().start, nowToken().end);
    expect("id");
    if (consume("["))
    {
        t->var       = findVar(t->name);
        t->expresson = expression();
        expect("]");
        t->type = ND_ARRDEREF;
    }
    else if (consume("."))
    {
        t->type = ND_DOT;
        t->var  = findVar(name);
        std::string name2(nowToken().start, nowToken().end);
        expect("id");
        t->name = name2;
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
    auto v = new std::vector<Node*>{};
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
返回包含所有变量的vector
declaration_list
    : declaration
    | declaration_list declaration
    ;
*/
std::vector<Var*>* declaration_list()
{
    std::vector<Var*>* arr;
    arr = new std::vector<Var*>{};
    declaration(arr);
    while (consume("void") or consume("char") or consume("bool") or consume("int") or consume("double") or
           consume("struct"))
    {
        --pos;
        declaration(arr);
    }
    return arr;
}

/*
变量定义
declaration
    : type_specifier declarator_list ';'
    ;
*/
void declaration(std::vector<Var*>* arr)
{
    auto ty = type_specifier();
    declarator_list(arr);
    expect(";");
    for (auto&& i : *arr)
    {
        if (i->isArray)
        {
            auto ty2    = new Type{};
            ty2->ty     = VarType::ARY;
            ty2->ary_of = ty;
            ty2->len    = i->arrLen;
            ty2->size   = ty2->len * ty2->ptr_to->size;
            i->ty       = ty2;
        }
        else
        {
            i->ty = ty;
        }
    }
}

/*
declarator_list
    : declarator
    | declarator_list ',' declarator
    ;
*/
void declarator_list(std::vector<Var*>* arr)
{
    auto t = declarator();
    arr->push_back(t);
    while (consume(","))
    {
        t = declarator();
        arr->push_back(t);
    }
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
    auto t     = new Var{};
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
        t->data = declaratorInit();
    }
    return t;
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
    expect("{");
    initScope(env);
    auto t  = newNode();
    t->type = ND_COMP_STMT;
    if (consume("return") || consume("{") || consume("if") || consume("while") || consume("for") || consume(";") ||
        consume("id"))
    {
        --pos;
        t->statementList = statement_list();
        expect("}");
    }
    else if (consume("void") || consume("char") || consume("bool") || consume("int") || consume("double") ||
             consume("struct"))
    {
        --pos;
        auto decl = declaration_list();
        for (auto&& i : *decl)
        {
            addVar(i);
        }
        t->statementList = statement_list();
        expect("}");
    }
    else
    {
        expect("}");
    }
    exitScope();
    return t;
}

/*
statement_list
   : statement
   | statement_list statement
   ;
*/

std::vector<Node*>* statement_list()
{
    auto v = new std::vector<Node*>{};
    auto t = statement();
    v->push_back(t);
    while (consume("return") || consume("{") || consume("if") || consume("while") || consume("for") || consume(";") ||
           consume("id") || consume("*"))
    {
        pos--;
        t = statement();
        v->push_back(t);
    }
    return v;
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
    if (consume("{"))
    {
        --pos;
        return compound_statement();
    }
    else if (consume("if"))
    {
        --pos;
        return selection_statement();
    }
    else if (consume("while") || consume("for"))
    {
        --pos;
        return iteration_statement();
    }
    else if (consume(";") || consume("id"))
    {
        --pos;
        return assignment_statement();
    }
    else if (consume("return"))
    {
        --pos;
        auto t       = newNode();
        t->type      = NodeType::ND_RETURN;
        t->expresson = expression();
        return t;
    }
    else
    {
        errorParse(nowToken(), "Expect statement");
    }
    return nullptr;
}

/*
assignment_statement
   : ';'
   |  l_expression '=' expression ';'
   ;
*/
Node* assignment_statement()
{
    auto t  = newNode();
    t->type = ND_ASSIGN;
    if (consume(";"))
    {
        t->type      = NodeType::ND_EXPR_STMT;
        t->expresson = nullptr; //表示空语句
    }
    else if (consume("id") or consume("*"))
    {
        pos--;
        t->lhs = l_expression();
        expect("=");
        t->rhs = expression();
        expect(";");
    }
    else
    {
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

Node* expression()
{
    auto t   = logical_and_expression();
    int flag = 0;
    while (consume("||"))
    {
        auto orll  = newNode();
        orll->type = ND_LOGOR;
        if (flag == 0)
        {
            orll->rhs = logical_and_expression();
            orll->lhs = t;
            t         = orll;
            flag      = (flag + 1) % 2;
        }
        else if (flag == 1)
        {
            orll->lhs = logical_and_expression();
            orll->rhs = t;
            t         = orll;
            flag      = (flag + 1) % 2;
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
Node* logical_and_expression()
{
    auto t   = equality_expression();
    int flag = 0;
    while (consume("&&"))
    {
        auto orll  = newNode();
        orll->type = ND_LOGAND;
        if (flag == 0)
        {
            orll->rhs = equality_expression();
            orll->lhs = t;
            t         = orll;
            flag      = (flag + 1) % 2;
        }
        else if (flag == 1)
        {
            orll->lhs = equality_expression();
            orll->rhs = t;
            t         = orll;
            flag      = (flag + 1) % 2;
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
Node* equality_expression()
{
    auto t   = relational_expression();
    int flag = 0;
    while (consume("==") || consume("!="))
    {
        auto orll = newNode();
        pos--;
        if (consume("=="))
        {
            orll->type = ND_EQ;
        }
        else if (consume("!="))
        {
            orll->type = ND_NE;
        }
        if (flag == 0)
        {
            orll->rhs = relational_expression();
            orll->lhs = t;
            t         = orll;
            flag      = (flag + 1) % 2;
        }
        else if (flag == 1)
        {
            orll->lhs = relational_expression();
            orll->rhs = t;
            t         = orll;
            flag      = (flag + 1) % 2;
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

Node* relational_expression()
{
    auto t   = additive_expression();
    int flag = 0;
    while (consume("<") || consume(">") || consume("<=") || consume(">="))
    {
        auto orll = newNode();
        pos--;
        if (consume("<"))
        {
            orll->type = ND_LESS;
        }
        else if (consume(">"))
        {
            orll->type = ND_GREAD;
        }
        else if (consume("<="))
        {
            orll->type = ND_LE;
        }
        else if (consume(">="))
        {
            orll->type = ND_GE;
        }
        if (flag == 0)
        {
            orll->rhs = additive_expression();
            orll->lhs = t;
            t         = orll;
            flag      = (flag + 1) % 2;
        }
        else if (flag == 1)
        {
            orll->lhs = additive_expression();
            orll->rhs = t;
            t         = orll;
            flag      = (flag + 1) % 2;
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

Node* additive_expression()
{
    auto t   = multiplicative_expression();
    int flag = 0;
    while (consume("+") || consume("-"))
    {
        auto orll = newNode();
        pos--;
        if (consume("+"))
        {
            orll->type = ND_ADD;
        }
        else if (consume("-"))
        {
            orll->type = ND_SUB;
        }
        if (flag == 0)
        {
            orll->rhs = multiplicative_expression();
            orll->lhs = t;
            t         = orll;
            flag      = (flag + 1) % 2;
        }
        else if (flag == 1)
        {
            orll->lhs = multiplicative_expression();
            orll->rhs = t;
            t         = orll;
            flag      = (flag + 1) % 2;
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
*/
Node* multiplicative_expression()
{
    auto t   = unary_expression();
    int flag = 0;
    while (consume("*") || consume("/"))
    {
        auto orll = newNode();
        pos--;
        if (consume("*"))
        {
            orll->type = ND_MUL;
        }
        else if (consume("/"))
        {
            orll->type = ND_DIV;
        }
        if (flag == 0)
        {
            orll->rhs = unary_expression();
            orll->lhs = t;
            t         = orll;
            flag      = (flag + 1) % 2;
        }
        else if (flag == 1)
        {
            orll->lhs = unary_expression();
            orll->rhs = t;
            t         = orll;
            flag      = (flag + 1) % 2;
        }
    }
    return t;
}
/*
unary_expression     //一元操作符
    : postfix_expression
    | unary_operator postfix_expression
    ;
*/

Node* unary_expression()
{
    if (consume("*") or consume("-") or consume("!"))
    {
        pos--;
        auto t = unary_operator();
        t->lhs = postfix_expression();
        return t;
    }
    else
    {
        return postfix_expression();
    }
}
/*
postfix_expression
    : primary_expression
    | IDENTIFIER '(' ')'      //函数调用
    | IDENTIFIER '(' expression_list ')'   //函数调用
    ;
*/
Node* postfix_expression()
{
    if (nowToken().type == tokenType.at("id") and nowToken(1).type == tokenType.at("("))
    {
        auto t    = newNode();
        t->type   = ND_CALL;
        auto name = nowName();
        auto f    = findVar(name);
        if (f->ty->ty != FUNC)
        {
            errorParse(nowToken(), "undefined function name");
        }
        consume("id");
        consume("(");
        if (consume(")"))
        {
            t->args = new std::vector<Node*>{};
        }
        else
        {
            t->args = expression_list();
            expect(")");
        }
        return t;
    }
    else
    {
        return primary_expression();
    }
}
/*
primary_expression
    : l_expression
    | l_expression '++'/'--'
    |  l_expression = expression       // added this production
    | INT_CONSTANT
    | FLOAT_CONSTANT
    | STRING_LITERAL
    | '(' expression ')'
    ;
*/
Node* primary_expression()
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
    else if (consume("str"))
    {
        t->type = ND_STR;
        t->name = nowName(-1);
    }
    else if (consume("("))
    {
        delete t;
        t = expression();
        expect(")");
    }
    else
    {
        delete t;
        t = l_expression();
        if (consume("="))
        {
            auto orll  = newNode();
            orll->lhs  = t;
            orll->rhs  = expression();
            orll->type = ND_ASSIGN;
            t          = orll;
        }
        else if (consume("++") or consume("--"))
        {
            pos--;
            auto orll = newNode();
            orll->lhs = t;
            if (consume("++"))
            {
                orll->type = ND_INC;
            }
            else if (consume("--"))
            {
                orll->type = ND_DEC;
            }
            t = orll;
        }
    }
    return t;
}
//--------------cc的分割线----------------
