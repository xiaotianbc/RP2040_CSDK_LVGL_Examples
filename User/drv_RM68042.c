//
// Created by xiaotian on 2022/12/15.
//
#include <stddef.h>
#include <hardware/gpio.h>
#include "drv_RM68042.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <errno.h>
#include "pico/binary_info.h"
#include "hardware/spi.h"
#include "hardware/dma.h"

static uint dma_tx;
static dma_channel_config dma_spi_tx_config;

//sck 18 mosi 19 reset 20
void drv_RM68042_spi_init() {
    // Enable SPI 0 at 1 MHz and connect to GPIOs
    spi_init(spi_default, 60000 * 1000);
    spi_set_format(spi_default, 9, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN,
                               GPIO_FUNC_SPI));
    const uint reset_PIN = 20;
    gpio_init(reset_PIN);
    gpio_set_dir(reset_PIN, GPIO_OUT);

    //配置DMA
    dma_tx = dma_claim_unused_channel(true);
    dma_spi_tx_config = dma_channel_get_default_config(dma_tx);
    channel_config_set_transfer_data_size(&dma_spi_tx_config, DMA_SIZE_16);
    channel_config_set_dreq(&dma_spi_tx_config, spi_get_dreq(spi_default, true));
    channel_config_set_read_increment(&dma_spi_tx_config, true);
    channel_config_set_write_increment(&dma_spi_tx_config, false);
}


static __force_inline void __not_in_flash_func(drv_lcd_tx_data_dma)(void *addr, uint len) {
    while (dma_channel_is_busy(dma_tx)) tight_loop_contents();
    // stop the compiler hoisting a non volatile buffer access above the DMA completion.
    __compiler_memory_barrier();

    dma_channel_configure(dma_tx, &dma_spi_tx_config,
                          &spi_get_hw(spi_default)->dr, // write address
                          addr, // 发送数据地址
                          len, // 按照transfer_data_size计算出的数据长度
                          true); // 立即启动DMA传输
}

static __force_inline void reset_low() {
    gpio_put(20, 0);
}

static __force_inline void set_low() {
    gpio_put(20, 1);
}


#define lcd_delay sleep_ms

#define openLcdBL()
#define offLcdBL()


void __not_in_flash_func(LCD_WR_REGs)(unsigned char command) {
    uint16_t dat = command;
    drv_lcd_tx_data_dma(&dat, 1);
}

void __not_in_flash_func(LCD_WR_DATAs)(unsigned char data) {
    uint16_t dat = data | 0x100;
    drv_lcd_tx_data_dma(&dat, 1);
}


void rm68042_init(void) {
    drv_RM68042_spi_init();
    reset_low();
    lcd_delay(300);
    set_low();
    lcd_delay(300);


//************* Start Initial Sequence **********//
// RM68042+ AUO3.2 320*480
    LCD_WR_REGs(0X11);
    lcd_delay(20);
    LCD_WR_REGs(0XD0);//VCI1  VCL  VGH  VGL DDVDH VREG1OUT power amplitude setting
    LCD_WR_DATAs(0X07);
    LCD_WR_DATAs(0X42);
    LCD_WR_DATAs(0X1D);
    LCD_WR_REGs(0XD1);//VCOMH VCOM_AC amplitude setting
    LCD_WR_DATAs(0X00);
    LCD_WR_DATAs(0X1a);
    LCD_WR_DATAs(0X09);
    LCD_WR_REGs(0XD2);//Operational Amplifier Circuit Constant Current Adjust , charge pump frequency setting
    LCD_WR_DATAs(0X01);
    LCD_WR_DATAs(0X22);
    LCD_WR_REGs(0XC0);//REV SM GS
    LCD_WR_DATAs(0X10);
    LCD_WR_DATAs(0X3B);
    LCD_WR_DATAs(0X00);
    LCD_WR_DATAs(0X02);
    LCD_WR_DATAs(0X11);

    LCD_WR_REGs(0XC5);// Frame rate setting = 72HZ  when setting 0x03
    LCD_WR_DATAs(0X03);

    LCD_WR_REGs(0XC8);//Gamma setting
    LCD_WR_DATAs(0X00);
    LCD_WR_DATAs(0X25);
    LCD_WR_DATAs(0X21);
    LCD_WR_DATAs(0X05);
    LCD_WR_DATAs(0X00);
    LCD_WR_DATAs(0X0a);
    LCD_WR_DATAs(0X65);
    LCD_WR_DATAs(0X25);
    LCD_WR_DATAs(0X77);
    LCD_WR_DATAs(0X50);
    LCD_WR_DATAs(0X0f);
    LCD_WR_DATAs(0X00);

    LCD_WR_REGs(0XF8);
    LCD_WR_DATAs(0X01);

    LCD_WR_REGs(0XFE);
    LCD_WR_DATAs(0X00);
    LCD_WR_DATAs(0X02);

    LCD_WR_REGs(0X20);//Exit invert mode

    LCD_WR_REGs(0X36);//Set_address_mode.XY,rgb.bgrºáÊúÆÁ
    LCD_WR_DATAs(0X28);//

    LCD_WR_REGs(0X3A);
    LCD_WR_DATAs(0X55);//rgb565.16bit
//SPI_WriteComm(0xB4); //RGB
//SPI_WriteData(0x11);
    LCD_WR_REGs(0X2B);
    LCD_WR_DATAs(0X00);
    LCD_WR_DATAs(0X00);
    LCD_WR_DATAs(0X01);
    LCD_WR_DATAs(0X3F);

    LCD_WR_REGs(0X2A);
    LCD_WR_DATAs(0X00);
    LCD_WR_DATAs(0X00);
    LCD_WR_DATAs(0X01);
    LCD_WR_DATAs(0XDF);
    lcd_delay(120);
    LCD_WR_REGs(0X29);

    putimage(0, 0, LCD_WIDTH, LCD_HEIGHT, NULL, BLACK);
    openLcdBL();

}


