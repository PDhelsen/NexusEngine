@echo off

set Root=%~dp0..\..\..\
call %Root%Scripts\Build\Steps\Build.bat Release Win64
