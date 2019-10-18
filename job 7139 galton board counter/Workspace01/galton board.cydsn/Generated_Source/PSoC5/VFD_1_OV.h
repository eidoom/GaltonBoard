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

#if !defined(CY_VFD_OV_VFD_1_H)
#define CY_VFD_OV_VFD_1_H

#include "VFD_1_PVT.h"
#include "VFD_1_Comp_OV.h"

#if (VFD_1_INT_VREF)
#include "VFD_1_VDAC_OV.h"
#include "VFD_1_OvDMA_dma.h"


/***************************************
*   Variable with external linkage
***************************************/

extern          uint8           VFD_1_OVFaultThreshold[VFD_1_NUMBER_OF_VOLTAGES];
extern  const   uint16  CYCODE  VFD_1_initOVFaultThreshold[VFD_1_NUMBER_OF_VOLTAGES];


/***************************************
*        Function Prototypes
***************************************/

cystatus VFD_1_SetOVFaultThreshold(uint8 voltageNum, uint16 ovFaultThreshold) ;
uint16   VFD_1_GetOVFaultThreshold(uint8 voltageNum) ;


/***************************************
*   Internal Function Prototypes
***************************************/

void   VFD_1_OVRefInit(void) ;
void   VFD_1_OVRefEnable(void) ;
void   VFD_1_OVRefStop(void) ;


/*******************************************************************************
* Function Name: VFD_1_SetOVDac
********************************************************************************
*
* Summary:
*  Calling this API does not change the OV VDAC setting immediately. It writes
*  a specified value to the SRAM buffer to be used by the DMA controller that
*  drives updates to the OV DAC for the specified voltage input.
*  This API does not apply when ExternalRef=true.
*
* Parameters:
*  uint8 voltageNum: Specifies the voltage input number. Valid range: 1..32.
*  uint8 dacValue  : Specifies the value to be written to the OV VDAC.
*                    Valid range: 1..255.
*
* Return:
*  None.
*
*******************************************************************************/
#define VFD_1_SetOVDac(voltageNum, dacValue)     \
    (VFD_1_OVFaultThreshold[VFD_1_NUMBER_OF_VOLTAGES - (voltageNum)] = (dacValue))


/*******************************************************************************
* Function Name: VFD_1_GetOVDac
********************************************************************************
*
* Summary:
*  Returns the DAC value currently being used by the DMA controller that drives 
*  updates to the OV DAC for the specified voltage input. The value is extracted
*  from the SRAM buffer, not from the VDAC directly.
*  This API does not apply when ExternalRef=true.
*
* Parameters:
*  uint8 voltageNum: Specifies the voltage input number. Valid range: 1..32.
*
* Return:
*  OV DAC value for the specified voltage input. Valid range: 1..255.
*
*******************************************************************************/
#define VFD_1_GetOVDac(voltageNum)   (VFD_1_OVFaultThreshold[VFD_1_NUMBER_OF_VOLTAGES - (voltageNum)])


/*******************************************************************************
* Function Name: VFD_1_SetOVDacDirect
********************************************************************************
*
* Summary:
*  Allows manual control of OV VDAC. The dacValue is written directly to
*  the VDAC component. Useful for OV VDAC calibration. Note that if the VFD
*  component is running when this API is called, the state machine controller
*  will override the OV VDAC value set by this API call. Call the Pause API to
*  stop the state machine controller if manual OV VDAC control is desired.
*  This API does not apply when ExternalRef=true.
*  
*
* Parameters:
*  uint8 dacValue: Specifies the value to be written to the OV VDAC directly.
*                  Valid range: 1..255
*
* Return:
*  None.
*
* Side Effects:
*  Calling this API may cause the comparator to trigger a fault condition. To 
*  prevent this, call the VFD_Pause() API prior to calling this API.
*
*******************************************************************************/
#define VFD_1_SetOVDacDirect(dacValue)   (VFD_1_VDAC_OV_Data_REG = (dacValue))


