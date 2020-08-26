@echo off
pushd ..\
CALL vendor\bin\premake\premake5.exe vs2019
popd
PAUSE