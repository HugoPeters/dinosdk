@echo OFF

SET romName=dino.z64
SET outName=out.z64

SET bindir=%cd%\..\bin
SET fst=%cd%\fst_work
SET inrom=%cd%\rom\%romName%
SET outrom=%cd%\rom\%outName%

Pushd %bindir%
CALL dinofst.exe -compile_rom -rom "%inrom%" -i "%fst%" -o "%outrom%"
popd