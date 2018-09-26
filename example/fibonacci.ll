; ModuleID = 'llmd'
source_filename = "llmd"

@gbv = private unnamed_addr constant [5 x i8] c"test\00"

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
  %call_printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @gbv, i32 0, i32 0))
}

declare i32 @printf(i8*, ...)