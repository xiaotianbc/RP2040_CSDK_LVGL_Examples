//
// Created by xiaotian on 2022/12/15.
//

#ifndef BLINKS_DRV_RM68042_H
#define BLINKS_DRV_RM68042_H

#define WHITE        0xFFFF
#define BLACK        0x0000
#define BLUE         0x001F
#define BRED         0XF81F
#define GRED               0XFFE0
#define GBLUE               0X07FF
#define RED          0xF800
#define MAGENTA      0xF81F
#define GREEN        0x07E0
#define CYAN         0x7FFF
#define YELLOW       0xFFE0
#define BROWN             0XBC40 //��?��?
#define BRRED             0XFC07 //��?o����?
#define GRAY             0X8430 //?����?
#define DARKBLUE     0X01CF    //��?��?��?
#define LIGHTBLUE    0X7D7C    //?3��?��?
#define GRAYBLUE     0X5458 //?����?��?
#define Orchid       0XF11F //��?��?
#define LCD_WIDTH  (480)
#define LCD_HEIGHT (320)


#ifdef __cplusplus
extern "C" {
#endif

/*SEND COMMAND*/
void LCD_WR_REG(unsigned char Command);

/*send data*/
void LCD_WR_DATA(unsigned char Data);

/*init_lcd*/
void rm68042_init(void);

/*setWindows*/
void setWindows(unsigned short start_row, unsigned short start_col, unsigned short end_row, unsigned short end_col);

/*......putimage......*/
void putimage(
        int x_start,              // ????????��? x ��?����
        int y_start,              // ????????��? y ��?����
        int dstWidth,          // ????��??��?��
        int dstHeight,       // ????��????��
        void *pSrcImg,        // ��a????��? IMAGE ???��????
        unsigned short fillcolor
);

/*.......DrawPixel.......*/
void DrawPixel(unsigned short x_start, unsigned short y_start, unsigned short color);

void portDisp_flush(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t *color_p);

#ifdef __cplusplus
}
#endif



//sck 18 mosi 19 reset 20

typedef enum {
    LCDCS,
    LCDSCK,
    LCDMOSI,
} lcd_pin;

#endif //BLINKS_DRV_RM68042_H
