/*******************************************************************************
* File Name: VFD_1.h
* Version 3.0
*
* Description:
*  This file contains global definitions for the component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_VFD_PVT_VFD_1_H)
#define CY_VFD_PVT_VFD_1_H

#include "cytypes.h"


/***************************************
*       External Dependencies
***************************************/
    
/* AMux constant and variable definitions */
#define VFD_1_AMUX_INPUTS                32

/* Array storing addresses of AMUX ports */
extern uint8 CYXDATA * const CYCODE VFD_1_AMux__addrTable[VFD_1_AMUX_INPUTS];

/* Array storing masks for pins on AMUX ports */
extern const uint8 CYCODE VFD_1_AMux__maskTable[VFD_1_AMUX_INPUTS];


/***************************************
*   Conditional Compilation Parameters
***************************************/

/* Number of inputs */
#define VFD_1_NUMBER_OF_VOLTAGES         1u

/* Comparator types */
#define VFD_1_OV_UV                      0u
#define VFD_1_OV                         1u
#define VFD_1_UV                         2u
#define VFD_1_COMP_TYPE                  0u
#define VFD_1_ENABLE_OV_DETECT           (VFD_1_COMP_TYPE != VFD_1_UV)
#define VFD_1_ENABLE_UV_DETECT           (VFD_1_COMP_TYPE != VFD_1_OV)


/***************************************
*    Internal API Constants
***************************************/

/* Voltage reference */
#define VFD_1_VREF                       0u
#define VFD_1_INT_VREF                   (VFD_1_VREF == 0u)
#define VFD_1_EXT_VREF                   (!VFD_1_INT_VREF)

/* Vin settling delay */
#define VFD_1_VIN_SETTLING_DELAY         8u
#define VFD_1_VIN_SETTLING_DELAY_MIN     3u

/* Common part of DMA configuration. Each burst requires request */
#define VFD_1_DMA_CFG(chan, size, src, dst)    VFD_1_##chan##_DmaInitialize((size), 1u, (src), (dst))
    
/* DAC max value is limited by DAC resolution */
#define VFD_1_DAC_MAX                    255u

/* DAC mV/LSb */
#define VFD_1_DAC_mV_LSb                 16u

/* Number of bits to shift right when dividing by DAC mV/LSb */
#define VFD_1_DAC_SHR_LSb                4u

/* This is the fault threshold saturation for the VDAC.
* The upper limit must account for the VDAC max value of 255, VDAC mV per LSb,
* the input scaling factor of 1000.
*/
#define VFD_1_FAULT_THRESHOLD_SAT_H      (255UL * VFD_1_DAC_mV_LSb * 1000UL)

/* Constants to convert DAC voltage to DAC value. Refer to description
* of VFD_1_ConvertDacVoltageToValue() below for more information.
*/
#define VFD_1_DAC_VOLT2VAL_SHR           (VFD_1_DAC_SHR_LSb + 2u)
#define VFD_1_DAC_VOLT2VAL_ADD           125u
#define VFD_1_DAC_VOLT2VAL_DIV           250u


/***************************************
*   Internal Global Variables
***************************************/

/* Input voltage scaling factor. Indicates amount of attenuation applied
* to converter output voltage before connecting to PSoC.
*/
#if (VFD_1_INT_VREF)
extern const uint16 CYCODE VFD_1_VoltageScale[VFD_1_NUMBER_OF_VOLTAGES];
#endif /* VFD_1_INT_VREF */


/***************************************
*   Macro helpers to simplify coding
***************************************/

/* The scaling factor is a real number in the range from 0.001 to 1.000 with 
*  step 0.001. To eliminate the float point arithmetic, the value entered in
*  the customizer is multiplied by 1000 before storing in memory. Thus the
*  scaled threshold is divided by 1000 to obtain the desired DAC voltage.
*
*       DAC voltage = threshold * scaling factor / 1000
*
*  Add 0.5 LSb so that fractions >= 0.5 LSb are rounded up.
*
*       DAC value = (DAC voltage + 0.5 LSb) / LSb
* 
*  DAC LSb is 4mV for 1V and 16mV for the 4V range. Substituting 1000 as 4 * 250
*  and replacing divisions by shifts right the equations are as follows.
*
*       1V: DAC value = (((threshold * scaling factor) >> 4) + 125) / 250
*       4V: DAC value = (((threshold * scaling factor) >> 6) + 125) / 250
*
*  These equations reduce the execution time significantly by replacing the 32-bit
*  division with a 32-bit shift right and a 16-bit division.
*/
#define VFD_1_ConvertDacVoltageToValue(voltage) \
    ((((uint16)((voltage) >> VFD_1_DAC_VOLT2VAL_SHR)) + VFD_1_DAC_VOLT2VAL_ADD) / VFD_1_DAC_VOLT2VAL_DIV)


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

#define VFD_1_External_Reference     0u
#define VFD_1_CompareType            0u
#define VFD_1_DAC_VOL_DIVIDER        VFD_1_DAC_mV_LSb
#define VFD_1_Dac_Range              1u
#define VFD_1_OV_ONLY                VFD_1_OV
#define VFD_1_UV_ONLY                VFD_1_UV

#endif /* CY_VFD_PVT_VFD_1_H */


/* [] END OF FILE */
