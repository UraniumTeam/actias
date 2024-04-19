# Actias SDK
Actias is a software development kit and an OS-agnostic runtime that enables binary compatibility on
desktop operating systems for dynamic libraries and executables written in C and C++.

Compile your native code once and run on any Windows or Linux machine with same processor architecture.

### Key features
Actias is not a compiler or a linker. Developers have their own preferred build tools and they can
continue using them.

Actias has a command-line tool that converts compiled OS-specific binaries (only Windows' PE is
supported for now) to its own ACBX format. The requirement is to use the ActiasSystem module for
any system calls. It is planned to implement standard C library on top in the near future.

### Usage
- Write a library or an executable that only uses ActiasSystem to call OS-specific functions;
- Libraries must use `ACTIAS_ABI` macro and C-linkage on every exported function;
- Compile your binaries and convert them using Actias CLI: `actias build [paths to files...]`
- Use `ActiasLoadModule()`, to load a library and `ActiasFindSymbolAddress()` to get a symbol pointer.
- Use CLI to run executables: `actias run [path to executable]`;

See the [Samples folder](https://github.com/UraniumTeam/actias/tree/main/Samples) in the repository
root for sample projects that use Actias.
