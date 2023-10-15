## Actias SDK System

This directory contains **OS-agnostic low-level shared C library** that should be used
in Actias applications instead of standard C library.

The purpose is to unify the *API* of all supported operating systems while also using the same
*ABI* everywhere.

This is implemented by using `[[gnu::ms_abi]]` attribute with every exported function
when compiling on Linux, so that both the Windows and the Linux versions will have the same
calling convention.

### Minimal API

The APIs are kept minimal and low-level to simplify testing and minimize the size of
platform-specific code. For instance, there is no `printf` function because it performs
multiple operations: formats a message, gets `STDOUT` handle and writes data to it.
Instead the user of Actias SDK System is expected to do this separately:

```cpp
// 1. formatting messages usually doesn't require system calls...
char message[] = "Hello, World!";

// 2. now get standard output handle
ActiasHandle handle;
ActiasGetStdFileHandle(ACTIAS_STDOUT, &handle);

// 3. and write
ActiasWrite(handle, message, sizeof(message), NULL);
```
