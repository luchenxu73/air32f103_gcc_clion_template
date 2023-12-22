# Clion Template of Air32f103 Chip

air32f103是合宙的一款芯片，见[官方WIKI](https://wiki.luatos.com/chips/air32f103/index.html)

本仓库是 air32f103 mcu 在 Clion IDE下的一份移植。

> 注：由于本人工作原因，可能无法及时从官方更新最新固件库，你可以留下Issue或向我发送邮件。

用户本地需要有对应的gcc交叉编译工具链和Clion IDE才能使用。

本仓库提供了gcc下的一个裸机模板，如需freertos模板，可以向我邮件索要。

`main.c`中内容为合宙官方的 `RCC` demo，芯片将运行在216M主频下，并在串口打印相应信息。可以使用官方提供的[开发板](https://wiki.luatos.com/chips/air32f103/board.html)验证。


## 注意事项
1. 晶振默认为8M，如需修改晶振大小，可以手动定义`HSE_VALUE`，或者在`CMakeLists.txt`中使用`add_definitions()`全局添加。
2. 本仓库移植了官方的Delay，`Component`下的代码将被编译后作为静态库链接。
对应`CMakeLists.txt`中
```cmake
target_link_libraries(${PROJECT_NAME}.elf USER_LIB)
```
用户请在熟悉cmake的情况下根据自己需求重新组织项目文件，并修改`CMakeLists.txt`。
3. 示例代码默认使用了精简版的C库以适应嵌入式需求，见`CMakeLists.txt`：
```cmake
add_link_options(-specs=nano.specs)
add_link_options(-specs=nosys.specs)
```
4. 示例代码默认开启`printf`的浮点数打印，见`CmakeLists.txt`：
```cmake
add_link_options(-u_printf_float)
```
5. 合宙官方未提供`crypt`在`gcc`下的静态库，因此该功能无法使用。
6. 本仓库包含一个`stm32f103c8_blue_pill.cfg`，可以在Clion IDE中使用dap下载器烧录程序并进行调试。 




