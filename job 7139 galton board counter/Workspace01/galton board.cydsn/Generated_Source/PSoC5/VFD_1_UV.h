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

#if !defined(CY_VFD_UV_VFD_1_H)
#define CY_VFD_UV_VFD_1_H
  
#include "VFD_1_PVT.h"
#include "VFD_1_Comp_UV.h"

#if (VFD_1_INT_VREF)
#include "VFD_1_VDAC_UV.h"
#include "VFD_1_UvDMA_dma.h"


/***************************************
*   Variable with external linkage
***************************************/

extern          uint8           VFD_1_UVFaultThreshold[VFD_1_NUMBER_OF_VOLTAGES];
extern  const   uint16  CYCODE  VFD_1_initUVFaultThreshold[VFD_1_NUMBER_OF_VOLTAGES];


/***************************************
*        Function Prototypes
***************************************/

cystatus VFD_1_SetUVFaultThreshold(uint8 voltageNum, uint16 uvFaultThreshold) ;
uint16   VFD_1_GetUVFaultThreshold(uint8 voltageNum) ;


/***************************************
*   Internal Function Prototypes
***************************************/

void   VFD_1_UVRefInit(void) ;
void   VFD_1_UVRefEnable(void) ;
void   VFD_1_UVRefStop(void) ;


/*******************************************************************************
* Function Name: VFD_1_SetUVDac
********************************************************************************
*
* Summary:
*  Calling this API does not change the UV VDAC setting immediately. It writes
*  a specified value to the SRAM buffer to be used by the DMA controller that
*  drives updates to the UV DAC for the specified voltage input.
*  This API does not apply when ExternalRef=true.
*
* Parameters:
*  uint8 voltageNum: Specifies the voltage input number. Valid range: 1..32.
*  uint8 dacValue  : Specifies the value to be written to the UV VDAC.
*                    Valid range: 1..255.
*
* Return:
*  None.
*
*******************************************************************************/
#define VFD_1_SetUVDac(voltageNum, dacValue)     \
    (VFD_1_UVFaultThreshold[VFD_1_NUMBER_OF_VOLTAGES - (voltageNum)] = (dacValue))


/*******************************************************************************
* Function Name: VFD_1_GetUVDac
********************************************************************************
*
* Summary:
*  Returns the DAC value currently being used by the DMA controller that drives
*  updates to UV DAC for the specified voltage input. The value is extracted
*  from the SRAM buffer, not from VDAC directly.
*  This API does not apply when ExternalRef=true.
*
* Parameters:
*  uint8 voltageNum: Specifies the voltage input number. Valid range: 1..32.
*
* Return:
*  UV DAC value for the specified voltage input. Valid range: 1..255.
*
*
*******************************************************************************/
#define VFD_1_GetUVDac(voltageNum)   (VFD_1_UVFaultThreshold[VFD_1_NUMBER_OF_VOLTAGES - (voltageNum)])


/*******************************************************************************
* Function Name: VFD_1_SetUVDacDirect
********************************************************************************
*
* Summary:
*  Allows manual control of the UV VDAC. The dacValue is written directly to
*  the VDAC component. Useful for UV VDAC calibration. Note that if the VFD
*  component is running when this API is called, the state machine controller
*  will override the UV VDAC value set by this API call. Call the Pause API to
*  stop the state machine controller if manual UV VDAC control is desired.
*  This API does not apply when ExternalRef=true.
*
* Parameters:
*  uint8 dacValue: Specifies the value to be written to the UV VDAC directly.
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
#define VFD_1_SetUVDacDirect(dacValue)   (VFD_1_VDAC_UV_Data_REG = (dacValue))


/*******************************************************************************
* Function Name: VFD_1_GetUVDacDirect
********************************************************************************
*
* Summary:
*  Returns the current UV VDAC. The returned dacValue is read directly from the
*  VDAC component. Useful for UV VDAC calibration. Note: If this API is called
*  while the component is running, it is not possible to know which voltage input
*  the UV VDAC value is associated with. Call the Pause API to stop the state
*  machine controller if manual UV VDAC control is desired.
*  This API does not apply when ExternalRef=true.
*
* Parameters:
*  None.
*
* Return:
*  A current UV DAC value.
*
*******************************************************************************/   
#define VFD_1_GetUVDacDirect()   VFD_1_VDAC_UV_Data_REG

#endif /* VFD_1_INT_VREF */


/*******************************************************************************
*   Encapsulate function calls into macro helpers to simplify coding
*******************************************************************************/

#define VFD_1_Comp_UV_LOWSPEED   VFD_1_Comp_UV_SLOWSPEED

#if (VFD_1_INT_VREF)
    
/* Initializes UV comparator and threshold generation */
#define VFD_1_UVInit()       do { \
                                            VFD_1_UVRefInit();    \
                                            VFD_1_Comp_UV_Init(); \
                                        } while(0)

/* Enables UV comparator and threshold generation */
#define VFD_1_UVEnable()     do { \
                                            VFD_1_UVRefEnable();    \
                                            VFD_1_Comp_UV_Enable(); \
                                        } while(0)

/* Disables UV comparator and threshold generation */
#define VFD_1_UVStop()       do { \
                                            VFD_1_UVRefStop();    \
                                            VFD_1_Comp_UV_Stop(); \
                                        } while(0)

/* Sets speed mode for UV DAC and comparator. Valid values are LOWSPEED and HIGHSPEED. */
#define VFD_1_SetUVSpeed(speed)  do { \
                                                VFD_1_Comp_UV_SetSpeed(VFD_1_Comp_UV_##speed); \
                                                VFD_1_VDAC_UV_SetSpeed(VFD_1_VDAC_UV_##speed); \
                                            } while(0)
#else
 
/* Initializes OV comparator */
#define VFD_1_UVInit()       VFD_1_Comp_UV_Init()

/* Enables OV comparator */
#define VFD_1_UVEnable()     VFD_1_Comp_UV_Enable()

/* Disables OV comparator */
#define VFD_1_UVStop()       VFD_1_Comp_UV_Stop()

/* Sets speed mode for UV comparator. Valid values are LOWSPEED and HIGHSPEED. */
#define VFD_1_SetUVSpeed(speed)  VFD_1_Comp_UV_SetSpeed(VFD_1_Comp_UV_##speed)

#endif /* VFD_1_INT_VREF */

#endif /* CY_VFD_UV_VFD_1_H */


/* [] END OF FILE */
