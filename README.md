本项目[代码规范](https://github.com/nulllaborg/c-code-style/blob/master/README_zh.md)
## 写在之前
  国产单片机发展至今，已经涌现出诸如GD，MM，STC等很多规模较大的厂家，芯片缺货大潮下，国产单片机知名原厂也同样缺货，涨价。而不缺货，价格厚道的小众单片机，却无厂家敢用。首先小众单片机，如果不是熟人介绍，几乎在网上找不到任何信息，另外就算找到之后，拿到样品测试时，发现芯片压根没有像大厂一样做完善的数据手册，示例程序，甚至有效的技术支持都得不到保证。原因是老板和核心就那么几个人，但是却要完成芯片产业链的全流程，只能抓大放小，如此循环。作为曾经供职于全球前三半导体设计公司以及狂爱电子技术的笔者，深受感慨。于是找了几个行业的老鸟一番吹牛，他们目前把我的想法叫做空想，但是最终这些人还是决定做一些行动，于是我们给自己套了一个叫空想实验室的名头。

  nulllab团队致力于开发出基于国产单片机，国产芯片，编写教程，demo，搭建一个开发软硬件生态平台，让更多开发着能方便快速地基于国产超高性价比的芯片做出自己的产品，空想有一天打破国外MCU垄断，让国内真正有实力超高性价比的好芯片，能站上舞台。

## 开发板
nulllab推出第一款基于国产蓝牙芯片xc620的开发板，这个芯片，抛开蓝牙之后，其实就是一颗基于cortex-m0的单片机，在缺货盛行的时候，依然能保持低于3元的持续供货。

## 开发环境搭建
- [keil配置和程序下载](keil_new_project_wizard.md)

## 特点
- 蓝牙4.2
- ARM-CortexM0 32M主频,128K ram,256K flash
- 2路Uart(Uart0带流控)
- 8路10bit ADC
- 1路SPI
- 1路I2C
- 6路PWM
- 最多19个GPIO
- 提供SOP16，SSOP16，QFN32(4x4mm)三种封装

## KEIL新建工程向导
- [工程向导](Keil_New_Project_Wizard.md)
## 基础示例程序

- [led](./Project/led/app/main.c) - gpio控制led灯
- [button]() - 按键输入
- [timer]() - 定时器
- [watch_dog]() - 看门狗程序
- [delay](./Project/led/app/main.c)- 系统级精确延时
- [extern_isr]() - 外部引脚输入中断
- [adc](./Project/adc/app/main.c) - ADC多路采样
- [uart](./Project/uart/app/main.c) - 串口示例程序
- [pwm]() - PWM输出程序
- [i2c_master]() - 硬件i2c主机程序
- [i2c_slave](./Project/i2c_slave/app/main.c) - 硬件i2c从机设备程序
- [spi]() - 硬件spi测试程序
- [spi_flash](./Project/spi_flash/app/main.c) - 内部falsh读写操作程序
- [ws2812b](./Project/ws2812b/app/main.c) - 基于spi硬件dma输出的ws2812b彩灯控制程序
- [onewire]() - 单总线协议驱动基于ds18b20芯片实现
- [rtc]() - 单片机内部rtc使用
- [pm]() - 电源管理，低功耗配置

## 蓝牙示例程序
- [peripheral](./Project/ble_bypass/app/main.c) - 从机蓝牙透传示例程序
- [centerl]() - 主机连接透传示例程序
- [ibeacon]() - ibeacon测试程序

## 综合应用程序
- [joystick]() - 蓝牙手柄
- [keypad]() - 蓝牙键盘
- [mouse]() - 蓝牙鼠标

## 写在最后
如果您和我们团队一样，不管您是芯片原厂（小厂更佳），还是爱好者，还是学生，希望为国产芯片贡献一份力量，请加我微信号null-lab
