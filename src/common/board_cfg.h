#ifndef SRC_COMMON_BOARD_CFG_H_
#define SRC_COMMON_BOARD_CFG_H_

/* Board ID */

#define BOARD_ZG_222Z                   1
#define BOARD_ZG_222ZA                  2

#ifndef BOARD
#define BOARD                           BOARD_ZG_222ZA
#endif

/* Board include */
#if (BOARD == BOARD_ZG_222Z)
#include "board_zg_222z.h"
#elif (BOARD == BOARD_ZG_222ZA)
#include "board_zg_222za.h"
#else
#error BOARD must be defined
#endif

#endif /* SRC_COMMON_BOARD_CFG_H_ */
