/*******************************************************************************
* File Name: Counter_13_PM.c  
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

#include "Counter_13.h"

static Counter_13_backupStruct Counter_13_backup;


/*******************************************************************************
* Function Name: Counter_13_SaveConfig
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
*  Counter_13_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_13_SaveConfig(void) 
{
    #if (!Counter_13_UsingFixedFunction)

        Counter_13_backup.CounterUdb = Counter_13_ReadCounter();

        #if(!Counter_13_ControlRegRemoved)
            Counter_13_backup.CounterControlRegister = Counter_13_ReadControlRegister();
        #endif /* (!Counter_13_ControlRegRemoved) */

    #endif /* (!Counter_13_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_13_RestoreConfig
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
*  Counter_13_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_13_RestoreConfig(void) 
{      
    #if (!Counter_13_UsingFixedFunction)

       Counter_13_WriteCounter(Counter_13_backup.CounterUdb);

        #if(!Counter_13_ControlRegRemoved)
            Counter_13_WriteControlRegister(Counter_13_backup.CounterControlRegister);
        #endif /* (!Counter_13_ControlRegRemoved) */

    #endif /* (!Counter_13_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_13_Sleep
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
*  Counter_13_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_13_Sleep(void) 
{
    #if(!Counter_13_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_13_CTRL_ENABLE == (Counter_13_CONTROL & Counter_13_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_13_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_13_backup.CounterEnableState = 0u;
        }
    #else
        Counter_13_backup.CounterEnableState = 1u;
        if(Counter_13_backup.CounterEnableState != 0u)
        {
            Counter_13_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_13_ControlRegRemoved) */
    
    Counter_13_Stop();
    Counter_13_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_13_Wakeup
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
*  Counter_13_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_13_Wakeup(void) 
{
    Counter_13_RestoreConfig();
    #if(!Counter_13_ControlRegRemoved)
        if(Counter_13_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_13_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_13_ControlRegRemoved) */
    
}


/* [] END OF FILE */
