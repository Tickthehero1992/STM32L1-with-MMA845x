#include "mma845x.h"

uint16_t slave_address =MMA_845XQ_DEFAULT_ADDRESS;
I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart1;
uint8_t scale;
_Bool _highres;
float step_factor;
float xg;
float yg;
float zg;
/*
typedef enum Interrupt_name
{
	FIFO_INT,
	TRANS_INT,
	LDPT_INT,
	PULSE_INT,
	FF_MT_INT,
	DRDY_INT,
	ASLP_INT
} InterName;*/


void I2C_find_devices()
{
	  uint8_t i = 0;
	  char exit[2];
	  for(i = 15; i<127; i++)
	  {
	      if(HAL_I2C_IsDeviceReady(&hi2c1, i<<1, 10, 100) == HAL_OK)
	      {
	    	 exit[0]=i/10+48;
	    	 exit[1]=i%10+48;
	    	 HAL_UART_Transmit(&huart1, (uint8_t*)&exit[0],1,100);
	    	 HAL_UART_Transmit(&huart1, (uint8_t*)&exit[1],1,100);
	    	 HAL_UART_Transmit(&huart1, (uint8_t*)&"\r\n",2,100);
	    	 //break;
	      }
	  }
	  HAL_UART_Transmit(&huart1, (uint8_t*)&"It all that i find\r\nFor using use address<<1\r\n",46,1000);
}
void MMA845X_init(uint16_t add)
{
	if(add) slave_address=add;
	else slave_address=0x38;
}

uint8_t MMA845X_write(uint8_t param_addr, uint8_t param)
{
	uint8_t message[2]= {param_addr,param};
	if(HAL_I2C_Master_Transmit(&hi2c1,(uint16_t)slave_address,message, 2, 100)!=HAL_OK)
		{
		HAL_UART_Transmit(&huart1, (uint8_t*)"Error of  write\r\n",17,1000);
		return 1;
		}
	 return 0;
}

uint8_t MMA845X_read(uint8_t param_addr)
{
	uint8_t Data=0;/*
	if(HAL_I2C_Master_Transmit(&hi2c1,(uint16_t)slave_address,(uint8_t *)&param_addr, 1, 100)==HAL_OK)
	{
		if(HAL_I2C_IsDeviceReady(&hi2c1,(uint16_t)slave_address,1,100)==HAL_OK)
		{
			if(HAL_I2C_Master_Receive(&hi2c1, (uint16_t)(slave_address+0x01),(uint8_t *)&Data, 1, 100)!=HAL_OK)HAL_UART_Transmit(&huart1, (uint8_t*)"Error of read\r\n",17,1000);
		}
		else HAL_UART_Transmit(&huart1, (uint8_t*)"Error of state\r\n",17,1000);
	}
	else HAL_UART_Transmit(&huart1, (uint8_t*)"Error of  write read\r\n",22,1000);*/
	if(HAL_I2C_Mem_Read(&hi2c1, (uint16_t)(slave_address+0x01),(uint16_t)param_addr, 1,(uint8_t *)&Data, 1, 100)!=HAL_OK)HAL_UART_Transmit(&huart1, (uint8_t*)"Error of read\r\n",17,1000);
	return Data;
}

void MMA845X_active()
{
	uint8_t Data;
	if(HAL_I2C_Master_Transmit(&hi2c1,(uint16_t)slave_address,MMA_845XQ_CTRL_REG1, 1, 1000)==HAL_OK)
		{
			if(HAL_I2C_IsDeviceReady(&hi2c1,(uint16_t)slave_address,1,1000)==HAL_OK)
			{
				if(HAL_I2C_Master_Receive(&hi2c1, (uint16_t)slave_address,(uint8_t *)&Data, 1, 1000)!=HAL_OK)HAL_UART_Transmit(&huart1, (uint8_t*)"Error of read\r\n",17,1000);;
			}
			else HAL_UART_Transmit(&huart1, (uint8_t*)"Error of state\r\n",17,1000);
		}
		else HAL_UART_Transmit(&huart1, (uint8_t*)"Error of  write\r\n",17,1000);
	uint8_t _highres = 1;
	MMA845X_write(MMA_845XQ_CTRL_REG1,Data|MMA_845XQ_CTRL_REG1_VALUE_ACTIVE|(_highres ? 0 : MMA_845XQ_CTRL_REG1_VALUE_F_READ) | 0x38);

}

