#include "optimize.h"

std::unordered_map<Var*, std::pair<int, double>> varInitVal;
using namespace std;

std::tuple<bool, int, double> constantCalculate(Node* node, bool inInitliza = true)
{
    using namespace std;
    switch (node->type)
    {
    case ND_NUM:
    case ND_DNUM:
    case ND_CHAR:
        node->constant = true;
        return {true, node->val, node->dval};
        break;
    case ND_STR: // name = str , ctype = char*
        //待处理
        break;
    case ND_VARREF:

        if (inInitliza and varInitVal.find(node->var) != varInitVal.end())
        {
            auto&& t       = varInitVal.find(node->var)->second;
            node->constant = true;
            node->val      = t.first;
            return {true, t.first, t.second};
        }
        else
        {
            return {false, 0, 0};
        }
        break;
    case ND_ADDR: // lhs= &p 的p
        //待处理
        break;
    case ND_DEREF: // lhs = *p 的 p
        //待处理
        break;
    case ND_EQ: {
        auto&& c1 = constantCalculate(node->lhs, inInitliza);
        auto&& c2 = constantCalculate(node->rhs, inInitliza);
        if (get<0>(c1))
        {
            node->lhs->constant = true;
            node->lhs->val      = get<1>(c1);
        }
        if (get<0>(c2))
        {
            node->rhs->constant = true;
            node->rhs->val      = get<1>(c2);
        }
        if (get<0>(c1) and get<0>(c2))
        {
            double a, b;
            if (node->lhs->type == ND_DNUM)
                a = get<2>(c1);
            else
                a = get<1>(c1);
            if (node->rhs->type == ND_DNUM)
                b = get<2>(c2);
            else
                b = get<1>(c2);
            node->constant = true;
            node->val      = a == b;
            return {true, a == b, 0};
        }
        break;
    }
    case ND_NE: {
        auto&& c1 = constantCalculate(node->lhs, inInitliza);
        auto&& c2 = constantCalculate(node->rhs, inInitliza);
        if (get<0>(c1))
        {
            node->lhs->constant = true;
            node->lhs->val      = get<1>(c1);
        }
        if (get<0>(c2))
        {
            node->rhs->constant = true;
            node->rhs->val      = get<1>(c2);
        }
        if (get<0>(c1) and get<0>(c2))
        {
            double a, b;
            if (node->lhs->type == ND_DNUM)
                a = get<2>(c1);
            else
                a = get<1>(c1);
            if (node->rhs->type == ND_DNUM)
                b = get<2>(c2);
            else
                b = get<1>(c2);
            node->constant = true;
            node->val      = a != b;
            return {true, a != b, 0};
        }
        break;
    }
    case ND_LE: {
        auto&& c1 = constantCalculate(node->lhs, inInitliza);
        auto&& c2 = constantCalculate(node->rhs, inInitliza);
        if (get<0>(c1))
        {
            node->lhs->constant = true;
            node->lhs->val      = get<1>(c1);
        }
        if (get<0>(c2))
        {
            node->rhs->constant = true;
            node->rhs->val      = get<1>(c2);
        }
        if (get<0>(c1) and get<0>(c2))
        {
            double a, b;
            if (node->lhs->type == ND_DNUM)
                a = get<2>(c1);
            else
                a = get<1>(c1);
            if (node->rhs->type == ND_DNUM)
                b = get<2>(c2);
            else
                b = get<1>(c2);
            node->constant = true;
            node->val      = a <= b;
            return {true, a <= b, 0};
        }
        break;
    }
    case ND_GE: {
        auto&& c1 = constantCalculate(node->lhs, inInitliza);
        auto&& c2 = constantCalculate(node->rhs, inInitliza);
        if (get<0>(c1))
        {
            node->lhs->constant = true;
            node->lhs->val      = get<1>(c1);
        }
        if (get<0>(c2))
        {
            node->rhs->constant = true;
            node->rhs->val      = get<1>(c2);
        }
        if (get<0>(c1) and get<0>(c2))
        {
            double a, b;
            if (node->lhs->type == ND_DNUM)
                a = get<2>(c1);
            else
                a = get<1>(c1);
            if (node->rhs->type == ND_DNUM)
                b = get<2>(c2);
            else
                b = get<1>(c2);
            node->constant = true;
            node->val      = a >= b;
            return {true, a >= b, 0};
        }
        break;
    }
    case ND_LESS: {
        auto&& c1 = constantCalculate(node->lhs, inInitliza);
        auto&& c2 = constantCalculate(node->rhs, inInitliza);
        if (get<0>(c1))
        {
            node->lhs->constant = true;
            node->lhs->val      = get<1>(c1);
        }
        if (get<0>(c2))
        {
            node->rhs->constant = true;
            node->rhs->val      = get<1>(c2);
        }
        if (get<0>(c1) and get<0>(c2))
        {
            double a, b;
            if (node->lhs->type == ND_DNUM)
                a = get<2>(c1);
            else
                a = get<1>(c1);
            if (node->rhs->type == ND_DNUM)
                b = get<2>(c2);
            else
                b = get<1>(c2);
            node->constant = true;
            node->val      = a < b;
            return {true, a < b, 0};
        }
        break;
    }
    case ND_GREAT: {
        auto&& c1 = constantCalculate(node->lhs, inInitliza);
        auto&& c2 = constantCalculate(node->rhs, inInitliza);
        if (get<0>(c1))
        {
            node->lhs->constant = true;
            node->lhs->val      = get<1>(c1);
        }
        if (get<0>(c2))
        {
            node->rhs->constant = true;
            node->rhs->val      = get<1>(c2);
        }
        if (get<0>(c1) and get<0>(c2))
        {
            double a, b;
            if (node->lhs->type == ND_DNUM)
                a = get<2>(c1);
            else
                a = get<1>(c1);
            if (node->rhs->type == ND_DNUM)
                b = get<2>(c2);
            else
                b = get<1>(c2);
            node->constant = true;
            node->val      = a > b;
            return {true, a > b, 0};
        }
        break;
    }
    case ND_ADD: {
        auto&& c1 = constantCalculate(node->lhs, inInitliza);
        auto&& c2 = constantCalculate(node->rhs, inInitliza);
        if (get<0>(c1))
        {
            node->lhs->constant = true;
            node->lhs->val      = get<1>(c1);
        }
        if (get<0>(c2))
        {
            node->rhs->constant = true;
            node->rhs->val      = get<1>(c2);
        }
        if (get<0>(c1) and get<0>(c2))
        {
            double a, b;
            if (node->lhs->type == ND_DNUM)
                a = get<2>(c1);
            else
                a = get<1>(c1);
            if (node->rhs->type == ND_DNUM)
                b = get<2>(c2);
            else
                b = get<1>(c2);
            node->constant = true;
            node->val      = a + b;
            return {true, a + b, a + b};
        }
        break;
    }
    case ND_SUB: {
        auto&& c1 = constantCalculate(node->lhs, inInitliza);
        auto&& c2 = constantCalculate(node->rhs, inInitliza);
        if (get<0>(c1))
        {
            node->lhs->constant = true;
            node->lhs->val      = get<1>(c1);
        }
        if (get<0>(c2))
        {
            node->rhs->constant = true;
            node->rhs->val      = get<1>(c2);
        }
        if (get<0>(c1) and get<0>(c2))
        {
            double a, b;
            if (node->lhs->type == ND_DNUM)
                a = get<2>(c1);
            else
                a = get<1>(c1);
            if (node->rhs->type == ND_DNUM)
                b = get<2>(c2);
            else
                b = get<1>(c2);
            node->constant = true;
            node->val      = a - b;
            return {true, a - b, a - b};
        }
        break;
    }
    case ND_LOGAND: {
        auto&& c1 = constantCalculate(node->lhs, inInitliza);
        auto&& c2 = constantCalculate(node->rhs, inInitliza);
        if (get<0>(c1))
        {
            node->lhs->constant = true;
            node->lhs->val      = get<1>(c1);
        }
        if (get<0>(c2))
        {
            node->rhs->constant = true;
            node->rhs->val      = get<1>(c2);
        }
        if (get<0>(c1) and get<0>(c2))
        {
            double a, b;
            if (node->lhs->type == ND_DNUM)
                a = get<2>(c1);
            else
                a = get<1>(c1);
            if (node->rhs->type == ND_DNUM)
                b = get<2>(c2);
            else
                b = get<1>(c2);
            node->constant = true;
            node->val      = a && b;
            return {true, a && b, a && b};
        }
        break;
    }
    case ND_LOGOR: {
        auto&& c1 = constantCalculate(node->lhs, inInitliza);
        auto&& c2 = constantCalculate(node->rhs, inInitliza);
        if (get<0>(c1))
        {
            node->lhs->constant = true;
            node->lhs->val      = get<1>(c1);
        }
        if (get<0>(c2))
        {
            node->rhs->constant = true;
            node->rhs->val      = get<1>(c2);
        }
        if (get<0>(c1) and get<0>(c2))
        {
            double a, b;
            if (node->lhs->type == ND_DNUM)
                a = get<2>(c1);
            else
                a = get<1>(c1);
            if (node->rhs->type == ND_DNUM)
                b = get<2>(c2);
            else
                b = get<1>(c2);
            node->constant = true;
            node->val      = a || b;
            return {true, a || b, a || b};
        }
        break;
    }
    case ND_NEG: {
        auto&& c1 = constantCalculate(node->lhs, inInitliza);
        if (get<0>(c1))
        {
            double a, b;
            if (node->lhs->type == ND_DNUM)
                a = get<2>(c1);
            else
                a = get<1>(c1);
            node->constant = true;
            node->val      = -a;
            return {true, -a, -a};
        }
        break;
    }
    case ND_NOT: {
        auto&& c1 = constantCalculate(node->lhs, inInitliza);
        if (get<0>(c1))
        {
            double a, b;
            if (node->lhs->type == ND_DNUM)
                a = get<2>(c1);
            else
                a = get<1>(c1);
            node->constant = true;
            node->val      = !a;
            return {true, !a, !a};
        }
        break;
    }
    case ND_MUL: {
        auto&& c1 = constantCalculate(node->lhs, inInitliza);
        auto&& c2 = constantCalculate(node->rhs, inInitliza);
        if (get<0>(c1))
        {
            node->lhs->constant = true;
            node->lhs->val      = get<1>(c1);
        }
        if (get<0>(c2))
        {
            node->rhs->constant = true;
            node->rhs->val      = get<1>(c2);
        }
        if (get<0>(c1) and get<0>(c2))
        {
            double a, b;
            if (node->lhs->type == ND_DNUM)
                a = get<2>(c1);
            else
                a = get<1>(c1);
            if (node->rhs->type == ND_DNUM)
                b = get<2>(c2);
            else
                b = get<1>(c2);
            node->constant = true;
            node->val      = a * b;
            return {true, a * b, a * b};
        }
        break;
    }
    case ND_DIV: {
        auto&& c1 = constantCalculate(node->lhs, inInitliza);
        auto&& c2 = constantCalculate(node->rhs, inInitliza);
        if (get<0>(c1))
        {
            node->lhs->constant = true;
            node->lhs->val      = get<1>(c1);
        }
        if (get<0>(c2))
        {
            node->rhs->constant = true;
            node->rhs->val      = get<1>(c2);
        }
        if (get<0>(c1) and get<0>(c2))
        {
            double a, b;
            if (node->lhs->type == ND_DNUM)
                a = get<2>(c1);
            else
                a = get<1>(c1);
            if (node->rhs->type == ND_DNUM)
                b = get<2>(c2);
            else
                b = get<1>(c2);
            node->constant = true;
            node->val      = a / b;
            return {true, a / b, a / b};
        }
        break;
    }
    case ND_MOD: {
        auto&& c1 = constantCalculate(node->lhs, inInitliza);
        auto&& c2 = constantCalculate(node->rhs, inInitliza);
        if (get<0>(c1))
        {
            node->lhs->constant = true;
            node->lhs->val      = get<1>(c1);
        }
        if (get<0>(c2))
        {
            node->rhs->constant = true;
            node->rhs->val      = get<1>(c2);
        }
        if (get<0>(c1) and get<0>(c2))
        {
            double a, b;
            if (node->lhs->type == ND_DNUM)
                a = get<2>(c1);
            else
                a = get<1>(c1);
            if (node->rhs->type == ND_DNUM)
                b = get<2>(c2);
            else
                b = get<1>(c2);
            node->constant = true;
            node->val      = int(a) % int(b);
            return {true, int(a) % int(b), int(a) % int(b)};
        }
        break;
    }
    default:
        break;
    }
    return {false, 0, 0};
}

