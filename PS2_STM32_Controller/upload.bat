@echo off

set target=%1

@REM flow
if [%1]==[] goto usage
if "%target%"=="build" goto build

@REM base run
pio run --target %target% --environment nucleo_l476rg
goto exit_bat

@REM build command
:build
pio run --environment nucleo_l476rg
goto exit_bat

@REM error state
:usage
echo Need environment name
goto exit_bat

@REM exit command
:exit_bat
exit /b 1
