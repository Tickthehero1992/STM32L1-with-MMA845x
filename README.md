MMA845x-Sensor library for STM32L152(probably you can change MCU), and CubeMX project for STM32L152 with test of working

MAIN Functions:
I2C_find_devices - find I2C devices.
MMA845X_init(uint16_t) - initialization of sensor
MMA845X_write(uint8_t param_addr, uint8_t param) - writing parameter to address.
MMA845X_read(uint8_t) - read parameter from address
MMA845X_active() - go active state of sensor
MMA845X_standby() - standby mode, using for setting up sensor


MMA845x_begin(_Bool, uint8_t) - begining work with sensor
MMA845x_getdata() - take data of positions
MMA845X_interrupt(InterName, uint8_t) - initializing interrupts
MMA845X_getstatus() - getstatus
MMA845X_disableInterrupt() - disable interrupts
 MMA845X_sendData() - make string and put string of data to USART1