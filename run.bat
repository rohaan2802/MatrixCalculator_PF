@echo off
setlocal
cd /d "%~dp0"
powershell.exe -NoProfile -File "%~dp0build.ps1"
if errorlevel 1 exit /b 1
if exist "build\Release\matrix_calculator.exe" (
    "build\Release\matrix_calculator.exe"
) else (
    "build\matrix_calculator.exe"
)
