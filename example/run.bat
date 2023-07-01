@echo off
echo Building the project...
make
if %errorlevel% neq 0 (
    echo Build failed. Exiting.
    exit /b %errorlevel%
)
echo Build completed successfully. Cleaning up...
del /Q /F *.o ..\src\*.o
.\example.exe