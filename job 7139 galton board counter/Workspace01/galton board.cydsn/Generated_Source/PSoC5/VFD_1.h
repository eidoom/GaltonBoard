/*******************************************************************************
* File Name: VFD_1.h
* Version 3.0
*
* Description:
*  Contains the function prototypes, constants and register definition of the 
*  Voltage Fault Detector Component.
*
*
********************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_VFD_1_H)
#define CY_VFD_1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "VFD_1_PVT.h"


/***************************************
*        Function Prototypes
***************************************/

void  VFD_1_Start(void) ;
void  VFD_1_Stop(void) ;
void  VFD_1_Init(void) ;
void  VFD_1_Enable(void) ;
void  VFD_1_SetSpeed(uint8 speedMode) ;
void  VFD_1_ComparatorCal(uint8 compType) ;

#if (VFD_1_COMP_TYPE == VFD_1_OV_UV)
    void VFD_1_GetOVUVFaultStatus(uint32 * ovStatus, uint32 * uvStatus) ;
#elif (VFD_1_COMP_TYPE == VFD_1_OV)
    void VFD_1_GetOVFaultStatus(uint32 * status) ;
#else 
    void VFD_1_GetUVFaultStatus(uint32 * status) ;
#endif /* (VFD_1_COMP_TYPE == VFD_1_OV_UV) */


/***************************************
*   Variable with external linkage
***************************************/

extern uint8 VFD_1_initVar;


/***************************************
*           API Constants
***************************************/

#define VFD_1_LOW_SPEED                  0u
#define VFD_1_HIGH_SPEED                 1u

#define VFD_1_STATUS_MASK                0x00000001u

#define VFD_1_DAC_RANGE                  1u
#define VFD_1_DAC_RANGE_1V               0u
#define VFD_1_DAC_RANGE_4V               1u

#define VFD_1_GF_LENGTH                  1u


/***************************************
*             Registers
***************************************/

#define VFD_1_CONTROL_REG                (* (reg8 *)VFD_1_bVFD_CtlReg__CONTROL_REG)
#define VFD_1_CONTROL_PTR                (  (reg8 *)VFD_1_bVFD_CtlReg__CONTROL_REG)

#define VFD_1_STATUS_REG                 (* (reg8 *)VFD_1_bVFD_StsReg__STATUS_REG)
#define VFD_1_STATUS_PTR                 (  (reg8 *)VFD_1_bVFD_StsReg__STATUS_REG)

/* Glitch filter definition */
#define VFD_1_PG_FILTER_LENGTH_REG       (* (reg8  *)VFD_1_bVFD_PgFilt__D0_REG)
#if (VFD_1_ENABLE_OV_DETECT)
#define VFD_1_FILTER_IN_FIFO_PTR         (  (reg16 *)VFD_1_bVFD_PgFilt__16BIT_F0_REG)
#define VFD_1_FILTER_OUT_FIFO_PTR        (  (reg16 *)VFD_1_bVFD_PgFilt__16BIT_F1_REG)
#define VFD_1_OV_FILTER_LENGTH_REG       (* (reg8  *)VFD_1_bVFD_OV_OvFilt__D0_REG)
#else
#define VFD_1_FILTER_IN_FIFO_PTR         (  (reg8 *)VFD_1_bVFD_PgFilt__F0_REG)
#define VFD_1_FILTER_OUT_FIFO_PTR        (  (reg8 *)VFD_1_bVFD_PgFilt__F1_REG)
#endif /* VFD_1_ENABLE_OV_DETECT */

#define VFD_1_PG_FILTER_FIFO_ACTL_REG    (* (reg8 *)VFD_1_bVFD_PgFilt__DP_AUX_CTL_REG)
#define VFD_1_OV_FILTER_FIFO_ACTL_REG    (* (reg8 *)VFD_1_bVFD_OV_OvFilt__DP_AUX_CTL_REG)

/* Voltage index and settling delay counters */ 
#define VFD_1_VIN_DELAY_CNT_ACTL_REG     (* (reg8 *)VFD_1_bVFD_Settling_Timer__CONTROL_AUX_CTL_REG)
#define VFD_1_VIN_INDEX_CNT_ACTL_REG     (* (reg8 *)VFD_1_bVFD_VoltCounter__CONTROL_AUX_CTL_REG)
#define VFD_1_VIN_INDEX_CNT_COUNT_REG    (* (reg8 *)VFD_1_bVFD_VoltCounter__COUNT_REG)

