#ifndef HARDWARE_CONFIG_H_
#define HARDWARE_CONFIG_H_
/*===========================================================================*/
/**
 * @file HARDWARE_CONFIG_H_
 *
 * Hardware configuration
 *
 */
/*==========================================================================*/

/*===========================================================================*
 * Header Files
 *===========================================================================*/

#   ifdef __cplusplus
extern "C"
{ /* ! Inclusion of header files should NOT be inside the extern "C" block */
#   endif /* __cplusplus */
/*===========================================================================*
 * Exported Preprocessor #define Constants
 *===========================================================================*/

#define PORT_I2C_SDA                GPIO_PORT_P1, GPIO_PIN2
#define PORT_I2C_SCL                GPIO_PORT_P1, GPIO_PIN3

#define PORT_CANRx                  GPIO_PORT_P2, GPIO_PIN3
#define PORT_PwrBtn                 GPIO_PORT_P2, GPIO_PIN5
#define PORT_Ignition               GPIO_PORT_P2, GPIO_PIN6
#define PORT_PwrOn                  GPIO_PORT_P2, GPIO_PIN7

//#define PORT_TEST                   GPIO_PORT_P1, GPIO_PIN6

#define PORT_XOUT                   GPIO_PORT_P2, GPIO_PIN0
#define PORT_XIN                    GPIO_PORT_P2, GPIO_PIN1
#define PORT_ACLK                   GPIO_PORT_P2, GPIO_PIN2

#define PORT_ADC0                   GPIO_PORT_P1, GPIO_PIN0
#define PORT_ADC1                   GPIO_PORT_P1, GPIO_PIN1
#define PORT_ADC4                   GPIO_PORT_P1, GPIO_PIN4
#define PORT_ADC5                   GPIO_PORT_P1, GPIO_PIN5
#define PORT_ADC6                   GPIO_PORT_P1, GPIO_PIN6
#define PORT_ADC7                   GPIO_PORT_P1, GPIO_PIN7

#define PORT_MFG0                   GPIO_PORT_P1, GPIO_PIN7
#define PORT_MFG1                   GPIO_PORT_P1, GPIO_PIN6

/*===========================================================================*
 * Exported Preprocessor #define MACROS
 *===========================================================================*/

/*===========================================================================*
 * Exported Type Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Function Prototypes
 *===========================================================================*/


#   ifdef __cplusplus
}         /* extern "C" */
#   endif /* __cplusplus */

#endif /* HARDWARE_CONFIG_H_ */
