REM LemonHaze - 2025
@echo off
setlocal

set IDA_EXE=%IDA_ROOT%\ida.exe
set SCRIPT_PATH=B:\_work\shenmue-hd-re\scripts\ida-export.py

set SM1_DEF=B:\_work\shenmue-hd-re\sm1.def
set SM2_DEF=B:\_work\shenmue-hd-re\sm2.def

if not exist "%SM1_DEF%" (
    echo Syncing SM1 defs...
    "%IDA_EXE%" -A -S"%SCRIPT_PATH% %SM1_DEF%" "C:\Users\LemonHaze\Documents\IDA\Shenmue\1.07\Shenmue_v1.07.i64"
    if errorlevel 1 (
        exit /b %errorlevel%
    )
)

if not exist "%SM2_DEF%" (
    echo Syncing SM2 defs...
    "%IDA_EXE%" -A -S"%SCRIPT_PATH% %SM2_DEF%" "C:\Users\LemonHaze\Documents\IDA\Shenmue\1.07\Shenmue2_v1.07.i64"
    if errorlevel 1 (
        exit /b %errorlevel%
    )
)
endlocal