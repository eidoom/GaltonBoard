/*******************************************************************************
* File Name: Counter_21_PM.c  
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

#include "Counter_21.h"

static Counter_21_backupStruct Counter_21_backup;


/*******************************************************************************
* Function Name: Counter_21_SaveConfig
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
*  Counter_21_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_21_SaveConfig(void) 
{
    #if (!Counter_21_UsingFixedFunction)

        Counter_21_backup.CounterUdb = Counter_21_ReadCounter();

        #if(!Counter_21_ControlRegRemoved)
            Counter_21_backup.CounterControlRegister = Counter_21_ReadControlRegister();
        #endif /* (!Counter_21_ControlRegRemoved) */

    #endif /* (!Counter_21_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_21_RestoreConfig
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
*  Counter_21_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_21_RestoreConfig(void) 
{      
    #if (!Counter_21_UsingFixedFunction)

       Counter_21_WriteCounter(Counter_21_backup.CounterUdb);

        #if(!Counter_21_ControlRegRemoved)
            Counter_21_WriteControlRegister(Counter_21_backup.CounterControlRegister);
        #endif /* (!Counter_21_ControlRegRemoved) */

    #endif /* (!Counter_21_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_21_Sleep
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
*  Counter_21_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_21_Sleep(void) 
{
    #if(!Counter_21_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_21_CTRL_ENABLE == (Counter_21_CONTROL & Counter_21_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_21_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_21_backup.CounterEnableState = 0u;
        }
    #else
        Counter_21_backup.CounterEnableState = 1u;
        if(Counter_21_backup.CounterEnableState != 0u)
        {
            Counter_21_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_21_ControlRegRemoved) */
    
    Counter_21_Stop();
    Counter_21_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_21_Wakeup
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
*  Counter_21_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_21_Wakeup(void) 
{
    Counter_21_RestoreConfig();
    #if(!Counter_21_ControlRegRemoved)
        if(Counter_21_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_21_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_21_ControlRegRemoved) */
    
}


/* [] END OF FILE */