void MMA845X_standby()
{
	uint8_t Data;
	if(HAL_I2C_Master_Transmit(&hi2c1,(uint16_t)slave_address,MMA_845XQ_CTRL_REG1, 1, 1000)==HAL_OK)
			{
				if(HAL_I2C_IsDeviceReady(&hi2c1,(uint16_t)slave_address,1,1000)==HAL_OK)
				{
					if(HAL_I2C_Master_Receive(&hi2c1, (uint16_t)slave_address,(uint8_t *)&Data, 1, 1000)!=HAL_OK)HAL_UART_Transmit(&huart1, (uint8_t*)"Error of read\r\n",17,1000);;
				}
				else HAL_UART_Transmit(&huart1, (uint8_t*)"Error of state\r\n",17,1000);
			}
			else HAL_UART_Transmit(&huart1, (uint8_t*)"Error of  write\r\n",17,1000);
		MMA845X_write(MMA_845XQ_CTRL_REG1,Data & ~MMA_845XQ_CTRL_REG1_VALUE_ACTIVE);
}


uint8_t MMA845x_begin(_Bool k, uint8_t a)
{
 uint8_t error_Init=0;
 scale=a;
 _highres=k;
 step_factor = (_highres ? 0.0039 : 0.0156);
 if(scale==4) step_factor*=2;
 else if (scale==8) step_factor*=4;
 MMA845X_read(0x0D);
 error_Init+=MMA845X_write(MMA_845XQ_CTRL_REG2,MMA_845XQ_CTRL_REG2_RESET);
 HAL_Delay(10);
 error_Init+=MMA845X_write(MMA_845XQ_PL_CFG,0x80 | MMA_845XQ_PL_EN);
 if(scale==4 || scale==8)
	 error_Init+=MMA845X_write(MMA_845XQ_XYZ_DATA_CFG,(scale==4)?MMA_845XQ_4G_MODE:MMA_845XQ_8G_MODE);
 else
	 error_Init+=MMA845X_write(MMA_845XQ_XYZ_DATA_CFG,(uint8_t)MMA_845XQ_2G_MODE);
 if (error_Init) return 1;
 MMA845X_active();
 return 0;
}

void MMA845x_getdata()
{
	uint8_t status;
	int16_t rx,ry,rz;
	rx=ry=rz=0;
	xg=yg=zg=0;
	uint8_t buffer[7];
	if(HAL_I2C_Master_Transmit(&hi2c1,(uint16_t)slave_address,0x00, 1, 100)==HAL_OK)
			{
				if(HAL_I2C_IsDeviceReady(&hi2c1,(uint16_t)slave_address,1,100)==HAL_OK)
				 {
					//if(HAL_I2C_Master_Transmit(&hi2c1,(uint16_t)slave_address,(uint8_t)(_highres ? 7 : 4), 1, 100)==HAL_OK)
					//	{}
						//if(HAL_I2C_Master_Receive(&hi2c1, (uint16_t)slave_address,(uint8_t *)&status, 1, 1000)!=HAL_OK)HAL_UART_Transmit(&huart1, (uint8_t*)"Error of read\r\n",17,100);
								//			else
										//	{

												if(_highres)
												{
													HAL_I2C_Master_Receive(&hi2c1, (uint16_t)slave_address,(uint8_t *)&buffer, (uint8_t)(_highres ? 7 : 4), 100);
													status = buffer[0];
													rx=(buffer[1]<<8|buffer[2]);
													ry=(buffer[3]<<8|buffer[4]);
													rz=(buffer[5]<<8|buffer[6]);
													xg=(rx/64)*step_factor;
													yg=(ry/64)*step_factor;
													zg=(rz/64)*step_factor;

												}
												else
												{
													HAL_I2C_Master_Receive(&hi2c1, (uint16_t)slave_address,(uint8_t *)&buffer[0], 1, 100);
													HAL_I2C_Master_Receive(&hi2c1, (uint16_t)slave_address,(uint8_t *)&buffer[0], 1, 100);
													HAL_I2C_Master_Receive(&hi2c1, (uint16_t)slave_address,(uint8_t *)&buffer[0], 1, 100);
													xg=(float)buffer[0]*step_factor;
													yg=(float)buffer[0]*step_factor;
													zg=(float)buffer[0]*step_factor;
													//atan2(xg,yg);
												}
											//}

					//else {HAL_UART_Transmit(&huart1, (uint8_t*)"Error of take bytes\r\n",25,100);}

				 }
				else HAL_UART_Transmit(&huart1, (uint8_t*)"Error of state\r\n",17,1000);
			}
	else HAL_UART_Transmit(&huart1, (uint8_t*)"Error of  write\r\n",17,1000);
}

