@echo off

set Root=%~dp0..\..\
call %Root%Scripts\Run\Run.bat Target=App DebugFolder=debugapp %*
