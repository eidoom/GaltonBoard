/*******************************************************************************
* File Name: VFD_1.c
* Version 3.0
*
* Description:
*  Contains the function prototypes, constants and register definition
*  of the Voltage Fault Detector Component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "VFD_1_OV.h"

/***************************************
*   Internal definitions
***************************************/

#define OV_DMA_DATA_SIZE     1u


/***************************************
*   Internal variables
***************************************/

static  uint8   ovDmaCh;


/***************************************
*   External variables
***************************************/

uint8 VFD_1_OVFaultThreshold[] = {
    250u
};
const uint16 CYCODE VFD_1_initOVFaultThreshold[] = {
    4000u
};


/*******************************************************************************
* Function Name: VFD_1_GetOVFaultThreshold
********************************************************************************
*
* Summary:
*  Returns the over voltage fault threshold for the specified voltage input.
*  This API does not apply when ExternalRef=true.
*
* Parameters:
*  uint8 voltageNum: Specifies the voltage input number. Valid range: 1..32.
*
* Return:
*  The over voltage fault threshold in mV. Valid range: 1..65,535.
*
*******************************************************************************/
uint16 VFD_1_GetOVFaultThreshold(uint8 voltageNum) 
{   
    uint32 threshold;

    /* Voltages are stored in decreasing order to reflect scanning sequence */
    voltageNum = VFD_1_NUMBER_OF_VOLTAGES - voltageNum;

	threshold = ((uint32)VFD_1_OVFaultThreshold[voltageNum] * VFD_1_DAC_mV_LSb * 1000u) /
		VFD_1_VoltageScale[voltageNum];
		
	return ((uint16)(threshold));
}


/*******************************************************************************
* Function Name: VFD_1_SetOVFaultThreshold
********************************************************************************
*
* Summary:
*  Sets the over voltage fault threshold for the specified voltage input.
*  Calculates the VDAC value to reflect a new OV threshold and writes it to an
*  SRAM buffer to be used by the DMA controller that drives the OV DAC.
*  This API does not apply when ExternalRef=true.
*
* Parameters:
*  voltageNum:       Specifies the voltage input number. Valid range: 1..32.
*  ovFaultThreshold: Specifies the over voltage fault threshold in mV. 
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
cystatus VFD_1_SetOVFaultThreshold(uint8 voltageNum, uint16 ovFaultThreshold) 
{
    cystatus    status;
    uint32      threshold;

    /* Voltages are stored in decreasing order to reflect scanning sequence */
    voltageNum = VFD_1_NUMBER_OF_VOLTAGES - voltageNum;

    /* OV threshold is scaled to fall within selected VDAC range */
    threshold = ovFaultThreshold * (uint32)VFD_1_VoltageScale[voltageNum];

    /* Saturate threshold to DAC range limit */
    if(threshold > VFD_1_FAULT_THRESHOLD_SAT_H)
    {
        VFD_1_OVFaultThreshold[voltageNum] = VFD_1_DAC_MAX;
        status = CYRET_BAD_PARAM;
    }
    else
    {
        VFD_1_OVFaultThreshold[voltageNum] = (uint8)VFD_1_ConvertDacVoltageToValue(threshold);
        status = CYRET_SUCCESS;
    }
    return (status);
}

    
/*******************************************************************************
* Function Name: VFD_1_OVRefInit
********************************************************************************
*
* Summary:
*  Initializes the internal OV DAC and configures a DMA controller to transfer
*  OV thresholds from the SRAM array to the DAC data register.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void VFD_1_OVRefInit(void) 
{
    static uint8 ovDmaTd = CY_DMA_INVALID_TD;

    VFD_1_VDAC_OV_Init();

    /* Configure OV DMA first time VFD_1_OVRefInit is called */
    if(ovDmaTd == CY_DMA_INVALID_TD)
    {
        ovDmaTd = CyDmaTdAllocate();
        ovDmaCh = VFD_1_DMA_CFG(OvDMA, OV_DMA_DATA_SIZE, HI16(CYDEV_SRAM_BASE), HI16(CYDEV_PERIPH_BASE));
        (void)CyDmaTdSetConfiguration(ovDmaTd, VFD_1_NUMBER_OF_VOLTAGES, ovDmaTd, CY_DMA_TD_INC_SRC_ADR);
        (void)CyDmaTdSetAddress(ovDmaTd, LO16((uint32)VFD_1_OVFaultThreshold), LO16((uint32)VFD_1_VDAC_OV_Data_PTR));
        (void)CyDmaChSetInitialTd(ovDmaCh, ovDmaTd);
    }
}

/*******************************************************************************
* Function Name: VFD_1_OVRefEnable
********************************************************************************
*
* Summary:
*  Enables OV DAC and the corresponding DMA channel.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void VFD_1_OVRefEnable(void) 
{
    VFD_1_VDAC_OV_Enable();
    (void)CyDmaChEnable(ovDmaCh, 1u);
}


/*******************************************************************************
* Function Name: VFD_1_OVRefDisable
********************************************************************************
*
* Summary:
*  Stops OV DAC. Terminates and disables the DMA channel that drives OV DAC.
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
void VFD_1_OVRefStop(void) 
{
    VFD_1_VDAC_OV_Stop();
    (void)CyDmaChSetRequest(ovDmaCh, CY_DMA_CPU_TERM_CHAIN);
    (void)CyDmaClearPendingDrq(ovDmaCh);
}


/* [] END OF FILE */
