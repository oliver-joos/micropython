#ifndef LISA_INCLUDED_STM32_LWIP_ARCH_CC_H
#define LISA_INCLUDED_STM32_LWIP_ARCH_CC_H

#include <lwip_inc/arch/cc.h>
#include "py/mphal.h"

#if LWIP_DEBUG


#undef LWIP_PLATFORM_DIAG
#define LWIP_PLATFORM_DIAG(x) do {printf("[%010ld] ", HAL_GetTick()); printf x;} while(0)
// #define LWIP_PLATFORM_DIAG(x) do {printf x;} while(0)
#include <stdio.h>

#endif


#endif // LISA_INCLUDED_STM32_LWIP_ARCH_CC_H
