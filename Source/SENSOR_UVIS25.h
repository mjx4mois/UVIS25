/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: LIGHT UV sensor
     File Name		: SENSOR_UVIS25.h
     Function		: SENSOR_UVIS25
     Create Date	: 2017/07/03
---------------------------------------------------------------------- */
#ifndef __UVIS25_HEADER__
#define __UVIS25_HEADER__  

//***********************************
//Note : if CS pin connect "VCC"	 -> I2C  
//		  if CS pin connect "GND"	 -> SPI
//***********************************



//--------------------------------- Define SLAVE ADDRESS -------------------------------------
// UVIS25 SLAVE ADDRESS
#define UVIS25_SLAVE_ADDRESS 				0x8E			
//--------------------------------- Define SLAVE ADDRESS -------------------------------------

// ------------- DEFINE UVIS25 REGISTER  ------------
// REG addr 0x00~0xE ,	0x10~0x1F  ,	0x26   reserved 
// Registers marked as Reserved must not be changed. 
// The writing to those registers may cause permanent damages to the device!!!

#define	UVIS25_REG_ID							0x0F	//R/W	  ,   value must be : 0xCA
#define	UVIS25_REG_CTRL1						0x20	//R/W	  ,   value must be : 0x00
//bit7~2 set "0" , ensure device work .
//bit 1  BDU[block data update] def:0 , "1" -> update until  UV data[REG 0x28] MSB & LSB have been read !
//										 "0" -> continuous update at 1HZ
//bit 0  ODR def:0  , "1" ->  Enable continuous reading   
//					   "0" ->  One Shot mode   
//set with REG 0x21  ONESHOT bit!
// REG_CTRL1 [bit0] , REG_CTRL2 [bit0]
//           0                          0				->	the device is in power down mode.
//           0					   1				->	a new UV index value is acquired.
// if  REG_CTRL1 [bit0][ODR] = 1 , REG_CTRL2 [bit][ONESHOT] must be "0" !
//-------------------------------------------------------------------------
#define	UVIS25_REG_CTRL2						0x21	//R/W	  ,   value must be : 0x00		
//bit7  BOOT def:0 , Reboot memory content.	, "1" -> reboot memory content ,The bit is self-cleared when the BOOT is completed.
//												  "0" -> normal mode
//bit 6,5,2,1 set "0"
//bit 4 I2SDIS	def:0 , "1" ->  DIS I2C; "0" -> EN I2C
//bit 3 SPIWIRE def:0 , "1" -> 4-wire ; "0" -> 3-wire
//bit 0 ONESHOT def:0 , "1" -> start for a new data set  ; "0" ->  waiting for start of conversion;
// when measurement done, the bit self-clear , STATUS_REG[0x27] bits are updated [STATUS].
// if  REG_CTRL1 [bit0][ODR] = 1 , REG_CTRL2 [bit][ONESHOT] must be "0" !
//-------------------------------------------------------------------------
#define	UVIS25_REG_CTRL3						0x22	//R/W	  ,   value must be : 0x00
//bit7 	INT_H_L def:0   Interrupt active high, low ,"1" -> active low  ; "0" -> active high
//bit6   PP_OD def:0     Push-pull/open drain selection on interrupt pads. "1" -> open drain ; "0" -> push-pull
//bit5~2 set "0", ensure device work .
//   [INT_S] bit1    bit0 	INT1 pin status
// 			   0       0           Data ready      
//			   0       1		  UV index High
//			   1       0		  UV index Low
//			   1       1           UV index High or Low
//-------------------------------------------------------------------------
#define	UVIS25_REG_INT_CFG					0x23	//R/W	  ,   value must be : 0x00		
//bit7~4 SET "0"
//bit3 DIFF_EN  Interrupt generation enable. def: 0  , "1" ->interrupt circuit enabled  , "0" ->interrupt circuit disable
//bit2 LIR  Latch interrupt request to the INT_SOURCE[0x24]REG   def:0
//				"1"-> interrupt request latched
//				"0"-> interrupt request not latched
//bit 1  UVLE   Enable interrupt generation on differential UV low event. def:0
//				"1"-> enable interrupt ,the UV values lower than preset threshold
//				"0"-> disable interrupt request

