@echo OFF

SET srcName=hello.c
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
SET src=%cd%\%srcName%
SET dll=%fst%\DLLS\%dllName%.dll

SET inrom=%romdir%\%romName%
SET outrom=%romdir%\out.z64

if not exist %outdir% mkdir %outdir%

pushd %gccdir%
CALL %gcc% -std=c99 -EB -march=r4300 -shared -fPIC -mabicalls -O3 -g -nostdlib -lc -G0 -fvisibility=hidden -ffunction-sections -fdata-sections -Wno-endif-labels -mhard-float -I%inc% %src% -o %elf% -Wl,-undefined,dynamic_lookup,--warn-constructors,--section-start=.bss=0x00800000 -T %bindir%\link_dll.ld
popd

pushd %bindir%
CALL dpfst.exe -elf2dll -i "%elf%" -o "%dll%" -defs "%importdef%"
CALL dpfst.exe -compile_rom -rom "%inrom%" -i "%fst%" -o "%outrom%"
popd
