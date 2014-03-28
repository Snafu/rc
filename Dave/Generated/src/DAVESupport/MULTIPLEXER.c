/*******************************************************************************
**  DAVE App Name : DAVESupport       App Version: 1.0.42               
**  This file is generated by DAVE, User modification to this file will be    **
**  overwritten at the next code generation.                                  **
*******************************************************************************/


/*******************************************************************************
 Copyright (c) 2011, Infineon Technologies AG                                 **
 All rights reserved.                                                         **
                                                                              **
 Redistribution and use in source and binary forms, with or without           **
 modification,are permitted provided that the following conditions are met:   **
                                                                              **
 *Redistributions of source code must retain the above copyright notice,      **
 this list of conditions and the following disclaimer.                        **
 *Redistributions in binary form must reproduce the above copyright notice,   **
 this list of conditions and the following disclaimer in the documentation    **
 and/or other materials provided with the distribution.                       **
 *Neither the name of the copyright holders nor the names of its contributors **
 may be used to endorse or promote products derived from this software without**
 specific prior written permission.                                           **
                                                                              **
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"  **
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE    **
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE   **
 ARE  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   **
 LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         **
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF         **
 SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    **
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN      **
 CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)       **
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE   **
 POSSIBILITY OF SUCH DAMAGE.                                                  **
                                                                              **
 To improve the quality of the software, users are encouraged to share        **
 modifications, enhancements or bug fixes with Infineon Technologies AG       **
 dave@infineon.com).                                                          **
                                                                              **
********************************************************************************
**                                                                            **
**                                                                            **
** PLATFORM : Infineon <Microcontroller name, step>                           **
**                                                                            **
** COMPILER : Compiler Independent                                            **
**                                                                            **
** MODIFICATION DATE : September 06, 2013                                     **
**                                                                            **
*******************************************************************************/
/**
 * @file  MULTIPLEXER.c
 *
 * @brief This file contains implementation of DAVE Support function which 
 * uses to configure multiplexer register values.
 * 24 July 2013 v1.0.34 Initial Version
 * 06 sep 2013 v1.0.36  UART001 : Mux : One instruction for output pins 
 *                      initialisation.
 *                      CAN001 & CAN002 : Avoid twice reset of the INIT bit in 
 *                      Can001_lNodeInit & DAVE_MUX_Init.
 *                      WR_REGHIB Macro definition has been changed.
 *                       
 *                      
 */

/*******************************************************************************
**                      Include Files                                         **
*******************************************************************************/
#include "../../inc/DAVESupport/DAVE3.h"

/*******************************************************************************
**                 Function declarations                                     **
*******************************************************************************/
/*******************************************************************************
** Syntax           : void DAVE_MUX_Init(void)                                **
**                                                                            **
** Reentrancy       : None                                                    **
**                                                                            **
** Parameters (in)  : void                                                    **
**                                                                            **
** Parameters (out) : none                                                    **
**                                                                            **
** Return value     : none                                                    **
**                                                                            **
** Description      : This is the Mux configuration                           **
**                                                                            **
*******************************************************************************/
           