/**
 * 设置窗口
 * @param x1 起始x
 * @param y1 起始y
 * @param x2 结束x
 * @param y2 结束y
 */
void setWindows(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2) {
    LCD_WR_REGs(0x2a);//发送窗口起始坐标
    LCD_WR_DATAs(x1 >> 8);
    LCD_WR_DATAs(x1);
    LCD_WR_DATAs(x2 >> 8);
    LCD_WR_DATAs(x2);
    LCD_WR_REGs(0x2b);//发送窗口结束坐标
    LCD_WR_DATAs(y1 >> 8);//
    LCD_WR_DATAs(y1);//
    LCD_WR_DATAs(y2 >> 8);
    LCD_WR_DATAs(y2);//
    LCD_WR_REGs(0x2C);// 发送开始写入像素的命令
}

void __not_in_flash_func(DrawPixel)(unsigned short x_start, unsigned short y_start, unsigned short color) {
    setWindows(x_start, y_start, LCD_WIDTH, LCD_HEIGHT);
    LCD_WR_DATAs(color >> 8);
    LCD_WR_DATAs(color);
}

/*putimage*/
void putimage(
        int x_start,              // ????????¦Ì? x ¡Á?¡À¨º
        int y_start,              // ????????¦Ì? y ¡Á?¡À¨º
        int dstWidth,          // ????¦Ì??¨ª?¨¨
        int dstHeight,       // ????¦Ì????¨¨
        void *pSrcImg,        // ¨°a????¦Ì? IMAGE ???¨®????
        unsigned short fillcolor
) {
    unsigned short int *index = (unsigned short int *) pSrcImg;
    setWindows(x_start, y_start, x_start + dstWidth - 1, y_start + dstHeight - 1);
    if (NULL == pSrcImg) {
        for (unsigned int scan = 0; scan < dstWidth * dstHeight; scan++) {
            LCD_WR_DATAs((unsigned char) (fillcolor >> 8));
            LCD_WR_DATAs((unsigned char) fillcolor);
        }
    } else {
        for (unsigned int scan = 0; scan < dstWidth * dstHeight; scan++) {
            LCD_WR_DATAs(*index >> 8);
            LCD_WR_DATAs(*index);
            index++;
        }
    }
}

//为了把8位spi转成9位spi
static uint16_t local_frame_buffer[2][9600];
static uint32_t buffer_index = 0;

void __not_in_flash_func(portDisp_flush)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t *color_p) {
    buffer_index = 1 - buffer_index;
    uint32_t pixel_num = (x2 - x1 + 1) * (y2 - y1 + 1); //总共需要刷的像素个数，每个像素有两个自己的颜色数据
    for (int i = 0; i < pixel_num * 2; ++i) {
        local_frame_buffer[buffer_index][i] = *(color_p + i) | 0x100;
    }
    //发送spi之前会等待dma传输完成，所以先修改缓冲区再发送spi数据
    setWindows(x1, y1, x2, y2);
    drv_lcd_tx_data_dma(local_frame_buffer[buffer_index], pixel_num * 2);
}