//bit 0  UVHE   Enable interrupt generation on differential UV high event. def:0
//				"1"-> enable interrupt ,the UV values higher than preset threshold
//				"0"->  disable interrupt request
//-------------------------------------------------------------------------
#define	UVIS25_REG_INT_SOURCE				0x24	//Read Only       ,   value must be : 0x00		  
//bit7~3 SET "0"
//bit2 Interrupt active. def:0  "1"->one or more interrupt events have been generated  ;  "0"->no interrupt has been generated

//bit1  Differential UV low. def:0     "1"-> UV low event has occurred   ; "0" -> no interrupt has been generated
//bit0  Differential UV high. def:0    "1"-> UV high event has occurred ; "0" -> no interrupt has been generated
//-------------------------------------------------------------------------
#define	UVIS25_REG_THU_UV					0x25	//R/W  ,   value must be : 0x00		
//Threshold value 
//This register contains the differential UV Interrupt " threshold value " for the interrupt generation.
//-------------------------------------------------------------------------
#define	UVIS25_REG_STATUS					0x27	//Read Only    ,   value must be : 0x00	
//bit7~1 SET "0"
//bit 0  UV_DA   UV data available. def:0.  "1" -> UV data ready to read  ; "0"-> UV data not ready.
//-------------------------------------------------------------------------
#define	UVIS25_REG_UV_DATA					0x28	//Read Only    ,   value must be : 	//output UV data REG 	
//the UV data ; range 0~15 ; resolution : 16 step .
// ------------- DEFINE UVIS25 REGISTER  ------------




//-------------------- USER SET MODE USE DEFINE -----------------------


//------------- DEFINE UVIS25 REG MASK ------------
#define UVIS25_REG_MASK_CTRL1				0x03
#define UVIS25_REG_MASK_CTRL2				0x01
#define	UVIS25_REG_MASK_CTRL3				0xC3
#define	UVIS25_REG_MASK_INT_CFG			0x0F
//------------- DEFINE UVIS25 REG MASK ------------


//------------- DEFINE UVIS25 READ MODE ------------
//-----------------------------
#define UVIS25_MODE_ONE_SHOT				0x01
#define UVIS25_MODE_CONT_READ			0x01
#define UVIS25_MODE_EN_BDU				0x02
#define UVIS25_CONT_UPDATE_1HZ			0x04
#define UVIS25_AFTER_READED				0x08

#define UVIS25_CTRL1_MODE  UVIS25_MODE_ONE_SHOT
//------------- DEFINE UVIS25 READ MODE ------------


//------------- DEFINE UVIS25 INTERRUPT MODE ------------
//CTRL3 set
//   [INT_S] bit1    bit0 	INT1 pin status
// 			   0       0           Data ready      
//			   0       1		  UV index High
//			   1       0		  UV index Low
//			   1       1           UV index High or Low
#define UVIS25_INT_MODE_READY				0x00
#define UVIS25_INT_MODE_UV_HIGH			0x01
#define UVIS25_INT_MODE_UV_LOW			0x10
#define UVIS25_INT_MODE_UV_H_L			0x11
//-------------------------------------------
#define UVIS25_INT_ACTIVE_H					0x00	//interrupt active high
#define UVIS25_INT_ACTIVE_L					0x80	//interrupt active low
//-------------------------------------------
#define UVIS25_INT_PUSH						0X00	//push pull
#define UVIS25_INT_DRAIN						0X40	//open drain
//set CTRL3 INT mode
#define UVIS25_INT		UVIS25_INT_MODE_READY | UVIS25_INT_ACTIVE_L | UVIS25_INT_PUSH
//------------- DEFINE UVIS25 INTERRUPT MODE ------------


