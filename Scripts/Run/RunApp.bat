@echo off

set Root=%~dp0..\..\

pushd %Root%
call %Root%builds\artifacts\NexusApp\NexusApp.exe %* DebugFolder=debugapp
popd %Current%

if errorlevel 1 (pause) else (exit /b 0)
