@echo off
chcp 65001 >nul
cd /d "%~dp0"

:: Запуск надежного скрипта PowerShell
if exist "%~dp0terminal.ps1" (
    powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0terminal.ps1"
) else (
    echo [ERROR] Файл terminal.ps1 не найден рядом с bat файлом!
    echo Пожалуйста, поместите terminal.ps1 и terminal.bat в одну папку.
    echo.
    pause
)

if errorlevel 1 (
    echo.
    echo [!] Произошла ошибка при выполнении.
    pause
)