void DAVE_MUX_Init(void)
{              	         
                                     
       /*        SCU Macro definitions     */                        
      /* Variable to store the CCR_MODE values for various USIC channels */
      uint32_t UsicCcrMode[6] = {0};      
//********* MODULE USIC CONFIGURATIONS *************************	        
        
           
   /* Disable mode before configuring all USIC registers to avoid unintended edges */   
          
    UsicCcrMode[0] |= (uint32_t) RD_REG(USIC0_CH0->CCR, USIC_CH_CCR_MODE_Msk, USIC_CH_CCR_MODE_Pos);
    WR_REG(USIC0_CH0->CCR, USIC_CH_CCR_MODE_Msk, USIC_CH_CCR_MODE_Pos,0);  
                         
    UsicCcrMode[2] |= (uint32_t) RD_REG(USIC1_CH0->CCR, USIC_CH_CCR_MODE_Msk, USIC_CH_CCR_MODE_Pos);   
    WR_REG(USIC1_CH0->CCR, USIC_CH_CCR_MODE_Msk, USIC_CH_CCR_MODE_Pos,0);
                  
      
    						
   /*USIC 0 Channel 0 Mux Related SFR/Bitfields Configurations*/ 									  					 				 				 		       				              				  					    					 					   				  					 				 				       				  										 									 					 					  									      					              					  						    					      
                 
   // Data Pointer & Buffer Size for Transmitter Buffer Control  
 WR_REG(USIC0_CH0->TBCTR, USIC_CH_TBCTR_DPTRSIZE_Msk, USIC_CH_TBCTR_DPTRSIZE_Pos,0x01000002);		/*    DPTR = 2,  SIZE = 1 */ 
         
  // Data Pointer & Buffer Size for Receiver Buffer Control  
 WR_REG(USIC0_CH0->RBCTR, USIC_CH_RBCTR_DPTRSIZE_Msk, USIC_CH_RBCTR_DPTRSIZE_Pos,0x01000000);		/*    DPTR = 0,  SIZE = 1 */ 
 						
   /*USIC 0 Channel 1 Mux Related SFR/Bitfields Configurations*/ 									  					 				 				 		       				              				  					    					 					   				  					 				 				       				  										 									 					 					  									      					              					  						    					      
         						
   /*USIC 1 Channel 0 Mux Related SFR/Bitfields Configurations*/ 						         
 WR_REG(USIC1_CH0->DX0CR, USIC_CH_DX0CR_DSEL_Msk, USIC_CH_DX0CR_DSEL_Pos,1); 
  			  					 				 				 		       				              				  					    					 					   				  					 				 				       				  										 									 					 					  									      					              					  						    					      
                 
   // Data Pointer & Buffer Size for Transmitter Buffer Control  
 WR_REG(USIC1_CH0->TBCTR, USIC_CH_TBCTR_DPTRSIZE_Msk, USIC_CH_TBCTR_DPTRSIZE_Pos,0x01000002);		/*    DPTR = 2,  SIZE = 1 */ 
         
  // Data Pointer & Buffer Size for Receiver Buffer Control  
 WR_REG(USIC1_CH0->RBCTR, USIC_CH_RBCTR_DPTRSIZE_Msk, USIC_CH_RBCTR_DPTRSIZE_Pos,0x01000000);		/*    DPTR = 0,  SIZE = 1 */ 
 						
   /*USIC 1 Channel 1 Mux Related SFR/Bitfields Configurations*/ 									  					 				 				 		       				              				  					    					 					   				  					 				 				       				  										 									 					 					  									      					              					  						    					      
         						
   /*USIC 2 Channel 0 Mux Related SFR/Bitfields Configurations*/ 									  					 				 				 		       				              				  					    					 					   				  					 				 				       				  										 									 					 					  									      					              					  						    					      
       						
   /*USIC 2 Channel 1 Mux Related SFR/Bitfields Configurations*/ 									  					 				 				 		       				              				  					    					 					   				  					 				 				       				  										 									 					 					  									      					              					  						    					      
         
  
  /* Enable mode after configuring all USIC registers to avoid unintended edges */  
            
   WR_REG(USIC0_CH0->CCR, USIC_CH_CCR_MODE_Msk, USIC_CH_CCR_MODE_Pos,UsicCcrMode[0]); 
          
   WR_REG(USIC1_CH0->CCR, USIC_CH_CCR_MODE_Msk, USIC_CH_CCR_MODE_Pos,UsicCcrMode[2]);
                        	         
                                              
        //********* Capture/Compare Unit 4 (CAPCOM4) CONFIGURATIONS *************************             	         
                                                          	         
                                                          	         
                                          

/*        PORT Macro definitions for IOCR_OE, IOCR_PCR & HWSEL_HW     */                                      
  WR_REG(PORT0->IOCR0, PORT_IOCR_PC0_OE_Msk, PORT_IOCR_PC0_OE_Pos, PORT_IOCR_OE1);                /*    P0.0 : PORT0_IOCR0_PC0_OE */					   
					                         
  WR_REG(PORT0->IOCR0, PORT_IOCR_PC1_OE_Msk, PORT_IOCR_PC1_OE_Pos, PORT_IOCR_OE1);                /*    P0.1 : PORT0_IOCR0_PC1_OE */					   
					                         
  WR_REG(PORT0->IOCR0, PORT_IOCR_PC2_OE_Msk, PORT_IOCR_PC2_OE_Pos, PORT_IOCR_OE1);                /*    P0.2 : PORT0_IOCR0_PC2_OE */					   
					                         
  WR_REG(PORT0->IOCR0, PORT_IOCR_PC3_OE_Msk, PORT_IOCR_PC3_OE_Pos, PORT_IOCR_OE1);                /*    P0.3 : PORT0_IOCR0_PC3_OE */					   
					                         
  WR_REG(PORT0->IOCR4, 0xb800U, PORT_IOCR_PC1_PCR_Pos, 0x12U);                /*P0.5 : PORT0_IOCR4_PC5_PCR and PORT0_IOCR4_PC5_OE */					   
					                         
  WR_REG(PORT0->IOCR8, PORT_IOCR_PC1_OE_Msk, PORT_IOCR_PC1_OE_Pos, PORT_IOCR_OE1);                /*    P0.9 : PORT0_IOCR8_PC9_OE */					   
					                         
  WR_REG(PORT0->IOCR8, PORT_IOCR_PC2_OE_Msk, PORT_IOCR_PC2_OE_Pos, PORT_IOCR_OE1);                /*    P0.10 : PORT0_IOCR8_PC10_OE */					   
					                         
  WR_REG(PORT0->IOCR8, 0xb8000000U, PORT_IOCR_PC3_PCR_Pos, 0x12U);                /*P0.11 : PORT0_IOCR8_PC11_PCR and PORT0_IOCR8_PC11_OE */					   
					                         
  WR_REG(PORT0->IOCR12, 0xb8U, PORT_IOCR_PC0_PCR_Pos, 0x13U);                /*P0.12 : PORT0_IOCR12_PC12_PCR and PORT0_IOCR12_PC12_OE */					   
					                         
  WR_REG(PORT1->IOCR0, 0xb800U, PORT_IOCR_PC1_PCR_Pos, 0x13U);                /*P1.1 : PORT1_IOCR0_PC1_PCR and PORT1_IOCR0_PC1_OE */					   
					                         
  WR_REG(PORT1->IOCR0, 0xb80000U, PORT_IOCR_PC2_PCR_Pos, 0x13U);                /*P1.2 : PORT1_IOCR0_PC2_PCR and PORT1_IOCR0_PC2_OE */					   
					   
              
  WR_REG(PORT2->HWSEL, PORT2_HWSEL_HW1_Msk, PORT2_HWSEL_HW1_Pos, PORT_HWSEL_SW);                    /*    P2.1 : PORT2_HWSEL_HW1 */                         
  WR_REG(PORT2->IOCR0, PORT_IOCR_PC1_OE_Msk, PORT_IOCR_PC1_OE_Pos, PORT_IOCR_OE1);                /*    P2.1 : PORT2_IOCR0_PC1_OE */					   
					                         
  WR_REG(PORT2->IOCR4, PORT_IOCR_PC2_OE_Msk, PORT_IOCR_PC2_OE_Pos, PORT_IOCR_OE1);                /*    P2.6 : PORT2_IOCR4_PC6_OE */					   
					                         
  WR_REG(PORT2->IOCR8, PORT_IOCR_PC2_OE_Msk, PORT_IOCR_PC2_OE_Pos, PORT_IOCR_OE1);                /*    P2.10 : PORT2_IOCR8_PC10_OE */					   
					                         
  WR_REG(PORT3->IOCR0, PORT_IOCR_PC0_OE_Msk, PORT_IOCR_PC0_OE_Pos, PORT_IOCR_OE1);                /*    P3.0 : PORT3_IOCR0_PC0_OE */					   
					                         
  WR_REG(PORT3->IOCR0, PORT_IOCR_PC1_OE_Msk, PORT_IOCR_PC1_OE_Pos, PORT_IOCR_OE1);                /*    P3.1 : PORT3_IOCR0_PC1_OE */					   
					                         
  WR_REG(PORT3->IOCR0, PORT_IOCR_PC2_OE_Msk, PORT_IOCR_PC2_OE_Pos, PORT_IOCR_OE1);                /*    P3.2 : PORT3_IOCR0_PC2_OE */					   
					                         
  WR_REG(PORT5->IOCR0, 0xb800U, PORT_IOCR_PC1_PCR_Pos, 0x11U);                /*P5.1 : PORT5_IOCR0_PC1_PCR and PORT5_IOCR0_PC1_OE */					   
					      
}


/*******************************************************************************
** Syntax           : void DAVE_MUX_PreInit(void)                             **
**                                                                            **
** Reentrancy       : None                                                    **
**                                                                            **
** Parameters (in)  : void                                                    **
**                                                                            **
** Parameters (out) : none                                                    **
**                                                                            **
** Return value     : none                                                    **
**                                                                            **
** Description      : This is the Mux configuration                           **
**                                                                            **
*******************************************************************************/
 
void DAVE_MUX_PreInit(void)
{                

/*        PORT Macro definitions for IOCR_OE, IOCR_PCR & HWSEL_HW     */                   
}

