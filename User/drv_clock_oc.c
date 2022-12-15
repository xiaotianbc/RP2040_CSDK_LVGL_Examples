//
// Created by xiaotian on 2022/12/15.
//
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

void measure_freqs(void) {
    uint f_pll_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY);
    uint f_pll_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_USB_CLKSRC_PRIMARY);
    uint f_rosc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC);
    uint f_clk_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
    uint f_clk_peri = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_PERI);
    uint f_clk_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_USB);
    uint f_clk_adc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_ADC);
    uint f_clk_rtc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_RTC);

    printf("pll_sys  = %dkHz\n", f_pll_sys);
    printf("pll_usb  = %dkHz\n", f_pll_usb);
    printf("rosc     = %dkHz\n", f_rosc);
    printf("clk_sys  = %dkHz\n", f_clk_sys);
    printf("clk_peri = %dkHz\n", f_clk_peri);
    printf("clk_usb  = %dkHz\n", f_clk_usb);
    printf("clk_adc  = %dkHz\n", f_clk_adc);
    printf("clk_rtc  = %dkHz\n", f_clk_rtc);

    // Can't measure clk_ref / xosc as it is the ref
}

/**
 * 超频到250MHz
 */
void drv_clock_oc2_250MHz(void){
    measure_freqs();

    // Change clk_sys to be 48MHz. The simplest way is to take this from PLL_USB
    // which has a source frequency of 48MHz
    clock_configure(clk_sys,
                    CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
                    CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB,
                    48 * MHZ,
                    48 * MHZ);

    // Turn off PLL sys for good measure
    pll_deinit(pll_sys);
    /*
     * 初始化PLL
     *  \ingroup hardware_pll
     * \param pll pll_sys or pll_usb
     * \param ref_div 输入时钟的分频，输入时钟一般指外部晶体振荡器.
     * \param vco_freq  需要的输出频率，必须是输入时钟的16-320整数倍之间
     * \param post_div1 Post Divider 1 - range 1-7. Must be >= post_div2
     * \param post_div2 Post Divider 2 - range 1-7
     */
    pll_init(pll_sys, 1, 1500 * MHZ, 3, 2);

    /*! \brief Configure the specified clock
     *  \ingroup hardware_clocks
     * \param clk_index The clock to configure
     * \param src The main clock source, can be 0.
     * \param auxsrc The auxiliary clock source, which depends on which clock is being set. Can be 0
     * \param src_freq Frequency of the input clock source
     * \param freq Requested frequency
     */
    clock_configure(clk_sys,
                    CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,  //使用辅助时钟
                    CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,   //辅助时钟是PLL_SYS
                    250 * MHZ,
                    250 * MHZ);

    clock_configure(clk_peri,
                    0,
                    CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,
                    125 * MHZ,
                    125 * MHZ);

    // Re init uart now that clk_peri has changed
    stdio_init_all();
    printf("\r\n***** AFTER OC ***********\r\n");
    measure_freqs();
}