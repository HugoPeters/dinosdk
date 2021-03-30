@echo OFF

SET dllName=core-015-unk
SET romName=dino.z64

SET bindir=%cd%\..\bin
SET buildir=%cd%\..\build
SET romdir=%buildir%\rom
SET outdir=%buildir%\out
SET gccdir=%bindir%\mips64-elf\bin
SET gcc=mips64-elf-gcc
SET inc=%cd%\..\include
SET elf=%outdir%\out.elf
SET fst=%buildir%\fst_work
SET importdef=%fst%\DLLSIMPORTTAB.def
SET srcdir=%cd%
SET dll=%fst%\DLLS\%dllName%.dll

@REM unfortunately we need -mabi=32 for va_args to work correctly with the game's vsnprintf
SET cargs=-std=c99 -EB -march=r4300 -mabi=32 -shared -fPIC -mabicalls -O3 -g -nostdlib -G0 -fno-builtin-fprintf -fno-builtin-fvsnprintf -fvisibility=hidden -ffunction-sections -fdata-sections -Wno-endif-labels -mhard-float -I%inc%
SET linkargs=-Wl,--undefined,dynamic_lookup,--warn-constructors,--section-start=.bss=0x00800000 -T %bindir%\link_dll.ld

SET inrom=%romdir%\%romName%
SET outrom=%romdir%\out.z64

if not exist %outdir% mkdir %outdir%

pushd %gccdir%
CALL %gcc% %cargs% -c %srcdir%\hello.c -o %outdir%\hello.o
CALL %gcc% %cargs% %linkargs% %outdir%\hello.o -o %elf%
popd

pushd %bindir%
CALL dinofst.exe -elf2dll -i "%elf%" -o "%dll%" -defs "%importdef%"
CALL dinofst.exe -compile_rom -rom "%inrom%" -i "%fst%" -o "%outrom%"
popd
