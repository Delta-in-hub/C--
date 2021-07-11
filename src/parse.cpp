#include "parse.h"

static std::vector<Var*> paraVar;
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
#ifdef DEBUG__
    if (not(pos >= 0 and pos < tokenArr.size()))
    {
        std::printf("%d out of range of TokenArr");
        getchar();
        exit(1);
    }
    if (tokenType.find(tokentype) == tokenType.end())
    {
        std::printf("%s does not in tokenType unordreed map", tokentype.c_str());
        getchar();
        exit(1);
    }
#endif

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
#ifdef DEBUG__
    if (not(pos >= 0 and pos < tokenArr.size()))
    {
        std::printf("%d out of range of TokenArr");
        getchar();
        exit(1);
    }
    if (tokenType.find(tokentype) == tokenType.end())
    {
        std::printf("%s does not in tokenType unordreed map", tokentype.c_str());
        getchar();
        exit(1);
    }
#endif
    auto&& t = tokenArr[pos];
    if (t.type != tokenType.at(tokentype))
        return false;
    pos++;
    return true;
}

Token& nowToken(int p = 0)
{
#ifdef DEBUG__
    if (not(pos + p >= 0 and pos + p < tokenArr.size()))
    {
        std::printf("%d out of range of TokenArr");
        getchar();
        exit(1);
    }
#endif
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
            t = t->prev;
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
            t = t->prev;
    }
    errorParse(nowToken(), "undefined struct");
    return nullptr;
}

Function* findFuncion(const std::string& name)
{
    for (auto&& i : prog->funcs)
    {
        if (i->name == name)
            return i;
    }
    errorParse(nowToken(), "undefined Function");
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
        errorParse(nowToken(), "Multi Variable Definition");
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
        errorParse(nowToken(), "Multi Struct Definition");
    }
    else
    {
        env->structs[name] = ty;
    }
}

void addFunction(Function* f)
{
    for (auto&& i : prog->funcs)
    {
        if (i->name == f->name)
            errorParse(nowToken(-1), "Multi Function Definition");
    }
    prog->funcs.push_back(f);
}

// ta,tb是否为同一类型
bool isSameType(const Type* ta, const Type* tb)
{
    //若类型相同,能有释放掉一个内存?
    if (ta == tb)
        return true;
    switch (ta->ty)
    {
    case VarType::ARY:
        if (tb->ty != ARY or ta->len != tb->len)
            return false;
        return isSameType(ta->ary_of, ta->ary_of);
        break;
    case VarType::BOOL:
        if (tb->ty == BOOL)
            return true;
        else
            return false;
        break;
    case VarType::CHAR:
        if (tb->ty == CHAR)
            return true;
        else
            return false;
        break;
    case VarType::DOUBLE:
        if (tb->ty == DOUBLE)
            return true;
        else
            return false;
        break;
    // case VarType::FUNC:
    // break;
    case VarType::INT:
        if (tb->ty == INT)
            return true;
        else
            return false;
        break;
    case VarType::PTR:
        if (tb->ty != PTR)
            return false;
        return isSameType(ta->ptr_to, tb->ptr_to);
        break;
    case VarType::STRUCT:
        if (tb->ty != STRUCT)
            return false;
        return ta->members == tb->members; //存疑
        break;
    case VarType::VOID:
        if (tb->ty == VOID)
            return true;
        else
            return false;
        break;
    default:
        errorParse(nowToken(), "Type Not Found");
        break;
    }
    return false;
}

// tb进行隐形类型转换后是否和ta类型相同
bool isMatchType(const Type* ta, const Type* tb)
{
    if (ta == tb)
        return true;
    switch (ta->ty)
    {
    case VarType::ARY:
        if (tb->ty != ARY or ta->len != tb->len)
            return false;
        return isSameType(ta->ary_of, ta->ary_of);
        break;
    case VarType::BOOL:
        if (tb->ty == BOOL or tb->ty == CHAR or tb->ty == INT)
            return true;
        else
            return false;
        break;
    case VarType::CHAR:
        if (tb->ty == CHAR or tb->ty == INT)
            return true;
        else
            return false;
        break;
    case VarType::DOUBLE:
        if (tb->ty == DOUBLE or tb->ty == INT or tb->ty == CHAR)
            return true;
        else
            return false;
        break;
    // case VarType::FUNC:
    // break;
    case VarType::INT:
        if (tb->ty == INT or tb->ty == CHAR or tb->ty == DOUBLE)
            return true;
        else
            return false;
        break;
    case VarType::PTR:
        if (tb->ty == ARY)
        {
            if (isSameType(ta->ptr_to, tb->ary_of))
                return true;
            else
                return false;
        }
        else if (tb->ty != PTR)
            return false;
        else
            return isSameType(ta->ptr_to, tb->ptr_to);
        break;
    case VarType::STRUCT:
        if (tb->ty != STRUCT)
            return false;
        return ta->members == tb->members; //存疑
        break;
    case VarType::VOID:
        if (tb->ty == VOID)
            return true;
        else
            return false;
        break;
    default:
        errorParse(nowToken(), "Type Not Found");
        break;
    }
    return false;
}

