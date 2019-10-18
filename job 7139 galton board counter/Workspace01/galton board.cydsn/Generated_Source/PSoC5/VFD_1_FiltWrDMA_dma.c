/***************************************************************************
* File Name: VFD_1_FiltWrDMA_dma.c  
* Version 1.70
*
*  Description:
*   Provides an API for the DMAC component. The API includes functions
*   for the DMA controller, DMA channels and Transfer Descriptors.
*
*
*   Note:
*     This module requires the developer to finish or fill in the auto
*     generated funcions and setup the dma channel and TD's.
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/
#include <CYLIB.H>
#include <CYDMAC.H>
#include <VFD_1_FiltWrDMA_dma.H>



/****************************************************************************
* 
* The following defines are available in Cyfitter.h
* 
* 
* 
* VFD_1_FiltWrDMA__DRQ_CTL_REG
* 
* 
* VFD_1_FiltWrDMA__DRQ_NUMBER
* 
* Number of TD's used by this channel.
* VFD_1_FiltWrDMA__NUMBEROF_TDS
* 
* Priority of this channel.
* VFD_1_FiltWrDMA__PRIORITY
* 
* True if VFD_1_FiltWrDMA_TERMIN_SEL is used.
* VFD_1_FiltWrDMA__TERMIN_EN
* 
* TERMIN interrupt line to signal terminate.
* VFD_1_FiltWrDMA__TERMIN_SEL
* 
* 
* True if VFD_1_FiltWrDMA_TERMOUT0_SEL is used.
* VFD_1_FiltWrDMA__TERMOUT0_EN
* 
* 
* TERMOUT0 interrupt line to signal completion.
* VFD_1_FiltWrDMA__TERMOUT0_SEL
* 
* 
* True if VFD_1_FiltWrDMA_TERMOUT1_SEL is used.
* VFD_1_FiltWrDMA__TERMOUT1_EN
* 
* 
* TERMOUT1 interrupt line to signal completion.
* VFD_1_FiltWrDMA__TERMOUT1_SEL
* 
****************************************************************************/


/* Zero based index of VFD_1_FiltWrDMA dma channel */
uint8 VFD_1_FiltWrDMA_DmaHandle = DMA_INVALID_CHANNEL;

/*********************************************************************
* Function Name: uint8 VFD_1_FiltWrDMA_DmaInitalize
**********************************************************************
* Summary:
*   Allocates and initialises a channel of the DMAC to be used by the
*   caller.
*
* Parameters:
*   BurstCount.
*       
*       
*   ReqestPerBurst.
*       
*       
*   UpperSrcAddress.
*       
*       
*   UpperDestAddress.
*       
*
* Return:
*   The channel that can be used by the caller for DMA activity.
*   DMA_INVALID_CHANNEL (0xFF) if there are no channels left. 
*
*
*******************************************************************/
uint8 VFD_1_FiltWrDMA_DmaInitialize(uint8 BurstCount, uint8 ReqestPerBurst, uint16 UpperSrcAddress, uint16 UpperDestAddress) 
{

    /* Allocate a DMA channel. */
    VFD_1_FiltWrDMA_DmaHandle = (uint8)VFD_1_FiltWrDMA__DRQ_NUMBER;

    /* Configure the channel. */
    (void)CyDmaChSetConfiguration(VFD_1_FiltWrDMA_DmaHandle,
                                  BurstCount,
                                  ReqestPerBurst,
                                  (uint8)VFD_1_FiltWrDMA__TERMOUT0_SEL,
                                  (uint8)VFD_1_FiltWrDMA__TERMOUT1_SEL,
                                  (uint8)VFD_1_FiltWrDMA__TERMIN_SEL);

    /* Set the extended address for the transfers */
    (void)CyDmaChSetExtendedAddress(VFD_1_FiltWrDMA_DmaHandle, UpperSrcAddress, UpperDestAddress);

    /* Set the priority for this channel */
    (void)CyDmaChPriority(VFD_1_FiltWrDMA_DmaHandle, (uint8)VFD_1_FiltWrDMA__PRIORITY);
    
    return VFD_1_FiltWrDMA_DmaHandle;
}

/*********************************************************************
* Function Name: void VFD_1_FiltWrDMA_DmaRelease
**********************************************************************
* Summary:
*   Frees the channel associated with VFD_1_FiltWrDMA.
*
*
* Parameters:
*   void.
*
*
*
* Return:
*   void.
*
*******************************************************************/
void VFD_1_FiltWrDMA_DmaRelease(void) 
{
    /* Disable the channel */
    (void)CyDmaChDisable(VFD_1_FiltWrDMA_DmaHandle);
}

