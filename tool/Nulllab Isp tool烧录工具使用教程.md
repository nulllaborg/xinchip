# Nulllab Isp tool 烧录工具使用教程

## 一. 前置条件

基于xinchip的蓝牙电路板，即Nulllab BLE开发板；

## 二. 准备内容

1. Nulllab Isp tool v1.7
2. 烧录bin文件
3. 一根 type C 数据线
4. 电脑提前安装 CH340驱动程序 

## 三. 烧录步骤

1. **识别串口。**将Nulllab BLE开发板和电脑通过type C数据线连接，通过windows 设备管理器观察设备的端口号数字（电脑需安装CH340驱动程序后方能显示）：

![image-20210829235511211](.\readme_pic\figure1.png)

*如图，通过红框端口类别，发现设备端口号。*


2. **连接串口。**打开nulllab isp tool，选择相应端口连接 ：

![image-20210829235720898](.\readme_pic\figure2.png)

*如图，通过红框位置串口选择指定端口号，点击连接串口即可完成连接。*

3. **进入烧录模式。**将硬件端口短接，并按reset按键：

![](.\readme_pic\board_sample.jpg)

*如图，先将硬件版上，BOOT和3V3引脚短接（图片红框2处），再按RESET按键复位进入烧录模式（图片红框1处）。*

进入烧录模式后，串口打印会出现：< 的字符。

4. **执行烧录。**选择烧录文件，执行烧写动作：

![image-20210830000754971](D:\work\myCash\emakefun\xin_chip\xinchip\tool\PC_BURN\readme_pic\figure3.png)

*如图，执行红框部分的【打开文件】选择烧录bin文件之后，再执行红框的【烧写flash】进行烧录，等待程序自动完成烧写动作。

5. **复位启动。**

程序烧录完毕，看到程序串口回显burning finish字样，即可按Nulllab BLE开发板上的RESET按键，复位启动正常程序。



以上，程序即烧录完毕并启动。