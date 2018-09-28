# Using markdown as a programming language
如果将markdown视作一门编程语言可以做哪些有趣的事情？这个项目就是为了解决这个好奇:)
它目前不会一步到位生成可执行文件，而是生成LLVM IR，如果需要进一步使用，需要`clang`。

# 图灵完备的语法集
```comment               // todo:注释

[var](value)             // 赋值(Literal,Arithmetic)

> hello `var`            // 输出

`a+b`                    // 算术运算(+,-,*) 

**label**                // 跳转标记

![condition](tag)       //  条件跳转(condition!=0)

```

# 编译构建
三步即可编译得到**llmd**(需要在CMakeLists.txt中配置LLVM libs)：
```bash
$ mkdir build && cd build && cmake .. && make
```

# 示例1： 你好，世界
[helloworld.md](example/helloworld.md):
```bash
$ llmd helloworld.md # 生成helloworld.ll
$ clang -c helloworld.ll # 生成helloworld.o
```
得到目标文件后，可以调用它：
```c
#include <stdio.h>

void helloworld();

int main(){
    printf("using markdown as programming language:\n");
    helloworld();
    return 0;
}
```
```bash
$ clang main.o helloworld.o -o helloworld.exe # 大功告成！
$ ./helloworld.exe
using markdown as programming language:
hello, world
```

# 示例2：计算斐波那契数列

比如以[fibonacci.md](example/fibonacci.md)为例子：
```bash
$ llmd fibonacci.md # 生成fibonacci.ll
$ clang -c fibonacci.ll # 生成fibonacci.o
```
现在我们有了markdown生成的目标文件，里面包含了`void fibonacci()`的实现，
我们可以这么使用它：
```c
#include <stdio.h>

void fibonacci();

int main(){
    printf("using markdown as programming language");
    fibonacci();
    return 0;
}
```
```bash
$ clang main.o fibonacci.o -o fibonacci.exe # 大功告成！
$ ./fibonacci.exe
using markdown as programming languagethe result of fibonacci(50) is 17711
```

# 计划
1. 如果给定的markdown源码无法满足编译要求会报错，而不是直接crash
2. 编译最终产物是对象文件或者库文件，可以与C系语言输出进行链接

