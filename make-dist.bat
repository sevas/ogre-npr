CLS
@echo OFF


if "%1" == "" (
   echo Usage : make-dist.bat name [build]
   goto END
)

set SOLUTION_NAME=%1
set DLLS=Release\*.dll
set EXE=Release\%1.exe
set MEDIA_DIR=media
set DIST_DIR=%1-dist

if "%2" == "" (
   GOTO MAKE_DIST
)


:BUILD
if not exist "c:\Program Files\Microsoft Visual Studio 9.0\VC\bin\vcvars32.bat" goto NO_VISUALSTUDIO_X86
call "c:\Program Files\Microsoft Visual Studio 9.0\VC\bin\vcvars32.bat"


:NO_VISUALSTUDIO_X86
if not exist "c:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\bin\vcvars32.bat" goto NO_VISUALSTUDIO
call "c:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\bin\vcvars32.bat"

set PROJECT_SOLUTION=%SOLUTION_NAME%.sln

echo Building sources (release mode)
echo --------------------------------------------------------------------------------
devenv %PROJECT_SOLUTION% /clean Release
devenv %PROJECT_SOLUTION% /build Release
echo --------------------------------------------------------------------------------




:MAKE_DIST

echo Cleaning up (%DIST_DIR%)
echo --------------------------------------------------------------------------------
del /S /Q /F %DIST_DIR%
echo --------------------------------------------------------------------------------


echo Copying resources and binaries
echo --------------------------------------------------------------------------------
echo %EXE%
md %DIST_DIR%
md %DIST_DIR%\bin
copy %DLLS%			%DIST_DIR%\bin
copy "Release\resources.cfg" 	%DIST_DIR%\bin\resources.cfg
copy "Release\Plugins.cfg" 	%DIST_DIR%\bin\Plugins.cfg
xcopy /e /y %CFG%		%DIST_DIR%\bin
xcopy /e /y %EXE%		%DIST_DIR%\bin
xcopy /e /y %MEDIA_DIR%		%DIST_DIR%\media\
echo --------------------------------------------------------------------------------
echo %MEDIA_DIR%

goto END


:NO_VISUALSTUDIO
echo "Install visual studio 2008 (VS90)" 
goto END


:END
pause