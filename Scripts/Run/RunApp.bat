@echo off

set Root=%~dp0..\..\
call %Root%Scripts\Run\Run.bat -Mode=App -DebugFolder=debugapp %*
