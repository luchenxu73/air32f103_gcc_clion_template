#include "air32f10x.h"
#include <stdio.h>
#include "delay.h"

#define GPIO_GROUP_TEST GPIOB
#define GPIO_MODE_TEST GPIO_Mode_Out_PP
#define GPIO_SPEED_TEST GPIO_Speed_50MHz
#define GPIO_PIN1_TEST GPIO_Pin_2
#define GPIO_PIN2_TEST GPIO_Pin_10
#define GPIO_PIN3_TEST GPIO_Pin_11

#define PRINTF_LOG printf

USART_TypeDef *USART_TEST = USART1;

void UART_Configuration(uint32_t bound);

void RCC_ClkConfiguration(void);

void GPIO_Configuration(void);

int main(void) {
    RCC_ClocksTypeDef clocks;

    RCC_ClkConfiguration();        //配置时钟
    Delay_Init();                //延时初始化
    UART_Configuration(115200); //串口初始化

    PRINTF_LOG("AIR32F103 RCC Clock Config.\r\n");
    RCC_GetClocksFreq(&clocks); //获取时钟频率

    PRINTF_LOG("\r\n");
    PRINTF_LOG(
            "SYSCLK: %3.1fMhz, \r\nHCLK: %3.1fMhz, \r\nPCLK1: %3.1fMhz, \r\nPCLK2: %3.1fMhz, \r\nADCCLK: %3.1fMhz\r\n",
            (float) clocks.SYSCLK_Frequency / 1000000, (float) clocks.HCLK_Frequency / 1000000,
            (float) clocks.PCLK1_Frequency / 1000000, (float) clocks.PCLK2_Frequency / 1000000,
            (float) clocks.ADCCLK_Frequency / 1000000);

    GPIO_Configuration();

    while (1) {
        GPIO_SetBits(GPIO_GROUP_TEST, GPIO_PIN1_TEST);
        Delay_Ms(200);
        GPIO_SetBits(GPIO_GROUP_TEST, GPIO_PIN2_TEST);
        Delay_Ms(200);
        GPIO_SetBits(GPIO_GROUP_TEST, GPIO_PIN3_TEST);
        Delay_Ms(200);
        GPIO_ResetBits(GPIO_GROUP_TEST, GPIO_PIN1_TEST);
        Delay_Ms(200);
        GPIO_ResetBits(GPIO_GROUP_TEST, GPIO_PIN2_TEST);
        Delay_Ms(200);
        GPIO_ResetBits(GPIO_GROUP_TEST, GPIO_PIN3_TEST);
        Delay_Ms(200);
    }
}

void GPIO_Configuration(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能GPIOB时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_PIN1_TEST | GPIO_PIN2_TEST | GPIO_PIN3_TEST;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_TEST; //速度50MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_TEST;     //输出模式
    GPIO_Init(GPIO_GROUP_TEST, &GPIO_InitStructure); //初始化GPIOB.2,10,11
}


void RCC_ClkConfiguration(void) {
    RCC_DeInit(); //复位RCC寄存器

    RCC_HSEConfig(RCC_HSE_ON); //使能HSE
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET); //等待HSE就绪

    RCC_PLLCmd(DISABLE);                                         //关闭PLL
    AIR_RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_27, FLASH_Div_2); //配置PLL,8*27=216MHz

    RCC_PLLCmd(ENABLE); //使能PLL
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); //等待PLL就绪

    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //选择PLL作为系统时钟

    RCC_HCLKConfig(RCC_SYSCLK_Div1); //配置AHB时钟
    RCC_PCLK1Config(RCC_HCLK_Div2);     //配置APB1时钟
    RCC_PCLK2Config(RCC_HCLK_Div1);     //配置APB2时钟

    RCC_LSICmd(ENABLE); //使能内部低速时钟
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);                //等待LSI就绪
    RCC_HSICmd(ENABLE); //使能内部高速时钟
    while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET); //等待HSI就绪
}

void UART_Configuration(uint32_t bound) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART_TEST, &USART_InitStructure);
    USART_Cmd(USART_TEST, ENABLE);
}

int SER_PutChar(int ch) {
    while (!USART_GetFlagStatus(USART_TEST, USART_FLAG_TC));
    USART_SendData(USART_TEST, (uint8_t) ch);

    return ch;
}

int _write(int fd, char *ptr, int len) {
    for (int i = 0; i < len; ++i) {
        SER_PutChar(ptr[i]);
    }
    return len;
}
