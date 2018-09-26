# Using markdown as a programming language
如果将markdown视作一门编程语言可以做哪些有趣的事情？这个项目就是为了解决这个好奇:)

# 示例:使用markdown计算斐波那契数列
+ 源码:[fibonacci.md](example/fibonacci.md)
+ llvm ir:
```llvm
; ModuleID = 'llmd'
source_filename = "llmd"

define void @main() {
entry:
  %times = alloca i32
  %result = alloca i32
  %b = alloca i32
  %a = alloca i32
  store i32 1, i32* %a
  store i32 1, i32* %b
  store i32 0, i32* %result
  store i32 50, i32* %times

label:                                            ; preds = %label
  %a1 = load i32, i32* %a
  %b2 = load i32, i32* %b
  %addtmp = add i32 %a1, %b2
  %times3 = load i32, i32* %times
  %subtmp = sub i32 %times3, 1
  %times4 = load i32, i32* %times
  %ifcond_var = icmp ne i32 %times4, 0
  br i1 %ifcond_var, label %label, label %end

end:                                              ; preds = %label
}
```

# 重新定义一个图灵完备的语法集
```comment               // Block comments

[var](value)             // Input(Literal,Arithmetic)

>                        // Output

`a+b`                    // Arithmetic(+,-,*) 

**label**                // Mark as a label

![condition](tag)       // Jump label if condition != 0

```