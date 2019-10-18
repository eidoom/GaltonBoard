/*******************************************************************************
* File Name: Counter_9_PM.c  
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

#include "Counter_9.h"

static Counter_9_backupStruct Counter_9_backup;


/*******************************************************************************
* Function Name: Counter_9_SaveConfig
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
*  Counter_9_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_9_SaveConfig(void) 
{
    #if (!Counter_9_UsingFixedFunction)

        Counter_9_backup.CounterUdb = Counter_9_ReadCounter();

        #if(!Counter_9_ControlRegRemoved)
            Counter_9_backup.CounterControlRegister = Counter_9_ReadControlRegister();
        #endif /* (!Counter_9_ControlRegRemoved) */

    #endif /* (!Counter_9_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_9_RestoreConfig
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
*  Counter_9_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_9_RestoreConfig(void) 
{      
    #if (!Counter_9_UsingFixedFunction)

       Counter_9_WriteCounter(Counter_9_backup.CounterUdb);

        #if(!Counter_9_ControlRegRemoved)
            Counter_9_WriteControlRegister(Counter_9_backup.CounterControlRegister);
        #endif /* (!Counter_9_ControlRegRemoved) */

    #endif /* (!Counter_9_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_9_Sleep
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
*  Counter_9_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_9_Sleep(void) 
{
    #if(!Counter_9_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_9_CTRL_ENABLE == (Counter_9_CONTROL & Counter_9_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_9_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_9_backup.CounterEnableState = 0u;
        }
    #else
        Counter_9_backup.CounterEnableState = 1u;
        if(Counter_9_backup.CounterEnableState != 0u)
        {
            Counter_9_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_9_ControlRegRemoved) */
    
    Counter_9_Stop();
    Counter_9_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_9_Wakeup
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
*  Counter_9_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_9_Wakeup(void) 
{
    Counter_9_RestoreConfig();
    #if(!Counter_9_ControlRegRemoved)
        if(Counter_9_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_9_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_9_ControlRegRemoved) */
    
}


/* [] END OF FILE */