bool isCalAble(Type* ta)
{
    switch (ta->ty)
    {
    case VarType::ARY:
    case VarType::BOOL:
    case VarType::VOID:
    case VarType::STRUCT:
        return false;
        break;
    case VarType::PTR:
    case VarType::DOUBLE:
    case VarType::INT:
    case VarType::CHAR:
        return true;
    default:
        break;
    }
    return false;
}

Type* topType(Type* ta, Type* tb)
{
    if (ta->ty == PTR)
    {
        if (tb->ty == INT)
            return ta;
        else
            return nullptr;
    }
    if (tb->ty == PTR)
    {
        if (ta->ty == INT)
            return tb;
        else
            return nullptr;
    }
    if (ta->ty == DOUBLE)
        return ta;
    if (tb->ty == DOUBLE)
        return tb;
    if (ta->ty == INT)
        return ta;
    if (tb->ty == INT)
        return tb;
    return nullptr;
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
#ifdef DEBUG__
    std::cout << "Complier done" << std::endl;
#endif
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
    // if (nowToken().type != tokenType.at("id"))
    // {
    //     errorParse(nowToken(), "Expect IDENTIFIER");
    // }
    if (nowToken(1).type == tokenType.at("(") and nowToken().type == tokenType.at("id"))
    {
        Function* f   = new Function{};
        auto fun      = fun_declarator();
        f->name       = fun->name;
        f->params     = fun->params;
        f->returnType = t;

        /*
        //变量名视作为变量的一种
        auto nv  = new Var{};
        nv->name = fun->name;
        auto ny  = new Type{};
        ny->ty   = FUNC;
        nv->ty   = ny;
        addVar(nv);
        */
        // expect(")");
        addFunction(f);
        // prog->funcs.push_back(f); // for recursive
        f->compound = compound_statement();
    }
    else
    {
        std::vector<Var*>* arr = new std::vector<Var*>{};
        /*
        before:struct A{}; error
        after:struct A{}; done
        */
        if (consume(";"))
            --pos;
        else
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
                ty2->size   = i->arrLen * (t->size);
                i->ty       = ty2;
            }
            else
            {
                i->ty = t;
            }
            addVar(i);
            prog->gvars.push_back(i);
            if (i->data != nullptr)
            {
                if (not isMatchType(i->ty, i->data->ctype))
                {
                    errorParse(nowToken(-1), "Unmatched type in Initialization");
                }
            }
        }
    }
}

