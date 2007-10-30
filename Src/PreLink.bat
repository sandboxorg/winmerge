@goto _MSDEV_BLD_ENV_(%_MSDEV_BLD_ENV_%)
:_MSDEV_BLD_ENV_()
@echo Merge.dsp/Merge.vcproj pre-link script
@echo Not intended for direct invocation through user interface
@echo Pre-link command line: PreLink.bat $(IntDir) $(TargetPath)
@pause
@exit

:_MSDEV_BLD_ENV_(1)
cd
echo %0
echo $(IntDir) = %1
echo $(TargetPath) = %2
mkdir ..\Build\expat
mkdir ..\Build\pcre

set info=echo *

REM _ACP_ATLPROV was introduced in VC7. If not set, assume VC6.

if "%_ACP_ATLPROV%" == "" goto MSDev
set msdev=rem
set devenv=devenv
goto %1

:MSDev
set msdev=msdev
set devenv=rem
goto %1

:.\..\BuildTmp\MergeDebug
%info% ANSI/Debug
goto Debug

:.\..\BuildTmp\MergeUnicodeDebug
%info% UNICODE/Debug
goto Debug

:.\..\BuildTmp\MergeRelease
%info% ANSI/Release
goto Release

:.\..\BuildTmp\MergeUnicodeRelease
%info% UNICODE/Release
goto Release

:Debug
%info% Debug

REM Build expat
cd %2\..\..\..\Externals\expat\lib
%msdev% "expat.dsp" /make "expat - Win32 Debug"
%devenv% "expat.vcproj" /build "Debug"
copy Debug\libexpat.lib %2\..\..\..\Build\expat
copy Debug\libexpat.dll %2\..

REM Build SCEW
cd %2\..\..\..\Externals\scew\win32
%msdev% "scew.dsp" /make "scew - Win32 Debug"
%devenv% "scew.vcproj" /build "Debug"
copy lib\scew_d.lib %2\..\..\..\Build\expat

REM Build PCRE
cd %2\..\..\..\Externals\pcre\dll_pcre
%msdev% "pcre.dsp" /make "pcre - Win32 Debug"
%devenv% "pcre.vcproj" /build "Debug"
copy ..\bin\debug\pcre.lib %2\..\..\..\Build\pcre
copy ..\bin\debug\pcre.dll %2\..
goto Common

:Release
%info% Release

REM Build expat
cd %2\..\..\..\Externals\expat\lib
%msdev% "expat.dsp" /make "expat - Win32 Release"
%devenv% "expat.vcproj" /build "Release"
copy Release\libexpat.lib %2\..\..\..\Build\expat
copy Release\libexpat.dll %2\..

REM Build SCEW
cd %2\..\..\..\Externals\scew\win32
%msdev% "scew.dsp" /make "scew - Win32 Release"
%devenv% "scew.vcproj" /build "Release"
copy lib\scew.lib %2\..\..\..\Build\expat

REM Build PCRE
cd %2\..\..\..\Externals\pcre\dll_pcre
%msdev% "pcre.dsp" /make "pcre - Win32 Release"
%devenv% "pcre.vcproj" /build "Release"
copy ..\bin\release\pcre.lib %2\..\..\..\Build\pcre
copy ..\bin\release\pcre.dll %2\..
goto Common

:Common
%info% Common