//------------- DEFINE UVIS25 INTERRUPT CONFIG ------------
//CTRLINT_CFG set
#define UVIS25_INT_EN							0x00
#define UVIS25_INT_DIS							0x08	
//-------------------------------------------
#define UVIS25_INT_LIR							0x04	//interrupt request latched
#define UVIS25_INT_NO_LIR					0x00
//-------------------------------------------
#define UVIS25_INT_UVLE						0x02	//enable interrupt ,the UV values lower than preset threshold
#define UVIS25_INT_NO_UVLE					0x00
//-------------------------------------------
#define UVIS25_INT_UVHE						0x01	//enable interrupt ,the UV values higher than preset threshold
#define UVIS25_INT_NO_UVHE					0x00
//-------------------------------------------
//set INT CFG mode
#define  UVIS25_INT_CFG	UVIS25_INT_EN | UVIS25_INT_NO_LIR | UVIS25_INT_NO_UVLE | UVIS25_INT_NO_UVHE
//------------- DEFINE UVIS25 INTERRUPT CONFIG ------------


//------------- DEFINE UVIS25 BIT  ---------------
//REG:0x20
#define REG_CTRL1_BDU				(1<<1)
#define REG_CTRL1_ODR				(1<<0)

//REG:0x21	
#define REG_CTRL2_BOOT			(1<<7)
#define REG_CTRL2_I2SDIS			(1<<4)
#define REG_CTRL2_SPIWIRE		(1<<3)
#define REG_CTRL2_ONESHOT		(1<<0)

//REG:0x22
#define	REG_CTRL3_INT_H_L		(1<<7)
#define	REG_CTRL3_PP_OD			(1<<6)
#define	REG_CTRL3_INT_S			(0x03<<0)

//REG:0x23
#define	REG_INT_CFG_DIFF_EN		(1<<3)
#define	REG_INT_CFG_LIR  			(1<<2)
#define	REG_INT_CFG_UVLE			(1<<1)
#define	REG_INT_CFG_UVHE			(1<<0)
//------------- DEFINE UVIS25 BIT  ---------------
//-------------------- USER SET MODE USE DEFINE -----------------------


//********************************************* SYSTEM *************************************************
//--------------------------------------------------------------------------------------------------
// read the UVIS25 ID -> 0xCA
CHAR8S UVIS25_ID(CHAR8U *id);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// INITIAL UVIS25 (include set read mode , set INT & INT CFG mode  , set threshold )
CHAR8S UVIS25_INITIAL(void);
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
CHAR8S UVIS25_SET_MODE(CHAR8U mode_update_type);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// set the UVIS25 interrupt mode & reference level & INT pin EN/DIS
// set CTRL3 REG & INT_CFG REG
// **** use define UVIS25_INT call this function ****
// **** use define UVIS25_INT_CFG call this function ****
CHAR8S UVIS25_SET_INTERRUPT_MODE(CHAR8U ctrl3_data,CHAR8U int_cfg);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// set the UVIS25 UV threshold value , range : 0~15
// set REG_THU_UV
CHAR8S UVIS25_SET_THRESHOLD(CHAR8U thr_data);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// read the UVIS25 UV threshold value , range : 0~15
// set REG_THU_UV
CHAR8S UVIS25_READ_THRESHOLD(CHAR8U *thr_data);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// check the UVIS25 interrupt status
// read REG_INT_SOURCE	[0x24]
CHAR8S UVIS25_READ_INTERRUPT_STATUS(CHAR8U *int_status);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// check the UVIS25 UV data ready
// read REG_STATUS (0x27)
CHAR8S UVIS25_CHECK_DATA_READY(CHAR8U *status);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// read UV data
// read REG_UV_DATA [0x28]
// if set continue read -> read data speed must higher than 1hz update data.
// **** use define UVIS25_CTRL1_MODE ****
// ****  UVIS25_CTRL1_MODE &  0x02 , if =1[Enable continuous reading  ]  REG_CTRL2 [bit][ONESHOT] must be "0" !****
CHAR8S UVIS25_READ_UV_DATA(CHAR8U *uv_data,CHAR8U mode_update_type);
//--------------------------------------------------------------------------------------------------
//delay for UVIS25
void UVIS25_DELAY(INT32U cnt);
//--------------------------------------------------------------------------------------------------
//********************************************* SYSTEM *************************************************

#endif //#ifndef __UVIS25_HEADER__ 
