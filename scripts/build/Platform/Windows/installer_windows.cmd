@ECHO OFF
REM
REM Copyright (c) Contributors to the Open 3D Engine Project.
REM For complete copyright and license terms please see the LICENSE at the root of this distribution.
REM
REM SPDX-License-Identifier: Apache-2.0 OR MIT
REM
REM

SETLOCAL EnableDelayedExpansion

CALL %~dp0env_windows.cmd

IF NOT EXIST %OUTPUT_DIRECTORY% (
    ECHO [ci_build] Error: $OUTPUT_DIRECTORY was not found
    GOTO :error
)
PUSHD %OUTPUT_DIRECTORY%

REM Override the temporary directory used by wix to the workspace
SET "WIX_TEMP=!WORKSPACE_TMP!/wix"
IF NOT EXIST "%WIX_TEMP%" (
    MKDIR "%WIX_TEMP%"
)

REM Make sure we are using the CMake version of CPack and not the one that comes with chocolatey
SET CPACK_PATH=
IF "%LY_CMAKE_PATH%"=="" (
    REM quote the paths from 'where' so we can properly tokenize ones in the list with spaces
    FOR /F delims^=^"^ tokens^=1 %%i in ('where /F cpack') DO (
        REM The cpack in chocolatey expects a number supplied with --version so it will error
        "%%i" --version > NUL
        IF !ERRORLEVEL!==0 (
            SET "CPACK_PATH=%%i"
            GOTO :run_cpack
        )
    )
) ELSE (
    SET "CPACK_PATH=%LY_CMAKE_PATH%\cpack.exe"
)

:run_cpack
ECHO [ci_build] "!CPACK_PATH!" --version
"!CPACK_PATH!" --version
IF ERRORLEVEL 1 (
    ECHO [ci_build] CPack not found!
    GOTO :popd_error
)

IF NOT "%CPACK_BUCKET%"=="" (
    SET "CPACK_OPTIONS=-D CPACK_UPLOAD_URL=s3://%CPACK_BUCKET% %CPACK_OPTIONS%"
)

ECHO [ci_build] "!CPACK_PATH!" -C %CONFIGURATION% %CPACK_OPTIONS%
"!CPACK_PATH!"  -C %CONFIGURATION% %CPACK_OPTIONS%
IF NOT %ERRORLEVEL%==0 (
    REM dump the log file generated by cpack specifically for WIX
    ECHO ****************************************************************
    TYPE "_CPack_Packages\\WIX\\wix.log"
    ECHO ****************************************************************
    GOTO :popd_error
)

POPD
EXIT /b 0

:popd_error
POPD

:error
EXIT /b 1
