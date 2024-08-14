#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

// $[CMU]
// [CMU]$

// $[PRS.ASYNCH0]
// [PRS.ASYNCH0]$

// $[PRS.ASYNCH1]
// [PRS.ASYNCH1]$

// $[PRS.ASYNCH2]
// [PRS.ASYNCH2]$

// $[PRS.ASYNCH3]
// [PRS.ASYNCH3]$

// $[PRS.ASYNCH4]
// [PRS.ASYNCH4]$

// $[PRS.ASYNCH5]
// [PRS.ASYNCH5]$

// $[PRS.ASYNCH6]
// [PRS.ASYNCH6]$

// $[PRS.ASYNCH7]
// [PRS.ASYNCH7]$

// $[PRS.ASYNCH8]
// [PRS.ASYNCH8]$

// $[PRS.ASYNCH9]
// [PRS.ASYNCH9]$

// $[PRS.ASYNCH10]
// [PRS.ASYNCH10]$

// $[PRS.ASYNCH11]
// [PRS.ASYNCH11]$

// $[PRS.SYNCH0]
// [PRS.SYNCH0]$

// $[PRS.SYNCH1]
// [PRS.SYNCH1]$

// $[PRS.SYNCH2]
// [PRS.SYNCH2]$

// $[PRS.SYNCH3]
// [PRS.SYNCH3]$

// $[GPIO]
// [GPIO]$

// $[TIMER0]
// [TIMER0]$

// $[TIMER1]
// [TIMER1]$

// $[TIMER2]
// [TIMER2]$

// $[TIMER3]
// [TIMER3]$

// $[TIMER4]
// [TIMER4]$

// $[USART0]
// [USART0]$

// $[USART1]
// USART1 RX on PA00
#ifndef USART1_RX_PORT                          
#define USART1_RX_PORT                           gpioPortA
#endif
#ifndef USART1_RX_PIN                           
#define USART1_RX_PIN                            0
#endif

// USART1 TX on PA03
#ifndef USART1_TX_PORT                          
#define USART1_TX_PORT                           gpioPortA
#endif
#ifndef USART1_TX_PIN                           
#define USART1_TX_PIN                            3
#endif

// [USART1]$

// $[I2C1]
// I2C1 SCL on PC05
#ifndef I2C1_SCL_PORT                           
#define I2C1_SCL_PORT                            gpioPortC
#endif
#ifndef I2C1_SCL_PIN                            
#define I2C1_SCL_PIN                             5
#endif

// I2C1 SDA on PC06
#ifndef I2C1_SDA_PORT                           
#define I2C1_SDA_PORT                            gpioPortC
#endif
#ifndef I2C1_SDA_PIN                            
#define I2C1_SDA_PIN                             6
#endif

// [I2C1]$

// $[PDM]
// [PDM]$

// $[LETIMER0]
// [LETIMER0]$

// $[IADC0]
// [IADC0]$

// $[I2C0]
// I2C0 SCL on PB03
#ifndef I2C0_SCL_PORT                           
#define I2C0_SCL_PORT                            gpioPortB
#endif
#ifndef I2C0_SCL_PIN                            
#define I2C0_SCL_PIN                             3
#endif

// I2C0 SDA on PB02
#ifndef I2C0_SDA_PORT                           
#define I2C0_SDA_PORT                            gpioPortB
#endif
#ifndef I2C0_SDA_PIN                            
#define I2C0_SDA_PIN                             2
#endif

// [I2C0]$

// $[EUART0]
// [EUART0]$

// $[PTI]
// [PTI]$

// $[MODEM]
// [MODEM]$

// $[CUSTOM_PIN_NAME]
#ifndef _PORT                                   
#define _PORT                                    gpioPortA
#endif
#ifndef _PIN                                    
#define _PIN                                     0
#endif

// [CUSTOM_PIN_NAME]$

#endif // PIN_CONFIG_H

