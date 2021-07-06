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
unary-expression assignment-operator assignment-expression
*/
Node* assignment_expression() {
    
    conditional_expression();
}

/*
conditional-expression::=
logical-OR-expression
logical-OR-expression"?"expression":"conditional-expression
*/
Node* conditional_expression() {

}
/*
logical-OR-expression::=
logical-AND-expression
logical-OR-expression"||"logical-AND-expression
*/


Node* logical_OR_expression(){

}

/*
logical-AND-expression::=
inclusive-OR-expression
logical-AND-expression"&&"inclusive-OR-expression
*/

Node* logical_AND_expression() {

}
/*
inclusive-OR-expression::=
exclusive-OR-expression
inclusive-OR-expression"|"exclusive-OR-expression
*/

Node* inclusive_OR_expression() {

}
/*
exclusive-OR-expression::=
AND-expression
exclusive-OR-expression"^"AND-expression
*/

Node* exclusive_OR_expression() {

}
/*
AND-expression::=
equality-expression
AND-expression"&"equality-expression
*/
Node* AND_expression() {

}
/*
equality-expression::=
relational-expression
equality-expression"=="relational-expression
equality-expression"!="relational-expression
*/

Node* equality_expression() {

}
/*
relational-expression::=
shift-expression
relational-expression"<"shift-expression
relational-expression">"shift-expression
relational-expression"<="shift-expression
relational-expression">="shift-expression
*/

Node* relational_expression() {

}

/*
shift-expression::=
additive-expression
shift-expression"<<"additive-expression
shift-expression">>"additive-expression
*/

Node* shift_expression() {

}

/*
additive-expression::=
multiplicative-expression
additive-expression"+"multiplicative-expression
additive-expression"-"multiplicative-expression
*/
Node* additive_expression() {

}
/*
multiplicative-expression::=
cast-expression
multiplicative-expression"*"cast-expression
multiplicative-expression"/"cast-expression
multiplicative-expression"%"cast-expression
*/

Node* multiplicative_expression(){

}

/*
cast-expression::=
unary-expression
"("type-name")"cast-expression
*/

Node* cast_expression() {

}
/*
unary-expression::=
postfix-expression
"++"unary-expression
"--"unary-expression
unary-operator cast-expression
"sizeof"unary-expression
"sizeof""("type-name")"
*/

Node* unary_expression() {

}
/*
postfix-expression::=
primary-expression
postfix-expression"["expression"]"
postfix-expression"("argument-expression-list?")"
postfix-expression"."identifier
postfix-expression"->"identifier
postfix-expression"++"
postfix-expression"--"
"("type-name")""{"initializer-list"}"
"("type-name")""{"initializer-list",""}"
*/

Node* postfix_expression() {

}
/*
primary-expression::=
identifier
constant
string_literal
"("expression")"
*/
Node* primary_expression() {

}
/*
expression::=
assignment-expression
expression","assignment-expression
*/

Node* expression() {

}

// [assignment_expression,expression] <-   武熠栋