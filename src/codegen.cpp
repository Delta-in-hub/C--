#include "codegen.h"

using namespace std;
// extern Program* prog;
// extern Env *env, *global;
// extern std::unordered_map<Var*, std::pair<int, double>> varInitVal;

FILE* outasm = nullptr;
void fileInit()
{
    using namespace std;
    string outputPath(path);
    size_t pos = 0;
    for (int i = outputPath.size() - 1; i >= 0; i--)
    {
        if (outputPath[i] == '.')
        {
            pos = i;
            break;
        }
    }
    auto substr = outputPath.substr(0, pos + 1);
    substr += "asm";
    fopen_s(&outasm, substr.c_str(), "w");
    if (outasm == nullptr)
    {
        printf("Open file %s failed", substr.c_str());
        exit(1);
    }
}

void fileClose()
{
    if (outasm)
        fclose(outasm);
}

void emit(const std::string& str)
{
    fprintf(outasm, str.c_str());
}

unordered_map<string, string> stringLabel;
string emitString(const string& str)
{
    static int cnt = 0;
    if (stringLabel.find(str) == stringLabel.end())
    {
        stringLabel[str] = "CSTRING_" + to_string(cnt++);
    }
    return stringLabel[str];
}

pair<int, string> tySize(Type* t)
{
    switch (t->ty)
    {
    case VOID:
        printf("VOID Variable is rejected");
        exit(1);
        break;
    case BOOL:
    case CHAR:
        return {1, "BYTE"};
        break;
    case INT:
        return {4, "DWORD"};
        break;
    case DOUBLE:
    case PTR:    //待处理
    case ARY:    //待处理 arr :   times 10 db 2
    case STRUCT: //待处理
        return {8, "QWORD "};
        break;
    default:
        break;
    }
    return {4, "DWORD"};
}

queue<string> regQueue;
unordered_set<string> allReg;
void initRegQueue()
{
    while (not regQueue.empty())
        regQueue.pop();
    regQueue.push("eax");
    regQueue.push("ecx");
    regQueue.push("edx");
    regQueue.push("esi");
    regQueue.push("edi");
    regQueue.push("r8d");
    regQueue.push("r9d");
    regQueue.push("r10d");
    regQueue.push("r11d");
    allReg.clear();
    int cnt = regQueue.size();
    while (cnt--)
    {
        auto p = regQueue.front();
        allReg.insert(p);
        regQueue.pop();
        regQueue.push(p);
    }
}

string beforReg;
string getReg()
{
    auto t   = regQueue.front();
    beforReg = regQueue.back();
    regQueue.pop();
    regQueue.push(t);
    return t;
}

struct Operation
{
    string op, lhs, rhs;
    // string type;
};
void genStatementList(vector<Node*>* arr, vector<string>& code, bool flag = false);
unordered_map<string, int> offset;
int labelNum = 0;

void to64bits(string& a)
{
    if (allReg.find(a) != allReg.end())
    {
        if (a[0] == 'e')
            a[0] = 'r';
        else if (a.back() == 'd')
        {
            a.back() = ' ';
        }
        return;
    }
    auto p = a.find("DWORD");
    if (p != string::npos)
    {
        a[p] = 'Q';
    }
    return;
}

void removeDWORD(string& a, string& b)
{
    auto p1 = a.find("DWORD");
    auto p2 = b.find("DWORD");
    if (p1 != string::npos and p2 != string::npos)
    {
        b = b.substr(p2 + 5);
    }
    return;
}

