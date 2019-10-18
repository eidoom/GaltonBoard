/*******************************************************************************
* File Name: Counter_11_PM.c  
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

#include "Counter_11.h"

static Counter_11_backupStruct Counter_11_backup;


/*******************************************************************************
* Function Name: Counter_11_SaveConfig
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
*  Counter_11_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_11_SaveConfig(void) 
{
    #if (!Counter_11_UsingFixedFunction)

        Counter_11_backup.CounterUdb = Counter_11_ReadCounter();

        #if(!Counter_11_ControlRegRemoved)
            Counter_11_backup.CounterControlRegister = Counter_11_ReadControlRegister();
        #endif /* (!Counter_11_ControlRegRemoved) */

    #endif /* (!Counter_11_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_11_RestoreConfig
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
*  Counter_11_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_11_RestoreConfig(void) 
{      
    #if (!Counter_11_UsingFixedFunction)

       Counter_11_WriteCounter(Counter_11_backup.CounterUdb);

        #if(!Counter_11_ControlRegRemoved)
            Counter_11_WriteControlRegister(Counter_11_backup.CounterControlRegister);
        #endif /* (!Counter_11_ControlRegRemoved) */

    #endif /* (!Counter_11_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_11_Sleep
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
*  Counter_11_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_11_Sleep(void) 
{
    #if(!Counter_11_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_11_CTRL_ENABLE == (Counter_11_CONTROL & Counter_11_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_11_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_11_backup.CounterEnableState = 0u;
        }
    #else
        Counter_11_backup.CounterEnableState = 1u;
        if(Counter_11_backup.CounterEnableState != 0u)
        {
            Counter_11_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_11_ControlRegRemoved) */
    
    Counter_11_Stop();
    Counter_11_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_11_Wakeup
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
*  Counter_11_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_11_Wakeup(void) 
{
    Counter_11_RestoreConfig();
    #if(!Counter_11_ControlRegRemoved)
        if(Counter_11_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_11_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_11_ControlRegRemoved) */
    
}


/* [] END OF FILE */
