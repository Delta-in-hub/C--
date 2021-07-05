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
/*
语法递归下降
*/

// translation-unit ::=
// external-declaration
// translation-unit external-declaration
Node* translation_unit()
{
    ;
}

/*
external-declaration ::=
function-definition
declaration
*/
Node* external_declaration()
{
    ;
}

/*
function-definition ::=
declaration-specifiers declarator declaration-list? compound-statement
*/
Node* function_definition()
{
    ;
}

/*
declaration-specifiers ::=
storage-class-specifier declaration-specifiers?
type-specifier declaration-specifiers?
type-qualifier declaration-specifiers?
function-specifier declaration-specifiers?
*/
Node* declaration_specifiers()
{
    ;
}

/*
storage-class-specifier ::=
?typedef?
?extern?
?static?
?auto?
?register?
*/
Node* storage_class_specifier()
{
    ;
}

/*
type-specifier ::=
?void?
?char?
?short?
?int?
?long?
?float?
?double?
?signed?
?unsigned?
?_Bool?
struct-or-union-specifier
enum-specifier
typedef-name
*/
Node* type_specifier()
{
    if (consume("void"))
    {
        ;
    }
    else if (consume("char"))
    {
        ;
    }
    else if (consume("short") or consume("int") or consume("long") or consume("signed") or consume("unsigned"))
    {
        ;
    }
    else if (consume("float") or consume("double"))
    {
        ;
    }
    else if (consume("_Bool"))
    {
        ;
    }
    else
    {
        ;
    }
}

/*
enumerator-list ::=
enumerator
enumerator-list "," enumerator
*/
Node* enumerator_list()
{
    ;
}

/*
enum-specifier ::=
?enum? identifier? "{" enumerator-list "}"
?enum? identifier? "{" enumerator-list "," "}"
?enum? identifier
*/
Node* enum_specifier()
{
    ;
}

/*
struct-or-union-specifier ::=
struct-or-union identifier? "{" struct-declaration-list "}"
struct-or-union identifier
*/
Node* struct_or_union_specifier()
{
    ;
}

/*
struct-or-union ::=
?struct?
?union?
*/
Node* struct_or_union()
{
    ;
}

/*
struct-declaration-list ::=
struct-declaration
struct-declaration-list struct-declaration
*/
Node* struct_declaration_list()
{
    ;
}

/*
struct-declaration ::=
specifier-qualifier-list struct-declarator-list ";"
*/
Node* struct_declaration()
{
    ;
}

/*
specifier-qualifier-list ::=
type-specifier specifier-qualifier-list?
type-qualifier specifier-qualifier-list?
*/
Node* specifier_qualifier_list()
{
    ;
}

/*
type-qualifier ::=
?const?
?restrict?
?volatile?
*/
Node* type_qualifier()
{
    if (consume("const"))
    {
        return nullptr;
    }
    else if (consume("restrict"))
    {
        return nullptr;
    }
    else if (consume("volatile"))
    {
        return nullptr;
    }
    else
        errorParse(tokenArr[pos], "Error:type-qualifier");
}

// [translation-unit,direct-declarator] <-   张天泽

/*
assignment-expression::=
conditional-expression
unary-expression
assignment-operator
assignment-expression
*/



// [assignment_expression,expression] <-   武熠栋