Operation genStatement(Node* i, vector<string>& code)
{
    if (i == nullptr)
        return {"", "", ""};
    switch (i->type)
    {
    case ND_COMP_STMT:
        genStatementList(i->statementList, code);
        break;
    case ND_VARDEF:
    case ND_VARREF: {
        auto p = offset.find(i->name);
        if (p == offset.end())
        { //全局变量
            return {"VAR", "DWORD [rel " + i->name + "]", ""};
        }
        else
        {
            string addres;
            if (p->second >= 0)
            {
                addres = "+";
            }
            addres += to_string(p->second);
            return {"VAR", "DWORD [rbp" + addres + "]", ""};
        }
    }
    case ND_RETURN: {
        code.push_back("mov\trsp,rbp");
        if (i->expresson)
        {
            auto&& res = genStatement(i->expresson, code);
            code.push_back("mov\t" + getReg() + "," + res.lhs);
            // to64bits(res.lhs);
            // code.push_back("push\t" + res.lhs);
        }
        code.push_back("pop\trbp");
        code.push_back("ret");
        break;
    }
    case ND_ASSIGN: {
        auto&& res2 = genStatement(i->rhs, code);
        auto&& res1 = genStatement(i->lhs, code);
        removeDWORD(res1.lhs, res2.lhs);
        code.push_back("mov\t" + res1.lhs + "," + res2.lhs);
        return {"=", res1.lhs, ""};
    }
    case ND_IF: {
        if (i->then == nullptr and i->els == nullptr)
        {
            break;
        }
        auto&& res = genStatement(i->condition, code);
        switch (i->condition->type)
        {
        case ND_EQ: {
            code.push_back("je\t.IF_THEN_" + to_string(labelNum));
            break;
        }
        case ND_NE: {
            code.push_back("jne\t.IF_THEN_" + to_string(labelNum));
            break;
        }
        case ND_LE: {
            code.push_back("jle\t.IF_THEN_" + to_string(labelNum));
            break;
        }
        case ND_GE: {
            code.push_back("jge\t.IF_THEN_" + to_string(labelNum));
            break;
        }
        case ND_LESS: {
            code.push_back("jl\t.IF_THEN_" + to_string(labelNum));
            break;
        }
        case ND_GREAT: {
            code.push_back("jg\t.IF_THEN_" + to_string(labelNum));
            break;
        }
        default:
            break;
        }
        auto&& res1 = genStatement(i->els, code);
        code.push_back("jmp\t.IF_END_" + to_string(labelNum));
        code.push_back(".IF_THEN_" + to_string(labelNum) + ": \n");
        auto&& res2 = genStatement(i->then, code);
        code.push_back(".IF_END_" + to_string(labelNum) + ": \n");
        labelNum++;
        break;
    }
    case ND_DO_WHILE: {
        if (i->body == nullptr)
            break;
        code.push_back(".WHILE_START_" + to_string(labelNum) + " :\n");
        auto&& res = genStatement(i->condition, code);
        switch (i->condition->type)
        {
        case ND_EQ: {
            code.push_back("je\t.WHILE_THEN_" + to_string(labelNum));
            break;
        }
        case ND_NE: {
            code.push_back("jne\t.WHILE_THEN_" + to_string(labelNum));
            break;
        }
        case ND_LE: {
            code.push_back("jle\t.WHILE_THEN_" + to_string(labelNum));
            break;
        }
        case ND_GE: {
            code.push_back("jge\t.WHILE_THEN_" + to_string(labelNum));
            break;
        }
        case ND_LESS: {
            code.push_back("jl\t.WHILE_THEN_" + to_string(labelNum));
            break;
        }
        case ND_GREAT: {
            code.push_back("jg\t.WHILE_THEN_" + to_string(labelNum));
            break;
        }
        default:
            break;
        }
        code.push_back("jmp\t.WHILE_END_" + to_string(labelNum));
        code.push_back(".WHILE_THEN_" + to_string(labelNum) + " : \n");
        auto&& res1 = genStatement(i->body, code);
        code.push_back("jmp\t.WHILE_START_" + to_string(labelNum));
        code.push_back(".WHILE_END_" + to_string(labelNum) + " :\n");
        labelNum++;
        break;
    }
    case ND_FOR: {
        if (i->body == nullptr)
            break;
        genStatement(i->init, code);
        code.push_back(".FOR_START_" + to_string(labelNum) + " : \n");
        auto&& res = genStatement(i->condition, code);
        switch (i->condition->type)
        {
        case ND_EQ: {
            code.push_back("je\t.FOR_THEN_" + to_string(labelNum));
            break;
        }
        case ND_NE: {
            code.push_back("jne\t.FOR_THEN_" + to_string(labelNum));
            break;
        }
        case ND_LE: {
            code.push_back("jle\t.FOR_THEN_" + to_string(labelNum));
            break;
        }
        case ND_GE: {
            code.push_back("jge\t.FOR_THEN_" + to_string(labelNum));
            break;
        }
        case ND_LESS: {
            code.push_back("jl\t.FOR_THEN_" + to_string(labelNum));
            break;
        }
        case ND_GREAT: {
            code.push_back("jg\t.FOR_THEN_" + to_string(labelNum));
            break;
        }
        default:
            break;
        }
        code.push_back("jmp\t.FOR_END_" + to_string(labelNum));
        code.push_back(".FOR_THEN_" + to_string(labelNum) + " : \n");
        genStatement(i->body, code);
        genStatement(i->inc, code);
        code.push_back("jmp\t.FOR_START_" + to_string(labelNum));
        code.push_back(".FOR_END_" + to_string(labelNum) + " : \n");
        labelNum++;
        break;
    }
    case ND_INC: {
        auto&& res = genStatement(i->lhs, code);
        code.push_back("INC\t" + res.lhs);
        return {"VAR", res.lhs, ""};
    }
    case ND_DEC: {
        auto&& res = genStatement(i->lhs, code);
        code.push_back("DEC\t" + res.lhs);
        return {"VAR", res.lhs, ""};
    }
    case ND_DEREF: {
        auto&& r = getReg();
        code.push_back("push\teax");
        code.push_back("mov\teax,[rel " + i->lhs->name + "]");
        code.push_back("mov\t" + r + ",[eax]");
        code.push_back("pop\teax");
        return {"VAR", r, ""};
    }
    case ND_ARRDEREF: {
        auto&& res    = genStatement(i->expresson, code);
        string addres = "DWORD [rel " + i->name + res.lhs + "]";
        return {"VAR", addres, ""};
    }
    case ND_NUM:
    case ND_CHAR:
    case ND_DNUM: {
        return {"NUM", to_string(i->val), ""};
    }
    case ND_EQ: {
    }
    case ND_NE: {
    }
    case ND_LE: {
    }
    case ND_GE: {
    }
    case ND_LESS: {
    }
    case ND_GREAT: {
        auto&& res1 = genStatement(i->lhs, code);
        auto&& res2 = genStatement(i->rhs, code);
        auto reg    = getReg();
        code.push_back("mov\t" + reg + "," + res1.lhs);
        code.push_back("cmp\t" + reg + "," + res2.lhs);
        return {"flag", "", ""};
    }
    case ND_ADD: {
        auto&& res1 = genStatement(i->lhs, code);
        auto&& res2 = genStatement(i->rhs, code);
        auto reg    = getReg();
        code.push_back("mov\t" + reg + "," + res1.lhs);
        code.push_back("add\t" + reg + "," + res2.lhs);
        return {"ADD", reg, ""};
    }
    case ND_SUB: {
        auto&& res1 = genStatement(i->lhs, code);
        auto&& res2 = genStatement(i->rhs, code);
        auto reg    = getReg();
        code.push_back("mov\t" + reg + "," + res1.lhs);
        code.push_back("sub\t" + reg + "," + res2.lhs);
        return {"SUB", reg, ""};
    }
    case ND_LOGAND: {
        auto&& res1 = genStatement(i->lhs, code);
        auto&& res2 = genStatement(i->rhs, code);
        auto reg    = getReg();
        code.push_back("mov\t" + reg + "," + res1.lhs);
        code.push_back("and\t" + reg + "," + res2.lhs);
        return {"AND", reg, ""};
    }
    case ND_LOGOR: {
        auto&& res1 = genStatement(i->lhs, code);
        auto&& res2 = genStatement(i->rhs, code);
        auto reg    = getReg();
        code.push_back("mov\t" + reg + "," + res1.lhs);
        code.push_back("or\t" + reg + "," + res2.lhs);
        return {"OR", reg, ""};
    }
    case ND_NEG: {
        auto&& res1 = genStatement(i->lhs, code);
        auto reg    = getReg();
        code.push_back("mov\t" + reg + "," + res1.lhs);
        code.push_back("neg\t" + reg);
        return {"NEG", reg, ""};
    }
    case ND_NOT: {
        auto&& res1 = genStatement(i->lhs, code);
        auto reg    = getReg();
        code.push_back("mov\t" + reg + "," + res1.lhs);
        code.push_back("not\t" + reg);
        return {"NOT", reg, ""};
    }
    case ND_MUL: {
        auto&& res1 = genStatement(i->lhs, code);
        auto&& res2 = genStatement(i->rhs, code);
        code.push_back("push rax");
        code.push_back("push rdx");
        code.push_back("mov\teax," + res1.lhs);
        code.push_back("imul\t" + res2.lhs);
        auto reg = getReg();
        code.push_back("mov\t" + reg + ",eax");
        code.push_back("pop\trdx");
        code.push_back("pop\trax");
        return {"MUL", reg, ""};
    }
    case ND_DIV: {
        auto&& res1 = genStatement(i->lhs, code);
        auto&& res2 = genStatement(i->rhs, code);
        code.push_back("push rax");
        code.push_back("push rdx");
        code.push_back("mov\tedx,0");
        code.push_back("mov\teax," + res1.lhs);
        code.push_back("idiv\t" + res2.lhs);
        auto reg = getReg();
        code.push_back("mov\t" + reg + ",eax");
        code.push_back("pop\trdx");
        code.push_back("pop\trax");
        return {"MUL", reg, ""};
    }
    case ND_MOD: {
        auto&& res1 = genStatement(i->lhs, code);
        auto&& res2 = genStatement(i->rhs, code);
        code.push_back("push rax");
        code.push_back("push rdx");
        code.push_back("mov\tedx,0");
        code.push_back("mov\teax," + res1.lhs);
        code.push_back("idiv\t" + res2.lhs);
        auto reg = getReg();
        code.push_back("mov\t" + reg + ",edx");
        code.push_back("pop\trdx");
        code.push_back("pop\trax");
        return {"MUL", reg, ""};
    }
    case ND_CALL: {
        /*
        mov     rcx,[rel str]
        sub     rsp,28h
        call    puts                            ; puts(message)
        add     rsp,28h
        */
        code.push_back("sub     rsp, 28h");
        if (i->name == "puts")
        {
            auto&& res    = genStatement(i->args->at(0), code);
            string substr = res.lhs.substr(res.lhs.find("["));
            code.push_back("mov\trcx," + substr);
            code.push_back("call\tputs");
            code.push_back("add     rsp,28h");
            return {"CALL", "", ""};
        }
        bool flag = false;
        int cnt   = 0;
        for (auto&& funs : prog->funcs)
        {
            if (funs->name == i->name)
            {
                for (auto&& fa : *(i->args))
                {
                    cnt++;
                    auto&& res = genStatement(fa, code);
                    to64bits(res.lhs);
                    code.push_back("push\t" + res.lhs);
                }
                if (funs->returnType->ty != VOID)
                {
                    flag = true;
                }
                break;
            }
        }
        code.push_back("call\t" + i->name);
        auto reg = getReg();
        if (flag)
        {
            to64bits(reg);
            code.push_back("pop\t" + reg);
        }
        for (int i = 0; i < cnt; i++)
        {
            auto reg2 = getReg();
            to64bits(reg2);
            code.push_back("pop\t" + reg2);
        }

        code.push_back("add     rsp, 28h ");
        return {"CALL", beforReg, ""};
        break;
    }
    case ND_STR: // wait for
        break;
    case ND_NULL:
    case ND_DOT:
    default:
        break;
    }
    return {"ERROR", "eax", ""};
}

