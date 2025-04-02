@echo off

set Root=%~dp0..\..\

pushd %Root%
call %Root%builds\artifacts\NexusEditor\NexusEditor.exe %* DebugFolder=debugeditor
popd %Current%

if errorlevel 1 (pause) else (exit /b 0)
