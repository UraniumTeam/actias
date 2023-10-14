/*
 * Actias SDK System
 * 
 * File and standard stream library
 * 
 * Copyright (c) 2023 UraniumTeam
 */

#pragma once
#include <Actias/System/Core.h>

ACTIAS_BEGIN_C

typedef enum ActiasStandardDescriptor
{
    ACTIAS_STDIN  = 0x0000,
    ACTIAS_STDOUT = 0x0001,
    ACTIAS_STDERR = 0x0002
} ActiasStandardDescriptor;

ActiasResult ActiasGetStdFileHandle(ActiasStandardDescriptor descriptor, ActiasHandle* pHandle);

ActiasResult ActiasWrite(ActiasHandle fileHandle, ACTIAS_CONST void* pBuffer, USize byteCount, USize* pBytesWritten);

ACTIAS_END_C
