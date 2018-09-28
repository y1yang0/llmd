; ModuleID = 'llmd'
source_filename = "llmd"

@0 = private unnamed_addr constant [35 x i8] c"the result of fibonacci(20) is %d\0A\00"

define void @fibonacci() {
entry:
  %times = alloca i32
  %result = alloca i32
  %b = alloca i32
  %a = alloca i32
  store i32 1, i32* %a
  store i32 1, i32* %b
  store i32 0, i32* %result
  store i32 20, i32* %times
  br label %label

label:                                            ; preds = %label, %entry
  %a1 = load i32, i32* %a
  %b2 = load i32, i32* %b
  %0 = add i32 %a1, %b2
  store i32 %0, i32* %result
  %1 = load i32, i32* %b
  store i32 %1, i32* %a
  %2 = load i32, i32* %result
  store i32 %2, i32* %b
  %times3 = load i32, i32* %times
  %3 = sub i32 %times3, 1
  store i32 %3, i32* %times
  %times4 = load i32, i32* %times
  %ifcond_var = icmp ne i32 %times4, 0
  br i1 %ifcond_var, label %label, label %end

end:                                              ; preds = %label
  %result5 = alloca i32
  %4 = load i32, i32* %result
  store i32 %4, i32* %result5
  %5 = load i32, i32* %result5
  %c_printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([35 x i8], [35 x i8]* @0, i32 0, i32 0), i32 %5)
  ret void
}

declare i32 @printf(i8*, ...)
