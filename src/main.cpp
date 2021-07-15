#include "codegen.h"
#include "optimize.h"
#include "parse.h"
#include "stdafx.h"
#include "token.h"
#include "util.h"
#include <iostream>
#include <string>
using namespace std;

void outPutToken(bool flag = false)
{
    if (flag == false)
        return;
    cout << endl;
    cout << path << endl;
    cout << endl;
    for (auto&& i : tokenArr)
    {
        if (i.type == "TK_INUM" or i.type == "TK_DNUM")
        {
            cout << i.type << ' ' << string(i.start, i.end) << ' ' << i.val << ' ' << i.dval << endl;
        }
        else
        {
            cout << i.type << ' ' << string(i.start, i.end) << endl;
        }
    }
    cout << endl;
    cout << endl;
}

void outPutParse(bool flag = false)
{
    if (not flag)
        return;
    cout << endl;

    cout << "Global Variable" << endl;
    for (auto&& i : prog->gvars)
    {
        printf("%s sizeof %d VarType: %d \n", i->name.c_str(), i->ty->size, i->ty->ty);
    }
    cout << endl;

    cout << "Function" << endl;
    for (auto&& i : prog->funcs)
    {
        printf("%s returnType: %d \n", i->name.c_str(), i->returnType->ty);
        printf("It has %lld params and function's address is %p\n", i->params->size(), i->compound);
        if (i->compound and i->compound->statementList)
            for (auto&& j : *(i->compound->statementList))
            {
                cout << j->type << ' ' << j->name << endl;
            }
        cout << "-----------------------------" << endl;
    }
    cout << endl;
}

bool debugMode = false, keepAsm = false, keepObj = false;
char* outPutFile = nullptr;
std::string gccPath("gcc"), nasmPath("nasm");

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        printf("c--.exe: fatal: no input file specified\nType c--.exe -h for help.");
        exit(1);
        // path = "../testCase/in11.txt";
    }
    else if (argc == 2)
    {
        char* arg = argv[1];
        if (strcmp(arg, "-v") == 0)
        {
            printf("C-- version 0.1.0 compiled on July 14 2021\n");
            printf("Using C-like grammar and Generate Windows x86_64 Executable File\n");
            printf("See https://github.com/Delta-in-hub/C-- for more information\n");
            exit(1);
        }
        else if (strcmp(arg, "-h") == 0)
        {
            puts("Usage: c--.exe [-v] [-d] [-s] [-c] <SourceFile> [-o <TargetFile>] [-n <NasmPath>] [-g <GccPath>]");
            puts("");
            puts("  -v                       Display compiler version information.");
            puts("  -d                       Enable Debug Mode.");
            puts("  -s                       Reserve Assemble File.");
            puts("  -c                       Reserve Object File.");
            puts("  -o <TargetFile>          Place the output into <TargetFile>.");
            puts("  -n <NasmPath>            Set nasm.exe path from <NasmPath>.");
            puts("  -g <GccPath>             Set gcc.exe path from <GccPath>.");
            puts("\nDefault:\n  Disable Debug Mode.Do not Reserve .asm and .obj File.Place the output into the same "
                 "folder of <SourceFile>");
            printf("\nSee https://github.com/Delta-in-hub/C-- for more information\n");
            exit(1);
        }
        else
        {
            path = arg;
        }
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            char* str = argv[i];
            if (strcmp(str, "-d") == 0)
            {
                debugMode = true;
            }
            else if (strcmp(str, "-s") == 0)
            {
                keepAsm = true;
            }
            else if (strcmp(str, "-c") == 0)
            {
                keepObj = true;
            }
            else if (strcmp(str, "-o") == 0)
            {
                outPutFile = argv[i + 1];
                i++;
            }
            else if (strcmp(str, "-n") == 0)
            {
                nasmPath = string(argv[i + 1]);
                i++;
            }
            else if (strcmp(str, "-g") == 0)
            {
                gccPath = string(argv[i + 1]);
                i++;
            }
            else
            {
                path = str;
            }
        }
    }

    const char* str;
    str      = openFile(path);
    buferror = str; // for parse error process
    scan(str);
    outPutToken(debugMode);
    parse();
    outPutParse(debugMode);
    optimize();
    codeGen(outPutFile); //.asm file done
    asmToExe();
    return 0;
}