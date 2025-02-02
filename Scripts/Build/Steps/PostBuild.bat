@echo off

set Root=%~dp0..\..\..\
set Binaries=%1
set Artifacts=%Root%builds\artifacts\
robocopy %Binaries% %Artifacts% *.lib *.dll *.exe /it /is /e /v

if %errorlevel% NEQ 0 pause
