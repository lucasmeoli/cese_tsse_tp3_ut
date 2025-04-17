/************************************************************************************************
Copyright (c) 2025, Lucas Meoli <meolilucas@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*************************************************************************************************/

/** @file test_pmic.c
 ** @brief Unit tests for the PMIC state machine
 **/

/* === Headers files inclusions =============================================================== */

#include "unity.h"
#include "pmic_interface.h"
#include "mock_can.h"
#include "mock_gpio.h"
#include "mock_inputs.h"

/* === Macros definitions ====================================================================== */
#define POWER_ON_PORT               GPIO_PORT_P2
#define POWER_ON_PIN                GPIO_PIN7

/* This value was changed from 800 to 4 to simplify unit testing */
#define FORCE_SHUTDOWN_TIMEOUT      4
/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */
void setUp(void) {
    GPIO_setAsOutputPin_Expect(POWER_ON_PORT, POWER_ON_PIN);    // PORT_PwrOn must be configurated
    GPIO_setOutputLowOnPin_Expect(POWER_ON_PORT, POWER_ON_PIN); // PORT_PwrOn must be cleared

    PMIC_Init();
}

static void simulate_power_button_pressed(void) {
    INPUTS_Get_Value_ExpectAndReturn(IN_PowerBtn, INPUT_ACTIVE);
    INPUTS_Get_Value_ExpectAndReturn(IN_PowerBtn, INPUT_ACTIVE);
}

static void simulate_ignition_on(void) {
    INPUTS_Get_Value_ExpectAndReturn(IN_PowerBtn, INPUT_INACTIVE);
    INPUTS_Get_Value_ExpectAndReturn(IN_Ignition, INPUT_ACTIVE);
    INPUTS_Get_Value_ExpectAndReturn(IN_PowerBtn, INPUT_INACTIVE);
    INPUTS_Get_Value_ExpectAndReturn(IN_Ignition, INPUT_ACTIVE);
}

static void simulate_can_activity(void) {
    CAN_Get_Value_ExpectAndReturn(CAN_BUS_OPERATIVE);
    INPUTS_Get_Value_ExpectAndReturn(IN_PowerBtn, INPUT_INACTIVE);
    INPUTS_Get_Value_ExpectAndReturn(IN_Ignition, INPUT_INACTIVE);
    INPUTS_Get_Value_ExpectAndReturn(IN_PowerBtn, INPUT_INACTIVE);
    INPUTS_Get_Value_ExpectAndReturn(IN_Ignition, INPUT_INACTIVE);
}


/* === Public function implementation ========================================================== */


/* Initial setup conditions*/
void test_pmic_initial_setup_conditions(void) {
    uint8_t pmic_state = (uint8_t)0xFF;
    uint8_t pmic_can_activity = (uint8_t)0xFF;
    uint8_t pmic_PowerUp_Condition = (uint8_t)0xFF;

    GPIO_setAsOutputPin_Expect(POWER_ON_PORT, POWER_ON_PIN);    // PORT_PwrOn must be configurated
    GPIO_setOutputLowOnPin_Expect(POWER_ON_PORT, POWER_ON_PIN); // PORT_PwrOn must be cleared

    PMIC_Init();
    PMIC_Get_State(&pmic_state);
    PMIC_Get_CAN_Activity(&pmic_can_activity);
    PMIC_Get_PowerUp_Condition(&pmic_PowerUp_Condition);
    
    TEST_ASSERT_EQUAL_UINT8((uint8_t)PMIC_CS_INIT, pmic_state);
    TEST_ASSERT_EQUAL_UINT8((uint8_t)CAN_BUS_IDLE, pmic_can_activity);
    TEST_ASSERT_EQUAL_UINT8((uint8_t)POWERDOWN, pmic_PowerUp_Condition);
}

/*********** INIT --> OFF transition ***********/

