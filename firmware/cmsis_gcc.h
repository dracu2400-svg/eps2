/**
  ******************************************************************************
  * @file     cmsis_gcc.h
  * @brief    CMSIS compiler GCC header file
  * @version  V1.0.0
  * @date     2025-01-18
  *
  * @note     Minimal CMSIS GCC compiler-specific definitions for STM32L4 port
  ******************************************************************************
  */

#ifndef __CMSIS_GCC_H
#define __CMSIS_GCC_H

/* CMSIS compiler specific defines */
#ifndef   __ASM
  #define __ASM                                  __asm
#endif
#ifndef   __INLINE
  #define __INLINE                               inline
#endif
#ifndef   __STATIC_INLINE
  #define __STATIC_INLINE                        static inline
#endif
#ifndef   __NO_RETURN
  #define __NO_RETURN                            __attribute__((__noreturn__))
#endif
#ifndef   __USED
  #define __USED                                 __attribute__((used))
#endif
#ifndef   __WEAK
  #define __WEAK                                 __attribute__((weak))
#endif
#ifndef   __PACKED
  #define __PACKED                               __attribute__((packed, aligned(1)))
#endif
#ifndef   __PACKED_STRUCT
  #define __PACKED_STRUCT                        struct __attribute__((packed, aligned(1)))
#endif
#ifndef   __ALIGNED
  #define __ALIGNED(x)                           __attribute__((aligned(x)))
#endif

/* NVIC Priority Bits */
#ifndef __NVIC_PRIO_BITS
#define __NVIC_PRIO_BITS    4  /**< STM32L4 uses 4 bits for priority levels */
#endif

#endif /* __CMSIS_GCC_H */
