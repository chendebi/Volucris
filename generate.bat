@echo off

setlocal

call %~dp0\Scripts\build_assimp.bat %~dp0
if %errorlevel% neq 0 (
    echo build assimp failed
    exit /b %errorlevel%
)

endlocal