/*******************************************************************************
* File Name: Counter_12.c  
* Version 3.0
*
*  Description:
*     The Counter component consists of a 8, 16, 24 or 32-bit counter with
*     a selectable period between 2 and 2^Width - 1.  
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

#include "Counter_12.h"

uint8 Counter_12_initVar = 0u;


/*******************************************************************************
* Function Name: Counter_12_Init
********************************************************************************
* Summary:
*     Initialize to the schematic state
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void Counter_12_Init(void) 
{
        #if (!Counter_12_UsingFixedFunction && !Counter_12_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!Counter_12_UsingFixedFunction && !Counter_12_ControlRegRemoved) */
        
        #if(!Counter_12_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 Counter_12_interruptState;
        #endif /* (!Counter_12_UsingFixedFunction) */
        
        #if (Counter_12_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            Counter_12_CONTROL &= Counter_12_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                Counter_12_CONTROL2 &= ((uint8)(~Counter_12_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                Counter_12_CONTROL3 &= ((uint8)(~Counter_12_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (Counter_12_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                Counter_12_CONTROL |= Counter_12_ONESHOT;
            #endif /* (Counter_12_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            Counter_12_CONTROL2 |= Counter_12_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            Counter_12_RT1 &= ((uint8)(~Counter_12_RT1_MASK));
            Counter_12_RT1 |= Counter_12_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            Counter_12_RT1 &= ((uint8)(~Counter_12_SYNCDSI_MASK));
            Counter_12_RT1 |= Counter_12_SYNCDSI_EN;

        #else
            #if(!Counter_12_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = Counter_12_CONTROL & ((uint8)(~Counter_12_CTRL_CMPMODE_MASK));
            Counter_12_CONTROL = ctrl | Counter_12_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = Counter_12_CONTROL & ((uint8)(~Counter_12_CTRL_CAPMODE_MASK));
            
            #if( 0 != Counter_12_CAPTURE_MODE_CONF)
                Counter_12_CONTROL = ctrl | Counter_12_DEFAULT_CAPTURE_MODE;
            #else
                Counter_12_CONTROL = ctrl;
            #endif /* 0 != Counter_12_CAPTURE_MODE */ 
            
            #endif /* (!Counter_12_ControlRegRemoved) */
        #endif /* (Counter_12_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!Counter_12_UsingFixedFunction)
            Counter_12_ClearFIFO();
        #endif /* (!Counter_12_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        Counter_12_WritePeriod(Counter_12_INIT_PERIOD_VALUE);
        #if (!(Counter_12_UsingFixedFunction && (CY_PSOC5A)))
            Counter_12_WriteCounter(Counter_12_INIT_COUNTER_VALUE);
        #endif /* (!(Counter_12_UsingFixedFunction && (CY_PSOC5A))) */
        Counter_12_SetInterruptMode(Counter_12_INIT_INTERRUPTS_MASK);
        
        #if (!Counter_12_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)Counter_12_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            Counter_12_WriteCompare(Counter_12_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            Counter_12_interruptState = CyEnterCriticalSection();
            
            Counter_12_STATUS_AUX_CTRL |= Counter_12_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(Counter_12_interruptState);
            
        #endif /* (!Counter_12_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_12_Enable
********************************************************************************
* Summary:
*     Enable the Counter
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: 
*   If the Enable mode is set to Hardware only then this function has no effect 
*   on the operation of the counter.
*
*******************************************************************************/
void Counter_12_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (Counter_12_UsingFixedFunction)
        Counter_12_GLOBAL_ENABLE |= Counter_12_BLOCK_EN_MASK;
        Counter_12_GLOBAL_STBY_ENABLE |= Counter_12_BLOCK_STBY_EN_MASK;
    #endif /* (Counter_12_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!Counter_12_ControlRegRemoved || Counter_12_UsingFixedFunction)
        Counter_12_CONTROL |= Counter_12_CTRL_ENABLE;                
    #endif /* (!Counter_12_ControlRegRemoved || Counter_12_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: Counter_12_Start
