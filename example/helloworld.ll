; ModuleID = 'llmd'
source_filename = "llmd"

@0 = private unnamed_addr constant [14 x i8] c"hello, world\0A\00"

define void @helloworld() {
entry:
  %c_printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @0, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)
