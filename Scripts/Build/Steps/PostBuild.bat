@echo off

set Root=%~dp0..\..\..\
set Artifacts=%Root%builds\artifacts\
set Binaries=%1
set Framework=%NexusFramework%\Builds\NexusFramework_%2_%3\

robocopy %Binaries% %Artifacts% *.lib *.dll *.exe /it /is /e /v
robocopy %Framework% %Artifacts% *.lib *.dll *.exe /it /is /e /v

if %errorlevel% NEQ 0 pause
