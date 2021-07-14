# C--
A simple C-like grammar complier,Generate executable files of windows x86_64 computer.

## Requirement
1. nasm
   1. Download it from https://www.nasm.us/
   2. [NASM Tutorial](https://cs.lmu.edu/~ray/notes/nasmtutorial/)
      1. Using NASM on Windows. Weird shadow space before each call.
   3. [x86_64 NASM Assembly Quick Reference ("Cheat Sheet")](https://www.cs.uaf.edu/2017/fall/cs301/reference/x86_64.html)
   4. [x86 and amd64 instruction reference](https://www.felixcloutier.com/x86/index.html)
2. gcc
   1. I recommend https://jmeubank.github.io/tdm-gcc/
   2. Or any gcc base on MinGW-w64
3. Set them to Environment variable.Or use`c--.exe` `-n <path>` `-g <path>` to set when it running.

## Build
1. `cd src/ && g++ *.cpp -ofast -o c--.exe`
2. Or Double click `complierTeam.sln` and then ...

## Example
```
>> ./c--.exe
Usage: c--.exe [-v] [-d] [-s] [-c] <SourceFile> [-o <TargetFile>] [-n <NasmPath>] [-g <GccPath>]

  -v                       Display compiler version information.
  -d                       Enable Debug Mode.
  -s                       Reserve Assemble File.
  -c                       Reserve Object File.
  -o <TargetFile>          Place the output into <TargetFile>.
  -n <NasmPath>            Set nasm.exe path from <NasmPath>.
  -g <GccPath>             Set gcc.exe path from <GccPath>.

Default:
  Disable Debug Mode.Do not Reserve .asm and .obj File.Place the output into the same folder of <SourceFile>

See https://github.com/Delta-in-hub/complierTeam for more information

>> cat ./testCase/in11.txt
int a = 10;
int b = 2*a+10;
char *str = "Hello\tWorld!\n";

void puts(char *str)
{
    ;
}

void getchar()
{
    ;
}

void fun()
{
    int c;
    int d;
    d;
    c;
    if(1 < 2)
    {
        ;
    }
    else
    {
        ;
    }
}

void fun1()
{
    a=30;
}

int main()
{
    int i;
    int m = 2;
    for(i = 0 ; i < m ;i++)
    {
        puts(str);
    }
    fun1();
    if(a == b + 1)
    {
        puts("A equals to B\n");
    }
    getchar();
    return 0;
}

>> ./c--.exe -d -s -c ./testCase/in11.txt -o test.exe

./testCase/in11.txt

TK_INT int
TK_IDENT a
TK_= =
TK_INUM 10 10 0
TK_; ;
TK_INT int
TK_IDENT b
TK_= =
TK_INUM 2 2 0
TK_* *
TK_IDENT a
TK_+ +
TK_INUM 10 10 0
...
...
TK_} }
TK_EOF


Complier done

Global Variable
a sizeof 4 VarType: 3
b sizeof 4 VarType: 3
str sizeof 8 VarType: 5

Function
puts returnType: 0
It has 1 params and function's address is 0000000000ea35d0
getchar returnType: 0
It has 0 params and function's address is 0000000000ea38d0
fun returnType: 0
It has 0 params and function's address is 0000000000ea3bc0
fun1 returnType: 0
It has 0 params and function's address is 0000000000e98b40
main returnType: 3
It has 0 params and function's address is 0000000000e98e40

m 2 0
b 30 30
a 10 0

>> ./test.exe
Hello   World!

Hello   World!

```

## 须知

### 命名规范

#### 函数/变量命名

小驼峰命名

`getNextToken()`

#### 类命名方式

大驼峰命名

`GetNextToken()`

## code format

vs里打开启用clang format
## 其他

在`*.h`中写函数声明,在`*.cpp`中写函数实现

## 文法

[见此文件](./Grammar Zoo - Browsable C99 Grammar.pdf)

这个文法存在左递归

```
declaration-list ::=
	declaration
	declaration-list declaration

declaration-list ::= (declaration)+ 正闭包
```



---

https://github.com/rui314/9cc

https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html

---