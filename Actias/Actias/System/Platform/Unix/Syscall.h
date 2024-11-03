#pragma once
#include <Actias/System/Base.h>
#include <Actias/System/Result.h>
#include <errno.h>
#include <unistd.h>

inline static ActiasResult ActiasConvertErrnoToResult(int err)
{
    switch (err)
    {
    case ENOENT:
        return ACTIAS_FAIL_NO_FILE_OR_DIRECTORY;
    case EACCES:
        return ACTIAS_FAIL_PERMISSION_DENIED;
    case ENAMETOOLONG:
        return ACTIAS_FAIL_FILENAME_TOO_LONG;
    case EIO:
        return ACTIAS_FAIL_IO_ERROR;
    case EEXIST:
        return ACTIAS_FAIL_FILE_EXISTS;
    case ENOTDIR:
        return ACTIAS_FAIL_NOT_DIRECTORY;
    case EISDIR:
        return ACTIAS_FAIL_IS_DIRECTORY;
    case EMFILE:
        return ACTIAS_FAIL_TOO_MANY_OPEN_FILES;
    case EFBIG:
        return ACTIAS_FAIL_FILE_TOO_LARGE;
    case ENOTEMPTY:
        return ACTIAS_FAIL_DIRECTORY_NOT_EMPTY;
    case EDEADLK:
        return ACTIAS_FAIL_DEAD_LOCK;
    case ESPIPE:
        return ACTIAS_FAIL_INVALID_SEEK;
    default:
        return ACTIAS_FAIL_UNKNOWN;
    }
}

#define ACTIAS_SYSCALL_NAME(name) ActiasSyscall_##name
#define ACTIAS_SYSCALL_ARG(arg) ((UInt64)(arg))

#define ACTIAS_SYSCALL_RETURN(type, res, retValue)                                                                               \
    do                                                                                                                           \
    {                                                                                                                            \
        *(retValue) = (type)(UInt64)(res);                                                                                       \
        if ((UInt64)(res) >= (UInt64)-4095)                                                                                      \
        {                                                                                                                        \
            return ActiasConvertErrnoToResult(-(res));                                                                           \
        }                                                                                                                        \
        return ACTIAS_SUCCESS;                                                                                                   \
    }                                                                                                                            \
    while (0)

