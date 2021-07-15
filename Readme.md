# C--
A simple C-like grammar complier,Generate executable files for windows x86_64 platform.


C-- Generate assembly file and then `nasm -fwin64 <file.asm> && gcc <file.obj> -o <file.exe>`

## Requirement
1. nasm
   1. Download it from https://www.nasm.us/
2. gcc
   1. I recommend https://jmeubank.github.io/tdm-gcc/
   2. Or any gcc base on **MinGW-w64**
3. Add them to Environment variable.Or use`c--.exe` `-n <path>` `-g <path>` to set them as argument.

## Build
1. `cd src/ && g++ *.cpp -std=c++17 -Wall -ofast -o c--.exe`
2. Or Double click `C--.sln` and then ...


## Example
```
>> ./c--.exe -h
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

See https://github.com/Delta-in-hub/C-- for more information

>> ./c--.exe -s ./testCase/in12.txt -o test.exe
>> ./test.exe
a == 10
While Looping
While Looping
While Looping
While Looping
For Looping
For Looping
For Looping
For Looping
For Looping

>> cat test.asm

section .data
arr :
	times 10 dd 0
a :
	dd 12

......

>> cat ./testCase/in12.txt

int arr[10];
int a = 12;
int *p;

void puts(char* str)
{
    /* Reserved Function. Link it to C library later. C:\WINDOWS\System32\msvcrt.dll */
}
int getchar()
{
    /* Reserved Function. Link it to C library later. C:\WINDOWS\System32\msvcrt.dll */
}

int main()
{
    arr[0] = 1+1+1+1+1+2+2+1;
    arr[1] = 10 - 10 + 20 - 2 * 10 + arr[0];
    p = &a;
    *p = 20;
    if(a == arr[0] + arr[1])
    {
        puts("a == 10");
    }
    while(a <= 23)
    {
        puts("While Looping");
        a++;
    }
    for(a = 5; a != 0; a-- )
    {
        puts("For Looping");
    }
    getchar();
    return 0;
}

```

## Notice

C-- did basically a complete front-end, including syntax/semantic analysis.But in back-end,we didn't do much work on code optimization.
In object code generation,C-- don't support struct member access and treat all types as
`int32_t` when you referencing variables.



## Reference

1. https://github.com/rui314/9cc     I learnd a lot from 9cc.Thanks so much.
2. [NASM Tutorial](https://cs.lmu.edu/~ray/notes/nasmtutorial/)
      1. Using NASM on Windows. Weird shadow space before each call.
3. [x86_64 NASM Assembly Quick Reference ("Cheat Sheet")](https://www.cs.uaf.edu/2017/fall/cs301/reference/x86_64.html)
4. [x86 and amd64 instruction reference](https://www.felixcloutier.com/x86/index.html)