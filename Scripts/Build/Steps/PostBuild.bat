@echo off
setlocal enabledelayedexpansion

set Root=%~dp0..\..\..\

set Project=""
set Platform=""
set Configuration=""

for /F "tokens=1-3 delims=_" %%A in ("%1") do (
    set Project=%%A
    set Platform=%%B
    set Configuration=%%C
)

set Artifacts=%Root%builds\artifacts\
set Binaries=%Root%builds\binaries\%Project%_%Platform%_%Configuration%\
set Framework=%NexusFramework%\Builds\NexusFramework_%Platform%_%Configuration%\

robocopy %Binaries% %Artifacts% *.lib *.dll *.exe /it /is /e /v
if "%Project%"=="NexusEngine" (
robocopy %Framework% %Artifacts% *.lib *.dll *.exe /it /is /e /v
)

if "%Project:~0,7%"=="Starter" (
	set Executable=""
	for /F "tokens=1,2 delims=-" %%A in ("%Project%") do (
		set Executable=%%B
	)
	move /Y %Artifacts%%Project%.exe %Artifacts%!Executable!.exe
)

if %errorlevel% NEQ 0 pause
