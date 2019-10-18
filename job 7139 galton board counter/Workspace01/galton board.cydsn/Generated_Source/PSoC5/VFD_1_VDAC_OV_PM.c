/*******************************************************************************
* File Name: VFD_1_VDAC_OV_PM.c  
* Version 1.90
*
* Description:
*  This file provides the power management source code to API for the
*  VDAC8.  
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "VFD_1_VDAC_OV.h"

static VFD_1_VDAC_OV_backupStruct VFD_1_VDAC_OV_backup;


/*******************************************************************************
* Function Name: VFD_1_VDAC_OV_SaveConfig
********************************************************************************
* Summary:
*  Save the current user configuration
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void VFD_1_VDAC_OV_SaveConfig(void) 
{
    if (!((VFD_1_VDAC_OV_CR1 & VFD_1_VDAC_OV_SRC_MASK) == VFD_1_VDAC_OV_SRC_UDB))
    {
        VFD_1_VDAC_OV_backup.data_value = VFD_1_VDAC_OV_Data;
    }
}


/*******************************************************************************
* Function Name: VFD_1_VDAC_OV_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
*******************************************************************************/
void VFD_1_VDAC_OV_RestoreConfig(void) 
{
    if (!((VFD_1_VDAC_OV_CR1 & VFD_1_VDAC_OV_SRC_MASK) == VFD_1_VDAC_OV_SRC_UDB))
    {
        if((VFD_1_VDAC_OV_Strobe & VFD_1_VDAC_OV_STRB_MASK) == VFD_1_VDAC_OV_STRB_EN)
        {
            VFD_1_VDAC_OV_Strobe &= (uint8)(~VFD_1_VDAC_OV_STRB_MASK);
            VFD_1_VDAC_OV_Data = VFD_1_VDAC_OV_backup.data_value;
            VFD_1_VDAC_OV_Strobe |= VFD_1_VDAC_OV_STRB_EN;
        }
        else
        {
            VFD_1_VDAC_OV_Data = VFD_1_VDAC_OV_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: VFD_1_VDAC_OV_Sleep
********************************************************************************
* Summary:
*  Stop and Save the user configuration
*
* Parameters:  
*  void:  
*
* Return: 
*  void
*
* Global variables:
*  VFD_1_VDAC_OV_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void VFD_1_VDAC_OV_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(VFD_1_VDAC_OV_ACT_PWR_EN == (VFD_1_VDAC_OV_PWRMGR & VFD_1_VDAC_OV_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        VFD_1_VDAC_OV_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        VFD_1_VDAC_OV_backup.enableState = 0u;
    }
    
    VFD_1_VDAC_OV_Stop();
    VFD_1_VDAC_OV_SaveConfig();
}


/*******************************************************************************
* Function Name: VFD_1_VDAC_OV_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  VFD_1_VDAC_OV_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void VFD_1_VDAC_OV_Wakeup(void) 
{
    VFD_1_VDAC_OV_RestoreConfig();
    
    if(VFD_1_VDAC_OV_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        VFD_1_VDAC_OV_Enable();

        /* Restore the data register */
        VFD_1_VDAC_OV_SetValue(VFD_1_VDAC_OV_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
