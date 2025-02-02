@echo off

set Root=%~dp0..\

pushd %Root%
call %Root%builds\artifacts\NexusSandbox.exe
popd %Current%

if %errorlevel% NEQ 0 pause
