﻿﻿## 新建工程步骤

所有应用工程请直接打开原有工程，进行配置修改即可
### 1、配置编译和下载选项

点击如下图魔术棒

![1]( picture/1.png)

选择处理器为ARMCM0

![2]( picture/2.png)

选择ARM编译器版本

![3]( picture/3.png)

添加编译路径![4]( picture/4.png)

![5]( picture/5.png)

添加xinchip\bsp\core\startup\Scat文件

![6]( picture/6.png)

![7]( picture/7.png)

### 2、烧录配置

烧录方式选择（J-LINK 或者 ST-LINK 这里我们选择使用 ST-LINK 来烧录）

![8](picture/8.png)

连接好 ST-LINK 和板子后点击 Settings 可以看到如下图所示

![9]( picture/9.png)

选择 Flash Download选项卡（进行此步骤前需要添加 FLM文件）

添加FLM文件：

将文件中的xinchip\bsp\core\startup\xc620.FLM文件添加到 Keil \ARM\Flash安装目录下

![10](picture/10.png)

点击 Add 按钮，选择 xc620 128KB Flash，点击添加

![1629878246071]( picture/12.png)

![13](picture/13.png)

### 3、编译文件![14](picture/14.png)

编译输出显示 0 Error(s)，我们的工程就新建完成了。如果有报错可以根据报错信息去修改错误。

### 4、Keil烧录程序到开发板

![16](picture/15.png)

### 5、串口烧录程序

如果没有jlink或者ST-link工具，我们也可以用串口给到芯片烧录程序，使用NulllabISP.exe工具下载程序
