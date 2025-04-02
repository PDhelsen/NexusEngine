@echo off

set Root=%~dp0..\..\..\

set Configuration=%1
if [%Configuration%] == [] (set /p Configuration=Enter Configuration:)
set Platform=%2
if [%Platform%] == [] (set /p Configuration=Enter Platform:)

msbuild %Root%NexusEngine.sln /p:Configuration=%Configuration% /p:Platform=%Platform%

if errorlevel 1 (pause) else (exit /b 0)
