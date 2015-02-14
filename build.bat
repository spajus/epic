@echo off

mkdir build
pushd build
cl -Zi ..\code\win32_epic.cpp User32.lib Gdi32.lib
popd
