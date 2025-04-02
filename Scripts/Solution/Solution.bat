@echo off

set Root=%~dp0..\..\
premake5 vs2022 --file=%Root%Scripts\Solution\Solution.lua --verbose

if errorlevel 1 (pause) else (exit /b 0)