void test_pmic_transition_init_to_off(void) {
    uint8_t pmic_state = (uint8_t)0xFF;
    uint8_t pmic_PowerUp_Condition = (uint8_t)0xFF;

    /* Trigger state machine transition */
    CAN_Get_Value_ExpectAndReturn(CAN_BUS_IDLE);
    PMIC_Controller();

    /* Get state machine values */
    PMIC_Get_State(&pmic_state);
    PMIC_Get_PowerUp_Condition(&pmic_PowerUp_Condition);

    TEST_ASSERT_EQUAL_UINT8((uint8_t)PMIC_CS_OFF, pmic_state);
    TEST_ASSERT_EQUAL_UINT8((uint8_t)POWERDOWN, pmic_PowerUp_Condition);
}

/*********** OFF --> ON transitions ***********/

/* Triggered by power button */
void test_pmic_transition_off_to_on_on_power_button(void) {
    uint8_t pmic_state = (uint8_t)0xFF;
    uint8_t pmic_PowerUp_Condition = (uint8_t)0xFF;

    // Setup OFF state
    CAN_Get_Value_ExpectAndReturn(CAN_BUS_IDLE);
    PMIC_Controller();

    // Mock inputs values
    CAN_Get_Value_ExpectAndReturn(CAN_BUS_IDLE);
    INPUTS_Get_Value_ExpectAndReturn(IN_PowerBtn, INPUT_ACTIVE);
    INPUTS_Get_Value_ExpectAndReturn(IN_PowerBtn, INPUT_ACTIVE);
    
    /* Power on MUST be set */
    GPIO_setOutputHighOnPin_Expect(POWER_ON_PORT, POWER_ON_PIN);

    PMIC_Controller(); 

    PMIC_Get_State(&pmic_state);
    PMIC_Get_PowerUp_Condition(&pmic_PowerUp_Condition);
    TEST_ASSERT_EQUAL_UINT8((uint8_t)PMIC_CS_ON, pmic_state);
    TEST_ASSERT_EQUAL_UINT8((uint8_t)POWERUP_CONDITION_POWERBTN, pmic_PowerUp_Condition);
}

/* Triggered by ignition */
void test_pmic_transition_off_to_on_by_ignition(void) {
    uint8_t pmic_state = (uint8_t)0xFF;
    uint8_t pmic_PowerUp_Condition = (uint8_t)0xFF;

    // Setup OFF state
    CAN_Get_Value_ExpectAndReturn(CAN_BUS_IDLE);
    PMIC_Controller();

    // Mock inputs values
    CAN_Get_Value_ExpectAndReturn(CAN_BUS_IDLE);
    INPUTS_Get_Value_ExpectAndReturn(IN_PowerBtn, INPUT_INACTIVE);
    INPUTS_Get_Value_ExpectAndReturn(IN_Ignition, INPUT_ACTIVE);
    INPUTS_Get_Value_ExpectAndReturn(IN_PowerBtn, INPUT_INACTIVE);
    INPUTS_Get_Value_ExpectAndReturn(IN_Ignition, INPUT_ACTIVE);
    
    /* Power on MUST be set */
    GPIO_setOutputHighOnPin_Expect(POWER_ON_PORT, POWER_ON_PIN);

    PMIC_Controller(); 

    PMIC_Get_State(&pmic_state);
    PMIC_Get_PowerUp_Condition(&pmic_PowerUp_Condition);
    TEST_ASSERT_EQUAL_UINT8((uint8_t)PMIC_CS_ON, pmic_state);
    TEST_ASSERT_EQUAL_UINT8((uint8_t)POWERUP_CONDITION_IGNITION, pmic_PowerUp_Condition);
}

/* Triggered by CAN */
void test_pmic_transition_off_to_on_by_can(void) {
    uint8_t pmic_state = (uint8_t)0xFF;
    uint8_t pmic_PowerUp_Condition = (uint8_t)0xFF;

    // Setup OFF state
    CAN_Get_Value_ExpectAndReturn(CAN_BUS_IDLE);
    PMIC_Controller();

    // Mock inputs values
    CAN_Get_Value_ExpectAndReturn(CAN_BUS_OPERATIVE);
    INPUTS_Get_Value_ExpectAndReturn(IN_PowerBtn, INPUT_INACTIVE);
    INPUTS_Get_Value_ExpectAndReturn(IN_Ignition, INPUT_INACTIVE);
    INPUTS_Get_Value_ExpectAndReturn(IN_PowerBtn, INPUT_INACTIVE);
    INPUTS_Get_Value_ExpectAndReturn(IN_Ignition, INPUT_INACTIVE);

    /* Power on MUST be set */
    GPIO_setOutputHighOnPin_Expect(POWER_ON_PORT, POWER_ON_PIN);

    PMIC_Controller(); 

    PMIC_Get_State(&pmic_state);
    PMIC_Get_PowerUp_Condition(&pmic_PowerUp_Condition);
    TEST_ASSERT_EQUAL_UINT8((uint8_t)PMIC_CS_ON, pmic_state);
    TEST_ASSERT_EQUAL_UINT8((uint8_t)POWERUP_CONDITION_CAN, pmic_PowerUp_Condition);
}


