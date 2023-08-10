rmdir /s /q CMakeFiles
del /f Makefile
del /f CMakeCache.txt
cmake . -G "Unix Makefiles"