#ifndef PMIC_INTERFACE_H_
#define PMIC_INTERFACE_H_
/*===========================================================================*/
/**
 * @file PMIC_INTERFACE.h
 *
 * Function definitions for the PMIC interface module.
 *
 */
/*==========================================================================*/

/*===========================================================================*
 * Header Files
 *===========================================================================*/
#include <stdint.h>
#include <stdbool.h>

#include "standard.h"

#   ifdef __cplusplus
extern "C"
{ /* ! Inclusion of header files should NOT be inside the extern "C" block */
#   endif /* __cplusplus */
/*===========================================================================*
 * Exported Preprocessor #define Constants
 *===========================================================================*/

/*===========================================================================*
 * Exported Preprocessor #define MACROS
 *===========================================================================*/

/*===========================================================================*
 * Exported Type Declarations
 *===========================================================================*/
typedef enum PMIC_states_tag
{
    PMIC_CS_INIT,
    PMIC_CS_OFF,
    PMIC_CS_ON,
    PMIC_CS_IDLE,
    PMIC_CS_MFG,
}PMIC_states_t;

typedef enum PMIC_PowerUp_Condition_tag
{
    POWERDOWN = 0,
    POWERUP_CONDITION_POWERBTN = 1,
    POWERUP_CONDITION_IGNITION = 2,
    POWERUP_CONDITION_CAN = 3,
    POWERUP_CONDITION_MFG = 4,
    POWERDOWN_BY_RESET = 5,
}PMIC_PowerUp_Condition_t;


/*===========================================================================*
 * Exported Function Prototypes
 *===========================================================================*/
void PMIC_Init (void);
void PMIC_Get_State (uint8_t *state);
void PMIC_Get_PowerUp_Condition (uint8_t *value);
void PMIC_Get_CAN_Activity (uint8_t *activity);
void PMIC_KeepAlive (bool_t enable);
void PMIC_Shutdown (uint8_t ForceShutdown_timeout);
void PMIC_Check_MfgConditions (void);
void PMIC_Controller (void);

#   ifdef __cplusplus
}         /* extern "C" */
#   endif /* __cplusplus */

#endif /* PMIC_INTERFACE_H_ */