/*********** ON --> IDLE transitions ***********/

/* Triggered by power button */
void test_pmic_transition_on_to_idle(void) {
    uint8_t pmic_state = (uint8_t)0xFF;
    uint8_t pmic_PowerUp_Condition = (uint8_t)0xFF;

    /* Transition to OFF state */
    CAN_Get_Value_ExpectAndReturn(CAN_BUS_IDLE);
    PMIC_Controller();

    /* Transition to ON state */
    CAN_Get_Value_ExpectAndReturn(CAN_BUS_OPERATIVE);
    INPUTS_Get_Value_ExpectAndReturn(IN_PowerBtn, INPUT_ACTIVE);
    INPUTS_Get_Value_ExpectAndReturn(IN_PowerBtn, INPUT_ACTIVE);
    /* Power on MUST be set */
    GPIO_setOutputHighOnPin_Expect(POWER_ON_PORT, POWER_ON_PIN);
    PMIC_Controller(); 

    PMIC_KeepAlive(false);
    for(uint16_t i = FORCE_SHUTDOWN_TIMEOUT; i > 0; i--) {
        CAN_Get_Value_ExpectAndReturn(CAN_BUS_OPERATIVE); /* This value is irrelevant */
        INPUTS_Get_Value_ExpectAndReturn(IN_PowerBtn, INPUT_ACTIVE);
        PMIC_Controller(); 
    }

    PMIC_Get_State(&pmic_state);
    TEST_ASSERT_EQUAL_UINT8((uint8_t)PMIC_CS_IDLE, pmic_state);
}

/*********** ON --> INIT transitions ***********/

/*********** IDLE --> INIT transitions ***********/

/* Triggered by power button */
void test_pmic_transition_idle_to_init(void) {
    uint8_t pmic_state = (uint8_t)0xFF;
    uint8_t pmic_PowerUp_Condition = (uint8_t)0xFF;

    /* Transition to OFF state */
    CAN_Get_Value_ExpectAndReturn(CAN_BUS_IDLE);
    PMIC_Controller();

    /* Transition to ON state */
    CAN_Get_Value_ExpectAndReturn(CAN_BUS_OPERATIVE);
    INPUTS_Get_Value_ExpectAndReturn(IN_PowerBtn, INPUT_ACTIVE);
    INPUTS_Get_Value_ExpectAndReturn(IN_PowerBtn, INPUT_ACTIVE);
    /* Power on MUST be set */
    GPIO_setOutputHighOnPin_Expect(POWER_ON_PORT, POWER_ON_PIN);
    PMIC_Controller();

    /* Transition to IDLE state*/
    PMIC_KeepAlive(false);
    for(uint16_t i = FORCE_SHUTDOWN_TIMEOUT; i > 0; i--) {
        CAN_Get_Value_ExpectAndReturn(CAN_BUS_OPERATIVE); /* This value is irrelevant */
        INPUTS_Get_Value_ExpectAndReturn(IN_PowerBtn, INPUT_ACTIVE);
        PMIC_Controller();
    }

    /* Tranistion to INIT state */
    CAN_Get_Value_ExpectAndReturn(CAN_BUS_OPERATIVE);
    INPUTS_Get_Value_ExpectAndReturn(IN_PowerBtn, INPUT_INACTIVE);
    PMIC_Controller(); 

    PMIC_Get_State(&pmic_state);
    TEST_ASSERT_EQUAL_UINT8((uint8_t)PMIC_CS_INIT, pmic_state);
}


/* === End of documentation ==================================================================== */

