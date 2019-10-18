/*******************************************************************************
* File Name: VFD_1.c
* Version 3.0
*
* Description:
*  Contains the function prototypes, constants and register definition
*  of the Voltage Fault Detector Component.
*
********************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "VFD_1_UV.h"

/***************************************
*   Internal definitions
***************************************/

#define UV_DMA_DATA_SIZE     1u


/***************************************
*   Internal variables
***************************************/

static  uint8   uvDmaCh;


/***************************************
*   External variables
***************************************/

uint8 VFD_1_UVFaultThreshold[] = {
    247u
};
const uint16 CYCODE VFD_1_initUVFaultThreshold[] = {
    3950u
};


/*******************************************************************************
* Function Name: VFD_1_GetUVFaultThreshold
********************************************************************************
*
* Summary:
*  Returns the under voltage fault threshold for the specified voltage input.
*  This API does not apply when ExternalRef=true. 
*
* Parameters:
*  uint8 voltageNum: Specifies the voltage input number. Valid range: 1..32.
*
* Return:
*  The over voltage fault threshold in mV. Valid range: 1..65,535.
*
*******************************************************************************/
uint16 VFD_1_GetUVFaultThreshold(uint8 voltageNum) 
{   
    uint32 threshold;

    /* Voltages are stored in decreasing order to reflect scanning sequence */
    voltageNum = VFD_1_NUMBER_OF_VOLTAGES - voltageNum;

	threshold = ((uint32)VFD_1_UVFaultThreshold[voltageNum] * VFD_1_DAC_mV_LSb * 1000u) /
		VFD_1_VoltageScale[voltageNum];
		
	return ((uint16)(threshold));		
}


/*******************************************************************************
* Function Name: VFD_1_SetUVFaultThreshold
********************************************************************************
*
* Summary:
*  Sets the under voltage fault threshold for the specified voltage input.
*  Calculates the VDAC value to reflect a new UV threshold and writes it to an
*  SRAM buffer to be used by the DMA controller that drives the UV DAC.
*  This API does not apply when ExternalRef=true.
*
* Parameters:
*  voltageNum:       Specifies the voltage input number. Valid range: 1..32.
*  uvFaultThreshold: Specifies the under voltage fault threshold in mV. 
*                    Valid range: 1..65,535.
*
* Return:
*  CYRET_SUCCESS  : Operation is successful.
*  CYRET_BAD_PARAM: The fault threshold exceeds DAC max value.
*  
* Reentrant:
*  No.
*
*******************************************************************************/
cystatus VFD_1_SetUVFaultThreshold(uint8 voltageNum, uint16 uvFaultThreshold) 
{
    cystatus    status;
    uint32      threshold;

    /* Voltages are stored in decreasing order to reflect scanning sequence */
    voltageNum = VFD_1_NUMBER_OF_VOLTAGES - voltageNum;

    /* UV threshold is scaled to fall within selected VDAC range */
    threshold = uvFaultThreshold * (uint32)VFD_1_VoltageScale[voltageNum];

    /* Saturate threshold to DAC range limit */
    if(threshold > VFD_1_FAULT_THRESHOLD_SAT_H)
    {
        VFD_1_UVFaultThreshold[voltageNum] = VFD_1_DAC_MAX;
        status = CYRET_BAD_PARAM;
    }
    else
    {
        VFD_1_UVFaultThreshold[voltageNum] = (uint8)VFD_1_ConvertDacVoltageToValue(threshold);
        status = CYRET_SUCCESS;
    }
    return (status);
}


/*******************************************************************************
* Function Name: VFD_1_UVRefInit
********************************************************************************
*
* Summary:
*  Initializes the internal UV DAC and configures a DMA controller to transfer
*  UV thresholds from the SRAM array to the DAC data register.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void VFD_1_UVRefInit(void) 
{
    static uint8 uvDmaTd = CY_DMA_INVALID_TD;

    VFD_1_VDAC_UV_Init();

    /* Configure UV DMA first time VFD_1_UVRefInit is called */
    if(uvDmaTd == CY_DMA_INVALID_TD)
    {
        uvDmaTd = CyDmaTdAllocate();
        uvDmaCh = VFD_1_DMA_CFG(UvDMA, UV_DMA_DATA_SIZE, HI16(CYDEV_SRAM_BASE), HI16(CYDEV_PERIPH_BASE));
        (void)CyDmaTdSetConfiguration(uvDmaTd, VFD_1_NUMBER_OF_VOLTAGES, uvDmaTd, CY_DMA_TD_INC_SRC_ADR);
        (void)CyDmaTdSetAddress(uvDmaTd, LO16((uint32)VFD_1_UVFaultThreshold), LO16((uint32)VFD_1_VDAC_UV_Data_PTR));
        (void)CyDmaChSetInitialTd(uvDmaCh, uvDmaTd);   
    }
}

/*******************************************************************************
* Function Name: VFD_1_UVRefEnable
********************************************************************************
*
* Summary:
*  Enables UV DAC and the corresponding DMA channel.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void VFD_1_UVRefEnable(void) 
{
    VFD_1_VDAC_UV_Enable();
    (void)CyDmaChEnable(uvDmaCh, 1u);
}


/*******************************************************************************
* Function Name: VFD_1_UVRefDisable
********************************************************************************
*
* Summary:
*  Stops UV DAC. Terminates and disables the DMA channel that drives UV DAC.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void VFD_1_UVRefStop(void) 
{
    VFD_1_VDAC_UV_Stop();
    (void)CyDmaChSetRequest(uvDmaCh, CY_DMA_CPU_TERM_CHAIN);
    (void)CyDmaClearPendingDrq(uvDmaCh);
}


/* [] END OF FILE */
