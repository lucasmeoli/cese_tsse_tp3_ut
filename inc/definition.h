#ifndef DEFINITION_H
#define DEFINITION_H
/*===========================================================================*/
/**
 * @file definition.h
 *
 * Global defines of miscellaneous useful stuff.
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
/*---------------------------------------------------------------------------*
 * Standard boolean labels
 *---------------------------------------------------------------------------*/
#ifndef  UP
#define  DOWN           false
#define  UP             !DOWN
#endif

#ifndef  OFF
#define  OFF             false
#define  ON              !OFF
#endif

#ifndef  CLEARED
#define  CLEARED         false
#define  SET             !CLEARED
#endif

#ifndef  DISABLE
#define  DISABLE         false
#define  ENABLE          !DISABLE
#endif

/*===========================================================================*
 * Exported Preprocessor #define MACROS
 *===========================================================================*/
/*---------------------------------------------------------------------------*
 * macros to append symbols together
 *---------------------------------------------------------------------------*/
#define  To_Boolean(bit)            ((bit) ? true : false)                 /* convert bit/logical to normalized boolean */
#define  Bit_Copy(target, source)   (target = ((source) ? true : false))   /* set target equal to source */
#define  Bit_Toggle(target)         (target = ((target) ? false : true))   /* toggle target              */
#define  FOREVER                    while(1)

#define icat(x, y) x ## y
#define iins(x, y, z) x ## y ## z
#define cat(x, y)  icat(x,y)

#define Num_Elems(array)    (sizeof(array)/sizeof(array[0]))

/*===========================================================================*
 * Exported Type Declarations
 *===========================================================================*/
typedef enum   Status_Type_Tag      /* Return status from RTOS functions */
   {
      E_OK             =  0,        /* NO Error, successful */
      E_ERROR,                      /* General Error        */
      E_OS_ACCESS,
      E_OS_CALLEVEL,
      E_OS_ID,
      E_OS_LIMIT,
      E_OS_NOFUNC,
      E_OS_RESOURCE,
      E_OS_STATE,
      E_OS_VALUE,
      E_COM_NOMSG,
      E_COM_LOCKED,
      E_TASK_SUSPENDED,
      E_TIMEOUT,                    /* Error due to timeout */
      E_OUT_OF_RANGE,               /* Error due to parameter out of range */
      E_INVALID_CONDITION           /* Error due to invalid conditions */
   } Status_Type;

typedef uint8_t bool_t;
typedef void (*void_fptr) (void);                 /* void void Function pointer typedef */
typedef bool_t (*bool_fptr) (void);                 /* boolean void function typedef */
typedef void (*void_int16_fptr) (int16_t data);   /* void funtion taking 16 bit integer */

/*===========================================================================*
 * Exported Const Object Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Function Prototypes
 *===========================================================================*/

/*===========================================================================*
 * Exported Inline Function Definitions and #define Function-Like Macros
 *===========================================================================*/

#   ifdef __cplusplus
}         /* extern "C" */
#   endif /* __cplusplus */

/** @} doxygen end group */
#endif                          /* DEFINITION_H */