#define ACTIAS_SYSCALL_BODY(nr, type, name, retValue, ...)                                                                       \
    long long syscallResult;                                                                                                     \
    __asm__ __volatile__(ACTIAS_SYSCALL_BODY_ASM##nr "syscall\n"                                                                 \
                         : "=a"(syscallResult)                                                                                   \
                         : "0"(__NR_##name)ACTIAS_SYSCALL_BODY_ARG##nr(__VA_ARGS__)                                              \
                         : ACTIAS_SYSCALL_BODY_CLOBBER##nr "r11", "rcx", "memory");                                              \
    ACTIAS_SYSCALL_RETURN(type, syscallResult, retValue)

#define ACTIAS_SYSCALL_BODY_ASM0
#define ACTIAS_SYSCALL_BODY_ASM1 ACTIAS_SYSCALL_BODY_ASM0
#define ACTIAS_SYSCALL_BODY_ASM2 ACTIAS_SYSCALL_BODY_ASM1
#define ACTIAS_SYSCALL_BODY_ASM3 ACTIAS_SYSCALL_BODY_ASM2
#define ACTIAS_SYSCALL_BODY_ASM4 ACTIAS_SYSCALL_BODY_ASM3 "movq %5,%%r10;"
#define ACTIAS_SYSCALL_BODY_ASM5 ACTIAS_SYSCALL_BODY_ASM4 "movq %6,%%r8;"
#define ACTIAS_SYSCALL_BODY_ASM6 ACTIAS_SYSCALL_BODY_ASM5 "movq %7,%%r9;"

#define ACTIAS_SYSCALL_BODY_CLOBBER0
#define ACTIAS_SYSCALL_BODY_CLOBBER1 ACTIAS_SYSCALL_BODY_CLOBBER0
#define ACTIAS_SYSCALL_BODY_CLOBBER2 ACTIAS_SYSCALL_BODY_CLOBBER1
#define ACTIAS_SYSCALL_BODY_CLOBBER3 ACTIAS_SYSCALL_BODY_CLOBBER2
#define ACTIAS_SYSCALL_BODY_CLOBBER4 ACTIAS_SYSCALL_BODY_CLOBBER3 "r10",
#define ACTIAS_SYSCALL_BODY_CLOBBER5 ACTIAS_SYSCALL_BODY_CLOBBER4 "r8",
#define ACTIAS_SYSCALL_BODY_CLOBBER6 ACTIAS_SYSCALL_BODY_CLOBBER5 "r9",

#define ACTIAS_SYSCALL_BODY_ARG0()
#define ACTIAS_SYSCALL_BODY_ARG1(arg1) ACTIAS_SYSCALL_BODY_ARG0(), "D"(arg1)
#define ACTIAS_SYSCALL_BODY_ARG2(arg1, arg2) ACTIAS_SYSCALL_BODY_ARG1(arg1), "S"(arg2)
#define ACTIAS_SYSCALL_BODY_ARG3(arg1, arg2, arg3) ACTIAS_SYSCALL_BODY_ARG2(arg1, arg2), "d"(arg3)
#define ACTIAS_SYSCALL_BODY_ARG4(arg1, arg2, arg3, arg4) ACTIAS_SYSCALL_BODY_ARG3(arg1, arg2, arg3), "r"(arg4)
#define ACTIAS_SYSCALL_BODY_ARG5(arg1, arg2, arg3, arg4, arg5) ACTIAS_SYSCALL_BODY_ARG4(arg1, arg2, arg3, arg4), "r"(arg5)
#define ACTIAS_SYSCALL_BODY_ARG6(arg1, arg2, arg3, arg4, arg5, arg6)                                                             \
    ACTIAS_SYSCALL_BODY_ARG5(arg1, arg2, arg3, arg4, arg5), "r"(arg6)

#define ACTIAS_SYSCALL_0(type, name)                                                                                             \
    ActiasResult ACTIAS_SYSCALL_NAME(name)(type * retValue)                                                                      \
    {                                                                                                                            \
        ACTIAS_SYSCALL_BODY(0, type, name, retValue);                                                                            \
    }

#define ACTIAS_SYSCALL_1(type, name, type1, arg1)                                                                                \
    ActiasResult ACTIAS_SYSCALL_NAME(name)(type * retValue, type1 arg1)                                                          \
    {                                                                                                                            \
        ACTIAS_SYSCALL_BODY(1, type, name, retValue, ACTIAS_SYSCALL_ARG(arg1));                                                  \
    }

#define ACTIAS_SYSCALL_2(type, name, type1, arg1, type2, arg2)                                                                   \
    ActiasResult ACTIAS_SYSCALL_NAME(name)(type * retValue, type1 arg1, type2 arg2)                                              \
    {                                                                                                                            \
        ACTIAS_SYSCALL_BODY(2, type, name, retValue, ACTIAS_SYSCALL_ARG(arg1), ACTIAS_SYSCALL_ARG(arg2));                        \
    }

#define ACTIAS_SYSCALL_3(type, name, type1, arg1, type2, arg2, type3, arg3)                                                      \
    ActiasResult ACTIAS_SYSCALL_NAME(name)(type * retValue, type1 arg1, type2 arg2, type3 arg3)                                  \
    {                                                                                                                            \
        ACTIAS_SYSCALL_BODY(                                                                                                     \
            3, type, name, retValue, ACTIAS_SYSCALL_ARG(arg1), ACTIAS_SYSCALL_ARG(arg2), ACTIAS_SYSCALL_ARG(arg3));              \
    }

#define ACTIAS_SYSCALL_4(type, name, type1, arg1, type2, arg2, type3, arg3, type4, arg4)                                         \
    ActiasResult ACTIAS_SYSCALL_NAME(name)(type * retValue, type1 arg1, type2 arg2, type3 arg3, type4 arg4)                      \
    {                                                                                                                            \
        ACTIAS_SYSCALL_BODY(4,                                                                                                   \
                            type,                                                                                                \
                            name,                                                                                                \
                            ACTIAS_SYSCALL_ARG(arg1),                                                                            \
                            ACTIAS_SYSCALL_ARG(arg2),                                                                            \
                            ACTIAS_SYSCALL_ARG(arg3),                                                                            \
                            ACTIAS_SYSCALL_ARG(arg4));                                                                           \
    }

#define ACTIAS_SYSCALL_5(type, name, type1, arg1, type2, arg2, type3, arg3, type4, arg4, type5, arg5)                            \
    ActiasResult ACTIAS_SYSCALL_NAME(name)(type * retValue, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5)          \
    {                                                                                                                            \
        ACTIAS_SYSCALL_BODY(5,                                                                                                   \
                            type,                                                                                                \
                            name,                                                                                                \
                            ACTIAS_SYSCALL_ARG(arg1),                                                                            \
                            ACTIAS_SYSCALL_ARG(arg2),                                                                            \
                            ACTIAS_SYSCALL_ARG(arg3),                                                                            \
                            ACTIAS_SYSCALL_ARG(arg4),                                                                            \
                            ACTIAS_SYSCALL_ARG(arg5));                                                                           \
    }

#define ACTIAS_SYSCALL_6(type, name, type1, arg1, type2, arg2, type3, arg3, type4, arg4, type5, arg5, type6, arg6)               \
    ActiasResult ACTIAS_SYSCALL_NAME(name)(                                                                                      \
        type * retValue, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6)                                 \
    {                                                                                                                            \
        ACTIAS_SYSCALL_BODY(6,                                                                                                   \
                            type,                                                                                                \
                            name,                                                                                                \
                            ACTIAS_SYSCALL_ARG(arg1),                                                                            \
                            ACTIAS_SYSCALL_ARG(arg2),                                                                            \
                            ACTIAS_SYSCALL_ARG(arg3),                                                                            \
                            ACTIAS_SYSCALL_ARG(arg4),                                                                            \
                            ACTIAS_SYSCALL_ARG(arg5),                                                                            \
                            ACTIAS_SYSCALL_ARG(arg6));                                                                           \
    }
