/*******************************************************************************
* File Name: ScanComp_1_Comp.c
* Version 2.0
*
* Description:
*  This file provides the power management source code APIs for the
*  Comparator.
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

#include "ScanComp_1_Comp.h"

static ScanComp_1_Comp_backupStruct ScanComp_1_Comp_backup;


/*******************************************************************************
* Function Name: ScanComp_1_Comp_SaveConfig
********************************************************************************
*
* Summary:
*  Save the current user configuration
*
* Parameters:
*  void:
*
* Return:
*  void
*
*******************************************************************************/
void ScanComp_1_Comp_SaveConfig(void) 
{
    /* Empty since all are system reset for retention flops */
}


/*******************************************************************************
* Function Name: ScanComp_1_Comp_RestoreConfig
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
********************************************************************************/
void ScanComp_1_Comp_RestoreConfig(void) 
{
    /* Empty since all are system reset for retention flops */    
}


/*******************************************************************************
* Function Name: ScanComp_1_Comp_Sleep
********************************************************************************
*
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
*  ScanComp_1_Comp_backup.enableState:  Is modified depending on the enable 
*   state of the block before entering sleep mode.
*
*******************************************************************************/
void ScanComp_1_Comp_Sleep(void) 
{
    /* Save Comp's enable state */    
    if(ScanComp_1_Comp_ACT_PWR_EN == (ScanComp_1_Comp_PWRMGR & ScanComp_1_Comp_ACT_PWR_EN))
    {
        /* Comp is enabled */
        ScanComp_1_Comp_backup.enableState = 1u;
    }
    else
    {
        /* Comp is disabled */
        ScanComp_1_Comp_backup.enableState = 0u;
    }    
    
    ScanComp_1_Comp_Stop();
    ScanComp_1_Comp_SaveConfig();
}


/*******************************************************************************
* Function Name: ScanComp_1_Comp_Wakeup
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
*  ScanComp_1_Comp_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void ScanComp_1_Comp_Wakeup(void) 
{
    ScanComp_1_Comp_RestoreConfig();
    
    if(ScanComp_1_Comp_backup.enableState == 1u)
    {
        /* Enable Comp's operation */
        ScanComp_1_Comp_Enable();

    } /* Do nothing if Comp was disabled before */ 
}


/* [] END OF FILE */
