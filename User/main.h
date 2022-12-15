//
// Created by xiaotian on 2022/12/15.
//

#ifndef BLINKS_MAIN_H
#define BLINKS_MAIN_H

#include "pico/stdlib.h"
#include "drv_RM68042.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <errno.h>
#include <hardware/spi.h>
#include <pico/binary_info/code.h>
#include <hardware/clocks.h>
#include <hardware/pll.h>



void drv_clock_oc2_250MHz(void);
void repeating_timer_init(void);
#endif //BLINKS_MAIN_H