********************************************************************************
* Summary:
*  Enables the counter for operation 
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Global variables:
*  Counter_12_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void Counter_12_Start(void) 
{
    if(Counter_12_initVar == 0u)
    {
        Counter_12_Init();
        
        Counter_12_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    Counter_12_Enable();        
}


/*******************************************************************************
* Function Name: Counter_12_Stop
********************************************************************************
* Summary:
* Halts the counter, but does not change any modes or disable interrupts.
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the counter.
*
*******************************************************************************/
void Counter_12_Stop(void) 
{
    /* Disable Counter */
    #if(!Counter_12_ControlRegRemoved || Counter_12_UsingFixedFunction)
        Counter_12_CONTROL &= ((uint8)(~Counter_12_CTRL_ENABLE));        
    #endif /* (!Counter_12_ControlRegRemoved || Counter_12_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (Counter_12_UsingFixedFunction)
        Counter_12_GLOBAL_ENABLE &= ((uint8)(~Counter_12_BLOCK_EN_MASK));
        Counter_12_GLOBAL_STBY_ENABLE &= ((uint8)(~Counter_12_BLOCK_STBY_EN_MASK));
    #endif /* (Counter_12_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_12_SetInterruptMode
********************************************************************************
* Summary:
* Configures which interrupt sources are enabled to generate the final interrupt
*
* Parameters:  
*  InterruptsMask: This parameter is an or'd collection of the status bits
*                   which will be allowed to generate the counters interrupt.   
*
* Return: 
*  void
*
*******************************************************************************/
void Counter_12_SetInterruptMode(uint8 interruptsMask) 
{
    Counter_12_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: Counter_12_ReadStatusRegister
********************************************************************************
* Summary:
*   Reads the status register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the status register
*
* Side Effects:
*   Status register bits may be clear on read. 
*
*******************************************************************************/
uint8   Counter_12_ReadStatusRegister(void) 
{
    return Counter_12_STATUS;
}


#if(!Counter_12_ControlRegRemoved)
/*******************************************************************************
* Function Name: Counter_12_ReadControlRegister
********************************************************************************
* Summary:
*   Reads the control register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
uint8   Counter_12_ReadControlRegister(void) 
{
    return Counter_12_CONTROL;
}


/*******************************************************************************
* Function Name: Counter_12_WriteControlRegister
********************************************************************************
* Summary:
*   Sets the bit-field of the control register.  This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
void    Counter_12_WriteControlRegister(uint8 control) 
{
    Counter_12_CONTROL = control;
}

#endif  /* (!Counter_12_ControlRegRemoved) */


#if (!(Counter_12_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: Counter_12_WriteCounter
********************************************************************************
* Summary:
*   This funtion is used to set the counter to a specific value
*
* Parameters:  
*  counter:  New counter value. 
*
* Return: 
*  void 
*
*******************************************************************************/
void Counter_12_WriteCounter(uint8 counter) \
                                   
{
    #if(Counter_12_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (Counter_12_GLOBAL_ENABLE & Counter_12_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        Counter_12_GLOBAL_ENABLE |= Counter_12_BLOCK_EN_MASK;
        CY_SET_REG16(Counter_12_COUNTER_LSB_PTR, (uint16)counter);
        Counter_12_GLOBAL_ENABLE &= ((uint8)(~Counter_12_BLOCK_EN_MASK));
    #else
        CY_SET_REG8(Counter_12_COUNTER_LSB_PTR, counter);
    #endif /* (Counter_12_UsingFixedFunction) */
}
#endif /* (!(Counter_12_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: Counter_12_ReadCounter
********************************************************************************
* Summary:
* Returns the current value of the counter.  It doesn't matter
* if the counter is enabled or running.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint8) The present value of the counter.
*
*******************************************************************************/
uint8 Counter_12_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(Counter_12_UsingFixedFunction)
		(void)CY_GET_REG16(Counter_12_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(Counter_12_COUNTER_LSB_PTR_8BIT);
	#endif/* (Counter_12_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(Counter_12_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(Counter_12_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(Counter_12_STATICCOUNT_LSB_PTR));
    #endif /* (Counter_12_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_12_ReadCapture
********************************************************************************
* Summary:
*   This function returns the last value captured.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) Present Capture value.
*
*******************************************************************************/
uint8 Counter_12_ReadCapture(void) 
{
    #if(Counter_12_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(Counter_12_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(Counter_12_STATICCOUNT_LSB_PTR));
    #endif /* (Counter_12_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_12_WritePeriod
********************************************************************************
* Summary:
* Changes the period of the counter.  The new period 
* will be loaded the next time terminal count is detected.
*
* Parameters:  
*  period: (uint8) A value of 0 will result in
*         the counter remaining at zero.  
*
* Return: 
*  void
*
*******************************************************************************/
void Counter_12_WritePeriod(uint8 period) 
{
    #if(Counter_12_UsingFixedFunction)
        CY_SET_REG16(Counter_12_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG8(Counter_12_PERIOD_LSB_PTR, period);
    #endif /* (Counter_12_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_12_ReadPeriod
********************************************************************************
* Summary:
* Reads the current period value without affecting counter operation.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint8) Present period value.
*
*******************************************************************************/
uint8 Counter_12_ReadPeriod(void) 
{
    #if(Counter_12_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(Counter_12_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(Counter_12_PERIOD_LSB_PTR));
    #endif /* (Counter_12_UsingFixedFunction) */
}


#if (!Counter_12_UsingFixedFunction)
/*******************************************************************************
* Function Name: Counter_12_WriteCompare
********************************************************************************
* Summary:
* Changes the compare value.  The compare output will 
* reflect the new value on the next UDB clock.  The compare output will be 
* driven high when the present counter value compares true based on the 
* configured compare mode setting. 
*
* Parameters:  
*  Compare:  New compare value. 
*
* Return: 
*  void
*
*******************************************************************************/
void Counter_12_WriteCompare(uint8 compare) \
                                   
{
    #if(Counter_12_UsingFixedFunction)
        CY_SET_REG16(Counter_12_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG8(Counter_12_COMPARE_LSB_PTR, compare);
    #endif /* (Counter_12_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_12_ReadCompare
********************************************************************************
* Summary:
* Returns the compare value.
*
* Parameters:  
*  void:
*
* Return: 
*  (uint8) Present compare value.
*
*******************************************************************************/
uint8 Counter_12_ReadCompare(void) 
{
    return (CY_GET_REG8(Counter_12_COMPARE_LSB_PTR));
}


#if (Counter_12_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: Counter_12_SetCompareMode
********************************************************************************
* Summary:
*  Sets the software controlled Compare Mode.
*
* Parameters:
*  compareMode:  Compare Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void Counter_12_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    Counter_12_CONTROL &= ((uint8)(~Counter_12_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    Counter_12_CONTROL |= compareMode;
}
#endif  /* (Counter_12_COMPARE_MODE_SOFTWARE) */


#if (Counter_12_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: Counter_12_SetCaptureMode
********************************************************************************
* Summary:
*  Sets the software controlled Capture Mode.
*
* Parameters:
*  captureMode:  Capture Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void Counter_12_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    Counter_12_CONTROL &= ((uint8)(~Counter_12_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    Counter_12_CONTROL |= ((uint8)((uint8)captureMode << Counter_12_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (Counter_12_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: Counter_12_ClearFIFO
********************************************************************************
* Summary:
*   This function clears all capture data from the capture FIFO
*
* Parameters:  
*  void:
*
* Return: 
*  None
*
*******************************************************************************/
void Counter_12_ClearFIFO(void) 
{

    while(0u != (Counter_12_ReadStatusRegister() & Counter_12_STATUS_FIFONEMP))
    {
        (void)Counter_12_ReadCapture();
    }

}
#endif  /* (!Counter_12_UsingFixedFunction) */


/* [] END OF FILE */

