@echo off
setlocal EnableDelayedExpansion

set Root=%~dp0..\

set Target=%1
if [%Target%] == [] (set /p Target=Enter target directory:)

set Deploy=%Target%\NexusEngine\
if exist %Deploy% rmdir /s /q %Deploy%
mkdir %Deploy%

set FolderLastIndex=4
set Folders[0]=NexusEngine
set Folders[1]=NexusApp
set Folders[2]=NexusEditor
set Folders[3]=NexusStarter
set Folders[4]=NexusUtility

call :Copy %Root%Sources %Deploy%Sources "*.h *.cpp *.natvis"
call :Copy %Root%builds\binaries %Deploy%Builds

if errorlevel 1 (pause) else (exit /b 0)

::------------------------------------------------
:Copy
setlocal

for /d %%F in (%1\*) do (
	call :ContainsSubstring %%~nxF
	if !Result!==true (
		robocopy %%F %2\%%~nxF %~3 /e
	)
)

endlocal
exit /b 0

::------------------------------------------------
:ContainsSubstring
setlocal
set "Contains=false"

for /L %%I in (0,1,%FolderLastIndex%) do (
	set "Folder=!Folders[%%I]!"
	echo %1 | findstr /i /c:"!Folder!" > nul
	if not errorlevel 1 (
		set "Contains=true"
	)
)

(endlocal & set "Result=%Contains%")
exit /b 0
