//
// Created by xiaotian on 2022/12/15.
//
#include "main.h"
#include "src/hal/lv_hal_tick.h"

//重复定时器

//声明一个定时器实体，在定时器运行的过程中，该实体必须存在，所以不能定义在栈上
static struct repeating_timer timer;

bool __not_in_flash_func(repeating_timer_callback)(struct repeating_timer *t) {
    lv_tick_inc(10);
    // printf("Repeat at %lld ms\n", time_us_64() / 1000);
    return true;
}

/**
 * 创建一个重复定时器调用回调函数。
 *   如果delay>0 ,这个delay就是上次运行结束到下次运行开始的间隔
 *  如果delay<0 ，这个delay就是上次调用开始到这次调用开始的间隔
 */
void repeating_timer_init(void) {
    add_repeating_timer_ms(-10, repeating_timer_callback, NULL, &timer);
}