/*******************************************************************************
* Function Name: VFD_1_GetOVDacDirect
********************************************************************************
*
* Summary:
*  Returns the current OV VDAC. The returned dacValue is read directly from the
*  VDAC component. Useful for OV VDAC calibration. Note: If this API is called
*  while  the component is running, it is not possible to know which voltage
*  input the OV VDAC value is associated with. Call the Pause API to stop the
*  state machine controller if manual OV VDAC control is desired.
*  This API does not apply when ExternalRef=true.
*
* Parameters:
*  None.
*
* Return:
*  A current OV DAC value.
*
*******************************************************************************/   
#define VFD_1_GetOVDacDirect()   VFD_1_VDAC_OV_Data_REG

#endif /* VFD_1_INT_VREF */


/*******************************************************************************
*   Encapsulate function calls into macro helpers to simplify coding
*******************************************************************************/

#define VFD_1_Comp_OV_LOWSPEED   VFD_1_Comp_OV_SLOWSPEED

#if (VFD_1_INT_VREF)
    
/* Initializes OV comparator and threshold generation */
#define VFD_1_OVInit()       do { \
                                            VFD_1_OVRefInit();    \
                                            VFD_1_Comp_OV_Init(); \
                                        } while(0)

/* Enables OV comparator and threshold generation */
#define VFD_1_OVEnable()     do { \
                                            VFD_1_OVRefEnable();    \
                                            VFD_1_Comp_OV_Enable(); \
                                        } while(0)

/* Disables OV comparator and threshold generation */
#define VFD_1_OVStop()       do { \
                                            VFD_1_OVRefStop();    \
                                            VFD_1_Comp_OV_Stop(); \
                                        } while(0)

/* Sets speed mode for OV DAC and comparator. Valid values are LOWSPEED and HIGHSPEED. */
#define VFD_1_SetOVSpeed(speed)  do { \
                                                VFD_1_Comp_OV_SetSpeed(VFD_1_Comp_OV_##speed); \
                                                VFD_1_VDAC_OV_SetSpeed(VFD_1_VDAC_OV_##speed); \
                                            } while(0)
#else

/* Initializes OV comparator */
#define VFD_1_OVInit()       VFD_1_Comp_OV_Init()

/* Enables OV comparator */
#define VFD_1_OVEnable()     VFD_1_Comp_OV_Enable()

/* Disables OV comparator */
#define VFD_1_OVStop()       VFD_1_Comp_OV_Stop()

/* Sets speed mode for OV comparator. Valid values are LOWSPEED and HIGHSPEED. */
#define VFD_1_SetOVSpeed(speed)  VFD_1_Comp_OV_SetSpeed(VFD_1_Comp_OV_##speed)

#endif /* VFD_1_INT_VREF */

/* Define power converters OV fault status bits */
#define VFD_1_OV_STS8_STATUS_REG         (* (reg8 *)VFD_1_bVFD_OV_STS_8__Sts__STATUS_REG)
#define VFD_1_OV_STS8_STATUS_PTR         (  (reg8 *)VFD_1_bVFD_OV_STS_8__Sts__STATUS_REG)
#if (VFD_1_NUMBER_OF_VOLTAGES > 8u)
#define VFD_1_OV_STS16_STATUS_REG        (* (reg8 *)VFD_1_bVFD_OV_STS_16__Sts__STATUS_REG)
#define VFD_1_OV_STS16_STATUS_PTR        (  (reg8 *)VFD_1_bVFD_OV_STS_16__Sts__STATUS_REG)
#endif /* VFD_1_NUMBER_OF_VOLTAGES > 8u */
#if (VFD_1_NUMBER_OF_VOLTAGES > 16u)
#define VFD_1_OV_STS24_STATUS_REG        (* (reg8 *)VFD_1_bVFD_OV_STS_24__Sts__STATUS_REG)
#define VFD_1_OV_STS24_STATUS_PTR        (  (reg8 *)VFD_1_bVFD_OV_STS_24__Sts__STATUS_REG)
#endif /* VFD_1_NUMBER_OF_VOLTAGES > 16u */
#if (VFD_1_NUMBER_OF_VOLTAGES > 24u)
#define VFD_1_OV_STS32_STATUS_REG        (* (reg8 *)VFD_1_bVFD_OV_STS_32__Sts__STATUS_REG)
#define VFD_1_OV_STS32_STATUS_PTR        (  (reg8 *)VFD_1_bVFD_OV_STS_32__Sts__STATUS_REG)
#endif /* VFD_1_NUMBER_OF_VOLTAGES > 24u */

#endif /* CY_VFD_OV_VFD_1_H */


/* [] END OF FILE */
