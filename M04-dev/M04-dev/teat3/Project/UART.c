#include "UART.h"

//缓存区结构体
USART_buff_Struct USART0_buff_Ctrl = 
{
	0,
	0,
	//1,
	0,//初始化至接收状态
	{0},
	{0},
};


void USART_init(uint32_t baudrate)
{
	//时钟初始化
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_USART0);
	
	//io口初始化
	gpio_mode_set(GPIOA,GPIO_MODE_AF,GPIO_PUPD_PULLUP,GPIO_PIN_9);
	gpio_output_options_set(GPIOA,GPIO_OTYPE_OD,GPIO_OSPEED_10MHZ,GPIO_PIN_9);
	gpio_af_set(GPIOA,GPIO_AF_1,GPIO_PIN_9);
	
	//工作参数
	usart_deinit(USART0);
	usart_baudrate_set(USART0,baudrate);
	usart_word_length_set(USART0,USART_WL_8BIT);//8位字长
	usart_parity_config(USART0,USART_PM_NONE);//无校验位
	usart_stop_bit_set(USART0,USART_STB_2BIT);//2位停止位
	usart_hardware_flow_rts_config(USART0,USART_RTS_DISABLE);
	usart_hardware_flow_cts_config(USART0,USART_CTS_DISABLE);
	usart_clock_disable(USART0);
	usart_irda_mode_disable(USART0);
	usart_smartcard_mode_disable(USART0);
	usart_lin_mode_disable(USART0);
	usart_receive_config(USART0,USART_RECEIVE_ENABLE);//使能接收
	usart_transmit_config(USART0,USART_TRANSMIT_ENABLE);//使能发送
	usart_halfduplex_enable(USART0);//半双工模式
	usart_enable(USART0);//串口使能
}

//串口中断初始化
static void NVIC_config(void)
{
	nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);   
    nvic_irq_enable(USART0_IRQn, 2, 2);                
    usart_interrupt_enable(USART0, USART_INT_IDLE);     //使能空闲中断
    usart_interrupt_enable(USART0, USART_INT_RBNE);     //使能数据缓冲区非空中断
}

//USART0的初始化
void USART0_init(uint32_t baudrate)
{
	USART_init(baudrate);
	NVIC_config();
	USART_receive_only();
}

//发送一个字节
void USART_send_byte(uint8_t Byte)
{
	usart_data_transmit(USART0,Byte);
}

//发送缓冲区数据
void USART_send_buffer(uint8_t *buffer,uint16_t len)
{	
	while(len != 0)
	{
		buffer++;

		USART_send_byte(*buffer);
		while(1)
		{
			if(usart_flag_get(USART0, USART_FLAG_TBE) == SET)break;//等待发送数据寄存器清空
		}
//		buffer++;
		len--;
	}
}

//USART串口printf重定向
int fputc(int ch, FILE *F)
{
	//uint16_t time_out = 0xffff;
	usart_data_transmit(USART0,(uint8_t)ch);
	
	while(RESET == usart_flag_get(USART0,USART_FLAG_TBE))
	{
		//if(time_out == 0)break;
		//time_out--;
	}
	return ch;
}

//设置为只发不收状态
void USART_send_only(void)
{
	usart_receive_config(USART0,USART_RECEIVE_DISABLE);
	usart_transmit_config(USART0,USART_TRANSMIT_ENABLE);
}

//设置为只收不发状态
void USART_receive_only(void)
{
	usart_receive_config(USART0,USART_RECEIVE_ENABLE);
	usart_transmit_config(USART0,USART_TRANSMIT_DISABLE);
}
	
//停止所有接收
void USART_stop_receive(void)
{
	usart_interrupt_disable(USART0,USART_INT_IDLE);//失能空闲中断
	usart_interrupt_disable(USART0,USART_INT_RBNE);//失能接收中断
	usart_receive_config(USART0,USART_RECEIVE_DISABLE);
}

//开始所有接收
void USART_start_receive(void)
{
	usart_interrupt_enable(USART0,USART_INT_IDLE);//使能空闲中断
	usart_interrupt_enable(USART0,USART_INT_RBNE);//使能接收中断
	USART_receive_only();
}
//测试灯io配置
void led_init(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_mode_set(GPIOA,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
}
//中断服务函数
void USART0_IRQHandler(void)
{
	//gpio_bit_set(GPIOA,GPIO_PIN_3);
	if(RESET != usart_interrupt_flag_get(USART0,USART_INT_FLAG_RBNE))//数据传入判定
	{
		usart_interrupt_flag_clear(USART0,USART_INT_FLAG_RBNE);
		//gpio_bit_set(GPIOA, GPIO_PIN_3);      /*************************************************/  

		if(USART0_buff_Ctrl.receive_buff_len < USART_buffsize)//判定数据包是否能存入缓冲区
		{
			gpio_bit_set(GPIOA, GPIO_PIN_3); 
			USART0_buff_Ctrl.BUFF_receive[USART0_buff_Ctrl.receive_buff_len] = usart_data_receive(USART0);
			USART0_buff_Ctrl.receive_buff_len++;
		}
		else 
			USART0_buff_Ctrl.BUFF_receive[USART_buffsize-1] = usart_data_receive(USART0);
		
	}
	
	if(RESET != usart_interrupt_flag_get(USART0,USART_INT_FLAG_IDLE))//空闲中断判定
	{
		usart_interrupt_flag_clear(USART0,USART_INT_FLAG_IDLE);
		//gpio_bit_set(GPIOA, GPIO_PIN_3);      /*************************************************/  
		if(USART0_buff_Ctrl.receive_buff_len > 0)
		{
			USART_stop_receive();
			USART0_buff_Ctrl.FLAG_receive_complete = 1;
			//len_open
		}
	}
}

