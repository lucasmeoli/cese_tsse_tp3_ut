/*===========================================================================*/
/**
 * @file pmic_interface.c
 *
 * PMIC controller interface
 *
/*==========================================================================*/

/*===========================================================================*
* Header Files
*===========================================================================*/
#include "pmic_interface.h"
#include "adc_interface.h"
#include "can.h"
#include "inputs.h"

/*===========================================================================*
 * Local Preprocessor #define Constants
 *===========================================================================*/
/* This value was changed from 500 to 4 to simplify unit testing */
#define KEEP_ALIVE_TIMEOUT                 4      /* 500 * 10ms = 5sec */

/* This value was changed from 800 to 4 to simplify unit testing */
#define FORCE_SHUTDOWN_TIMEOUT             4        /* 800 * 10ms = 8sec */

/*===========================================================================*
 * Local Preprocessor #define MACROS
 *===========================================================================*/
/* PowerOn */
#define OUT_Set_PowerOn()                   GPIO_setOutputHighOnPin(PORT_PwrOn);
#define OUT_Clr_PowerOn()                   GPIO_setOutputLowOnPin(PORT_PwrOn);

/*===========================================================================*
 * Local Type Declarations
 *===========================================================================*/

/*===========================================================================*
 * Local Object Definitions
 *===========================================================================*/

bool_t ForceShutdown;
bool_t ForceReset;
bool_t ReqShutdown;
bool_t ReqAutoStartup;
bool_t EnableKeepAlive;
uint32_t ForceShutdown_timeout;
uint32_t KeepAlive_timeout;
uint32_t ReqShutdown_timeout;

PMIC_states_t PMIC_state;

PMIC_PowerUp_Condition_t PMIC_PowerUp_Condition;

CAN_activity_t can_active;

/*===========================================================================*
 * Local Function Prototypes
 *===========================================================================*/
void pmic_start_activities (void);
void pmic_stop_activities (void);

/*===========================================================================*
 * Local Inline Function Definitions and Function-Like Macros
 *===========================================================================*/

/*===========================================================================*
 * Function Definitions
 *===========================================================================*/
/***************************************************************************//**
*
* @fn         PMIC_Get_PowerUp_Condition
*
* @brief      Return the PowerUp condition
*
* @param [in] value
*
* @return     void
*
******************************************************************************/
void PMIC_Get_PowerUp_Condition (uint8_t *value)
{
    *value = (uint8_t)PMIC_PowerUp_Condition;
}

/***************************************************************************//**
*
* @fn         PMIC_Get_Inputs
*
* @brief      Return the validated state of the inputs
*
* @param [in] activity
*
* @return     void
*
******************************************************************************/
void PMIC_Get_CAN_Activity (uint8_t *activity)
{
    *activity = (uint8_t)can_active;
}

 /***************************************************************************//**
*
* @fn         PMIC_KeepAlive
*
* @brief      Update the KeepAlive (like a watchdog timeout)
*
* @param [in] void
*
* @return     void
*
******************************************************************************/
void PMIC_KeepAlive (bool_t enable)
{
    /* Restart the KeepAlive timer */
    KeepAlive_timeout = KEEP_ALIVE_TIMEOUT;
    EnableKeepAlive = enable;
}

/***************************************************************************//**
*
* @fn         PMIC_Shutdown
*
* @brief      Force the KeepAlive_timeout to elapse
*
* @param [in] void
*
* @return     void
*
******************************************************************************/
void PMIC_Shutdown (uint8_t reqShutdown_timeout)
{
    if(0 == reqShutdown_timeout)
    {
        ReqShutdown = true;
    }
    else
    {
        CAN_Prepare();
        ReqShutdown_timeout = reqShutdown_timeout * 100; /* t*100 = x [10ms]*/
    }
}

/***************************************************************************//**
*
* @fn         pmic_reset_timer
*
* @brief      Reset the shutdown/reset timers
*
* @param [in] void
*
* @return     void
*
******************************************************************************/
void pmic_reset_timers (void)
{
    ForceShutdown = false;
    ForceReset = false;
    ReqShutdown = false;
    ReqAutoStartup = false;
    EnableKeepAlive = false;
    ForceShutdown_timeout = FORCE_SHUTDOWN_TIMEOUT;
    KeepAlive_timeout = KEEP_ALIVE_TIMEOUT;
    ReqShutdown_timeout = 0;
}

/***************************************************************************//**
*
* @fn         pmic_check_inputs
*
* @brief      Return true if any of the inputs is active
*
* @param [in] void
*
* @return     void
*
******************************************************************************/
bool_t pmic_check_inputs (void)
{
    if ( (INPUT_ACTIVE == INPUTS_Get_Value(IN_PowerBtn)) || (INPUT_ACTIVE == INPUTS_Get_Value(IN_Ignition)) || (CAN_BUS_OPERATIVE == can_active) )
    {
        if (INPUT_ACTIVE == INPUTS_Get_Value(IN_PowerBtn))
        {
            PMIC_PowerUp_Condition = POWERUP_CONDITION_POWERBTN;
        }
        else if (INPUT_ACTIVE == INPUTS_Get_Value(IN_Ignition))
        {
            PMIC_PowerUp_Condition = POWERUP_CONDITION_IGNITION;
        }
        else
        {
            PMIC_PowerUp_Condition = POWERUP_CONDITION_CAN;
        }
        return true;
    }

    return false;
}

