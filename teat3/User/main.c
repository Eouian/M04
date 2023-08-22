#include "gd32f1x0.h"
#include "systick.h"
#include "stdio.h"
#include "SPI.h"
#include "RTC6705.h"

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/


void Key_init(void)
{
	;
}

int main(void)
{
    systick_config();
		Key_init();
		SPI_init_();
		
    while(1)
			{
        
        
			}
}
