@echo off

set Root=%~dp0..\..\..\
call %Root%Scripts\Build\Steps\Build.bat Distrib Win64
