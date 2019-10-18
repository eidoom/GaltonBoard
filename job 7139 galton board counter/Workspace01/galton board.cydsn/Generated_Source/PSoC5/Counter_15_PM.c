/*******************************************************************************
* File Name: Counter_15_PM.c  
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

#include "Counter_15.h"

static Counter_15_backupStruct Counter_15_backup;


/*******************************************************************************
* Function Name: Counter_15_SaveConfig
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
*  Counter_15_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_15_SaveConfig(void) 
{
    #if (!Counter_15_UsingFixedFunction)

        Counter_15_backup.CounterUdb = Counter_15_ReadCounter();

        #if(!Counter_15_ControlRegRemoved)
            Counter_15_backup.CounterControlRegister = Counter_15_ReadControlRegister();
        #endif /* (!Counter_15_ControlRegRemoved) */

    #endif /* (!Counter_15_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_15_RestoreConfig
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
*  Counter_15_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_15_RestoreConfig(void) 
{      
    #if (!Counter_15_UsingFixedFunction)

       Counter_15_WriteCounter(Counter_15_backup.CounterUdb);

        #if(!Counter_15_ControlRegRemoved)
            Counter_15_WriteControlRegister(Counter_15_backup.CounterControlRegister);
        #endif /* (!Counter_15_ControlRegRemoved) */

    #endif /* (!Counter_15_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_15_Sleep
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
*  Counter_15_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_15_Sleep(void) 
{
    #if(!Counter_15_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_15_CTRL_ENABLE == (Counter_15_CONTROL & Counter_15_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_15_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_15_backup.CounterEnableState = 0u;
        }
    #else
        Counter_15_backup.CounterEnableState = 1u;
        if(Counter_15_backup.CounterEnableState != 0u)
        {
            Counter_15_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_15_ControlRegRemoved) */
    
    Counter_15_Stop();
    Counter_15_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_15_Wakeup
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
*  Counter_15_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_15_Wakeup(void) 
{
    Counter_15_RestoreConfig();
    #if(!Counter_15_ControlRegRemoved)
        if(Counter_15_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_15_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_15_ControlRegRemoved) */
    
}


/* [] END OF FILE */
