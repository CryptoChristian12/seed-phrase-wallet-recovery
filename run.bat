@echo off
set filename=main 
g++ "%filename%.cpp" -o "%filename%.exe"

if %errorlevel% neq 0 (
    echo Compilation failed.
    pause
    exit /b
)

echo Running %filename%.exe...
"%filename%.exe"

pause
