; ModuleID = 'llmd'
source_filename = "llmd"

@0 = private unnamed_addr constant [35 x i8] c"the result of fibonacci(%d) is %d\0A\00"

define void @fibonacci() {
entry:
  %times = alloca i32
  %result = alloca i32
  %b = alloca i32
  %a = alloca i32
  store i32 1, i32* %a
  store i32 1, i32* %b
  store i32 0, i32* %result
  store i32 50, i32* %times
  br label %label

label:                                            ; preds = %label, %entry
  %a1 = load i32, i32* %a
  %b2 = load i32, i32* %b
  %addtmp = add i32 %a1, %b2
  %times3 = load i32, i32* %times
  %subtmp = sub i32 %times3, 1
  %times4 = load i32, i32* %times
  %ifcond_var = icmp ne i32 %times4, 0
  br i1 %ifcond_var, label %label, label %end

end:                                              ; preds = %label
  %times5 = alloca i32
  %0 = load i32, i32* %times
  store i32 %0, i32* %times5
  %1 = load i32, i32* %times5
  %result6 = alloca i32
  %2 = load i32, i32* %result
  store i32 %2, i32* %result6
  %3 = load i32, i32* %result6
  %c_printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([35 x i8], [35 x i8]* @0, i32 0, i32 0), i32 %1, i32 %3)
  ret void
}

declare i32 @printf(i8*, ...)