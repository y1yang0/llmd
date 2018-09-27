# Using markdown as a programming language
如果将markdown视作一门编程语言可以做哪些有趣的事情？这个项目就是为了解决这个好奇:)

# 示例:使用markdown计算斐波那契数列
+ markdown源码:[fibonacci.md](example/fibonacci.md)
+ 生成的llvm ir: [fibonacci.ll](example/fibonacci.ll)

# 重新定义一个图灵完备的语法集
```comment               // todo:注释

[var](value)             // 赋值(Literal,Arithmetic)

>                        // 输出

`a+b`                    // 算术运算(+,-,*) 

**label**                // 跳转标记

![condition](tag)       //  条件跳转(condition!=0)

```

# 计划
1. 如果给定的markdown源码无法满足编译要求会报错，而不是直接crash
2. 编译最终产物是对象文件或者库文件，可以与C系语言输出进行链接

