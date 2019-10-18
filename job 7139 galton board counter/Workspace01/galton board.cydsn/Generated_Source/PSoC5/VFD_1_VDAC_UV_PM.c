/*******************************************************************************
* File Name: VFD_1_VDAC_UV_PM.c  
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

#include "VFD_1_VDAC_UV.h"

static VFD_1_VDAC_UV_backupStruct VFD_1_VDAC_UV_backup;


/*******************************************************************************
* Function Name: VFD_1_VDAC_UV_SaveConfig
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
void VFD_1_VDAC_UV_SaveConfig(void) 
{
    if (!((VFD_1_VDAC_UV_CR1 & VFD_1_VDAC_UV_SRC_MASK) == VFD_1_VDAC_UV_SRC_UDB))
    {
        VFD_1_VDAC_UV_backup.data_value = VFD_1_VDAC_UV_Data;
    }
}


/*******************************************************************************
* Function Name: VFD_1_VDAC_UV_RestoreConfig
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
void VFD_1_VDAC_UV_RestoreConfig(void) 
{
    if (!((VFD_1_VDAC_UV_CR1 & VFD_1_VDAC_UV_SRC_MASK) == VFD_1_VDAC_UV_SRC_UDB))
    {
        if((VFD_1_VDAC_UV_Strobe & VFD_1_VDAC_UV_STRB_MASK) == VFD_1_VDAC_UV_STRB_EN)
        {
            VFD_1_VDAC_UV_Strobe &= (uint8)(~VFD_1_VDAC_UV_STRB_MASK);
            VFD_1_VDAC_UV_Data = VFD_1_VDAC_UV_backup.data_value;
            VFD_1_VDAC_UV_Strobe |= VFD_1_VDAC_UV_STRB_EN;
        }
        else
        {
            VFD_1_VDAC_UV_Data = VFD_1_VDAC_UV_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: VFD_1_VDAC_UV_Sleep
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
*  VFD_1_VDAC_UV_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void VFD_1_VDAC_UV_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(VFD_1_VDAC_UV_ACT_PWR_EN == (VFD_1_VDAC_UV_PWRMGR & VFD_1_VDAC_UV_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        VFD_1_VDAC_UV_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        VFD_1_VDAC_UV_backup.enableState = 0u;
    }
    
    VFD_1_VDAC_UV_Stop();
    VFD_1_VDAC_UV_SaveConfig();
}


/*******************************************************************************
* Function Name: VFD_1_VDAC_UV_Wakeup
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
*  VFD_1_VDAC_UV_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void VFD_1_VDAC_UV_Wakeup(void) 
{
    VFD_1_VDAC_UV_RestoreConfig();
    
    if(VFD_1_VDAC_UV_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        VFD_1_VDAC_UV_Enable();

        /* Restore the data register */
        VFD_1_VDAC_UV_SetValue(VFD_1_VDAC_UV_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
