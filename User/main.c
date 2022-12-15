#include "main.h"
#include "src/core/lv_obj.h"
#include "demos/benchmark/lv_demo_benchmark.h"
#include "examples/porting/lv_port_disp.h"
#include "ui.h"

//RP2040 rm68042 3.2inch IPS LCD 480*320
//PIN:       sck=18 mosi=19 reset=20

int main() {
    drv_clock_oc2_250MHz();
    stdio_init_all(); //初始化USB-CDC串口或者UART串口，根据CMake配置决定
    const uint LED_PIN = 25;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    rm68042_init();

    lv_init();
    lv_port_disp_init();
    repeating_timer_init();
    // lv_demo_benchmark();
    ui_init();

    while (1) {
        lv_task_handler();
        lv_timer_handler();
        //gpio_xor_mask(1 << LED_PIN);
        //  printf("Hello, world! %lu\n", cnt++);
        sleep_ms(10);
    }
}
