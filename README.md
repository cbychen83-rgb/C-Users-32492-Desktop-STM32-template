# STM32 标准库工程模板

这是一个基于 STM32F103C8 和 STM32 标准外设库的 Keil MDK 工程模板。

## 工程结构

- `user/`：用户程序、系统初始化和中断处理代码
- `my_lib/`：按键、OLED、串口、SPI、I2C 等自定义驱动
- `std_periph_driver/`：STM32F10x 标准外设库
- `startup/`：Cortex-M3 启动文件
- `RTE/`：Keil 运行时环境配置
- `template.uvprojx`：Keil 工程文件
- `Objects/`、`Listings/`：编译输出目录

## 使用方法

1. 使用 Keil MDK 打开 `template.uvprojx`。
2. 确认目标芯片和下载器配置正确。
3. 编译工程并下载到 STM32F103C8 开发板。
4. 在 `user/main.c` 中编写应用逻辑，在 `my_lib/` 中维护外设驱动。

## Git 工作流

```powershell
git pull
# 修改代码
git add .
git commit -m "描述本次修改"
git push
```

提交前建议执行：

```powershell
git status
```

