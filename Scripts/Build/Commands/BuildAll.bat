@echo off

set Root=%~dp0..\..\..\
call %Root%Scripts\Build\Steps\Build.bat Debug Win64
call %Root%Scripts\Build\Steps\Build.bat Distrib Win64
call %Root%Scripts\Build\Steps\Build.bat Release Win64
