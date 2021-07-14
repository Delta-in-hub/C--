#include "util.h"

bool fileExist(const char* path)
{
    FILE* f;
    fopen_s(&f, path, "r");
    if (f == nullptr)
        return false;
    fclose(f);
    return true;
}

void asmToExe()
{
    using namespace std;
    if (not fileExist((asmFilePath + "asm").c_str()))
    {
        cout << asmFilePath.c_str() << "asm do not exist" << endl;
        getchar();
        exit(1);
    }

    char cmd[100] = {0};
    if (debugMode)
        sprintf(cmd, "%s -g -fwin64 %sasm", nasmPath.c_str(), asmFilePath.c_str());
    else
        sprintf(cmd, "%s -fwin64 %sasm", nasmPath.c_str(), asmFilePath.c_str());
    system(cmd);
    if (not fileExist((asmFilePath + "obj").c_str()))
    {
        cout << asmFilePath.c_str() << "obj do not exist" << endl;
        getchar();
        exit(1);
    }
    sprintf(cmd, "%s %sobj -o %sexe", gccPath.c_str(), asmFilePath.c_str(), asmFilePath.c_str());
    system(cmd);
    if (not fileExist((asmFilePath + "exe").c_str()))
    {
        cout << asmFilePath.c_str() << "exe do not exist" << endl;
        getchar();
        exit(1);
    }
    if (not keepAsm)
        remove((asmFilePath + "asm").c_str());
    if (not keepObj)
        remove((asmFilePath + "obj").c_str());
}