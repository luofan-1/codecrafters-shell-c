@echo off
if exist "build" (
    echo removing build
    rd /s /q "build"
    echo build removed
) else (
    echo build not found
)
mkdir build
cd build
cmake ..
make
