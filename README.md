# birdflu – ELF Binary Scanner using Linux syscalls

## Overview
birdflu is a low-level Linux filesystem scanner written in C that
recursively traverses directories and identifies ELF binaries by
reading raw file headers.

## Key Features
- Uses SYS_getdents syscall (no libc directory helpers)
- Custom dynamic in-memory database
- Function-pointer based filtering
- Raw ELF header inspection
- Recursive directory traversal

## Why this project?
This project was built to deeply understand Linux internals,
filesystem layouts, and low-level memory management.

## Build
make

## Usage
./birdflu /usr/bin

## Internals
See docs/internals.md