/* Power converters status bits */
#define VFD_1_PG_STS8_STATUS_REG         (* (reg8 *)VFD_1_bVFD_PG_STS_8__Sts__STATUS_REG)
#define VFD_1_PG_STS8_STATUS_PTR         (  (reg8 *)VFD_1_bVFD_PG_STS_8__Sts__STATUS_REG)
#if (VFD_1_NUMBER_OF_VOLTAGES > 8u)
#define VFD_1_PG_STS16_STATUS_REG        (* (reg8 *)VFD_1_bVFD_PG_STS_16__Sts__STATUS_REG)
#define VFD_1_PG_STS16_STATUS_PTR        (  (reg8 *)VFD_1_bVFD_PG_STS_16__Sts__STATUS_REG)
#endif /* VFD_1_NUMBER_OF_VOLTAGES > 8u */
#if (VFD_1_NUMBER_OF_VOLTAGES > 16u)
#define VFD_1_PG_STS24_STATUS_REG        (* (reg8 *)VFD_1_bVFD_PG_STS_24__Sts__STATUS_REG)
#define VFD_1_PG_STS24_STATUS_PTR        (  (reg8 *)VFD_1_bVFD_PG_STS_24__Sts__STATUS_REG)
#endif /* VFD_1_NUMBER_OF_VOLTAGES > 16u */
#if (VFD_1_NUMBER_OF_VOLTAGES > 24u)
#define VFD_1_PG_STS32_STATUS_REG        (* (reg8 *)VFD_1_bVFD_PG_STS_32__Sts__STATUS_REG)
#define VFD_1_PG_STS32_STATUS_PTR        (  (reg8 *)VFD_1_bVFD_PG_STS_32__Sts__STATUS_REG)
#endif /* VFD_1_NUMBER_OF_VOLTAGES > 24u */


/***************************************
*       Register Constants
***************************************/

/* Control register bits */
#define VFD_1_EN                         0x01u
#define VFD_1_PAUSE                      0x02u

/* FIFO clearing bits */
#define VFD_1_FIFO_CLR                   0x03u

/* Firmware enable signal for internal counters */
#define VFD_1_CNT_START                  0x20u


/*******************************************************************************
* Function Name: VFD_1_Pause
********************************************************************************
*
* Summary:
*  Pauses the controller state machine. The current PGOOD states are kept when
*  the component is paused.
*  Note that calling this API does not stop the component until it completes
*  the current process cycle. Therefore, if the purpose of calling this API is
*  specifically to change the VDAC settings (for calibration purposes for example),
*  sufficient time should be allowed to let the component run to a completion
*  before an attempt to access the VDACs directly. This can be checked by
*  calling VFD_IsPaused().
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  Stops the fault detection state machine. Does not stop the component 
*  immediately.
*
*******************************************************************************/
#define VFD_1_Pause()        (VFD_1_CONTROL_REG |= VFD_1_PAUSE)


/*******************************************************************************
* Function Name: VFD_1_IsPaused
********************************************************************************
*
* Summary:
*  Checks to see if the component is paused.
*
* Parameters:
*  None.
*
* Return:
*  True if the component is paused.
*
*******************************************************************************/
#define VFD_1_IsPaused()     (VFD_1_STATUS_REG == 0u)


/*******************************************************************************
* Function Name: VFD_1_Resume
********************************************************************************
*
* Summary:
*  Resumes the controller state machine.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  Restarts the fault detection logic.
*
*******************************************************************************/
#define VFD_1_Resume()       (VFD_1_CONTROL_REG = VFD_1_EN)


/*******************************************************************************
* Function Name: VFD_1_SetGlitchFilterLength
********************************************************************************
*
* Summary:
*  Sets glitch filter length.
*
* Parameters:
*  filterLength: Filter length. Absolute time units depend on the input clock
*  frequency. Valid range: 1..255
*
* Return:
*  None.
*
*******************************************************************************/
#ifdef VFD_1_OV_FILTER_LENGTH_REG
#define VFD_1_SetGlitchFilterLength(filterLength) do { \
                                                                 VFD_1_PG_FILTER_LENGTH_REG = (filterLength); \
                                                                 VFD_1_OV_FILTER_LENGTH_REG = (filterLength); \
                                                             } while(0)
#else
#define VFD_1_SetGlitchFilterLength(filterLength)    (VFD_1_PG_FILTER_LENGTH_REG = (filterLength))
#endif  /*VFD_1_OV_FILTER_LENGTH_REG */


/*******************************************************************************
* Function Name: VFD_1_GetGlitchFilterLength
********************************************************************************
*
* Summary:
*  Returns glitch filter length. 
*
* Parameters:
*  None.
*
* Return:
*  Glitch filter length. Absolute time units depend on input clock frequency.
*  Valid range: 1..255.
*
*******************************************************************************/
#define VFD_1_GetGlitchFilterLength()    VFD_1_PG_FILTER_LENGTH_REG


#endif /* CY_VFD_1_H */


/* [] END OF FILE */
