/*******************************************************************************
* File Name: ScanComp_1_VDAC_PM.c  
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

#include "ScanComp_1_VDAC.h"

static ScanComp_1_VDAC_backupStruct ScanComp_1_VDAC_backup;


/*******************************************************************************
* Function Name: ScanComp_1_VDAC_SaveConfig
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
void ScanComp_1_VDAC_SaveConfig(void) 
{
    if (!((ScanComp_1_VDAC_CR1 & ScanComp_1_VDAC_SRC_MASK) == ScanComp_1_VDAC_SRC_UDB))
    {
        ScanComp_1_VDAC_backup.data_value = ScanComp_1_VDAC_Data;
    }
}


/*******************************************************************************
* Function Name: ScanComp_1_VDAC_RestoreConfig
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
void ScanComp_1_VDAC_RestoreConfig(void) 
{
    if (!((ScanComp_1_VDAC_CR1 & ScanComp_1_VDAC_SRC_MASK) == ScanComp_1_VDAC_SRC_UDB))
    {
        if((ScanComp_1_VDAC_Strobe & ScanComp_1_VDAC_STRB_MASK) == ScanComp_1_VDAC_STRB_EN)
        {
            ScanComp_1_VDAC_Strobe &= (uint8)(~ScanComp_1_VDAC_STRB_MASK);
            ScanComp_1_VDAC_Data = ScanComp_1_VDAC_backup.data_value;
            ScanComp_1_VDAC_Strobe |= ScanComp_1_VDAC_STRB_EN;
        }
        else
        {
            ScanComp_1_VDAC_Data = ScanComp_1_VDAC_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: ScanComp_1_VDAC_Sleep
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
*  ScanComp_1_VDAC_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void ScanComp_1_VDAC_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(ScanComp_1_VDAC_ACT_PWR_EN == (ScanComp_1_VDAC_PWRMGR & ScanComp_1_VDAC_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        ScanComp_1_VDAC_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        ScanComp_1_VDAC_backup.enableState = 0u;
    }
    
    ScanComp_1_VDAC_Stop();
    ScanComp_1_VDAC_SaveConfig();
}


/*******************************************************************************
* Function Name: ScanComp_1_VDAC_Wakeup
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
*  ScanComp_1_VDAC_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void ScanComp_1_VDAC_Wakeup(void) 
{
    ScanComp_1_VDAC_RestoreConfig();
    
    if(ScanComp_1_VDAC_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        ScanComp_1_VDAC_Enable();

        /* Restore the data register */
        ScanComp_1_VDAC_SetValue(ScanComp_1_VDAC_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
