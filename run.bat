@echo off
setlocal
cd /d "%~dp0" || exit /b 1
if not exist build\CMakeCache.txt (
    cmake -S . -B build -G "MinGW Makefiles" || exit /b 1
)
cmake --build build --target run
exit /b %errorlevel%
