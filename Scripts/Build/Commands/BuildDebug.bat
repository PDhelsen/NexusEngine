@echo off

set Root=%~dp0..\..\..\
call %Root%Scripts\Build\Steps\Build.bat Debug Win64
