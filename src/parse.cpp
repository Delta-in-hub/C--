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









//张雯越
/*translation_unit
    | type_specifier fun_declarator compound_statement  translation_unit?
    | type_specifier declarator_list ';' translation_unit?
    ;
*/
Node* translation_unit()
{

}

/*type_specifier
    : VOID pointer?
    | INT pointer?
    | FLOAT pointer?
    | char pointer?
    | double pointer?
    | bool pointer?
    | struct-declaration pointer?
    ;
*/
Node* type_specifier()
{

}


/*struct-declaration :
    'struct' IDENTIFIER '{'  struct-declaration-list  '}'                                  ?????????????????????????//
*/
Node* struct_declaration()
{

}
/*struct-declaration-list :                                                                    左递归
    : declaration_list 
    | struct-declaration-list declaration_list
    ;
*/    
Node* struct_eclaration_list()
{

}


/*pointer :
    | '*' 
    | '*' pointer
*/
Node* pointer()
{

}


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
parameter_list                                                 左递归
    : parameter_declaration
    | parameter_list ',' parameter_declaration
    ;
*/
Node* parameter_list()                   
{

}



/*parameter_declaration
    : type_specifier declarator
    ;
*/

Node* parameter_declaration()
{
    type_specifier();
    declarator();
}