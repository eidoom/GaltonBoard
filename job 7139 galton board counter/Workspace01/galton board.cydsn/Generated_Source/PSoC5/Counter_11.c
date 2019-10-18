/*******************************************************************************
* File Name: Counter_11.c  
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

#include "Counter_11.h"

uint8 Counter_11_initVar = 0u;


/*******************************************************************************
* Function Name: Counter_11_Init
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
void Counter_11_Init(void) 
{
        #if (!Counter_11_UsingFixedFunction && !Counter_11_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!Counter_11_UsingFixedFunction && !Counter_11_ControlRegRemoved) */
        
        #if(!Counter_11_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 Counter_11_interruptState;
        #endif /* (!Counter_11_UsingFixedFunction) */
        
        #if (Counter_11_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            Counter_11_CONTROL &= Counter_11_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                Counter_11_CONTROL2 &= ((uint8)(~Counter_11_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                Counter_11_CONTROL3 &= ((uint8)(~Counter_11_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (Counter_11_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                Counter_11_CONTROL |= Counter_11_ONESHOT;
            #endif /* (Counter_11_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            Counter_11_CONTROL2 |= Counter_11_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            Counter_11_RT1 &= ((uint8)(~Counter_11_RT1_MASK));
            Counter_11_RT1 |= Counter_11_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            Counter_11_RT1 &= ((uint8)(~Counter_11_SYNCDSI_MASK));
            Counter_11_RT1 |= Counter_11_SYNCDSI_EN;

        #else
            #if(!Counter_11_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = Counter_11_CONTROL & ((uint8)(~Counter_11_CTRL_CMPMODE_MASK));
            Counter_11_CONTROL = ctrl | Counter_11_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = Counter_11_CONTROL & ((uint8)(~Counter_11_CTRL_CAPMODE_MASK));
            
            #if( 0 != Counter_11_CAPTURE_MODE_CONF)
                Counter_11_CONTROL = ctrl | Counter_11_DEFAULT_CAPTURE_MODE;
            #else
                Counter_11_CONTROL = ctrl;
            #endif /* 0 != Counter_11_CAPTURE_MODE */ 
            
            #endif /* (!Counter_11_ControlRegRemoved) */
        #endif /* (Counter_11_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!Counter_11_UsingFixedFunction)
            Counter_11_ClearFIFO();
        #endif /* (!Counter_11_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        Counter_11_WritePeriod(Counter_11_INIT_PERIOD_VALUE);
        #if (!(Counter_11_UsingFixedFunction && (CY_PSOC5A)))
            Counter_11_WriteCounter(Counter_11_INIT_COUNTER_VALUE);
        #endif /* (!(Counter_11_UsingFixedFunction && (CY_PSOC5A))) */
        Counter_11_SetInterruptMode(Counter_11_INIT_INTERRUPTS_MASK);
        
        #if (!Counter_11_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)Counter_11_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            Counter_11_WriteCompare(Counter_11_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            Counter_11_interruptState = CyEnterCriticalSection();
            
            Counter_11_STATUS_AUX_CTRL |= Counter_11_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(Counter_11_interruptState);
            
        #endif /* (!Counter_11_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_11_Enable
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
void Counter_11_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (Counter_11_UsingFixedFunction)
        Counter_11_GLOBAL_ENABLE |= Counter_11_BLOCK_EN_MASK;
        Counter_11_GLOBAL_STBY_ENABLE |= Counter_11_BLOCK_STBY_EN_MASK;
    #endif /* (Counter_11_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!Counter_11_ControlRegRemoved || Counter_11_UsingFixedFunction)
        Counter_11_CONTROL |= Counter_11_CTRL_ENABLE;                
    #endif /* (!Counter_11_ControlRegRemoved || Counter_11_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: Counter_11_Start
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
*  Counter_11_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void Counter_11_Start(void) 
{
    if(Counter_11_initVar == 0u)
    {
        Counter_11_Init();
        
        Counter_11_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    Counter_11_Enable();        
}


/*******************************************************************************
* Function Name: Counter_11_Stop
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
void Counter_11_Stop(void) 
{
    /* Disable Counter */
    #if(!Counter_11_ControlRegRemoved || Counter_11_UsingFixedFunction)
        Counter_11_CONTROL &= ((uint8)(~Counter_11_CTRL_ENABLE));        
    #endif /* (!Counter_11_ControlRegRemoved || Counter_11_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (Counter_11_UsingFixedFunction)
        Counter_11_GLOBAL_ENABLE &= ((uint8)(~Counter_11_BLOCK_EN_MASK));
        Counter_11_GLOBAL_STBY_ENABLE &= ((uint8)(~Counter_11_BLOCK_STBY_EN_MASK));
    #endif /* (Counter_11_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_11_SetInterruptMode
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
void Counter_11_SetInterruptMode(uint8 interruptsMask) 
{
    Counter_11_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: Counter_11_ReadStatusRegister
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
uint8   Counter_11_ReadStatusRegister(void) 
{
    return Counter_11_STATUS;
}


#if(!Counter_11_ControlRegRemoved)
/*******************************************************************************
* Function Name: Counter_11_ReadControlRegister
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
uint8   Counter_11_ReadControlRegister(void) 
{
    return Counter_11_CONTROL;
}


/*******************************************************************************
* Function Name: Counter_11_WriteControlRegister
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
void    Counter_11_WriteControlRegister(uint8 control) 
{
    Counter_11_CONTROL = control;
}

#endif  /* (!Counter_11_ControlRegRemoved) */


#if (!(Counter_11_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: Counter_11_WriteCounter
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
void Counter_11_WriteCounter(uint8 counter) \
                                   
{
    #if(Counter_11_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (Counter_11_GLOBAL_ENABLE & Counter_11_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        Counter_11_GLOBAL_ENABLE |= Counter_11_BLOCK_EN_MASK;
        CY_SET_REG16(Counter_11_COUNTER_LSB_PTR, (uint16)counter);
        Counter_11_GLOBAL_ENABLE &= ((uint8)(~Counter_11_BLOCK_EN_MASK));
    #else
        CY_SET_REG8(Counter_11_COUNTER_LSB_PTR, counter);
    #endif /* (Counter_11_UsingFixedFunction) */
}
#endif /* (!(Counter_11_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: Counter_11_ReadCounter
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
uint8 Counter_11_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(Counter_11_UsingFixedFunction)
		(void)CY_GET_REG16(Counter_11_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(Counter_11_COUNTER_LSB_PTR_8BIT);
	#endif/* (Counter_11_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(Counter_11_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(Counter_11_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(Counter_11_STATICCOUNT_LSB_PTR));
    #endif /* (Counter_11_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_11_ReadCapture
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
uint8 Counter_11_ReadCapture(void) 
{
    #if(Counter_11_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(Counter_11_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(Counter_11_STATICCOUNT_LSB_PTR));
    #endif /* (Counter_11_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_11_WritePeriod
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
void Counter_11_WritePeriod(uint8 period) 
{
    #if(Counter_11_UsingFixedFunction)
        CY_SET_REG16(Counter_11_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG8(Counter_11_PERIOD_LSB_PTR, period);
    #endif /* (Counter_11_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_11_ReadPeriod
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
uint8 Counter_11_ReadPeriod(void) 
{
    #if(Counter_11_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(Counter_11_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(Counter_11_PERIOD_LSB_PTR));
    #endif /* (Counter_11_UsingFixedFunction) */
}


#if (!Counter_11_UsingFixedFunction)
/*******************************************************************************
* Function Name: Counter_11_WriteCompare
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
void Counter_11_WriteCompare(uint8 compare) \
                                   
{
    #if(Counter_11_UsingFixedFunction)
        CY_SET_REG16(Counter_11_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG8(Counter_11_COMPARE_LSB_PTR, compare);
    #endif /* (Counter_11_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_11_ReadCompare
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
uint8 Counter_11_ReadCompare(void) 
{
    return (CY_GET_REG8(Counter_11_COMPARE_LSB_PTR));
}


#if (Counter_11_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: Counter_11_SetCompareMode
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
void Counter_11_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    Counter_11_CONTROL &= ((uint8)(~Counter_11_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    Counter_11_CONTROL |= compareMode;
}
#endif  /* (Counter_11_COMPARE_MODE_SOFTWARE) */


#if (Counter_11_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: Counter_11_SetCaptureMode
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
void Counter_11_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    Counter_11_CONTROL &= ((uint8)(~Counter_11_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    Counter_11_CONTROL |= ((uint8)((uint8)captureMode << Counter_11_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (Counter_11_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: Counter_11_ClearFIFO
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
void Counter_11_ClearFIFO(void) 
{

    while(0u != (Counter_11_ReadStatusRegister() & Counter_11_STATUS_FIFONEMP))
    {
        (void)Counter_11_ReadCapture();
    }

}
#endif  /* (!Counter_11_UsingFixedFunction) */


/* [] END OF FILE */

