@echo OFF

SET romName=dino.z64

SET bindir=%cd%\..\bin
SET fst=%cd%\fst_work
SET rom=%cd%\rom\%romName%
SET defs=%cd%\DLLSIMPORTTAB.def

mkdir %fst%

Pushd %bindir%
CALL dpfst.exe -extract_files -rom %rom% -o %fst% -defs %defs%
popd