/***************************************************************************//**
*
* @fn         pmic_start_activities
*
* @brief      Start internal activities which should be sleep during OFF
*
* @param [in] void
*
* @return     void
*
******************************************************************************/
void pmic_start_activities (void)
{
    OUT_Set_PowerOn();  /* POWER ON */
    ADC_Start();
}

/***************************************************************************//**
*
* @fn         pmic_stop_activities
*
* @brief      Stop internal activities which should be sleep during OFF
*
* @param [in] void
*
* @return     void
*
******************************************************************************/
void pmic_stop_activities (void)
{
    OUT_Clr_PowerOn(); /* POWER OFF */
    ADC_Stop();
}

/***************************************************************************//**
*
* @fn         pmic_check_keep_alive
*
* @brief      Verify the state of the keep alive timer
*
* @param [in] void
*
* @return     void
*
******************************************************************************/
void pmic_check_keep_alive (void)
{
    if (true == EnableKeepAlive)
    {
        if(KeepAlive_timeout)
        {
            KeepAlive_timeout--;
            if (0 == KeepAlive_timeout)
            {
                ForceReset = true;
            }
        }
    }
}

/***************************************************************************//**
*
* @fn         pmic_check_keep_alive
*
* @brief      Verify the state of the force shutdown timer (IN_PowerBtn_LP)
*
* @param [in] void
*
* @return     void
*
******************************************************************************/
void pmic_check_force_shutdown (void)
{
    if (INPUT_ACTIVE == INPUTS_Get_Value(IN_PowerBtn))
    {
        if (ForceShutdown_timeout)
        {
            ForceShutdown_timeout--;
            if (0 == ForceShutdown_timeout)
            {
                ForceShutdown = true;
            }
        }
    }
    else
    {
        ForceShutdown_timeout = FORCE_SHUTDOWN_TIMEOUT;
    }
}

/***************************************************************************//**
*
* @fn         pmic_check_req_shutdown
*
* @brief      Verify the state of the request shutdown timer
*
* @param [in] void
*
* @return     void
*
******************************************************************************/
void pmic_check_req_shutdown (void)
{
    if (ReqShutdown_timeout)
    {
        /* If during the shutdown request timer an input activation event is detected
         * upon timeout the pmic shall start again */
        if(pmic_check_inputs())
        {
            ReqAutoStartup = true;
        }

        ReqShutdown_timeout--;
        if (0 == ReqShutdown_timeout)
        {
            ReqShutdown = true;
        }
    }
}

/***************************************************************************//**
*
* @fn         PMIC_Controller
*
* @brief      This function analyze the inputs state and manage the PMIC
*
* @param [in] void
*
* @return     void
*
******************************************************************************/
void PMIC_Controller (void)
{
    /**************Prepare data****************/
    can_active = CAN_Get_Value();

    /**************PMIC CONTROL****************/
    switch(PMIC_state)
    {
        case PMIC_CS_INIT:
                PMIC_PowerUp_Condition = POWERDOWN;
                pmic_stop_activities();
                PMIC_state = PMIC_CS_OFF;
        break;

        case PMIC_CS_OFF: /* Wait for ON condition */
            if (pmic_check_inputs() || ReqAutoStartup)
            {
                pmic_reset_timers();
                pmic_start_activities();
                PMIC_state = PMIC_CS_ON;
            }
            break;

        case PMIC_CS_ON: /* Wait for shutdown condition */
            pmic_check_keep_alive();
            pmic_check_force_shutdown();
            pmic_check_req_shutdown();

            if (true == ForceReset)
            {
                PMIC_state = PMIC_CS_INIT;  /* Reset */
            }
            else if (true == ForceShutdown)
            {
                PMIC_state = PMIC_CS_IDLE;
            }
            else if (true == ReqShutdown)
            {
                PMIC_state = PMIC_CS_INIT;
            }
            break;

        case PMIC_CS_IDLE: /* Wait for all inputs to go to off/idle */
            //if (!pmic_check_inputs())
            if (INPUT_INACTIVE == INPUTS_Get_Value(IN_PowerBtn))
            {
                PMIC_state = PMIC_CS_INIT;
            }
            break;

        case PMIC_CS_MFG:
            pmic_reset_timers();
            pmic_start_activities();
            PMIC_state = PMIC_CS_ON;
            break;

        default:
            break;
    }
}

/***************************************************************************//**
*
* @fn         PMIC_Get_State
*
* @brief      Return the PMIC state
*
* @param [in] state
*
* @return     void
*
******************************************************************************/
void PMIC_Get_State (uint8_t *state)
{
    *state = (uint8_t)PMIC_state;
}


/***************************************************************************//**
*
* @fn         PMIC_Init
*
* @brief      Initialize the communication interface
*
* @param [in] None
*
* @return     None
*
******************************************************************************/
void PMIC_Init (void)
{
    /******** PINs configuration ************/
    GPIO_setAsOutputPin(PORT_PwrOn); /* PwrOn */

    // /******* Variables initialization *******/
    can_active = CAN_BUS_IDLE;
    PMIC_state = PMIC_CS_INIT;
    pmic_reset_timers();

    PMIC_PowerUp_Condition = POWERDOWN;

    /******* Output pin configuration *******/
    OUT_Clr_PowerOn();
}
