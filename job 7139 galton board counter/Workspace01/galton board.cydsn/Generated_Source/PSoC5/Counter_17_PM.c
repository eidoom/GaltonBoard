/*******************************************************************************
* File Name: Counter_17_PM.c  
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

#include "Counter_17.h"

static Counter_17_backupStruct Counter_17_backup;


/*******************************************************************************
* Function Name: Counter_17_SaveConfig
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
*  Counter_17_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_17_SaveConfig(void) 
{
    #if (!Counter_17_UsingFixedFunction)

        Counter_17_backup.CounterUdb = Counter_17_ReadCounter();

        #if(!Counter_17_ControlRegRemoved)
            Counter_17_backup.CounterControlRegister = Counter_17_ReadControlRegister();
        #endif /* (!Counter_17_ControlRegRemoved) */

    #endif /* (!Counter_17_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_17_RestoreConfig
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
*  Counter_17_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_17_RestoreConfig(void) 
{      
    #if (!Counter_17_UsingFixedFunction)

       Counter_17_WriteCounter(Counter_17_backup.CounterUdb);

        #if(!Counter_17_ControlRegRemoved)
            Counter_17_WriteControlRegister(Counter_17_backup.CounterControlRegister);
        #endif /* (!Counter_17_ControlRegRemoved) */

    #endif /* (!Counter_17_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_17_Sleep
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
*  Counter_17_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_17_Sleep(void) 
{
    #if(!Counter_17_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_17_CTRL_ENABLE == (Counter_17_CONTROL & Counter_17_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_17_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_17_backup.CounterEnableState = 0u;
        }
    #else
        Counter_17_backup.CounterEnableState = 1u;
        if(Counter_17_backup.CounterEnableState != 0u)
        {
            Counter_17_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_17_ControlRegRemoved) */
    
    Counter_17_Stop();
    Counter_17_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_17_Wakeup
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
*  Counter_17_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_17_Wakeup(void) 
{
    Counter_17_RestoreConfig();
    #if(!Counter_17_ControlRegRemoved)
        if(Counter_17_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_17_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_17_ControlRegRemoved) */
    
}


/* [] END OF FILE */
