#ifndef UART_H
#define UART_H
#include "gd32f1x0.h"
#include <stdio.h>
#include "systick.h"
#include "stdio.h"

#define USART_buffsize  ((uint16_t)32)

void USART_init(uint32_t baudrate);//IO、时钟、工作参数初始化
void NVIC_config(void);//串口中断初始化
void USART0_init(uint32_t baudrate);//USART0初始化
void USART_send_byte(uint8_t Byte);
void USART_send_buffer(uint8_t *buffer,uint16_t len);
int fputc(int ch, FILE *F);//printf的重定向
//半双工通信
void USART_send_only(void);
void USART_receive_only(void);
void USART_stop_receive(void);
void USART_start_receive(void);
/******************************/
void led_init(void);//测试灯——绿
/******************************/
//USART0_BUFF结构体
typedef struct
{
	uint8_t send_buff_len;
	uint8_t receive_buff_len;
	//uint8_t FLAG_send_complete;//1：完成，0：未完成
	uint8_t FLAG_receive_complete;//1：完成，0：未完成
	uint8_t BUFF_send[USART_buffsize];
	uint8_t BUFF_receive[USART_buffsize];
}USART_buff_Struct;
extern USART_buff_Struct USART0_buff_Ctrl;

#endif