/*
返回函数名(形参列表仅类型)
返回值类型,函数体待填充
fun_declarator
    : IDENTIFIER '(' parameter_list ')'
    | IDENTIFIER '(' ')'
    ;
*/
Node* fun_declarator()
{
    /*
    1.node->type = ND_FUNC
    2.node->name = funName
    3.params列表,此时将数组退化为指针Type
    */
    auto t  = newNode();
    t->type = ND_FUNC;
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
        expect(")");
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
        t->ty       = nty;
    }
    else
    {
        t->ty = ty;
    }
    arr->push_back(t->ty);
    paraVar.push_back(t);
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
Type* voidType()
{
    static Type* t = nullptr;
    if (t == nullptr)
    {
        t       = new Type{};
        t->size = 1;
        t->ty   = VarType::VOID;
    }
    return t;
}
Type* intType()
{
    static Type* t = nullptr;
    if (t == nullptr)
    {
        t       = new Type{};
        t->size = 4;
        t->ty   = VarType::INT;
    }
    return t;
}
Type* doubleType()
{
    static Type* t = nullptr;
    if (t == nullptr)
    {
        t       = new Type{};
        t->size = 8;
        t->ty   = VarType::DOUBLE;
    }
    return t;
}
Type* charType()
{
    static Type* t = nullptr;
    if (t == nullptr)
    {
        t       = new Type{};
        t->size = 1;
        t->ty   = VarType::CHAR;
    }
    return t;
}
Type* boolType()
{
    static Type* t = nullptr;
    if (t == nullptr)
    {
        t       = new Type{};
        t->size = 1;
        t->ty   = VarType::BOOL;
    }
    return t;
}
Type* type_specifier()
{
    Type* t = nullptr;
    if (consume("void"))
    {
        t = voidType();
    }
    else if (consume("int"))
    {
        t = intType();
    }
    else if (consume("float") or consume("double"))
    {
        t = doubleType();
    }
    else if (consume("char"))
    {
        t = charType();
    }
    else if (consume("_Bool"))
    {
        t = boolType();
    }
    else if (consume("struct"))
    {
        --pos;
        t = new Type{};
        t = struct_declaration(t);
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
    /*
    1.VarType::STRUCT
    2.填充members
    3.计算size
    */
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
        base->size = 0;
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
    auto arr = declaration_list();
    for (auto&& i : *arr)
    {
        base->size += i->size;
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
        t->type = NodeType::ND_DEREF; //类型检查在之后
        t->lhs  = l_expression();
        if (t->lhs->ctype->ty != VarType::PTR)
            errorParse(nowToken(), "invalid type argument of unary *");
        t->ctype = t->lhs->ctype->ptr_to;
        return t;
    }
    Node* t = newNode();
    std::string name(nowToken().start, nowToken().end);
    expect("id");
    if (consume("["))
    {
        t->var = findVar(t->name);
        if (t->var->ty->ty != ARY)
            errorParse(nowToken(-2), name + " is not array type");
        t->expresson = expression();
        expect("]");
        t->type  = ND_ARRDEREF;
        t->ctype = t->var->ty->ary_of;
    }
    else if (consume("."))
    {
        t->type = ND_DOT;
        t->var  = findVar(name);
        if (t->var->ty->ty != VarType::STRUCT)
        {
            errorParse(nowToken(), name + " is not a struct");
        }
        std::string name2(nowToken().start, nowToken().end);
        expect("id");
        if (t->var->ty->members.find(name2) == t->var->ty->members.end())
        {
            errorParse(nowToken(), name2 + " is not a member of " + name + " struct");
        }
        t->name  = name2;
        t->ctype = t->var->ty->members[name2];
    }
    else
    {
        t->type  = ND_VARREF;
        t->var   = findVar(t->name);
        t->ctype = t->var->ty;
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
    //"*"
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
        errorParse(nowToken(), "Expect unary operator");
    }
    return n;
}

/*
selection_statement
        : IF '(' expression ')' statement ELSE statement
        | IF '(' expression ')' statement
    ;
*/
Node* selection_statement()
{
    auto n  = newNode();
    n->type = ND_IF;
    expect("if");
    expect("(");
    n->condition = expression();
    if (not isMatchType(boolType(), n->condition->ctype))
    {
        errorParse(nowToken(), "Condition expression must be boolean");
    }
    expect(")");
    n->then = statement();
    if (consume("else"))
    {
        n->els = statement();
    }
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
        if (not isMatchType(boolType(), n->condition->ctype))
        {
            errorParse(nowToken(), "Condition expression must be boolean");
        }
        n->body = statement();
    }
    else if (consume("for"))
    {
        n->type = ND_FOR;
        expect("(");
        n->init = expression();
        expect(";");
        n->condition = expression();
        if (not isMatchType(boolType(), n->condition->ctype))
        {
            errorParse(nowToken(), "Condition expression must be boolean");
        }
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
    std::vector<Var*>* arr = new std::vector<Var*>{};
    declaration(arr);
    while (consume("void") or consume("char") or consume("_Bool") or consume("int") or consume("double") or
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
    auto be = arr->size();
    declarator_list(arr);
    expect(";");
    for (be; be != arr->size(); be++)
    {
        auto&& i = (*arr)[be];
        if (i->isArray)
        {
            auto ty2    = new Type{};
            ty2->ty     = VarType::ARY;
            ty2->ary_of = ty;
            ty2->len    = i->arrLen;
            ty2->size   = i->arrLen * ty->size;
            i->ty       = ty2;
        }
        else
        {
            i->ty = ty;
        }
        addVar(i);
        if (i->data != nullptr)
        {
            if (not isMatchType(i->ty, i->data->ctype))
            {
                errorParse(nowToken(-1), "Unmatched type in Initialization");
            }
        }
    }
}

/*
返回 说明符vector ,其他信息待填充
仅包含变量名,是否为数组/数组长度,
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
    for (auto&& i : paraVar)
    {
        addVar(i);
    }
    paraVar.clear(); //!!内存泄露
    auto t  = newNode();
    t->type = ND_COMP_STMT;
    while (not consume("}"))
    {
        if (consume("return") || consume("{") || consume("if") || consume("while") || consume("for") || consume(";") ||
            consume("id") || consume("*"))
        {
            --pos;
            t->statementList = statement_list();
        }
        else if (consume("void") || consume("char") || consume("_Bool") || consume("int") || consume("double") ||
                 consume("struct"))
        {
            --pos;
            auto decl = declaration_list();
            // for (auto&& i : *decl)
            // {
            //     addVar(i);
            // }
            if (consume("}"))
            {
                --pos;
                t->statementList = nullptr;
            }
            else
            {
                t->statementList = statement_list(); //函数调用
            }
        }
    }
    exitScope();
    return t;
}

/*
返回语句的vector
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
   | iteration_statement     //while for
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
    else if (consume(";") || consume("id") || consume("*") || consume("("))
    {
        --pos;
        return assignment_statement();
    }
    else if (consume("return"))
    {
        // --pos;
        auto t       = newNode();
        t->type      = NodeType::ND_RETURN;
        t->expresson = expression();
        expect(";");
        t->ctype = t->expresson->ctype;
        if (not isMatchType(prog->funcs.back()->returnType, t->expresson->ctype))
        {
            errorParse(nowToken(-1), "Error return type");
        }
        return t;
    }
    else
    {
        errorParse(nowToken(), "Expect statement");
    }
    return nullptr;
}

/*
单行语句
assignment_statement
   : ';'
   ;  postfix_expression ";"
//    |  l_expression '=' expression ';'
*/
Node* assignment_statement()
{
    if (consume(";"))
    {
        auto t       = newNode();
        t->type      = NodeType::ND_EXPR_STMT;
        t->ctype     = voidType();
        t->expresson = nullptr; //表示空语句
        return t;
    }
    auto t = postfix_expression();
    expect(";");
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
        auto orll   = newNode();
        orll->type  = ND_LOGOR;
        orll->ctype = boolType();
        if (flag == 0)
        {
            orll->lhs = t;
            orll->rhs = logical_and_expression();
            t         = orll;
            // flag      = (flag + 1) % 2;
            //涉及到短路求值,运算顺序不能变
        }
        else if (flag == 1)
        {
            orll->lhs = logical_and_expression();
            orll->rhs = t;
            t         = orll;
            flag      = (flag + 1) % 2;
        }
        if ((not isMatchType(boolType(), orll->lhs->ctype)) or (not isMatchType(boolType(), orll->rhs->ctype)))
        {
            errorParse(nowToken(), "invalid type argument of || ");
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
        auto orll   = newNode();
        orll->type  = ND_LOGAND;
        orll->ctype = boolType();
        if (flag == 0)
        {
            orll->lhs = t;
            orll->rhs = equality_expression();
            t         = orll;
            // flag      = (flag + 1) % 2;
        }
        else if (flag == 1)
        {
            orll->lhs = equality_expression();
            orll->rhs = t;
            t         = orll;
            flag      = (flag + 1) % 2;
        }
        if ((not isMatchType(boolType(), orll->lhs->ctype)) or (not isMatchType(boolType(), orll->rhs->ctype)))
        {
            errorParse(nowToken(), "invalid type argument of && ");
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
        auto orll   = newNode();
        orll->ctype = boolType();
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
            orll->lhs = t;
            orll->rhs = relational_expression();
            t         = orll;
            // flag      = (flag + 1) % 2;
        }
        else if (flag == 1)
        {
            orll->lhs = relational_expression();
            orll->rhs = t;
            t         = orll;
            flag      = (flag + 1) % 2;
        }
        if (not isMatchType(orll->lhs->ctype, orll->rhs->ctype))
        {
            errorParse(nowToken(), "invalid type argument of ==/!= ");
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
        auto orll   = newNode();
        orll->ctype = boolType();
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
            orll->lhs = t;
            orll->rhs = additive_expression();
            t         = orll;
            // flag      = (flag + 1) % 2;
        }
        else if (flag == 1)
        {
            orll->lhs = additive_expression();
            orll->rhs = t;
            t         = orll;
            flag      = (flag + 1) % 2;
        }
        if (not isMatchType(orll->lhs->ctype, orll->rhs->ctype))
        {
            errorParse(nowToken(), "invalid type argument of < <= > >= ");
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
        if (not(isCalAble(orll->lhs->ctype) and isCalAble(orll->rhs->ctype)))
        {
            errorParse(nowToken(), "invalid type argument of + / - ");
        }
        orll->ctype = topType(orll->lhs->ctype, orll->rhs->ctype);
        if (orll->ctype == nullptr)
        {
            errorParse(nowToken(), "invalid type argument of + / - ");
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
        if (not(isCalAble(orll->lhs->ctype) and isCalAble(orll->rhs->ctype)))
        {
            errorParse(nowToken(), "invalid type argument of * / / ");
        }
        orll->ctype = topType(orll->lhs->ctype, orll->rhs->ctype);
        if (orll->ctype == nullptr)
        {
            errorParse(nowToken(), "invalid type argument of * / / ");
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
    if (consume("-") or consume("!")) //取地址符 &
    {
        pos--;
        auto t = unary_operator();
        t->lhs = postfix_expression();
        if (not isCalAble(t->lhs->ctype))
        {
            errorParse(nowToken(), "invalid type argument of unary expression");
        }
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
        t->fun    = findFuncion(name);
        t->ctype  = t->fun->returnType;
        consume("id");
        consume("(");
        if (consume(")"))
        {
            if (t->fun->params == nullptr or t->fun->params->empty())
                t->args = new std::vector<Node*>{};
            else
                errorParse(nowToken(-3), "Arguments do not match parameters");
        }
        else
        {
            t->args       = expression_list();
            auto&& argarr = *(t->args);
            if (t->args->size() != t->fun->params->size())
                errorParse(nowToken(), "Arguments do not match parameters");
            for (size_t i = 0; i < t->fun->params->size(); i++)
            {
                if (not isMatchType(t->fun->params->at(i), argarr[i]->ctype))
                {
                    errorParse(nowToken(), "Arguments do not match parameters");
                }
            }
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
    | CHAR_CONSTANT
    | '(' expression ')'
    ;
*/
Node* primary_expression()
{
    auto t = newNode();
    if (consume("num"))
    {
        t->type  = ND_NUM;
        t->ctype = intType();
        t->val   = nowToken(-1).val;
    }
    else if (consume("lchar"))
    {
        t->type  = ND_CHAR;
        t->ctype = charType();
        t->val   = nowToken(-1).val;
    }
    else if (consume("dnum"))
    {
        t->type  = ND_DNUM;
        t->ctype = doubleType();
        t->dval  = nowToken(-1).dval;
    }
    else if (consume("str")) //要特别判断
    {
        t->type = ND_STR;
        t->name = nowName(-1);
        // prog->stringlit.push_back(t->name);
        auto charp    = new Type{};
        charp->size   = 8;
        charp->ty     = PTR;
        charp->ptr_to = charType();
        t->ctype      = charp;
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
            if (not isMatchType(orll->lhs->ctype, orll->rhs->ctype))
                errorParse(nowToken(), "invalid type argument of = ");
        }
        else if (consume("++") or consume("--"))
        {
            pos--;
            auto orll   = newNode();
            orll->ctype = t->ctype;
            orll->lhs   = t;
            if (consume("++"))
            {
                switch (orll->ctype->ty)
                {
                case VarType::ARY:
                case VarType::BOOL:
                case VarType::VOID:
                case VarType::STRUCT:
                    errorParse(nowToken(), "invalid type argument of ++");
                    break;
                case VarType::PTR:
                case VarType::DOUBLE:
                case VarType::INT:
                case VarType::CHAR:
                default:
                    break;
                }
                orll->type = ND_INC;
            }
            else if (consume("--"))
            {
                switch (orll->ctype->ty)
                {
                case VarType::ARY:
                case VarType::BOOL:
                case VarType::VOID:
                case VarType::STRUCT:
                    errorParse(nowToken(), "invalid type argument of --");
                    break;
                case VarType::PTR:
                case VarType::DOUBLE:
                case VarType::INT:
                case VarType::CHAR:
                default:
                    break;
                }
                orll->type = ND_DEC;
            }
            t = orll;
        }
        else if (consume("(")) // Function call
        {
            ;
        }
    }
    return t;
}
//--------------cc的分割线----------------
