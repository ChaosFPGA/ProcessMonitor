Process Monitor
A C++ project demonstrating Windows internals, DLL injection, and function hooking by monitoring WriteFile API calls in a target process.
Overview
This project includes:

Injector.exe: A console application that injects a DLL into a specified process using CreateRemoteThread and LoadLibraryW.
HookDll.dll: A DLL that hooks the Windows WriteFile API to log calls to a file (C:\Temp\ProcessMonitorLog.txt).

Features

Uses Windows API for process enumeration and memory manipulation.
Implements inline hooking to intercept WriteFile calls.
Logs API call details (bytes written, timestamp) to a file.
Demonstrates proficiency in C++ and Windows internals.

Prerequisites

Windows OS (tested on Windows 10/11).
C++ compiler (e.g., MSVC in Visual Studio).
Administrative privileges for injection into certain processes.

Build Instructions

Clone the repository.
Open the solution in Visual Studio.
Build HookDll as a DLL project.
Build Injector as an executable.
Ensure both projects are compiled for the same architecture (x86 or x64) as the target process.

Usage

Run a target process (e.g., notepad.exe).
Execute the injector:Injector.exe <process_name> <path_to_HookDll.dll>

Example:Injector.exe notepad.exe C:\Path\To\HookDll.dll


Check C:\Temp\ProcessMonitorLog.txt for logged WriteFile calls.

Notes

Requires administrative privileges for some processes.
Ensure the target process and DLL are compatible (32-bit vs. 64-bit).
Use responsibly for educational purposes only.

License
MIT License