void genStatementList(vector<Node*>* arr, vector<string>& code, bool flag)
{
    int varSize = 0;
    // code.push_back("push\trbp");
    if (flag)
    {
        code.push_back("push\trbp");
        code.push_back("mov\trbp,rsp");
    }
    for (auto&& i : *(arr))
    {
        switch (i->type)
        {
        case ND_COMP_STMT:
            genStatementList(i->statementList, code);
            break;
        case ND_VARDEF: {
            auto&& ty = tySize(i->var->ty);
            varSize += ty.first;
            code.push_back("sub\trsp," + to_string(ty.first));
            offset[i->name] = -varSize;
            if (i->var->data)
            {
                auto&& res = genStatement(i->var->data, code);
                string l   = "DWORD [rbp" + to_string(offset[i->name]) + "]";
                removeDWORD(l, res.lhs);
                code.push_back("mov\t " + l + "," + res.lhs);
            }
            break;
        }
        case ND_RETURN: {
        }
        case ND_ASSIGN: {
        }
        case ND_IF: {
        }
        case ND_DO_WHILE: {
        }
        case ND_FOR: {
        }
        case ND_INC:
        case ND_DEC:

        case ND_VARREF:
        case ND_DEREF:
        case ND_ARRDEREF:
        case ND_DOT:
        case ND_NUM:
        case ND_CHAR:
        case ND_DNUM:
        case ND_STR:
        case ND_NULL:
        default:
            genStatement(i, code);
            break;
        }
    }
    if (varSize)
    {
        code.push_back("add\trsp," + to_string(varSize));
    }
    if (flag)
    {
        code.push_back("mov\trsp,rbp");
        code.push_back("pop\trbp");
    }
}

