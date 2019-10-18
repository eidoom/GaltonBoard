/*******************************************************************************
* File Name: Counter_14_PM.c  
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

#include "Counter_14.h"

static Counter_14_backupStruct Counter_14_backup;


/*******************************************************************************
* Function Name: Counter_14_SaveConfig
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
*  Counter_14_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_14_SaveConfig(void) 
{
    #if (!Counter_14_UsingFixedFunction)

        Counter_14_backup.CounterUdb = Counter_14_ReadCounter();

        #if(!Counter_14_ControlRegRemoved)
            Counter_14_backup.CounterControlRegister = Counter_14_ReadControlRegister();
        #endif /* (!Counter_14_ControlRegRemoved) */

    #endif /* (!Counter_14_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_14_RestoreConfig
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
*  Counter_14_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_14_RestoreConfig(void) 
{      
    #if (!Counter_14_UsingFixedFunction)

       Counter_14_WriteCounter(Counter_14_backup.CounterUdb);

        #if(!Counter_14_ControlRegRemoved)
            Counter_14_WriteControlRegister(Counter_14_backup.CounterControlRegister);
        #endif /* (!Counter_14_ControlRegRemoved) */

    #endif /* (!Counter_14_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_14_Sleep
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
*  Counter_14_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_14_Sleep(void) 
{
    #if(!Counter_14_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_14_CTRL_ENABLE == (Counter_14_CONTROL & Counter_14_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_14_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_14_backup.CounterEnableState = 0u;
        }
    #else
        Counter_14_backup.CounterEnableState = 1u;
        if(Counter_14_backup.CounterEnableState != 0u)
        {
            Counter_14_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_14_ControlRegRemoved) */
    
    Counter_14_Stop();
    Counter_14_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_14_Wakeup
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
*  Counter_14_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_14_Wakeup(void) 
{
    Counter_14_RestoreConfig();
    #if(!Counter_14_ControlRegRemoved)
        if(Counter_14_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_14_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_14_ControlRegRemoved) */
    
}


/* [] END OF FILE */
