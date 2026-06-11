@echo off
setlocal

where wsl.exe >nul 2>nul
if errorlevel 1 (
    echo WSL is not installed or wsl.exe is not available.
    pause
    exit /b 1
)

for /f "delims=" %%I in ('wsl.exe wslpath -a "%~dp0"') do set "WSL_DIR=%%I"

if not defined WSL_DIR (
    echo Could not convert this project path to a WSL path.
    pause
    exit /b 1
)

wsl.exe --cd "%WSL_DIR%" -e bash -lc "exec ./snake_game"

echo.
pause
