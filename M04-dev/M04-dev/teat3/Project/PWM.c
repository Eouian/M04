#include "gd32f1x0.h"
#include <stdio.h>
 
#include "systick.h"


void PWM_Output(uint16_t PSC, uint16_t ARR, uint16_t Val)
{   
		//PB5初始化
    rcu_periph_clock_enable(RCU_GPIOB);             
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5);                 
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_5);    
    gpio_af_set(GPIOB, GPIO_AF_1, GPIO_PIN_5);     
        
    rcu_periph_clock_enable(RCU_TIMER2);//使能定时器2的时钟           
    timer_deinit(TIMER2); //清空定时器2寄存器的数据
	
    timer_oc_parameter_struct timer_ocinitpara;//比较捕获参数结构体	
    timer_parameter_struct timer_initpara; //定时器基本参数结构体
	
    timer_initpara.prescaler         = PSC;//预分频参数                     
		timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;//计数模式为边沿      
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;//计数方向为递增       
    timer_initpara.period            = ARR;//计数周期                     
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;        
    timer_initpara.repetitioncounter = 0;                       
    timer_init(TIMER2,&timer_initpara);
    
    timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;//使能捕获比较通道               
    timer_ocinitpara.outputnstate = TIMER_CCXN_DISABLE;//禁能捕获比较通道（反向端N）
    timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;//输出极性         
    timer_ocinitpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;     
    timer_ocinitpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;//空闲电平        
    timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;    
    timer_channel_output_config(TIMER2, TIMER_CH_1, &timer_ocinitpara);
    timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_1, Val);//输出的占空比           
    timer_channel_output_mode_config(TIMER2, TIMER_CH_1, TIMER_OC_MODE_PWM1);//选择模式1，当计数值小于比较值时为低电平       
    timer_channel_output_shadow_config(TIMER2, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);
    timer_primary_output_config(TIMER2, ENABLE);
    
    timer_auto_reload_shadow_enable(TIMER2);//使能自动重装
    timer_enable(TIMER2);//使能定时器2
}
