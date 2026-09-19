@echo off

set Root=%~dp0..\

call :DeleteFolder %Root%.vs
call :DeleteFolder %Root%builds
call :DeleteFolder %Root%cooked
call :DeleteFolder %Root%saved
call :DeleteFolder %Root%temp

call :DeleteVisualStudioProject %Root%Libraries
call :DeleteVisualStudioProject %Root%Sources

del %Root%NexusEngine.sln

if errorlevel 1 (pause) else (exit /b 0)

::------------------------------------------------
:DeleteVisualStudioProject
setlocal

call :DeleteByExtension %1 vcxproj
call :DeleteByExtension %1 vcxproj.filters
call :DeleteByExtension %1 vcxproj.user

endlocal
exit /b 0

::------------------------------------------------
:DeleteByExtension
setlocal

set FolderPath=%1
set Extensions=%2
if exist %FolderPath% (
	for /R "%FolderPath%" %%F in (*.%Extensions%) do (
		del "%%F"
   		echo File %%F deleted.
	)
)

endlocal
exit /b 0

::------------------------------------------------
:DeleteFolder
setlocal

set FolderPath=%1
if exist %FolderPath% (
	rmdir /s /q %FolderPath%
    echo Folder %FolderPath% deleted.
)

endlocal
exit /b 0
