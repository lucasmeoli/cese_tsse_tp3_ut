#ifndef INPUTS_H_
#define INPUTS_H_
/*===========================================================================*/
/**
 * @file INPUTS.h
 *
 * Function definitions for the SEND CODE module.
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
typedef enum input_type_tag
{
    IN_Ignition,
    IN_PowerBtn,

    IN_NUM
}input_type_t;

typedef enum input_value_tag
{
    INPUT_INACTIVE,
    INPUT_ACTIVE
}input_value_t;

/*===========================================================================*
 * Exported Preprocessor #define MACROS
 *===========================================================================*/

/*===========================================================================*
 * Exported Type Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Function Prototypes
 *===========================================================================*/
input_value_t INPUTS_Get_Value (input_type_t input);
void INPUTS_Detection (void);
void INPUTS_Init (void);

#   ifdef __cplusplus
}         /* extern "C" */
#   endif /* __cplusplus */

#endif /* INPUTS_H_ */
