@echo off

set Root=%~dp0..\..\

pushd %Root%
call %Root%builds\artifacts\NexusEditor\NexusEditor.exe %* DebugFolder=debugeditor
popd %Current%

if %errorlevel% NEQ 0 pause
