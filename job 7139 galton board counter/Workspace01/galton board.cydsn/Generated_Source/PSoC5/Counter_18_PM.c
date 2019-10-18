/*******************************************************************************
* File Name: Counter_18_PM.c  
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

#include "Counter_18.h"

static Counter_18_backupStruct Counter_18_backup;


/*******************************************************************************
* Function Name: Counter_18_SaveConfig
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
*  Counter_18_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_18_SaveConfig(void) 
{
    #if (!Counter_18_UsingFixedFunction)

        Counter_18_backup.CounterUdb = Counter_18_ReadCounter();

        #if(!Counter_18_ControlRegRemoved)
            Counter_18_backup.CounterControlRegister = Counter_18_ReadControlRegister();
        #endif /* (!Counter_18_ControlRegRemoved) */

    #endif /* (!Counter_18_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_18_RestoreConfig
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
*  Counter_18_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_18_RestoreConfig(void) 
{      
    #if (!Counter_18_UsingFixedFunction)

       Counter_18_WriteCounter(Counter_18_backup.CounterUdb);

        #if(!Counter_18_ControlRegRemoved)
            Counter_18_WriteControlRegister(Counter_18_backup.CounterControlRegister);
        #endif /* (!Counter_18_ControlRegRemoved) */

    #endif /* (!Counter_18_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_18_Sleep
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
*  Counter_18_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_18_Sleep(void) 
{
    #if(!Counter_18_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_18_CTRL_ENABLE == (Counter_18_CONTROL & Counter_18_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_18_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_18_backup.CounterEnableState = 0u;
        }
    #else
        Counter_18_backup.CounterEnableState = 1u;
        if(Counter_18_backup.CounterEnableState != 0u)
        {
            Counter_18_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_18_ControlRegRemoved) */
    
    Counter_18_Stop();
    Counter_18_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_18_Wakeup
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
*  Counter_18_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_18_Wakeup(void) 
{
    Counter_18_RestoreConfig();
    #if(!Counter_18_ControlRegRemoved)
        if(Counter_18_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_18_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_18_ControlRegRemoved) */
    
}


/* [] END OF FILE */
