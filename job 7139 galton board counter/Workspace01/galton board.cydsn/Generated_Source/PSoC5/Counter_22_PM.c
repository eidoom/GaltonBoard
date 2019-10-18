/*******************************************************************************
* File Name: Counter_22_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Counter_22.h"

static Counter_22_backupStruct Counter_22_backup;


/*******************************************************************************
* Function Name: Counter_22_SaveConfig
********************************************************************************
* Summary:
*     Save the current user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Counter_22_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_22_SaveConfig(void) 
{
    #if (!Counter_22_UsingFixedFunction)

        Counter_22_backup.CounterUdb = Counter_22_ReadCounter();

        #if(!Counter_22_ControlRegRemoved)
            Counter_22_backup.CounterControlRegister = Counter_22_ReadControlRegister();
        #endif /* (!Counter_22_ControlRegRemoved) */

    #endif /* (!Counter_22_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_22_RestoreConfig
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
* Global variables:
*  Counter_22_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_22_RestoreConfig(void) 
{      
    #if (!Counter_22_UsingFixedFunction)

       Counter_22_WriteCounter(Counter_22_backup.CounterUdb);

        #if(!Counter_22_ControlRegRemoved)
            Counter_22_WriteControlRegister(Counter_22_backup.CounterControlRegister);
        #endif /* (!Counter_22_ControlRegRemoved) */

    #endif /* (!Counter_22_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_22_Sleep
********************************************************************************
* Summary:
*     Stop and Save the user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Counter_22_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_22_Sleep(void) 
{
    #if(!Counter_22_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_22_CTRL_ENABLE == (Counter_22_CONTROL & Counter_22_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_22_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_22_backup.CounterEnableState = 0u;
        }
    #else
        Counter_22_backup.CounterEnableState = 1u;
        if(Counter_22_backup.CounterEnableState != 0u)
        {
            Counter_22_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_22_ControlRegRemoved) */
    
    Counter_22_Stop();
    Counter_22_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_22_Wakeup
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
*  Counter_22_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_22_Wakeup(void) 
{
    Counter_22_RestoreConfig();
    #if(!Counter_22_ControlRegRemoved)
        if(Counter_22_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_22_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_22_ControlRegRemoved) */
    
}


/* [] END OF FILE */
