#ifndef _SIM900A_H
#define _SIM900A_H

#include "stm32f4xx.h"


#define SIM900_USART USART3
void uart3_init(u32 bound);
void SendBookingData( unsigned char * phone,unsigned char * BookingData );

#endif /*  */
