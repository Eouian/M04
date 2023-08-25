#include "gd32f1x0.h"
#include <stdio.h>
 
#include "systick.h"


void PWM_Output(uint16_t PSC, uint16_t ARR, uint16_t Val)
{   
    rcu_periph_clock_enable(RCU_GPIOB);             
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5);                 
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_5);    
    gpio_af_set(GPIOB, GPIO_AF_1, GPIO_PIN_5);     
        
    rcu_periph_clock_enable(RCU_TIMER2);           
    timer_deinit(TIMER2); 
	
    timer_oc_parameter_struct timer_ocinitpara;      
    timer_parameter_struct timer_initpara; 
    timer_initpara.prescaler         = PSC;                     
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;      
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;       
    timer_initpara.period            = ARR;                     
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;        
    timer_initpara.repetitioncounter = 0;                       
    timer_init(TIMER2,&timer_initpara);
    
    timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;               
    timer_ocinitpara.outputnstate = TIMER_CCXN_DISABLE;             
    timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;         
    timer_ocinitpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;     
    timer_ocinitpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;        
    timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;    
    timer_channel_output_config(TIMER2, TIMER_CH_1, &timer_ocinitpara);
    timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_1, Val);           
    timer_channel_output_mode_config(TIMER2, TIMER_CH_1, TIMER_OC_MODE_PWM0);       
    timer_channel_output_shadow_config(TIMER2, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);
    timer_primary_output_config(TIMER2, ENABLE);
    
    timer_auto_reload_shadow_enable(TIMER2);
    timer_enable(TIMER2);
}
