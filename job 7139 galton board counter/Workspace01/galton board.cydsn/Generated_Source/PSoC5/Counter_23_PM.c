/*******************************************************************************
* File Name: Counter_23_PM.c  
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

#include "Counter_23.h"

static Counter_23_backupStruct Counter_23_backup;


/*******************************************************************************
* Function Name: Counter_23_SaveConfig
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
*  Counter_23_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_23_SaveConfig(void) 
{
    #if (!Counter_23_UsingFixedFunction)

        Counter_23_backup.CounterUdb = Counter_23_ReadCounter();

        #if(!Counter_23_ControlRegRemoved)
            Counter_23_backup.CounterControlRegister = Counter_23_ReadControlRegister();
        #endif /* (!Counter_23_ControlRegRemoved) */

    #endif /* (!Counter_23_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_23_RestoreConfig
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
*  Counter_23_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_23_RestoreConfig(void) 
{      
    #if (!Counter_23_UsingFixedFunction)

       Counter_23_WriteCounter(Counter_23_backup.CounterUdb);

        #if(!Counter_23_ControlRegRemoved)
            Counter_23_WriteControlRegister(Counter_23_backup.CounterControlRegister);
        #endif /* (!Counter_23_ControlRegRemoved) */

    #endif /* (!Counter_23_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_23_Sleep
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
*  Counter_23_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_23_Sleep(void) 
{
    #if(!Counter_23_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_23_CTRL_ENABLE == (Counter_23_CONTROL & Counter_23_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_23_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_23_backup.CounterEnableState = 0u;
        }
    #else
        Counter_23_backup.CounterEnableState = 1u;
        if(Counter_23_backup.CounterEnableState != 0u)
        {
            Counter_23_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_23_ControlRegRemoved) */
    
    Counter_23_Stop();
    Counter_23_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_23_Wakeup
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
*  Counter_23_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_23_Wakeup(void) 
{
    Counter_23_RestoreConfig();
    #if(!Counter_23_ControlRegRemoved)
        if(Counter_23_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_23_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_23_ControlRegRemoved) */
    
}


/* [] END OF FILE */
