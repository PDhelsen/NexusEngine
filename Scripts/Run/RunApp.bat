@echo off

set Root=%~dp0..\..\

pushd %Root%
call %Root%builds\artifacts\NexusApp\NexusApp.exe %* DebugFolder=debugapp
popd %Current%

if %errorlevel% NEQ 0 pause