uint8_t MMA845X_interrupt(InterName A, uint8_t pin)
{
uint8_t to_C, to_D, to_E;
to_C=0;
to_D=0;
to_E=0;
switch (A)
{
	case FIFO_INT:
		to_C|=FIFO_int;
		to_D|=EN_FIFO;
		if(pin==1)
			to_E|=INT_FIFO;
		break;
	case TRANS_INT:
		to_C|=TRANS;
		to_D|=EN_TRANS;
		if(pin==1)
			to_E|=INT_TRANS;
		break;
	case LDPT_INT:
		to_C|=LDPRT;
		to_D|=EN_LDPRT;
		if(pin==1)
			to_E|=INT_LDPRT;
		break;
	case PULSE_INT:
		to_C|=PULSE;
		to_D|=EN_PULSE;
		if(pin==1)
			to_E|=INT_PULSE;
		break;
	case FF_MT_INT:
		to_C|=FF_MT;
		to_D|=EN_FF_MT;
		if(pin==1)
			to_E|=INT_FF_MT;
		break;
	case DRDY_INT:
		to_D|=EN_DRDY;
		if(pin==1)
			to_E|=INT_DRDY;
		break;
	case ASLP_INT:
		to_D|=EN_ASLP ;
		if(pin==1)
			to_E|=INT_ASLP;
		break;
}
//to_C|=0x03;
MMA845X_write(0x2A, A_setup);
MMA845X_write(0x2B, B_setup);

/*
MMA845X_write(0x2C, C_setup); //interrupt
MMA845X_write(0x2D, D_setup);// interrupt
MMA845X_write(0x2E, E_setup);//interrupt configuration pins
*/

MMA845X_write(0x2C,to_C);
MMA845X_write(0x2D,to_D);
MMA845X_write(0x2E,to_E);

MMA845X_write(0x2A, A_setup|0x01);
//MMA845X_standby();//?
return 0;
}

uint8_t MMA845X_disableInterrupt()
{
  MMA845X_write(0x2D,0x00);
  return 0;
}


uint8_t MMA845X_threshold_correct(InterName A, uint8_t Param)
{

	switch (A)
	{
		case TRANS_INT:
			MMA845X_write(MMA845XQ_TR_THS ,Param);
			break;
		case PULSE_INT:
			MMA845X_write(MMA845XQ_PULSE_THSX,Param);
			MMA845X_write(MMA845XQ_PULSE_THSY,Param);
			MMA845X_write(MMA845XQ_PULSE_THSZ,Param);
			break;
		case FF_MT_INT:
			MMA845X_write(MMA845X_FF_MT_THS,Param);
			break;
	}
	return 0;
}

void MMA845X_setup_FT(uint8_t Param, Axes A, uint8_t MF, uint8_t COUNT) // MF==1 motion
{
	uint8_t CFG=0;
	switch(A)
	{
	case X:
		CFG=MMA_845XQ_FF_MT_X;
		break;
	case Y:
			CFG=MMA_845XQ_FF_MT_Y;
			break;
	case Z:
			CFG=MMA_845XQ_FF_MT_Z;
			break;
	case XY:
				CFG=MMA_845XQ_FF_MT_X+MMA_845XQ_FF_MT_Y;
				break;
	case YZ:
				CFG=MMA_845XQ_FF_MT_Y+MMA_845XQ_FF_MT_Z;
				break;
	case XZ:
				CFG=MMA_845XQ_FF_MT_X+MMA_845XQ_FF_MT_Z;
				break;
	case XYZ:
				CFG=MMA_845XQ_FF_MT_X+MMA_845XQ_FF_MT_Z+MMA_845XQ_FF_MT_Y;
				break;

	}
	if(MF==1) CFG=CFG+MMA_845XQ_FF_MT_FALL_MOTION;
	CFG=MMA_845XQ_FF_MT_CFG_ELE+CFG;
	MMA845X_write(0x2A,A_setup);
	MMA845X_write(MMA_845XQ_FF_MT_CFG_ADD,CFG);
	MMA845X_threshold_correct(FF_MT_INT, Param);
	MMA845X_write(MMA845XQ_FF_MT_CNT,COUNT);

}

