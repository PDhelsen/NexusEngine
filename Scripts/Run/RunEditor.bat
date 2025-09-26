@echo off

set Root=%~dp0..\..\
call %Root%Scripts\Run\Run.bat -Mode=Editor -DebugFolder=debugeditor %*
