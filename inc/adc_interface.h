#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_
/*===========================================================================*/
/**
 * @file ADC_INTERFACE.h
 *
 * Function definitions for the ADC interface module.
 *
 */
/*==========================================================================*/

/*===========================================================================*
 * Header Files
 *===========================================================================*/
#include "standard.h"

#   ifdef __cplusplus
extern "C"
{ /* ! Inclusion of header files should NOT be inside the extern "C" block */
#   endif /* __cplusplus */
/*===========================================================================*
 * Exported Preprocessor #define Constants
 *===========================================================================*/
/* 1 - the channel will be enabled */
/* 0 - the channle will be disabled */
                           /* A0 A1 A2 A3 A4 A5 A6 A7 */
#define ADC_CHANNELS_TO_READ {1, 1, 0, 0, 1, 1, 1, 1}

/*===========================================================================*
 * Exported Preprocessor #define MACROS
 *===========================================================================*/

/*===========================================================================*
 * Exported Type Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Function Prototypes
 *===========================================================================*/
void ADC_Init (void);
void ADC_Get_Values (uint8_t* channels_val);
void ADC_Get_Value (uint8_t channel, uint8_t* channels_val);
void ADC_Start (void);
void ADC_Stop (void);

#   ifdef __cplusplus
}         /* extern "C" */
#   endif /* __cplusplus */

#endif /* ADC_INTERFACE_H_ */
