/*******************************************************************************
* File Name: Counter_12_PM.c  
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

#include "Counter_12.h"

static Counter_12_backupStruct Counter_12_backup;


/*******************************************************************************
* Function Name: Counter_12_SaveConfig
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
*  Counter_12_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_12_SaveConfig(void) 
{
    #if (!Counter_12_UsingFixedFunction)

        Counter_12_backup.CounterUdb = Counter_12_ReadCounter();

        #if(!Counter_12_ControlRegRemoved)
            Counter_12_backup.CounterControlRegister = Counter_12_ReadControlRegister();
        #endif /* (!Counter_12_ControlRegRemoved) */

    #endif /* (!Counter_12_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_12_RestoreConfig
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
*  Counter_12_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_12_RestoreConfig(void) 
{      
    #if (!Counter_12_UsingFixedFunction)

       Counter_12_WriteCounter(Counter_12_backup.CounterUdb);

        #if(!Counter_12_ControlRegRemoved)
            Counter_12_WriteControlRegister(Counter_12_backup.CounterControlRegister);
        #endif /* (!Counter_12_ControlRegRemoved) */

    #endif /* (!Counter_12_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_12_Sleep
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
*  Counter_12_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_12_Sleep(void) 
{
    #if(!Counter_12_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_12_CTRL_ENABLE == (Counter_12_CONTROL & Counter_12_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_12_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_12_backup.CounterEnableState = 0u;
        }
    #else
        Counter_12_backup.CounterEnableState = 1u;
        if(Counter_12_backup.CounterEnableState != 0u)
        {
            Counter_12_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_12_ControlRegRemoved) */
    
    Counter_12_Stop();
    Counter_12_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_12_Wakeup
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
*  Counter_12_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_12_Wakeup(void) 
{
    Counter_12_RestoreConfig();
    #if(!Counter_12_ControlRegRemoved)
        if(Counter_12_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_12_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_12_ControlRegRemoved) */
    
}


/* [] END OF FILE */
