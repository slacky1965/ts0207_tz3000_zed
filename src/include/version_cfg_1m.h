#ifndef SRC_INCLUDE_VERSION_CFG_1M_H_
#define SRC_INCLUDE_APP_VERSION_CFG_1M_H_

#if defined(MCU_CORE_826x)
    #if (CHIP_8269)
        #define CHIP_TYPE                   TLSR_8269
    #else
        #define CHIP_TYPE                   TLSR_8267
    #endif
#elif defined(MCU_CORE_8258)
//    #error TLSR_8258_1M
    #define CHIP_TYPE                       TLSR_8258_1M//TLSR_8258_512K//
#elif defined(MCU_CORE_8278)
        #define CHIP_TYPE                   TLSR_8278
#elif defined(MCU_CORE_B91)
        #define CHIP_TYPE                   TLSR_B91
#endif

#endif
