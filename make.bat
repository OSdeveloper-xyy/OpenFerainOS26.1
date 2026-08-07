REM This is a batch script for compiling OpenFerainOS, and the final results are presented in the Image folder.
mkdir Image
REM This is the step of reading compilation options.
set "NASM=" 
for /f "skip=2 delims=" %%a in (Option\Compilation_options.Inf) do (
    set "NASM=%%a"
    goto :got_opt1
)
:got_opt1
set "GCC=" 
for /f "skip=4 delims=" %%a in (Option\Compilation_options.Inf) do (
    set "GCC=%%a"
    goto :got_opt2
)
:got_opt2
set "OBJ=" 
for /f "skip=5 delims=" %%a in (Option\Compilation_options.Inf) do (
    set "OBJ=%%a"
    goto :got_opt3
)
:got_opt3
REM This is the source file compilation stage.
%NASM% Sources\Boot\setup.asm -o Image\setup.bin

%GCC%Sources\init\linker.ld Sources\init\init.c Sources\init\init.s -o Image\init.elf
%OBJ% Image\init.elf Image\init.bin

%GCC%Option\linker.ld -Ofast Sources\Application\cmd.c -o Image\cmd.elf
%OBJ% Image\cmd.elf Image\cmd.bin
REM This is the mirror stitching step.
copy /b Image\setup.bin + Image\init.bin + Image\cmd.bin Image\OpenFerainOS26.1.img
fsutil file seteof "Image\OpenFerainOS26.1.img" 4194304
REM This is the deletion step.
del Image\init.elf
del Image\cmd.elf
del Image\setup.bin
del Image\init.bin
del Image\cmd.bin

pause
exit