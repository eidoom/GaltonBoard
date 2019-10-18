/*******************************************************************************
* File Name: Counter_7_PM.c  
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

#include "Counter_7.h"

static Counter_7_backupStruct Counter_7_backup;


/*******************************************************************************
* Function Name: Counter_7_SaveConfig
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
*  Counter_7_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_7_SaveConfig(void) 
{
    #if (!Counter_7_UsingFixedFunction)

        Counter_7_backup.CounterUdb = Counter_7_ReadCounter();

        #if(!Counter_7_ControlRegRemoved)
            Counter_7_backup.CounterControlRegister = Counter_7_ReadControlRegister();
        #endif /* (!Counter_7_ControlRegRemoved) */

    #endif /* (!Counter_7_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_7_RestoreConfig
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
*  Counter_7_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_7_RestoreConfig(void) 
{      
    #if (!Counter_7_UsingFixedFunction)

       Counter_7_WriteCounter(Counter_7_backup.CounterUdb);

        #if(!Counter_7_ControlRegRemoved)
            Counter_7_WriteControlRegister(Counter_7_backup.CounterControlRegister);
        #endif /* (!Counter_7_ControlRegRemoved) */

    #endif /* (!Counter_7_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_7_Sleep
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
*  Counter_7_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_7_Sleep(void) 
{
    #if(!Counter_7_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_7_CTRL_ENABLE == (Counter_7_CONTROL & Counter_7_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_7_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_7_backup.CounterEnableState = 0u;
        }
    #else
        Counter_7_backup.CounterEnableState = 1u;
        if(Counter_7_backup.CounterEnableState != 0u)
        {
            Counter_7_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_7_ControlRegRemoved) */
    
    Counter_7_Stop();
    Counter_7_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_7_Wakeup
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
*  Counter_7_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_7_Wakeup(void) 
{
    Counter_7_RestoreConfig();
    #if(!Counter_7_ControlRegRemoved)
        if(Counter_7_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_7_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_7_ControlRegRemoved) */
    
}


/* [] END OF FILE */
