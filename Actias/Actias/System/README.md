## Actias SDK System

This directory contains **OS-agnostic low-level shared C library** that should be used
in Actias applications instead of standard C library.

The purpose is to unify the *API* of all supported operating systems while also using the same
*ABI* everywhere.

This is implemented by using `[[gnu::ms_abi]]` attribute with every exported function
when compiling on Linux, so that both the Windows and the Linux versions will have the same
calling convention.
