/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: LIGHT UV sensor
     File Name		: SENSOR_UVIS25.c
     Function		: SENSOR_UVIS25
     Create Date	: 2017/07/17
---------------------------------------------------------------------- */
#ifndef __UVIS25_FUNCTION__
#define __UVIS25_FUNCTION__  

#include <stdio.h>

#include <math.h>
#include <delay.h>
#include "SENSOR_UVIS25.h"
#include "Porting_Layer.h"


//********************************************* SYSTEM *************************************************
//--------------------------------------------------------------------------------------------------
// read the UVIS25 ID -> 0xCA
CHAR8S UVIS25_ID(CHAR8U *id)
{
		CHAR8S status = 0;
		CHAR8U temp = 0;
		
		status = i2c_read_1_byte_data(UVIS25_SLAVE_ADDRESS,UVIS25_REG_ID,&temp);
		if(status !=1) return -1; 	//read fail

		*id  = temp ;			//read ID = 0xCA .
		return 0; 	//read success .
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// INITIAL UVIS25 (include set read mode , set INT & INT CFG mode  , set threshold )
CHAR8S UVIS25_INITIAL(void)
{
		CHAR8S status = 0;

		//step1 . set read mode 
		//Enable DBU ,DISABLE ODR
		status =  UVIS25_SET_MODE(UVIS25_CTRL1_MODE);	//set CTRL1 reg
		if(status !=0) return -1; //set CTRL1 mode fail

		//step2 . set INT & INT CFG mode
		//	UVIS25_INT_MODE_READY
		//  UVIS25_INT_ACTIVE_L 
		//  UVIS25_INT_PUSH
		//	UVIS25_INT_EN
		//  UVIS25_INT_NO_LIR 
		//	UVIS25_INT_NO_UVLE 
		//	UVIS25_INT_NO_UVHE
		status = UVIS25_SET_INTERRUPT_MODE(UVIS25_INT,UVIS25_INT_CFG);		//set CTRL3 , INT CFG reg
		if(status !=0) return -1; //set INT & INT CFG mode fail

		//step3 ; set threshold value
		// set "0x07"
		status = UVIS25_SET_THRESHOLD(0x07);//set THRESHOLD = 0x07.
		if(status !=0) return -1; //set THRESHOLD fail		

		return 0; // initial success!!
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// set the UVIS25 read mode -> use CTRL1 reg
//**** use UVIS25_CTRL1_MODE to set the mode ****
//mode set	1.one shot 				"0x01"
//				2.continue read data.	"0x02"
//update_type 1.1hz update data		"0x04"
//				2.update until  UV data[REG 0x28] MSB & LSB have been read !	"0x08"
//----------------------------------------------------------------------
// if  REG_CTRL1 [bit0][ODR] = 1[Enable continuous reading ] , REG_CTRL2 [bit][ONESHOT] must be "0" !
CHAR8S UVIS25_SET_MODE(CHAR8U mode_update_type)
{
	CHAR8S status = 0;
	CHAR8U ctrl1_update = 0;
	
		if(mode_update_type & 0xF0) return -2; // mode_update_type error value


			//check one shot & continue read data
			if(mode_update_type & 0x01)  ctrl1_update |= 0x00 ; //set one shot mode	

			if(mode_update_type & 0x02) ctrl1_update |=0x01 ;//set Enable continuous reading


			//check  continuous update at 1HZ  &  UV data[REG 0x28] MSB & LSB have been read  
			if(mode_update_type & 0x04) ctrl1_update |=0x00 ; // continuous update at 1HZ	

			if(mode_update_type & 0x08) ctrl1_update |=0x02 ;//update until  UV data[REG 0x28] MSB & LSB have been read !
			
			
			ctrl1_update&=UVIS25_REG_MASK_CTRL1; //mask bit 7~2

			status = i2c_write_1_byte_data(UVIS25_SLAVE_ADDRESS,UVIS25_REG_CTRL1,ctrl1_update);
			if(status!=1)return -1; // write CTRL1 reg fail.
			return 0; //write CTRL1 reg success.
		 
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// set the UVIS25 interrupt mode & reference level & INT pin EN/DIS
// set CTRL3 REG & INT_CFG REG
// **** use define UVIS25_INT call this function ****
// **** use define UVIS25_INT_CFG call this function ****
CHAR8S UVIS25_SET_INTERRUPT_MODE(CHAR8U ctrl3_data,CHAR8U int_cfg)
{
	CHAR8S status = 0;
	CHAR8U ctrl3_temp=0,int_cfg_temp=0;

	ctrl3_temp = UVIS25_REG_MASK_CTRL3 & ctrl3_data;		// mask	
	int_cfg_temp = UVIS25_REG_MASK_INT_CFG & int_cfg;	// mask	
	
	status = i2c_write_1_byte_data(UVIS25_SLAVE_ADDRESS,UVIS25_REG_CTRL3,ctrl3_temp);
	if(status!=1)return -1; // write CTRL3 reg fail.	
	
	status = i2c_write_1_byte_data(UVIS25_SLAVE_ADDRESS,UVIS25_REG_INT_CFG,int_cfg_temp);
	if(status!=1)return -1; // write INT CFG reg fail.

	return 0; // set INT mode ok!!
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// set the UVIS25 UV threshold value , range : 0~15
// set REG_THU_UV
CHAR8S UVIS25_SET_THRESHOLD(CHAR8U thr_data)
{
	CHAR8S status = 0;
	CHAR8U data = thr_data;
	
		status = i2c_write_1_byte_data(UVIS25_SLAVE_ADDRESS,UVIS25_REG_THU_UV,data);
		if(status!=1)return -1; // write REG_THU_UV  reg fail.
		return 0; //write REG_THU_UV success.
	
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// read the UVIS25 UV threshold value , range : 0~15
// set REG_THU_UV
CHAR8S UVIS25_READ_THRESHOLD(CHAR8U *thr_data)
{
	CHAR8S status = 0;
	CHAR8U data = 0;

		status = i2c_read_1_byte_data(UVIS25_SLAVE_ADDRESS,UVIS25_REG_THU_UV,&data);
		if(status!=1)return -1; //read REG_THU_UV fail
		*thr_data = data;
		return 0; //read REG_THU_UV success.

}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// check the UVIS25 interrupt status
// read REG_INT_SOURCE	[0x24]
CHAR8S UVIS25_READ_INTERRUPT_STATUS(CHAR8U *int_status)
{
	CHAR8S status = 0;
	CHAR8U data = 0;

		status = i2c_read_1_byte_data(UVIS25_SLAVE_ADDRESS,UVIS25_REG_THU_UV,&data);
		if(status!=1)return -1; //read  REG_INT_SOURCE fail
		*int_status = data;
		return 0; //read  REG_INT_SOURCE success.
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// check the UVIS25 UV data ready
// read REG_STATUS (0x27)
CHAR8S UVIS25_CHECK_DATA_READY(CHAR8U *status)
{

	CHAR8S	status_return = 0;
	CHAR8U  data = 0;
		
		status_return = i2c_read_1_byte_data(UVIS25_SLAVE_ADDRESS,UVIS25_REG_STATUS,&data);
		if(status_return!=1)return -1; //read fail
		*status = data;
		return 0; //read success.
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// read UV data
// read REG_UV_DATA [0x28]
// if set continue read -> read data speed must higher than 1hz update data.
// **** use define UVIS25_CTRL1_MODE ****
// ****  UVIS25_CTRL1_MODE &  0x02 , if =1[Enable continuous reading  ]  REG_CTRL2 [bit][ONESHOT] must be "0" !****
CHAR8S UVIS25_READ_UV_DATA(CHAR8U *uv_data,CHAR8U mode_update_type)
{
		CHAR8S	status = 0;
		CHAR8U  ctrl2_data = 0,read_data = 0;
		CHAR8U  data_ready_status = 0;
		CHAR8U  wait_data_cnt = 0;
		
       	 //avoid I2C hang , release I2C to STOP
		i2c_stop_hang();
		i2c_stop_hang();
		i2c_stop_hang();	
			//check one shot & continue read data
			if(mode_update_type & 0x01) //set one shot mode	
			{
				//write CTRL2 bit 0 , to start start for a new dataset 
				
				ctrl2_data|= 0x01 ; // bit 0 ->set "1" 				
				ctrl2_data&= UVIS25_REG_MASK_CTRL2;	//mask

				status = i2c_write_1_byte_data(UVIS25_SLAVE_ADDRESS,UVIS25_REG_CTRL2,ctrl2_data);   
				if(status!=1)return -1 ;// write CTRL2 reg fail.

				//check data ready
				for(wait_data_cnt=0;wait_data_cnt<80;wait_data_cnt++)
					{
						//one shot mode wait data ready.
						status =  UVIS25_CHECK_DATA_READY(&data_ready_status);
						if(status !=0)return -1; //read status fail.
						
						if(data_ready_status & 0x01 == 1)	//data ready to read
							{  
								status = i2c_read_1_byte_data(UVIS25_SLAVE_ADDRESS,UVIS25_REG_UV_DATA,&read_data);
								if(status!=1) return -1; //read data fail.
								*uv_data = read_data; // UV data;
								return 0 ; //read_data ok!!.
							}
						else	// data not ready
							{
							UVIS25_DELAY(100);	//delay
							}
					}	
					return -3 ; //wait data to long .
			}

			UVIS25_DELAY(100);	//delay
			if(mode_update_type & 0x02) //set Enable continuous reading
				{
					status = i2c_read_1_byte_data(UVIS25_SLAVE_ADDRESS,UVIS25_REG_STATUS,&read_data);
					if(status!=1) return -1; //read data fail.
					*uv_data = read_data; // UV data;
					return 0 ; //read_data ok!!.
				}
			
}
//--------------------------------------------------------------------------------------------------
//delay for UVIS25
void UVIS25_DELAY(INT32U cnt)
{
		INT32U temp1,temp2,temp3;

		for(temp1=0;temp1<cnt;temp1++)
			{
				for(temp2=0;temp2<255;temp2++)
					{
						temp3 ++;
					}
			}
	}
//--------------------------------------------------------------------------------------------------
//********************************************* SYSTEM *************************************************




#endif //#ifndef __UVIS25_FUNCTION__ 
