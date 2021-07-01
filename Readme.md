# ComplierTeam

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