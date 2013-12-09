File Meta Checker
=================
  
File Meta Checker is a tool to extract metadatas from various files as images, video, sound etc.  
It based on libraryless extractor, only a human readable specification describe the standard.  

___
Compilation
===========

> `git clone https://github.com/mikrosimage/FileMetaChecker.git`
> `git submodule update -i`
> `scons`

and to launch unittest
> `scons unittest`


___
Tested compilers
================

- Linux
 + GCC 4.6.2 on OpenSUSE 12.1 64bits

___
User Guide
================

Write a specification file
--------------------------

[How to write a specification file ?](fileSpecification/README.md)