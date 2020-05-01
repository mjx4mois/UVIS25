/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: UV sensor
     File Name		: EXAMPLE_UVIS25.c
     Function		: EXAMPLE_UVIS25
     Create Date	: 2017/07/17
---------------------------------------------------------------------- */

#ifndef __UVIS25_EXAMPLE__
#define __UVIS25_EXAMPLE__


#define UVIS25_INIFINITE 			1		//set "1" -> read UVIS25 inifinite
#define UVIS25_RESET_ONCE		1		//reset UVIS25 once!
#define UVIS25_LCD_DISPLAY		1		//if set "1 " enable LCD display data 


#include <mega32a.h>
#include <stdio.h>
#include <delay.h>
#include <math.h>
#include <i2c.h>
#include "SENSOR_UVIS25.h"
#include "Porting_Layer.h"


#if UVIS25_LCD_DISPLAY==1
#include "alcd.h"
#endif

#if UVIS25_RESET_ONCE==1
		CHAR8U UVIS25_reset_once =1;
#endif

void  EXAMPLE_UVIS25(void);


void  EXAMPLE_UVIS25(void)
{
		
		CHAR8S status =0;
		CHAR8U id=0,uv_data;

#if UVIS25_LCD_DISPLAY      
        	CHAR8U lcd_char_data[2]={0};
#endif

		

               printf("-------------------- UV sensor UVIS25 --------------------\r\n");
#if UVIS25_RESET_ONCE==1	
			if(UVIS25_reset_once == 1)		
#endif
{
			//UV sensor UVIS25 ;
			status = UVIS25_ID(&id);
			   
			if(status==0)printf("UVIS25 ID = 0x%X\r\n",id);
			else printf("read UVIS25 ID error\r\n");
			

			//set THRESHOLD = 0x07
			//**** must be initial!
			status =  UVIS25_INITIAL();
			if(status!=0)printf("initial UVIS25 fail!!\r\n");
			else printf("initial UVIS25 success!!\r\n");
			
#if UVIS25_RESET_ONCE==1	
			UVIS25_reset_once=0;	
#endif			
}



#if UVIS25_INIFINITE ==1
		while(1)
			{
#endif	
			// UVIS25_CTRL1_MODE -> define in UVIS25.h
			status =  UVIS25_READ_UV_DATA(&uv_data,UVIS25_MODE_ONE_SHOT);
			if(status==0)printf(" UVIS25 UV data = 0x%x \r\n",uv_data);
			else printf(" UVIS25 read UV data fail\r\n");


#if UVIS25_LCD_DISPLAY == 1

                    lcd_char_data[0] = (INT32U)(uv_data/10)%10;     
                    lcd_char_data[1] = (INT32U)(uv_data)%10;
  
                    lcd_gotoxy(0,1);
                    lcd_putsf("UV-LV:"); 
	             lcd_gotoxy(7,1); 
                    lcd_putchar(48+lcd_char_data[0]);
                    lcd_putchar(48+lcd_char_data[1]);


#endif 


#if UVIS25_INIFINITE ==1
			UVIS25_DELAY(0x8F0);
			}
#endif			
                printf("-------------------- UV sensor UVIS25 --------------------\r\n");
}


#endif		//#ifndef __UVIS25_EXAMPLE__
