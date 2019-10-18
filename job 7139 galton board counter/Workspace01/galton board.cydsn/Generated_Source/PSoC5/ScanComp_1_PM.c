/*******************************************************************************
* File Name: ScanComp_1_PM.c
* Version 1.10
*
* Description:
*  This file provides Sleep APIs for Scanning Comparator component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ScanComp_1.h"

static ScanComp_1_BACKUP_STRUCT ScanComp_1_backup;


/*******************************************************************************
* Function Name: ScanComp_1_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for low power mode operation.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ScanComp_1_Sleep(void) 
{
    if(0u != (ScanComp_1_CONTROL_REG & ScanComp_1_CONTROL_ENABLE))
    {
        ScanComp_1_backup.enableState = 1u;
        ScanComp_1_Stop();
    }
    else /* The component is disabled */
    {
        ScanComp_1_backup.enableState = 0u;
    }

    ScanComp_1_Comp_Sleep();

    #if (ScanComp_1_INTERNAL_REF_IS_USED)
        ScanComp_1_VDAC_Sleep();
    #endif /* ScanComp_1_INTERNAL_REF_IS_USED */
}


/*******************************************************************************
* Function Name: ScanComp_1_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component to the state when ScanComp_1_Sleep() was called.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ScanComp_1_Wakeup(void) 
{
    ScanComp_1_Comp_Wakeup();
    
    #if (ScanComp_1_INTERNAL_REF_IS_USED)
        ScanComp_1_VDAC_Start();

        #if (ScanComp_1_VDAC_PER_CHANNEL_MODE_USED)
            ScanComp_1_Init();
        #endif /* ScanComp_1_VDAC_PER_CHANNEL_MODE_USED */
    #endif /* ScanComp_1_INTERNAL_REF_IS_USED */

    if(0u != ScanComp_1_backup.enableState)
    {
        ScanComp_1_Enable();
    }
}


/* [] END OF FILE */
