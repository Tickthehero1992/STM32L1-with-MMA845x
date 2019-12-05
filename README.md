MMA845x-Sensor library for STM32L152(probably you can change MCU), and CubeMX project for STM32L152 with test of working

MAIN Functions:
I2C_find_devices - find I2C devices.
MMA845X_init(uint16_t) - initialization of sensor
MMA845X_write(uint8_t param_addr, uint8_t param) - writing parameter to address.
MMA845X_read(uint8_t) - read parameter from address, return uint8_t
MMA845X_active() - go active state of sensor
MMA845X_standby() - standby mode, using for setting up sensor


MMA845x_begin(_Bool, uint8_t) - begining work with sensor
MMA845x_getdata() - take data of positions
MMA845X_interrupt(InterName, uint8_t) - initializing interrupts
MMA845X_reinterrupt(InterName) -  clear Latch Bit of interrupt
MMA845X_threshold_correct(InterName, uint8_t) - edit Threshold for Interrupt by name and param
MMA845X_setup_FT(uint8_t Param, Axes A, uint8_t MF, uint8_t COUNT)- setup Freefall and Motion interrupt
 MMA845X_setup_TR(uint8_t, Axes, uint8_t , _Bool)- setup transient interrupt
MMA845X_setup_PULSE(uint8_t Param, Axes A, uint8_t COUNT, uint8_t LTCY, uint8_t WIND, _Bool Double)-setup pulse interrupt

MMA845X_getstatus() - getstatus
MMA845X_disableInterrupt() - disable interrupts
 MMA845X_sendData() - make string and put string of data to USART1


 Axes :  {X,Y,Z,XY,XZ,YZ,XYZ}
 InterName:{FIFO_INT, TRANS_INT,PULSE_INT, FF_MT_INT, LDPT_INT, DRDY_INT, ASLP_INT}

 LDPT_INT- landscape interrupt, portrait interrupt
 FIFO _INT - for Fifo_interrupt
 TRANS_INT - TRANSIENT Interrupt
 PULSE_INT-PULSE Interrupt
 FF_MT_INT - freefall, motion interrupt
 DRDY_INT - dataready interrupt
ASLP_INT- asleep interrupt

For using interrupt u Should  :
1) disable all interrupts
2) setup interrupt (now u can use only 3 interrupts, which u can edit TRANS_INT, PULSE_INT,  FF_MT_INT. DRDY_INT
is working with default setups, as a LDPT_INT)
3) initialize interrupt by MMA845X_interrupt()
4) when interrupt is catched u sould use MMA845X_reinterrupt() 
