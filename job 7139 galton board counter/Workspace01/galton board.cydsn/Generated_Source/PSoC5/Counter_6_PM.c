/*******************************************************************************
* File Name: Counter_6_PM.c  
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

#include "Counter_6.h"

static Counter_6_backupStruct Counter_6_backup;


/*******************************************************************************
* Function Name: Counter_6_SaveConfig
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
*  Counter_6_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_6_SaveConfig(void) 
{
    #if (!Counter_6_UsingFixedFunction)

        Counter_6_backup.CounterUdb = Counter_6_ReadCounter();

        #if(!Counter_6_ControlRegRemoved)
            Counter_6_backup.CounterControlRegister = Counter_6_ReadControlRegister();
        #endif /* (!Counter_6_ControlRegRemoved) */

    #endif /* (!Counter_6_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_6_RestoreConfig
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
*  Counter_6_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_6_RestoreConfig(void) 
{      
    #if (!Counter_6_UsingFixedFunction)

       Counter_6_WriteCounter(Counter_6_backup.CounterUdb);

        #if(!Counter_6_ControlRegRemoved)
            Counter_6_WriteControlRegister(Counter_6_backup.CounterControlRegister);
        #endif /* (!Counter_6_ControlRegRemoved) */

    #endif /* (!Counter_6_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_6_Sleep
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
*  Counter_6_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_6_Sleep(void) 
{
    #if(!Counter_6_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_6_CTRL_ENABLE == (Counter_6_CONTROL & Counter_6_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_6_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_6_backup.CounterEnableState = 0u;
        }
    #else
        Counter_6_backup.CounterEnableState = 1u;
        if(Counter_6_backup.CounterEnableState != 0u)
        {
            Counter_6_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_6_ControlRegRemoved) */
    
    Counter_6_Stop();
    Counter_6_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_6_Wakeup
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
*  Counter_6_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_6_Wakeup(void) 
{
    Counter_6_RestoreConfig();
    #if(!Counter_6_ControlRegRemoved)
        if(Counter_6_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_6_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_6_ControlRegRemoved) */
    
}


/* [] END OF FILE */
