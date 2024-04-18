## About NtSourceTree

This project is being developed for Windows research purposes and is indirectly related to the larger NTective project.
The main use of NtSourceTree is to build a source tree of the core Windows components. 
Roughly speaking, it intended to be an automated and updated version of https://windows-internals.com/source/files.html
However:
- NtSourceTree replaces extensions for files that are known to have some specific extension (e.g. C++ files in the kernel).
  _Motivation_: getting as close to reality as possible.
- NtSourceTree is intended to be as automatic as possible.
  _Motivation_: to replace the author's past and inconvenient workflow with manual processing of PDB files.
- NtSourceTree is intended to be easily usable for comparing kernels of different versions unlike https://windows-internals.com/source/files.html which is a one-off product generated many years ago.
  _Motivation_: to have a tool to compare different versions of Windows always at hand.

## How it works

For debugging purposes, Microsoft provides public debugging symbols for major Windows components. 
Debug symbols are PDB (Program DataBase) files that are created at compile time by the MSVC linker on Microsoft's build servers.
PDB files store information about all debug symbols, types, modules, etc. NtSourceTree uses the MS-DIA (Microsoft Debug Interface Access) SDK to query a PDB file for this information.


