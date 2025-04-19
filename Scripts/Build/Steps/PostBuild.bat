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
robocopy %Binaries% %Artifacts% %2 /it /is /e /v

if "%Project%"=="NexusEngine" (
	set FrameworkPlatform=%Platform:-Editor=%
	set Framework=%NexusFramework%\Builds\NexusFramework_!FrameworkPlatform!_%Configuration%\
	robocopy !Framework! %Artifacts% *.dll /it /is /e /v
	set Yaml=%NexusFramework%\Builds\yaml-cpp_!FrameworkPlatform!_%Configuration%\
	robocopy !Yaml! %Artifacts% *.dll /it /is /e /v
)

if errorlevel 1 (pause) else (exit /b 0)
