cmake_minimum_required(VERSION 3.12)

# 设置pico-sdk的路径
set(PICO_SDK_PATH "E:/MP/RP2040/new_20221214/pico-sdk")

# 引入SDK (必须写在下面的project行前)
include(pico_sdk_import.cmake)

# 工程配置
project(Blinks)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# 初始化SDK
pico_sdk_init()

# 自定义优化等级
add_compile_options(-Os)

#预处理器搜索头文件的路径
include_directories(
        User
        middlewares/lvgl-8.3.3
        middlewares/lvgl_userapp
)

#所有需要编译的源文件
file(GLOB_RECURSE SOURCES
        "User/*.*"
        "middlewares/lvgl-8.3.3/*.*"
        "middlewares/lvgl_userapp/*.*"
        )


#添加项目的源文件
add_executable(${PROJECT_NAME}
        ${SOURCES}
        )

# 引入pico_stdlib等库，一般用到外设的时候参考例程来写
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_spi hardware_clocks  hardware_dma)

# enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(${PROJECT_NAME})