void MMA845X_setup_TR(uint8_t Param, Axes A,  uint8_t COUNT, _Bool Filter)
{
	uint8_t CFG=0;
	if (Filter) CFG+=1;
	switch(A)
	{
	case X:
		CFG=MMA845XQ_TR_X;
		break;
	case Y:
			CFG=MMA845XQ_TR_Y;
			break;
	case Z:
			CFG=MMA845XQ_TR_Z;
			break;
	case XY:
				CFG=MMA845XQ_TR_X+MMA845XQ_TR_Y;
				break;
	case YZ:
				CFG=MMA845XQ_TR_Y+MMA845XQ_TR_Z;
				break;
	case XZ:
				CFG=MMA845XQ_TR_X+MMA845XQ_TR_Z;
				break;
	case XYZ:
				CFG=MMA845XQ_TR_Z+MMA845XQ_TR_Y+MMA845XQ_TR_X;
				break;

	}
	CFG=CFG+MMA845XQ_TR_ELE;
	MMA845X_write(0x2A,A_setup);
	MMA845X_write(MMA845XQ_TR_CFG ,CFG);
	MMA845X_threshold_correct(TRANS_INT, Param);
	MMA845X_write(MMA845XQ_TR_CNT,COUNT);
}


void MMA845X_setup_PULSE(uint8_t Param, Axes A, uint8_t COUNT, uint8_t LTCY, uint8_t WIND, _Bool Double)
{
	uint8_t CFG=0;
	if(Double) CFG+=MMA845XQ_PULSE_DP+MMA845XQ_PULSE_ELE;
	switch(A)
	{
	case X:
		if(Double) CFG+=MMA845XQ_PULSE_XD;
		else CFG+=MMA845XQ_PULSE_XO;
		break;
	case Y:
		if(Double) CFG+=MMA845XQ_PULSE_YD;
		else CFG+=MMA845XQ_PULSE_YO;
		break;
	case Z:
		if(Double) CFG+=MMA845XQ_PULSE_ZD;
		else CFG+=MMA845XQ_PULSE_ZO;
				break;
	case XY:
		if(Double) CFG+=MMA845XQ_PULSE_XD+MMA845XQ_PULSE_YD;
		else CFG+=MMA845XQ_PULSE_XO+MMA845XQ_PULSE_YO;
				break;

	case YZ:
		if(Double) CFG+=MMA845XQ_PULSE_ZD+MMA845XQ_PULSE_YD;
		else CFG+=MMA845XQ_PULSE_ZO+MMA845XQ_PULSE_YO;
				break;
	case XZ:
		if(Double) CFG+=MMA845XQ_PULSE_XD+MMA845XQ_PULSE_ZD;
		else CFG+=MMA845XQ_PULSE_XO+MMA845XQ_PULSE_ZO;
				break;
	case XYZ:
		if(Double) CFG+=MMA845XQ_PULSE_XD+MMA845XQ_PULSE_YD+MMA845XQ_PULSE_ZD;
		else CFG+=MMA845XQ_PULSE_XO+MMA845XQ_PULSE_YO+MMA845XQ_PULSE_ZO;
				break;

	}
	MMA845X_write(0x2A,A_setup);
	MMA845X_write(MMA_845XQ_PULSE_CFG ,CFG);
	MMA845X_threshold_correct(PULSE_INT, Param);
	MMA845X_write(MMA845XQ_PULSE_LTCY,LTCY);
	MMA845X_write(MMA845XQ_PULSE_WIND,WIND);
	MMA845X_write(MMA845XQ_PULSE_TMLT ,COUNT);
}



uint8_t MMA845X_reinterrupt(InterName A)
{
	switch (A)
	{
		case TRANS_INT:
			MMA845X_read(MMA845XQ_TR_THS);
			break;
		case PULSE_INT:
			MMA845X_read(MMA_845XQ_PULSE_SRC);
			break;
		case FF_MT_INT:
			MMA845X_read(MMA845XQ_FF_MT_SRC);
			break;
	}
	return 0;
}

void MMA845X_sendData()
{
	char hui[27];
	sprintf(hui, "%f%c%f%c%f%c",xg,';',yg,';',zg,';');
	HAL_UART_Transmit(&huart1,(uint8_t*)&hui,27,1000);
	HAL_UART_Transmit(&huart1,(uint8_t*)&"\r\n",2,1000);
	xg=0;
	yg=0;
	zg=0;
}
