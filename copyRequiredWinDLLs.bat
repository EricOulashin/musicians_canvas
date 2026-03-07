@echo off
REM This batch file copies the required Windows DLLs to the current
REM directory. This is meant to be run from the directory where the
REM executables are.
REM This assumes msys64 has been installed to C:\msys64 and the
REM correct packages have been installed.
copy C:\msys64\mingw64\bin\libFLAC.dll .
copy C:\msys64\mingw64\bin\libfluidsynth-3.dll .
copy C:\msys64\mingw64\bin\libglib-2.0-0.dll .
copy C:\msys64\mingw64\bin\libgmodule-2.0-0.dll .
copy C:\msys64\mingw64\bin\libgomp-1.dll .
copy C:\msys64\mingw64\bin\libiconv-2.dll .
copy C:\msys64\mingw64\bin\libintl-8.dll .
copy C:\msys64\mingw64\bin\libmp3lame-0.dll .
copy C:\msys64\mingw64\bin\libmpg123-0.dll .
copy C:\msys64\mingw64\bin\libogg-0.dll .
copy C:\msys64\mingw64\bin\libopus-0.dll .
copy C:\msys64\mingw64\bin\libpcre2-8-0.dll .
copy C:\msys64\mingw64\bin\libportaudio.dll .
copy C:\msys64\mingw64\bin\libreadline8.dll .
copy C:\msys64\mingw64\bin\librtmidi-7.dll .
copy C:\msys64\mingw64\bin\libsndfile-1.dll .
copy C:\msys64\mingw64\bin\libtermcap-0.dll .
copy C:\msys64\mingw64\bin\libvorbis-0.dll .
copy C:\msys64\mingw64\bin\libvorbisenc-2.dll .