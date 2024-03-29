#ifndef MMA845X_H
#define MMA845X_H

#include "stm32l1xx_hal.h"
#define MMA_845XQ_DEFAULT_ADDRESS 0x0038//(0x1C<<1)


////////////////////////////////////////////
// Interrupts

/*
// Auto SLEEP/WAKE interrupt
#define INT_ASLP   (1<<7)
// Transient interrupt
#define INT_TRANS  (1<<5)
// Orientation (landscape/portrait) interrupt
#define INT_LNDPRT (1<<4)
// Pulse detection interrupt
#define INT_PULSE  (1<<3)
// Freefall/Motion interrupt
#define INT_FF_MT  (1<<2)
// Data ready interrupt
#define INT_DRDY   (1<<0)
*/

/*Interrupt setups*/
#define A_setup 0xF8 //f8
#define B_setup 0x0D //1f
#define C_setup 0x20 //20
#define D_setup 0x10 //10
#define E_setup 0xFF //ff
/*Interrupt registers*/
/*reg 2C*/
#define PP_OD 0x01
#define IPOL 0x02
#define FF_MT 0x08
#define PULSE 0x10
#define LDPRT 0x20
#define TRANS 0x40
#define FIFO_int 0x80
/*rec 2D*/
#define EN_ASLP 0x80
#define EN_FIFO 0x40
#define EN_TRANS 0x20
#define EN_LDPRT 0x10
#define EN_PULSE 0x08
#define EN_FF_MT 0x04
#define EN_DRDY 0x01
/*rec 2E on pin 1*/
#define INT_ASLP 0x80
#define INT_FIFO 0x40
#define INT_TRANS 0x20
#define INT_LDPRT 0x10
#define INT_PULSE 0x08
#define INT_FF_MT 0x04
#define INT_DRDY 0x01

#define MMA845X_PL_THS 0x14

#define MMA_845XQ_CTRL_REG1 0x2A
#define MMA_845XQ_CTRL_REG1_VALUE_ACTIVE 0x01
#define MMA_845XQ_CTRL_REG1_VALUE_F_READ 0x02

#define MMA_845XQ_CTRL_REG2 0x2B
#define MMA_845XQ_CTRL_REG2_RESET 0x40

#define MMA_845XQ_PL_STATUS 0x10
#define MMA_845XQ_PL_CFG 0x11

#define MMA_845XQ_PL_EN 0x40

#define MMA_845XQ_XYZ_DATA_CFG 0x0E
#define MMA_845XQ_2G_MODE 0x00 //Set Sensitivity to 2g
#define MMA_845XQ_4G_MODE 0x01 //Set Sensitivity to 4g
#define MMA_845XQ_8G_MODE 0x02 //Set Sensitivity to 8g

/*initialize FT_MT*/
#define MMA_845XQ_FF_MT_CFG_ADD 0x15
#define MMA845XQ_FF_MT_SRC 0x16
#define MMA845X_FF_MT_THS 0x17
#define MMA845XQ_FF_MT_CNT 0x18
/*registers to setup*/
#define MMA_845XQ_FF_MT_FALL_MOTION 0x40
#define MMA_845XQ_FF_MT_Z 0x20
#define MMA_845XQ_FF_MT_Y 0x10
#define MMA_845XQ_FF_MT_X 0x08
#define MMA_845XQ_FF_MT_CFG_ELE 0x80

/*initialize TR*/
#define MMA845XQ_TR_CFG 0x1D
#define MMA845XQ_TR_SRC 0x1E
#define MMA845XQ_TR_THS 0x1F
#define MMA845XQ_TR_CNT 0x20
/*setups*/
#define MMA845XQ_TR_ELE 0x10
#define MMA845XQ_TR_Z 0x08
#define MMA845XQ_TR_Y 0x04
#define MMA845XQ_TR_X 0x02
#define MMA845XQ_FF_MT_SRC_EA 0x80

/*Pulse initialization*/
#define MMA_845XQ_PULSE_CFG 0x21
#define MMA_845XQ_PULSE_SRC 0x22
#define MMA845XQ_PULSE_THSX 0x23
#define MMA845XQ_PULSE_THSY 0x24
#define MMA845XQ_PULSE_THSZ 0x25
#define MMA845XQ_PULSE_TMLT 0x26
#define MMA845XQ_PULSE_LTCY 0x27
#define MMA845XQ_PULSE_WIND 0x28 //second time window
/*setups*/
#define MMA845XQ_PULSE_DP 0x80
#define MMA845XQ_PULSE_ELE 0x40
#define MMA845XQ_PULSE_ZD 0x20
#define MMA845XQ_PULSE_ZO 0x10
#define MMA845XQ_PULSE_YD 0x08
#define MMA845XQ_PULSE_YO 0x04
#define MMA845XQ_PULSE_XD 0x02
#define MMA845XQ_PULSE_XO 0x01


#define MMA_845XQ_PULSE_SRC_EA 0x80

typedef enum Interrupt_name
{
	FIFO_INT,
	TRANS_INT,
	LDPT_INT,
	PULSE_INT,
	FF_MT_INT,
	DRDY_INT,
	ASLP_INT
} InterName;


typedef enum
{
	X=0,
	Y=1,
	Z=2,
	XY=3,
	XZ=4,
	YZ=5,
	XYZ=6
} Axes;

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;
extern uint16_t slave_address;
extern uint8_t scale;
extern _Bool _highres;
extern float step_factor;
extern float xg;
extern float yg;
extern float zg;

void I2C_find_devices();
void MMA845X_init(uint16_t);
uint8_t MMA845X_write(uint8_t param_addr, uint8_t param);
uint8_t MMA845X_read(uint8_t);
void MMA845X_active();
void MMA845X_standby();


uint8_t MMA845x_begin(_Bool, uint8_t);
void MMA845x_getdata();
uint8_t MMA845X_interrupt(InterName, uint8_t);
uint8_t MMA845X_reinterrupt(InterName);
uint8_t MMA845X_getstatus();
uint8_t MMA845X_disableInterrupt();
uint8_t MMA845X_threshold_correct(InterName, uint8_t);
void MMA845X_setup_FT(uint8_t Param, Axes A, uint8_t MF, uint8_t COUNT);
void MMA845X_setup_TR(uint8_t, Axes, uint8_t , _Bool);
void MMA845X_setup_PULSE(uint8_t Param, Axes A, uint8_t COUNT, uint8_t LTCY, uint8_t WIND, _Bool Double);
float MMA845X_getX();
float MMA845X_getY();
float MMA845X_getZ();
void MMA845X_sendData();




#endif