//变量初始化的常值传递,全局变量存在varInitVal
void varInitOptimize(Var* i)
{
    using namespace std;
    if (i and i->data)
    {
        auto&& t = constantCalculate(i->data);
        if (get<0>(t))
        {
            int a = get<1>(t);
            if (a == 0)
            {
                a = get<2>(t);
            }
            varInitVal[i]     = {a, get<2>(t)};
            i->data->constant = true;
            i->data->val      = a;
        }
    }
}

// runtime , not in Inilization
// expression optimize of constant calculation
bool constantOptimize(Node* i)
{
    auto res = constantCalculate(i, false);
    if (get<0>(res))
    {
        i->constant = true;
        i->val      = get<1>(res);
        i->dval     = get<2>(res);
        return true;
    }
    i->constant = false;
    return false;
}

bool statementListOptimize(std::vector<Node*>* arr);

// statementOptimize
// Return whether it can be deleted.
bool statementOptimize(Node* i)
{
    if (i == nullptr)
        return true;
    switch (i->type)
    {
    case ND_COMP_STMT:
        return statementListOptimize(i->statementList);
    case ND_VARDEF:
        varInitOptimize(i->var);
        return false;
    case ND_RETURN: {
        constantOptimize(i->expresson);
        return false;
    }
    case ND_ASSIGN: {
        constantOptimize(i->rhs);
        return false;
    }
    case ND_CALL: {
        for (auto&& m : prog->funcs)
        {
            if (m->name == "puts" or m->name == "getchar")
                continue;
            if (m->name == i->name and m->compound == nullptr)
            {
                return true;
            }
        }
        for (auto&& j : *(i->args))
        {
            constantOptimize(j);
        }
        return false;
    }
    case ND_IF: {
        if (constantOptimize(i->condition))
        {
            if (i->condition->val != 0)
            {
                i->els = nullptr;
            }
            else
            {
                i->then = nullptr;
            }
        }
        if (statementOptimize(i->then))
        {
            i->then = nullptr; //!
        }
        if (statementOptimize(i->els))
        {
            i->els = nullptr;
        }
        if (i->then == nullptr and i->els == nullptr)
            return true;
        else
            return false;
    }
    case ND_DO_WHILE: {
        if (constantOptimize(i->condition))
        {
            if (i->condition->val == 0)
            {
                i->body = nullptr;
            }
        }
        if (statementOptimize(i->body))
            i->body = nullptr;
        if (i->body == nullptr)
            return true;
        else
            return false;
    }
    case ND_FOR: {
        if (constantOptimize(i->condition))
        {
            if (i->condition->val == 0)
            {
                i->body = nullptr;
            }
        }
        if (statementOptimize(i->body))
            i->body = nullptr;
        if (i->body == nullptr)
            return true;
        else
            return false;
    }
    case ND_INC:
    case ND_DEC:
        return false;
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
        return true;
    }
    return false;
}

// Return whether this statement List can be deleted
bool statementListOptimize(std::vector<Node*>* arr)
{
    if (arr == nullptr)
        return true;
    using namespace std;
    vector<Node*> final;
    for (auto&& i : *arr)
    {
        if (not statementOptimize(i))
            final.push_back(i);
    }
    (*arr).swap(final);

    bool flag = true;
    for (auto&& i : *arr)
    {
        if (i->type != ND_VARDEF)
        {
            flag = false;
            break;
        }
    }
    return flag;
}

void optimize()
{
    using namespace std;
    // extern Program* prog;
    // extern Env *env, *global;
    for (auto&& i : prog->gvars)
    {
        varInitOptimize(i);
    }

    for (auto&& i : prog->funcs)
    {
        if (statementListOptimize(i->compound->statementList))
        {
            i->compound = nullptr;
        }
    }

    if (debugMode)
    {
        for (auto&& i : varInitVal)
        {
            cout << i.first->name << ' ' << i.second.first << ' ' << i.second.second << endl;
        }
    }
}