/*
myputs:

        ;mov     rcx, message                    ; First argument is address of message
        lea     rcx,[rel message]
        sub     rsp,28h
        call    puts                            ; puts(message)
        add     rsp,28h
        ret
.mainend:
*/
vector<string> genFunction(Function* f)
{
    initRegQueue();
    offset.clear();
    auto&& arr = *(f->paraName);
    int off    = 0x10; // Return address -> 8h     rbp -> 8h   (bytes)
    for (int i = arr.size() - 1; i >= 0; i--)
    {
        offset[arr[i]] = off;
        off += 4;
    }
    vector<string> code;
    auto pb = [&](const string& str) { code.emplace_back(move(str)); };
    pb(f->name + " :\n\n");
    // sub rsp, 28h;
    genStatementList(f->compound->statementList, code, true);
    code.push_back("ret");
    return code;
}

void codeGen()
{
    fileInit();
    emit("section .data");
    for (auto&& i : prog->gvars)
    {
        emit("\n" + i->name + " : \n");
        switch (i->ty->ty)
        {
        case VOID:
            printf("VOID Variable is rejected");
            exit(1);
            break;
        case BOOL:
            if (i->data == nullptr)
                emit("\tdb 0");
            else
                emit("\tdb " + to_string(i->data->val));
            break;
        case CHAR:
            if (i->data == nullptr)
                emit("\tdb 0");
            else
                emit("\tdb " + to_string(i->data->val));
            break;
        case INT:
            if (i->data == nullptr)
                emit("\tdd 0");
            else
                emit("\tdd " + to_string(i->data->val));
            // db  1 byte     dd 4 byte        dq 8byte
            break;
        case DOUBLE:
            if (i->data == nullptr)
                emit("\tdq 0");
            else
                emit("\tdq " + to_string(i->data->dval));
            break;
        case PTR: //待处理
            if (i->data == nullptr)
                emit("\tdq 0");
            else
            {
                if (i->data->type == ND_STR)
                {
                    auto&& res = emitString(i->data->name);
                    emit("\tdq " + res);
                    emit("\n" + res + " : \n");
                    emit("\tdb \"" + i->data->name + "\"");
                }
                else
                {
                    emit("\tdq " + i->data->lhs->name);
                }
            }
            break;
        case ARY: //待处理 arr :   times 10 db 2
        {
            emit("\ttimes " + to_string(i->arrLen) + " ");
            switch (i->ty->ary_of->ty)
            {
            case VOID:
                printf("VOID Variable is rejected");
                exit(1);
                break;
            case BOOL:
            case CHAR:
                emit("db 0");
                break;
            case INT:
                emit("dd 0");
                break;
            case DOUBLE:
            case PTR:    //待处理
            case ARY:    //待处理 arr :   times 10 db 2
            case STRUCT: //待处理
                emit("dq 0");
                break;
            default:
                break;
            }
            break;
        }
        case STRUCT: //待处理
        {
            for (auto&& j : i->ty->members)
            {
                switch (j.second->ty)
                {
                case VOID:
                    printf("VOID Variable is rejected");
                    exit(1);
                    break;
                case BOOL:
                case CHAR:
                    emit("\tdb 0\n");
                    break;
                case INT:
                    emit("\tdd 0\n");
                    break;
                case DOUBLE:
                case PTR:    //待处理
                case ARY:    //待处理 arr :   times 10 db 2
                case STRUCT: //待处理
                    emit("\tdq 0\n");
                    break;
                default:
                    break;
                }
            }
            break;
        }
        default:
            break;
        }
    }

    emit("\n\n        global  main\n        extern  puts\n        section .text\n");

    vector<vector<string>> codes;
    int p = 0;
    for (auto&& i : prog->funcs)
    {
        if (i->name == "main")
        {
            p = codes.size();
        }
        if (i->compound == nullptr or i->name == "puts")
            continue;
        codes.emplace_back(genFunction(i));
    }
    for (auto&& i : codes[p])
    {
        if (i[0] == '.' or i.find(':') != string::npos)
        {
            emit(i + "\n");
        }
        else
        {
            emit("\t" + i + "\n");
        }
    }
    for (int j = 0; j < codes.size(); j++)
    {
        if (j == p)
            continue;
        for (auto&& i : codes[j])
        {
            if (i[0] == '.' or i.find(':') != string::npos)
            {
                emit(i + "\n");
            }
            else
            {
                emit("\t" + i + "\n");
            }
        }
    }

    fileClose();
}