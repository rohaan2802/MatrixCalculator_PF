@echo off
setlocal EnableExtensions
cd /d "%~dp0"

set "SRC=%~dp0Source.cpp"
set "EXE=%~dp0Source.exe"

if not exist "%SRC%" (
  echo [ERROR] Source not found: %SRC%
  pause
  exit /b 1
)

echo ============================================
echo  Matrix Calculator PF - Clean Build ^& Run
echo ============================================
echo.

echo Cleaning previous build...
if exist "%EXE%" del /f /q "%EXE%" 2>nul
if exist "%~dp0Source.obj" del /f /q "%~dp0Source.obj" 2>nul
if exist "%~dp0*.obj" del /f /q "%~dp0*.obj" 2>nul

echo Building latest sources...
call "%~dp0msvc_build.bat" "%SRC%"
if errorlevel 1 (
  echo.
  echo [ERROR] Build failed - cannot run.
  pause
  exit /b 1
)

if not exist "%EXE%" (
  echo.
  echo [ERROR] Expected exe not found after build:
  echo   %EXE%
  pause
  exit /b 1
)

echo.
echo [OK] Built: %EXE%
echo Starting in a new console window...
echo Working directory: %cd%
echo.
echo Controls: 0 = go back / exit where shown
echo           Menu 8 = exit program
echo.

start "Matrix Calculator PF" /D "%~dp0" cmd /k ""%EXE%""

endlocal
exit /b 0
