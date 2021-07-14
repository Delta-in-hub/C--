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
    sprintf(cmd, "nasm -g -fwin64 %sasm", asmFilePath.c_str());
    system(cmd);
    if (not fileExist((asmFilePath + "obj").c_str()))
    {
        cout << asmFilePath.c_str() << "obj do not exist" << endl;
        getchar();
        exit(1);
    }
    sprintf(cmd, "gcc %sobj -o %sexe", asmFilePath.c_str(), asmFilePath.c_str());
    system(cmd);
    if (not fileExist((asmFilePath + "exe").c_str()))
    {
        cout << asmFilePath.c_str() << "exe do not exist" << endl;
        getchar();
        exit(1);
    }
    if (false)
    {
        remove((asmFilePath + "asm").c_str());
        remove((asmFilePath + "obj").c_str());
    }
}