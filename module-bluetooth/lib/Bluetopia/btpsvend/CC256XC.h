/*****< CC256XC.h >************************************************************/
/*      Copyright 2012 - 2014 Stonestreet One.                                */
/*      All Rights Reserved.                                                  */
/*                                                                            */
/*      Copyright 2015 Texas Instruments Incorporated.                        */
/*      All Rights Reserved.                                                  */
/*                                                                            */
/*  CC256XC.h - TI Specific Patch RAM for CC256xC chips.                      */
/*                                                                            */
/*  Author: Self generated.                                                   */
/*                                                                            */
/*** MODIFICATION HISTORY *****************************************************/
/*                                                                            */
/*   mm/dd/yy  F. Lastname    Description of Modification                     */
/*   --------  -----------    ------------------------------------------------*/
/*   05/03/16                 Initial Creation. (Based On CC256XB.h)          */
/*   05/03/16  D. Horowitz    Alignment to Patch 12_26_008                    */
/*   08/22/16  D. Horowitz    Alignment to Patch 12_26_010                    */
/*   11/05/16  D. Horowitz    Alignment to Patch 12_26_012 (V1.0)             */
/*   10/27/17  V. Parmar      Alignment to Patch 12_26_016 (V1.1)             */
/******************************************************************************/

#ifndef _CC256XC_H_
#define _CC256XC_H_

#include "BTTypes.h"

   /* *********************************************************************** */
   /* ************************** Patch RAM Version ************************** */
   /* - Base                                                                  */
   /*   - BL6450C (CC256xC) BT SP 1.1                                         */
   /* - Add Ons                                                               */
   /*   - LE Add-On Pack for SP 1.1 (Official).                               */
   /*   - AVPR Add-On Pack for SP 1.1 (Official).                             */
   /* *********************************************************************** */

   
static BTPSCONST unsigned char BasePatch[] =
{

	0x01, 0x37, 0xfe, 0x02, 0x0c, 0x1a, 0x01, 0x05, 0xff, 0xff, 0xd0, 0x65, 0x08, 0x00, 0xfa, 0x0c, 
	0x1a, 0x09, 0x10, 0x01, 0x6a, 0xc8, 0x7b, 0x00, 0x02, 0x89, 0x7b, 0x01, 0x43, 0x09, 0x48, 0x51, 
	0x30, 0x02, 0x88, 0x06, 0x48, 0x91, 0x42, 0x03, 0xd1, 0x04, 0x49, 0x09, 0x78, 0x01, 0x29, 0x01, 
	0xd0, 0x5d, 0x30, 0xf7, 0x46, 0xff, 0x30, 0xd8, 0x30, 0xf7, 0x46, 0x76, 0x24, 0x08, 0x00, 0xbd, 
	0x28, 0x02, 0x00, 0x69, 0x53, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0xb5, 0x00, 0x69, 0xff, 0x21, 0x02, 0x31, 0x09, 0x5c, 0x09, 0x29, 0x05, 
	0xd1, 0x01, 0x21, 0x00, 0x22, 0x8e, 0x46, 0xcb, 0x4b, 0xfe, 0x44, 0x18, 0x47, 0x00, 0xbd, 0xca, 
	0x4a, 0x11, 0x88, 0x01, 0x20, 0x40, 0x03, 0x08, 0x43, 0x10, 0x80, 0xf7, 0x46, 0x30, 0xb5, 0x00, 
	0x69, 0xf4, 0x21, 0x08, 0x5c, 0x01, 0x28, 0x16, 0xd1, 0xc5, 0x48, 0x00, 0x78, 0x03, 0x28, 0x12, 
	0xd0, 0x00, 0x25, 0x28, 0x1c, 0xc3, 0x49, 0x01, 0x24, 0xa6, 0x46, 0xc2, 0x4a, 0xfe, 0x44, 0x10, 
	0x47, 0x00, 0x28, 0x08, 0xd0, 0xc1, 0x49, 0xc1, 0x4a, 0xbb, 0x32, 0x20, 0x20, 0x2b, 0x1c, 0xa6, 
	0x46, 0xc0, 0x4c, 0xfe, 0x44, 0x20, 0x47, 0x30, 0xbd, 0x70, 0xb5, 0x85, 0x69, 0x00, 0x7d, 0x80, 
	0x1f, 0x11, 0xd0, 0x47, 0x38, 0x2e, 0xd1, 0xa9, 0x79, 0x01, 0x05, 0xff, 0xff, 0xca, 0x66, 0x08, 
	0x00, 0xfa, 0x28, 0x20, 0x48, 0x43, 0xea, 0x4a, 0x10, 0x18, 0x23, 0x22, 0x12, 0x5c, 0x01, 0x2a, 
	0x25, 0xd1, 0x80, 0x7b, 0x00, 0x28, 0x22, 0xd0, 0xe6, 0x4a, 0x00, 0x20, 0x50, 0x54, 0x70, 0xbd, 
	0x01, 0x24, 0xa6, 0x46, 0xec, 0x48, 0xfe, 0x44, 0x00, 0x47, 0x01, 0x28, 0x05, 0xd0, 0xa6, 0x46, 
	0xe9, 0x48, 0xfe, 0x44, 0x00, 0x47, 0x04, 0x28, 0x11, 0xd1, 0xe8, 0x78, 0x00, 0x28, 0x0e, 0xd1, 
	0x0e, 0x26, 0x31, 0x1c, 0xe6, 0x4d, 0x28, 0x1c, 0x14, 0x38, 0xa6, 0x46, 0xe3, 0x4a, 0xfe, 0x44, 
	0x10, 0x47, 0x28, 0x1c, 0x31, 0x1c, 0xa6, 0x46, 0xe0, 0x4a, 0xfe, 0x44, 0x10, 0x47, 0x70, 0xbd, 
	0x70, 0xb5, 0x01, 0x1c, 0x88, 0x69, 0x89, 0x8a, 0xde, 0x4a, 0x89, 0x1a, 0x1c, 0xd0, 0x1c, 0x39, 
	0x20, 0xd1, 0xc5, 0x7a, 0x01, 0x21, 0x0c, 0x1c, 0x8e, 0x46, 0xdb, 0x4a, 0xfe, 0x44, 0x10, 0x47, 
	0x06, 0x1c, 0x10, 0x20, 0xa6, 0x46, 0xd9, 0x49, 0xfe, 0x44, 0x08, 0x47, 0x00, 0x2d, 0x11, 0xd0, 
	0x02, 0x2e, 0x0f, 0xd0, 0xd6, 0x49, 0xd7, 0x4a, 0xd9, 0x32, 0x20, 0x20, 0x00, 0x23, 0xa6, 0x46, 
	0x92, 0x4c, 0xfe, 0x44, 0x20, 0x47, 0x70, 0xbd, 0xc0, 0x7a, 0x00, 0x28, 0x02, 0xd1, 0x00, 0x20, 
	0xeb, 0x49, 0x08, 0x70, 0x70, 0xbd, 0x00, 0xb5, 0x00, 0x69, 0xff, 0x21, 0x04, 0x31, 0x09, 0x5c, 
	0x06, 0x29, 0x06, 0xd1, 0xff, 0x21, 0x05, 0x31, 0x0a, 0x5c, 0x2c, 0x2a, 0x01, 0xd1, 0x2d, 0x22, 
	0x0a, 0x54, 0xff, 0x21, 0x05, 0x31, 0x09, 0x5c, 0x34, 0x29, 0x11, 0xd1, 0xff, 0x21, 0x0b, 0x31, 
	0x09, 0x5c, 0x91, 0x29, 0x0c, 0xd1, 0xf1, 0x21, 0x09, 0x5c, 0x60, 0x22, 0x4a, 0x43, 0xc2, 0x4b, 
	0x00, 0x21, 0x99, 0x54, 0x06, 0x21, 0x01, 0x22, 0x96, 0x46, 0xc0, 0x4a, 0x01, 0x05, 0xff, 0xff, 
	0xc4, 0x67, 0x08, 0x00, 0xfa, 0xfe, 0x44, 0x10, 0x47, 0x00, 0xbd, 0xf0, 0xb5, 0x06, 0x1c, 0xf7, 
	0x69, 0x08, 0x20, 0xc0, 0x19, 0x01, 0x24, 0xa6, 0x46, 0xbb, 0x49, 0xfe, 0x44, 0x08, 0x47, 0xc1, 
	0x7b, 0x09, 0x02, 0x80, 0x7b, 0x08, 0x43, 0x05, 0x04, 0x2d, 0x0c, 0x02, 0x2d, 0x12, 0xd0, 0xb7, 
	0x48, 0x00, 0x88, 0xa8, 0x42, 0x0e, 0xd1, 0xb6, 0x48, 0x00, 0x78, 0x01, 0x28, 0x0a, 0xd1, 0xb5, 
	0x48, 0x82, 0x8f, 0x81, 0x6b, 0x08, 0x1c, 0x10, 0x43, 0x04, 0xd0, 0x38, 0x1c, 0xa6, 0x46, 0xb2, 
	0x4b, 0xfe, 0x44, 0x18, 0x47, 0x35, 0x60, 0xb1, 0x48, 0x24, 0x30, 0x30, 0x62, 0xf0, 0xbd, 0xf0, 
	0xb5, 0x85, 0xb0, 0x04, 0x90, 0x87, 0x69, 0x81, 0x8a, 0xad, 0x48, 0x08, 0x1a, 0x74, 0xd0, 0xad, 
	0x49, 0x40, 0x1a, 0x28, 0xd0, 0xac, 0x49, 0x40, 0x1a, 0x1d, 0xd1, 0xe2, 0x48, 0x05, 0x1c, 0x84, 
	0x3d, 0x00, 0x78, 0x02, 0x28, 0x17, 0xd1, 0x01, 0x24, 0xa6, 0x46, 0xa8, 0x48, 0xfe, 0x44, 0x00, 
	0x47, 0x00, 0x28, 0x10, 0xd1, 0xa6, 0x46, 0xa6, 0x48, 0xfe, 0x44, 0x00, 0x47, 0x00, 0x28, 0x0a, 
	0xd1, 0xa6, 0x46, 0xf0, 0x48, 0xfe, 0x44, 0x00, 0x47, 0x00, 0x28, 0x04, 0xd1, 0x28, 0x78, 0x00, 
	0x28, 0x01, 0xd1, 0xa8, 0x78, 0x00, 0x28, 0x00, 0xd0, 0x8b, 0xe0, 0xa6, 0x20, 0xa6, 0x46, 0xea, 
	0x49, 0xfe, 0x44, 0x08, 0x47, 0x85, 0xe0, 0x91, 0x48, 0x51, 0x38, 0x00, 0x78, 0x2a, 0x28, 0x7c, 
	0xd1, 0x0e, 0x20, 0xc6, 0x19, 0x44, 0x20, 0x0c, 0x21, 0x1a, 0x22, 0x01, 0x24, 0xa6, 0x46, 0xf1, 
	0x4b, 0xfe, 0x44, 0x18, 0x47, 0xf0, 0x48, 0x81, 0x78, 0xc2, 0x78, 0xf0, 0x48, 0xa6, 0x46, 0xf0, 
	0x4b, 0xfe, 0x44, 0x18, 0x47, 0x06, 0x20, 0xb8, 0x80, 0x08, 0x25, 0x35, 0x70, 0x01, 0x36, 0x01, 
	0x05, 0xff, 0xff, 0xbe, 0x68, 0x08, 0x00, 0xfa, 0x30, 0x1c, 0x00, 0x21, 0xa6, 0x46, 0xeb, 0x4a, 
	0xfe, 0x44, 0x10, 0x47, 0x06, 0x1c, 0x35, 0x70, 0x01, 0x36, 0x30, 0x1c, 0x0d, 0x21, 0xa6, 0x46, 
	0xe7, 0x4a, 0xfe, 0x44, 0x10, 0x47, 0x06, 0x1c, 0xa6, 0x46, 0xe5, 0x48, 0xfe, 0x44, 0x00, 0x47, 
	0x81, 0x02, 0xdf, 0x48, 0xc0, 0x78, 0x40, 0x06, 0x40, 0x0e, 0x08, 0x43, 0xee, 0x49, 0x01, 0x43, 
	0x09, 0x04, 0x09, 0x0c, 0x30, 0x1c, 0xa6, 0x46, 0xdd, 0x4a, 0xfe, 0x44, 0x10, 0x47, 0x7d, 0x80, 
	0x38, 0x1c, 0xff, 0x21, 0x02, 0x31, 0x00, 0x22, 0xa6, 0x46, 0xe7, 0x4b, 0xfe, 0x44, 0x18, 0x47, 
	0x38, 0xe0, 0x38, 0x1c, 0x00, 0x21, 0x6a, 0x46, 0x01, 0x24, 0xa6, 0x46, 0xe4, 0x4b, 0xfe, 0x44, 
	0x18, 0x47, 0x02, 0xa8, 0x00, 0x21, 0x06, 0x22, 0xa6, 0x46, 0xe1, 0x4b, 0xfe, 0x44, 0x18, 0x47, 
	0x00, 0x25, 0x02, 0xe0, 0x68, 0x1c, 0x05, 0x04, 0x2d, 0x0c, 0x62, 0x48, 0x4f, 0x38, 0x00, 0x78, 
	0x85, 0x42, 0x23, 0xda, 0x11, 0x20, 0x40, 0x01, 0x68, 0x43, 0xe7, 0x49, 0x0e, 0x18, 0x10, 0x20, 
	0x80, 0x19, 0x69, 0x46, 0xa6, 0x46, 0xe5, 0x4a, 0xfe, 0x44, 0x10, 0x47, 0x00, 0x28, 0xe9, 0xd1, 
	0x68, 0x46, 0x02, 0xa9, 0xa6, 0x46, 0xe1, 0x4a, 0xfe, 0x44, 0x10, 0x47, 0x00, 0x28, 0xe1, 0xd0, 
	0xb0, 0x78, 0x01, 0x28, 0xde, 0xd0, 0x38, 0x1c, 0x58, 0x49, 0x3a, 0x22, 0xa6, 0x46, 0xeb, 0x4b, 
	0xfe, 0x44, 0x18, 0x47, 0xe9, 0x49, 0xe2, 0x31, 0x04, 0x98, 0x01, 0x62, 0x05, 0xb0, 0xf0, 0xbd, 
	0xc0, 0x46, 0xdd, 0x9d, 0x00, 0x00, 0x3e, 0xa6, 0x1b, 0x00, 0x18, 0x32, 0x08, 0x00, 0xce, 0x04, 
	0x00, 0x00, 0x23, 0xb9, 0x02, 0x00, 0xb1, 0x6e, 0x08, 0x00, 0xfd, 0x79, 0x00, 0x00, 0xf5, 0x6d, 
	0x08, 0x00, 0x01, 0x05, 0xff, 0xff, 0xb8, 0x69, 0x08, 0x00, 0xfa, 0xf0, 0xb5, 0x07, 0x1c, 0xb9, 
	0x69, 0x48, 0x78, 0xfd, 0x69, 0x25, 0x22, 0x82, 0x1a, 0x28, 0xd0, 0x01, 0x3a, 0x54, 0xd1, 0x04, 
	0x22, 0xaa, 0x80, 0x0c, 0x22, 0x56, 0x19, 0x40, 0x00, 0x09, 0x78, 0x01, 0x43, 0x31, 0x70, 0x08, 
	0x20, 0x70, 0x70, 0x02, 0x36, 0x30, 0x1c, 0x0d, 0x21, 0x01, 0x24, 0xa6, 0x46, 0xa2, 0x4a, 0xfe, 
	0x44, 0x10, 0x47, 0x06, 0x1c, 0xa6, 0x46, 0xa0, 0x48, 0xfe, 0x44, 0x00, 0x47, 0x82, 0x02, 0xe7, 
	0x48, 0x00, 0x78, 0x40, 0x06, 0x41, 0x0e, 0x11, 0x43, 0xa9, 0x48, 0x08, 0x43, 0x01, 0x04, 0x09, 
	0x0c, 0x30, 0x1c, 0xa6, 0x46, 0x98, 0x4a, 0xfe, 0x44, 0x10, 0x47, 0x25, 0xe0, 0x04, 0x22, 0xaa, 
	0x80, 0x0c, 0x22, 0x56, 0x19, 0x40, 0x00, 0x09, 0x78, 0x01, 0x43, 0x31, 0x70, 0x08, 0x20, 0x70, 
	0x70, 0x02, 0x36, 0x30, 0x1c, 0x0d, 0x21, 0x01, 0x24, 0xa6, 0x46, 0x8e, 0x4a, 0xfe, 0x44, 0x10, 
	0x47, 0x06, 0x1c, 0xa6, 0x46, 0x8d, 0x48, 0xfe, 0x44, 0x00, 0x47, 0x82, 0x02, 0xd4, 0x48, 0x00, 
	0x78, 0x40, 0x06, 0x41, 0x0e, 0x11, 0x43, 0x95, 0x48, 0x08, 0x43, 0x01, 0x04, 0x09, 0x0c, 0x30, 
	0x1c, 0xa6, 0x46, 0x84, 0x4a, 0xfe, 0x44, 0x10, 0x47, 0x06, 0x20, 0x68, 0x80, 0x03, 0x20, 0x28, 
	0x80, 0xcd, 0x48, 0xcb, 0x49, 0x08, 0x18, 0x38, 0x62, 0xf0, 0xbd, 0xd4, 0x1d, 0x08, 0x00, 0x94, 
	0x54, 0x08, 0x00, 0x00, 0xb5, 0x01, 0x20, 0x86, 0x46, 0xc8, 0x48, 0xfe, 0x44, 0x00, 0x47, 0x00, 
	0x28, 0x06, 0xd0, 0x4c, 0x48, 0x00, 0x78, 0x02, 0x28, 0x02, 0xd1, 0x02, 0x20, 0xc4, 0x49, 0x08, 
	0x80, 0x00, 0xbd, 0x8f, 0x8d, 0x01, 0x00, 0xfd, 0x06, 0x05, 0x00, 0x14, 0x05, 0x1a, 0x00, 0xa2, 
	0xfd, 0x00, 0x00, 0x65, 0x2d, 0x01, 0x05, 0xff, 0xff, 0xb2, 0x6a, 0x08, 0x00, 0xfa, 0x00, 0x00, 
	0x7d, 0xca, 0x03, 0x00, 0xf1, 0x6e, 0x08, 0x00, 0x79, 0x47, 0x00, 0x00, 0x76, 0xa0, 0x1b, 0x00, 
	0xc5, 0x8e, 0x00, 0x00, 0x5b, 0x19, 0x04, 0x00, 0xba, 0x53, 0x08, 0x00, 0x90, 0xa1, 0x1b, 0x00, 
	0xa8, 0x59, 0x08, 0x00, 0x25, 0x6f, 0x04, 0x00, 0x79, 0x6c, 0x04, 0x00, 0x05, 0x04, 0x00, 0x00, 
	0xfc, 0x0b, 0x00, 0x00, 0x1d, 0x10, 0x00, 0x00, 0xc1, 0x72, 0x03, 0x00, 0x1b, 0x5f, 0x03, 0x00, 
	0xf8, 0xb5, 0x80, 0x8a, 0xff, 0x21, 0x0b, 0x31, 0x88, 0x42, 0x63, 0xd0, 0xff, 0x21, 0x45, 0x31, 
	0x88, 0x42, 0x1a, 0xd1, 0x2e, 0x48, 0x00, 0x78, 0x02, 0x28, 0x16, 0xd1, 0x05, 0x49, 0x24, 0x39, 
	0xff, 0x20, 0x41, 0x30, 0x40, 0x18, 0x00, 0x90, 0xa4, 0x48, 0x40, 0x5c, 0x00, 0x26, 0x86, 0x42, 
	0x13, 0xd3, 0x01, 0xe0, 0x78, 0x24, 0x08, 0x00, 0x51, 0x25, 0xad, 0x00, 0x28, 0x1c, 0x01, 0x24, 
	0xa6, 0x46, 0x9f, 0x49, 0xfe, 0x44, 0x08, 0x47, 0x00, 0x28, 0x70, 0xd1, 0x28, 0x1c, 0x9d, 0x49, 
	0xa6, 0x46, 0x9d, 0x4a, 0xfe, 0x44, 0x10, 0x47, 0xf8, 0xbd, 0xb1, 0x00, 0x00, 0x98, 0x45, 0x58, 
	0x00, 0x2d, 0x30, 0xd0, 0xfe, 0x20, 0x40, 0x5d, 0x00, 0x28, 0x2c, 0xd0, 0xf2, 0x20, 0x41, 0x5d, 
	0x00, 0x29, 0x28, 0xd1, 0xff, 0x20, 0x02, 0x30, 0x40, 0x5d, 0x01, 0x28, 0x23, 0xd0, 0x24, 0x20, 
	0x01, 0x24, 0xa6, 0x46, 0x91, 0x4a, 0xfe, 0x44, 0x10, 0x47, 0x00, 0x06, 0x00, 0x0e, 0xf3, 0x22, 
	0x51, 0x5d, 0x81, 0x42, 0x17, 0xd0, 0x50, 0x55, 0x00, 0x28, 0x14, 0xd1, 0x2f, 0x8d, 0x39, 0x1c, 
	0x8b, 0x48, 0xa6, 0x46, 0x8b, 0x4a, 0xfe, 0x44, 0x10, 0x47, 0x29, 0x8e, 0x79, 0x18, 0x08, 0x1a, 
	0x40, 0x1e, 0x39, 0x1c, 0xa6, 0x46, 0x87, 0x4a, 0x01, 0x05, 0xff, 0xff, 0xac, 0x6b, 0x08, 0x00, 
	0xfa, 0xfe, 0x44, 0x10, 0x47, 0x40, 0x00, 0x28, 0x86, 0x28, 0x8e, 0x40, 0x08, 0x28, 0x86, 0x01, 
	0x36, 0x83, 0x48, 0x00, 0x78, 0xaf, 0xe7, 0xc0, 0x46, 0x45, 0x10, 0x08, 0x00, 0x00, 0x20, 0x01, 
	0x24, 0xa6, 0x46, 0x80, 0x49, 0xfe, 0x44, 0x08, 0x47, 0x01, 0x1c, 0x00, 0x90, 0x7e, 0x4d, 0x28, 
	0x68, 0x01, 0x30, 0x28, 0x60, 0x0b, 0x27, 0x7f, 0x01, 0x7c, 0x4e, 0x7d, 0x48, 0x00, 0x8d, 0x32, 
	0x68, 0x10, 0x18, 0x80, 0x01, 0x80, 0x09, 0xa6, 0x46, 0x7a, 0x4a, 0xfe, 0x44, 0x10, 0x47, 0x00, 
	0x28, 0x11, 0xdd, 0x00, 0x98, 0x30, 0x60, 0x28, 0x68, 0x03, 0x28, 0x0a, 0xd9, 0x00, 0x20, 0xa6, 
	0x46, 0x75, 0x49, 0xfe, 0x44, 0x08, 0x47, 0x72, 0x48, 0x38, 0x5c, 0xa6, 0x46, 0x72, 0x49, 0xfe, 
	0x44, 0x08, 0x47, 0x00, 0x20, 0x28, 0x60, 0xf8, 0xbd, 0x53, 0x38, 0x02, 0x00, 0x21, 0xf0, 0x04, 
	0x00, 0x30, 0xb5, 0x05, 0x69, 0x24, 0x20, 0x00, 0x21, 0x01, 0x24, 0xa6, 0x46, 0x61, 0x4a, 0xfe, 
	0x44, 0x10, 0x47, 0xf3, 0x21, 0x48, 0x55, 0x51, 0x25, 0xad, 0x00, 0x28, 0x1c, 0xa6, 0x46, 0x5a, 
	0x49, 0xfe, 0x44, 0x08, 0x47, 0x00, 0x28, 0x05, 0xd1, 0x28, 0x1c, 0x58, 0x49, 0xa6, 0x46, 0x58, 
	0x4a, 0xfe, 0x44, 0x10, 0x47, 0x30, 0xbd, 0xc0, 0x46, 0xdb, 0x8e, 0x04, 0x00, 0xfc, 0x53, 0x08, 
	0x00, 0xc6, 0x02, 0x00, 0x00, 0x8d, 0x8f, 0x04, 0x00, 0xf9, 0x2d, 0x00, 0x00, 0xa9, 0x57, 0x05, 
	0x00, 0x41, 0x69, 0x00, 0x7e, 0x00, 0x28, 0x11, 0xd1, 0xff, 0x22, 0x04, 0x32, 0x50, 0x5c, 0x01, 
	0x28, 0x0c, 0xd1, 0xff, 0x20, 0x05, 0x30, 0x43, 0x5c, 0x1b, 0x2b, 0x07, 0xd1, 0xff, 0x23, 0x0a, 
	0x33, 0x5b, 0x5c, 0x01, 0x2b, 0x02, 0xd1, 0x00, 0x23, 0x53, 0x54, 0x01, 0x05, 0xff, 0xff, 0xa6, 
	0x6c, 0x08, 0x00, 0xfa, 0x43, 0x54, 0xf7, 0x46, 0xc0, 0x46, 0x00, 0x82, 0xff, 0xff, 0xed, 0x49, 
	0x02, 0x00, 0x99, 0x2d, 0x00, 0x00, 0xe9, 0x63, 0x05, 0x00, 0x10, 0xb5, 0x80, 0x69, 0x00, 0x88, 
	0x02, 0x28, 0x12, 0xd1, 0x48, 0x49, 0x08, 0x79, 0xff, 0x28, 0x0e, 0xd0, 0x08, 0x78, 0x03, 0x28, 
	0x03, 0xd0, 0x01, 0x28, 0x01, 0xd0, 0x02, 0x28, 0x07, 0xd1, 0x01, 0x20, 0x04, 0x1c, 0x86, 0x46, 
	0x42, 0x49, 0xfe, 0x44, 0x08, 0x47, 0x41, 0x48, 0x04, 0x70, 0x10, 0xbd, 0xc0, 0x46, 0x00, 0x00, 
	0x08, 0x00, 0x49, 0x8f, 0x03, 0x00, 0x10, 0xb5, 0xc0, 0x69, 0x80, 0x78, 0x80, 0x38, 0x00, 0x06, 
	0x00, 0x0e, 0x02, 0x28, 0x12, 0xd1, 0x37, 0x49, 0x08, 0x79, 0xff, 0x28, 0x0e, 0xd0, 0x08, 0x78, 
	0x03, 0x28, 0x03, 0xd0, 0x01, 0x28, 0x01, 0xd0, 0x02, 0x28, 0x07, 0xd1, 0x01, 0x20, 0x04, 0x1c, 
	0x86, 0x46, 0x31, 0x49, 0xfe, 0x44, 0x08, 0x47, 0x31, 0x48, 0x04, 0x70, 0x10, 0xbd, 0x25, 0x00, 
	0x00, 0x00, 0x85, 0x48, 0x02, 0x00, 0x10, 0xb5, 0x00, 0x69, 0xfe, 0x21, 0x09, 0x5c, 0x02, 0x29, 
	0x06, 0xd0, 0xd0, 0x21, 0x09, 0x58, 0x82, 0x8d, 0xd2, 0x00, 0x54, 0x23, 0x1b, 0x18, 0x05, 0xe0, 
	0x54, 0x21, 0x0b, 0x18, 0x02, 0x8d, 0x06, 0x20, 0x42, 0x43, 0xd9, 0x6f, 0x50, 0x18, 0x80, 0x01, 
	0x81, 0x09, 0xd8, 0x6d, 0x01, 0x24, 0xa6, 0x46, 0x1d, 0x4a, 0xfe, 0x44, 0x10, 0x47, 0x00, 0x28, 
	0x11, 0xdc, 0x1c, 0x49, 0x08, 0x79, 0xff, 0x28, 0x0d, 0xd0, 0x08, 0x78, 0x03, 0x28, 0x03, 0xd0, 
	0x01, 0x28, 0x01, 0xd0, 0x02, 0x28, 0x06, 0xd1, 0x20, 0x1c, 0xa6, 0x46, 0x17, 0x49, 0xfe, 0x44, 
	0x08, 0x47, 0x16, 0x48, 0x04, 0x70, 0x10, 0xbd, 0xc0, 0x46, 0xff, 0x53, 0x08, 0x00, 0x01, 0x05, 
	0xff, 0xff, 0xa0, 0x6d, 0x08, 0x00, 0xfa, 0x4e, 0x05, 0x00, 0x00, 0xb5, 0xcc, 0x00, 0x00, 0x71, 
	0xcd, 0x01, 0x00, 0x4a, 0x24, 0x19, 0x00, 0x07, 0x0e, 0x00, 0x00, 0x21, 0xcb, 0x03, 0x00, 0x00, 
	0x66, 0xe3, 0x01, 0x0b, 0xc9, 0x03, 0x00, 0x2b, 0xf0, 0x04, 0x00, 0xff, 0xff, 0xff, 0x01, 0x95, 
	0x49, 0x05, 0x00, 0x5b, 0x32, 0x08, 0x00, 0xd1, 0xa1, 0x04, 0x00, 0x48, 0x66, 0x08, 0x00, 0x4c, 
	0x66, 0x08, 0x00, 0x48, 0x10, 0x08, 0x00, 0x3f, 0xa2, 0x04, 0x00, 0xab, 0xea, 0x01, 0x00, 0xf0, 
	0x24, 0x08, 0x00, 0x3d, 0x1d, 0x03, 0x00, 0x50, 0x66, 0x08, 0x00, 0x40, 0x1e, 0x80, 0x00, 0x8c, 
	0x4b, 0x19, 0x50, 0x8a, 0x49, 0x0a, 0x50, 0xf7, 0x46, 0xf0, 0xb5, 0x01, 0x1c, 0x88, 0x69, 0x82, 
	0x88, 0x53, 0x04, 0x5b, 0x0c, 0x88, 0x4e, 0x88, 0x4f, 0x89, 0x4d, 0x03, 0xd0, 0x60, 0x2b, 0x01, 
	0xdc, 0xb2, 0x42, 0x14, 0xd0, 0x82, 0x88, 0x01, 0x23, 0x1b, 0x03, 0x54, 0x04, 0x64, 0x0f, 0x24, 
	0x03, 0x9c, 0x42, 0x08, 0xdb, 0x3b, 0x1c, 0x01, 0x33, 0x54, 0x04, 0x24, 0x0d, 0xe4, 0x00, 0x9c, 
	0x42, 0x01, 0xda, 0xba, 0x42, 0x03, 0xd0, 0xff, 0x20, 0x88, 0x60, 0xe8, 0x1d, 0xf0, 0xbd, 0x01, 
	0x24, 0xa6, 0x46, 0x7b, 0x49, 0xfe, 0x44, 0x08, 0x47, 0x38, 0x1c, 0xa6, 0x46, 0x7a, 0x49, 0xfe, 
	0x44, 0x08, 0x47, 0x30, 0x1c, 0xa6, 0x46, 0x77, 0x49, 0xfe, 0x44, 0x08, 0x47, 0xa6, 0x46, 0x76, 
	0x48, 0xfe, 0x44, 0x00, 0x47, 0x38, 0x1c, 0xa6, 0x46, 0x73, 0x49, 0xfe, 0x44, 0x08, 0x47, 0xe8, 
	0x48, 0x01, 0x68, 0x30, 0x1c, 0xa6, 0x46, 0x71, 0x4a, 0xfe, 0x44, 0x10, 0x47, 0x71, 0x48, 0x40, 
	0x19, 0xf0, 0xbd, 0x01, 0x1c, 0x0a, 0x7d, 0x6f, 0x48, 0x00, 0x2a, 0x02, 0xd0, 0xc9, 0x68, 0x01, 
	0x29, 0x01, 0x05, 0xff, 0xff, 0x9a, 0x6e, 0x08, 0x00, 0xfa, 0x01, 0xd0, 0x4f, 0x30, 0xf7, 0x46, 
	0x31, 0x30, 0xf7, 0x46, 0x41, 0x68, 0x02, 0x39, 0x41, 0x60, 0xe9, 0x48, 0x3f, 0x30, 0xf7, 0x46, 
	0x1c, 0xb5, 0x41, 0x68, 0xe7, 0x4c, 0x00, 0x29, 0x17, 0xd0, 0x41, 0x69, 0xb0, 0x20, 0x40, 0x18, 
	0xb8, 0x31, 0x6a, 0x46, 0x01, 0x23, 0x9e, 0x46, 0xe9, 0x4b, 0xfe, 0x44, 0x18, 0x47, 0x00, 0x99, 
	0xe8, 0x48, 0xe4, 0x38, 0x41, 0x43, 0x68, 0x46, 0x80, 0x88, 0x41, 0x18, 0xff, 0x20, 0xae, 0x30, 
	0x81, 0x42, 0x02, 0xd9, 0x20, 0x1c, 0xbf, 0x30, 0x1c, 0xbd, 0x20, 0x1c, 0xdf, 0x30, 0x1c, 0xbd, 
	0x10, 0xb5, 0x04, 0x1c, 0xc8, 0x68, 0x0a, 0x21, 0x01, 0x22, 0x96, 0x46, 0xde, 0x4a, 0xfe, 0x44, 
	0x10, 0x47, 0x00, 0x28, 0x00, 0xd1, 0x8c, 0x20, 0xa0, 0x60, 0xdc, 0x48, 0xdb, 0x30, 0x10, 0xbd, 
	0xdc, 0x49, 0x04, 0x39, 0x09, 0x78, 0x00, 0x29, 0x01, 0xd1, 0x00, 0x21, 0x41, 0x60, 0x41, 0x68, 
	0x42, 0x69, 0x51, 0x1a, 0x41, 0x60, 0xd6, 0x48, 0x55, 0x30, 0xf7, 0x46, 0xd6, 0x49, 0x09, 0x78, 
	0x00, 0x29, 0x04, 0xd1, 0x01, 0x7d, 0xd3, 0x48, 0x02, 0x30, 0xff, 0x22, 0x42, 0x54, 0xd3, 0x48, 
	0x4f, 0x30, 0xf7, 0x46, 0x01, 0x1c, 0x8a, 0x69, 0x4b, 0x68, 0xd1, 0x48, 0x9a, 0x42, 0x01, 0xd9, 
	0x3b, 0x30, 0xf7, 0x46, 0xca, 0x60, 0x79, 0x30, 0xf7, 0x46, 0xcf, 0x48, 0xcd, 0x49, 0x08, 0x80, 
	0xce, 0x48, 0xff, 0x30, 0xde, 0x30, 0xf7, 0x46, 0xc2, 0x69, 0xff, 0x21, 0x11, 0x31, 0x8b, 0x5c, 
	0xcb, 0x49, 0x5b, 0x08, 0x08, 0xd3, 0xff, 0x23, 0x02, 0x33, 0x9a, 0x5c, 0x02, 0x2a, 0x03, 0xd0, 
	0x01, 0x2a, 0x01, 0xd0, 0x03, 0x2a, 0x02, 0xd1, 0x08, 0x1c, 0x5b, 0x30, 0xf7, 0x46, 0xff, 0x22, 
	0x42, 0x60, 0x08, 0x1c, 0x01, 0x05, 0xff, 0xff, 0x94, 0x6f, 0x08, 0x00, 0xfa, 0x39, 0x30, 0xf7, 
	0x46, 0x00, 0xb5, 0x40, 0x68, 0x01, 0x21, 0x8e, 0x46, 0xe9, 0x49, 0xfe, 0x44, 0x08, 0x47, 0xe9, 
	0x48, 0x57, 0x30, 0x00, 0xbd, 0x02, 0x8a, 0x01, 0x79, 0x0a, 0x29, 0x00, 0xdb, 0x0a, 0x21, 0xe6, 
	0x48, 0x8a, 0x42, 0x01, 0xdd, 0x5f, 0x30, 0xf7, 0x46, 0x5b, 0x30, 0xf7, 0x46, 0xf0, 0xb5, 0x01, 
	0x24, 0xa6, 0x46, 0xe2, 0x48, 0xfe, 0x44, 0x00, 0x47, 0x01, 0x28, 0x05, 0xd0, 0xa6, 0x46, 0xdf, 
	0x48, 0xfe, 0x44, 0x00, 0x47, 0x04, 0x28, 0x19, 0xd1, 0x02, 0x26, 0xeb, 0x4d, 0x28, 0x79, 0x00, 
	0x28, 0x11, 0xd0, 0xe8, 0x7a, 0x06, 0x28, 0x0e, 0xd1, 0x0e, 0x20, 0x01, 0x1c, 0xe8, 0x4f, 0x38, 
	0x1c, 0x14, 0x38, 0xa6, 0x46, 0xe5, 0x4a, 0xfe, 0x44, 0x10, 0x47, 0x38, 0x1c, 0x0e, 0x21, 0xa6, 
	0x46, 0xe2, 0x4a, 0xfe, 0x44, 0x10, 0x47, 0x70, 0x35, 0x01, 0x3e, 0xe7, 0xd1, 0xa6, 0x46, 0xe1, 
	0x48, 0xfe, 0x44, 0x00, 0x47, 0xe0, 0x49, 0x0b, 0x48, 0x54, 0x30, 0x40, 0x18, 0xf0, 0xbd, 0xc0, 
	0x46, 0x04, 0xf3, 0x1a, 0x00, 0x80, 0x7b, 0x08, 0x00, 0x03, 0x80, 0x00, 0x00, 0x17, 0x10, 0x00, 
	0x00, 0xc9, 0xfb, 0x04, 0x00, 0xbb, 0x15, 0x04, 0x00, 0x7d, 0xca, 0x03, 0x00, 0x05, 0x43, 0x02, 
	0x00, 0x0b, 0xc9, 0x03, 0x00, 0xab, 0x02, 0x00, 0x00, 0xb1, 0x33, 0x02, 0x00, 0xf0, 0xb5, 0x8d, 
	0xb0, 0x01, 0x90, 0x81, 0x69, 0x02, 0x91, 0x01, 0x7d, 0x03, 0x91, 0x42, 0x68, 0x00, 0x92, 0x80, 
	0x8b, 0x40, 0x00, 0x04, 0x90, 0x6b, 0x48, 0x83, 0x30, 0x00, 0x78, 0x40, 0x00, 0x05, 0x90, 0x00, 
	0x29, 0x01, 0xd1, 0x00, 0x20, 0xe1, 0xe0, 0x04, 0x98, 0x02, 0x04, 0x12, 0x0c, 0x06, 0x92, 0x02, 
	0x98, 0x03, 0x99, 0x6b, 0x46, 0x01, 0x24, 0x01, 0x05, 0xff, 0xff, 0x8e, 0x70, 0x08, 0x00, 0xfa, 
	0xa6, 0x46, 0xc4, 0x4d, 0xfe, 0x44, 0x28, 0x47, 0x07, 0x90, 0x00, 0x9e, 0x00, 0x20, 0x08, 0x90, 
	0x7e, 0x48, 0x09, 0x90, 0x0a, 0x90, 0x7f, 0xe0, 0x09, 0x98, 0x00, 0x28, 0x2b, 0xd0, 0x0b, 0x98, 
	0x09, 0x90, 0x28, 0xe0, 0x05, 0x98, 0x87, 0x42, 0x25, 0xdb, 0x90, 0x79, 0x03, 0x28, 0x01, 0xd0, 
	0x01, 0x28, 0x20, 0xd1, 0x79, 0x19, 0x05, 0x98, 0x08, 0x1a, 0x07, 0x99, 0xa6, 0x46, 0xb6, 0x4a, 
	0xfe, 0x44, 0x10, 0x47, 0x07, 0x1c, 0x28, 0x1c, 0x07, 0x99, 0xa6, 0x46, 0xb3, 0x4a, 0xfe, 0x44, 
	0x10, 0x47, 0x01, 0x04, 0x09, 0x0c, 0x3a, 0x04, 0x12, 0x0c, 0x00, 0x20, 0xa6, 0x46, 0xaf, 0x4b, 
	0xfe, 0x44, 0x18, 0x47, 0x00, 0x28, 0x04, 0xd1, 0x0a, 0x98, 0x00, 0x28, 0x03, 0xd0, 0x0a, 0x97, 
	0x01, 0xe0, 0x00, 0x20, 0x0a, 0x90, 0xb6, 0x68, 0x00, 0x2e, 0x03, 0xd0, 0x30, 0x88, 0x07, 0x99, 
	0x88, 0x42, 0x49, 0xdb, 0x08, 0x98, 0x00, 0x28, 0x46, 0xd1, 0x5f, 0x49, 0x09, 0x98, 0x88, 0x42, 
	0x42, 0xd1, 0x0a, 0x98, 0x88, 0x42, 0x3f, 0xd1, 0x03, 0x98, 0x00, 0x28, 0x2c, 0xd0, 0x02, 0x9d, 
	0x03, 0x98, 0x0c, 0x90, 0x00, 0x27, 0x28, 0x88, 0x0b, 0x90, 0x04, 0x99, 0xa6, 0x46, 0x9a, 0x4a, 
	0xfe, 0x44, 0x10, 0x47, 0x00, 0x28, 0x07, 0xd0, 0x0b, 0x99, 0x04, 0x98, 0xa6, 0x46, 0x96, 0x4a, 
	0xfe, 0x44, 0x10, 0x47, 0x00, 0x28, 0x11, 0xd1, 0x08, 0x98, 0xb8, 0x42, 0x09, 0xd0, 0x08, 0x98, 
	0xc1, 0x00, 0x02, 0x98, 0x40, 0x18, 0x29, 0x1c, 0x08, 0x22, 0xa6, 0x46, 0xee, 0x4b, 0xfe, 0x44, 
	0x18, 0x47, 0x08, 0x98, 0x40, 0x1c, 0x00, 0x06, 0x00, 0x0e, 0x08, 0x90, 0x08, 0x35, 0x01, 0x37, 
	0x0c, 0x98, 0x01, 0x38, 0x0c, 0x90, 0xd6, 0xd1, 0x08, 0x98, 0x01, 0x05, 0xff, 0xff, 0x88, 0x71, 
	0x08, 0x00, 0xfa, 0x00, 0x28, 0x0c, 0xd0, 0x02, 0x98, 0x08, 0x99, 0x06, 0x9a, 0x6b, 0x46, 0xa6, 
	0x46, 0x83, 0x4d, 0xfe, 0x44, 0x28, 0x47, 0x07, 0x90, 0x00, 0x9e, 0x3d, 0x48, 0x09, 0x90, 0x0a, 
	0x90, 0x00, 0x2e, 0x3a, 0xd0, 0x35, 0x88, 0x07, 0x98, 0x85, 0x42, 0x36, 0xda, 0xb0, 0x68, 0x00, 
	0x28, 0x05, 0xd0, 0x00, 0x88, 0x07, 0x99, 0x88, 0x42, 0x01, 0xdc, 0x47, 0x1b, 0x04, 0xe0, 0x07, 
	0x98, 0x40, 0x1b, 0x00, 0x99, 0x09, 0x88, 0x0f, 0x18, 0x72, 0x68, 0x91, 0x88, 0x05, 0x98, 0x40, 
	0x18, 0x87, 0x42, 0x00, 0xda, 0x6a, 0xe7, 0x48, 0x19, 0x07, 0x99, 0xa6, 0x46, 0x71, 0x4a, 0xfe, 
	0x44, 0x10, 0x47, 0x0b, 0x90, 0x79, 0x19, 0x05, 0x98, 0x08, 0x1a, 0x07, 0x99, 0xa6, 0x46, 0x6c, 
	0x4a, 0xfe, 0x44, 0x10, 0x47, 0x02, 0x04, 0x12, 0x0c, 0x0b, 0x98, 0x01, 0x04, 0x09, 0x0c, 0x00, 
	0x20, 0xa6, 0x46, 0x68, 0x4b, 0xfe, 0x44, 0x18, 0x47, 0x00, 0x28, 0x00, 0xd1, 0x48, 0xe7, 0x00, 
	0x20, 0x09, 0x90, 0x06, 0xe0, 0xc0, 0x46, 0xe0, 0x31, 0x08, 0x00, 0x1d, 0x48, 0x09, 0x99, 0x81, 
	0x42, 0x02, 0xd0, 0x09, 0x98, 0x0a, 0x90, 0x02, 0xe0, 0x0a, 0x99, 0x81, 0x42, 0x01, 0xd0, 0x0a, 
	0x98, 0x04, 0xe0, 0x06, 0x98, 0xa6, 0x46, 0xba, 0x49, 0xfe, 0x44, 0x08, 0x47, 0x01, 0x99, 0x48, 
	0x60, 0xb6, 0x48, 0xff, 0x30, 0x10, 0x30, 0x0d, 0xb0, 0xf0, 0xbd, 0xb7, 0x4b, 0x04, 0x00, 0xfd, 
	0x79, 0x00, 0x00, 0xb5, 0x49, 0x09, 0x78, 0x2a, 0x29, 0x01, 0xd1, 0x08, 0x21, 0x00, 0xe0, 0x06, 
	0x21, 0x41, 0x60, 0xb0, 0x48, 0x43, 0x30, 0xf7, 0x46, 0xc0, 0x46, 0x4f, 0x81, 0x03, 0x00, 0xc6, 
	0x05, 0x00, 0x00, 0x65, 0x2d, 0x00, 0x00, 0x79, 0x47, 0x00, 0x00, 0x0d, 0x13, 0x01, 0x05, 0xff, 
	0xff, 0x82, 0x72, 0x08, 0x00, 0xfa, 0x02, 0x00, 0x76, 0x24, 0x08, 0x00, 0xe0, 0xa0, 0x1b, 0x00, 
	0x89, 0x28, 0x05, 0x00, 0x3d, 0x39, 0x02, 0x00, 0x60, 0x5b, 0x08, 0x00, 0xff, 0xff, 0x00, 0x00, 
	0xd9, 0xaa, 0x00, 0x00, 0xd5, 0x75, 0x00, 0x00, 0xfc, 0xb5, 0x00, 0x90, 0xa2, 0x48, 0x00, 0x78, 
	0x02, 0x28, 0x43, 0xd1, 0xa1, 0x49, 0xff, 0x20, 0x41, 0x30, 0x40, 0x18, 0x01, 0x90, 0xa0, 0x48, 
	0x40, 0x5c, 0x00, 0x26, 0x38, 0xe0, 0xb1, 0x00, 0x01, 0x98, 0x45, 0x58, 0x00, 0x2d, 0x30, 0xd0, 
	0xfe, 0x20, 0x40, 0x5d, 0x00, 0x28, 0x2c, 0xd0, 0xf2, 0x20, 0x41, 0x5d, 0x00, 0x29, 0x28, 0xd1, 
	0xff, 0x20, 0x02, 0x30, 0x40, 0x5d, 0x01, 0x28, 0x23, 0xd0, 0x24, 0x20, 0x01, 0x24, 0xa6, 0x46, 
	0x94, 0x4a, 0xfe, 0x44, 0x10, 0x47, 0x00, 0x06, 0x00, 0x0e, 0xf3, 0x22, 0x51, 0x5d, 0x81, 0x42, 
	0x17, 0xd0, 0x50, 0x55, 0x00, 0x28, 0x14, 0xd1, 0x2f, 0x8d, 0x39, 0x1c, 0x8e, 0x48, 0xa6, 0x46, 
	0x26, 0x4a, 0xfe, 0x44, 0x10, 0x47, 0x29, 0x8e, 0x79, 0x18, 0x08, 0x1a, 0x40, 0x1e, 0x39, 0x1c, 
	0xa6, 0x46, 0x22, 0x4a, 0xfe, 0x44, 0x10, 0x47, 0x40, 0x00, 0x28, 0x86, 0x28, 0x8e, 0x40, 0x08, 
	0x28, 0x86, 0x01, 0x36, 0x85, 0x48, 0x00, 0x78, 0x86, 0x42, 0xc4, 0xd3, 0xff, 0x21, 0x41, 0x31, 
	0x00, 0x98, 0x41, 0x60, 0x83, 0x48, 0x82, 0x49, 0x08, 0x18, 0xfc, 0xbd, 0x23, 0x01, 0x05, 0x00, 
	0xb3, 0x09, 0x02, 0x00, 0xf9, 0x97, 0x00, 0x00, 0x8f, 0x8d, 0x01, 0x00, 0x10, 0xb5, 0x00, 0x79, 
	0x01, 0x28, 0x06, 0xd1, 0x88, 0x8b, 0x09, 0x7d, 0x01, 0x22, 0x96, 0x46, 0x7a, 0x4a, 0xfe, 0x44, 
	0x10, 0x47, 0x7a, 0x4c, 0x20, 0x78, 0x01, 0x28, 0x04, 0xd1, 0x01, 0x20, 0x86, 0x46, 0x78, 0x48, 
	0x01, 0x05, 0xff, 0xff, 0x7c, 0x73, 0x08, 0x00, 0xfa, 0xfe, 0x44, 0x00, 0x47, 0x00, 0x20, 0x20, 
	0x70, 0x76, 0x49, 0x71, 0x48, 0x88, 0x38, 0x40, 0x18, 0x10, 0xbd, 0xc0, 0x46, 0xa4, 0x13, 0x08, 
	0x00, 0xe5, 0x06, 0x05, 0x00, 0x14, 0x05, 0x1a, 0x00, 0x77, 0xc5, 0x01, 0x00, 0xb9, 0xc5, 0x01, 
	0x00, 0xa1, 0x95, 0x01, 0x00, 0x95, 0x49, 0x05, 0x00, 0x21, 0x96, 0x01, 0x00, 0xf0, 0xb5, 0x86, 
	0xb0, 0x01, 0x92, 0x02, 0x91, 0x06, 0x1c, 0x88, 0x68, 0x00, 0x6a, 0x03, 0x90, 0x80, 0x00, 0x01, 
	0x99, 0x44, 0x18, 0x04, 0x94, 0x04, 0x19, 0x05, 0x94, 0x04, 0x19, 0x01, 0x98, 0x03, 0x99, 0x01, 
	0x25, 0xae, 0x46, 0x63, 0x4a, 0xfe, 0x44, 0x10, 0x47, 0x04, 0x98, 0x03, 0x99, 0xae, 0x46, 0x60, 
	0x4a, 0xfe, 0x44, 0x10, 0x47, 0x05, 0x98, 0x03, 0x99, 0xae, 0x46, 0x5d, 0x4a, 0xfe, 0x44, 0x10, 
	0x47, 0x02, 0x98, 0x80, 0x68, 0xc7, 0x69, 0x30, 0x68, 0x39, 0x1c, 0x03, 0x9a, 0xae, 0x46, 0x59, 
	0x4b, 0xfe, 0x44, 0x18, 0x47, 0x00, 0x28, 0x47, 0xd5, 0x70, 0x68, 0x39, 0x1c, 0x03, 0x9a, 0xae, 
	0x46, 0x54, 0x4b, 0xfe, 0x44, 0x18, 0x47, 0x00, 0x28, 0x3e, 0xd5, 0x30, 0x68, 0x01, 0x99, 0x02, 
	0x9a, 0x23, 0x1c, 0xae, 0x46, 0x50, 0x4f, 0xfe, 0x44, 0x38, 0x47, 0x00, 0x94, 0x30, 0x68, 0x01, 
	0x99, 0x04, 0x9a, 0x02, 0x9b, 0xae, 0x46, 0x4d, 0x4f, 0xfe, 0x44, 0x38, 0x47, 0x00, 0x94, 0x30, 
	0x68, 0x02, 0x99, 0x89, 0x68, 0xc9, 0x6a, 0x01, 0x9a, 0x02, 0x9b, 0xae, 0x46, 0x47, 0x4f, 0xfe, 
	0x44, 0x38, 0x47, 0x01, 0x98, 0x04, 0x99, 0x05, 0x9a, 0x02, 0x9b, 0xae, 0x46, 0x44, 0x4f, 0xfe, 
	0x44, 0x38, 0x47, 0x02, 0x98, 0x80, 0x68, 0x01, 0x6b, 0x05, 0x98, 0x01, 0x9a, 0x02, 0x9b, 0xae, 
	0x46, 0x3f, 0x4f, 0x01, 0x05, 0xff, 0xff, 0x76, 0x74, 0x08, 0x00, 0xfa, 0xfe, 0x44, 0x38, 0x47, 
	0x70, 0x68, 0x04, 0x99, 0x02, 0x9a, 0x23, 0x1c, 0xae, 0x46, 0x39, 0x4c, 0xfe, 0x44, 0x20, 0x47, 
	0x01, 0x98, 0x04, 0x99, 0x03, 0x9a, 0xae, 0x46, 0x35, 0x4b, 0xfe, 0x44, 0x18, 0x47, 0x00, 0x28, 
	0x02, 0xd0, 0x00, 0x20, 0xc0, 0x43, 0x00, 0xe0, 0x00, 0x20, 0x06, 0xb0, 0xf0, 0xbd, 0xf0, 0xb5, 
	0x85, 0xb0, 0x45, 0x69, 0x86, 0x69, 0xc0, 0x69, 0x02, 0x90, 0x0f, 0x6a, 0x48, 0x6a, 0x03, 0x90, 
	0x88, 0x6b, 0x04, 0x90, 0x28, 0x1c, 0x00, 0x95, 0x31, 0x6a, 0x89, 0x00, 0x4d, 0x19, 0x01, 0x95, 
	0x4d, 0x19, 0x31, 0x68, 0x01, 0x29, 0x0d, 0xd1, 0x72, 0x69, 0x39, 0x1c, 0x01, 0x24, 0xa6, 0x46, 
	0x12, 0x4b, 0xfe, 0x44, 0x18, 0x47, 0x72, 0x69, 0x01, 0x98, 0xb9, 0x18, 0xa6, 0x46, 0x0e, 0x4b, 
	0xfe, 0x44, 0x18, 0x47, 0x68, 0x46, 0x02, 0x99, 0x2a, 0x1c, 0x01, 0x24, 0xa6, 0x46, 0x1f, 0x4b, 
	0xfe, 0x44, 0x18, 0x47, 0x1f, 0x4d, 0x00, 0x28, 0x02, 0xd0, 0x28, 0x1c, 0x21, 0x30, 0x08, 0xe0, 
	0xb2, 0x69, 0x03, 0x98, 0x04, 0x99, 0xa6, 0x46, 0x1b, 0x4b, 0xfe, 0x44, 0x18, 0x47, 0x28, 0x1c, 
	0xa9, 0x30, 0x05, 0xb0, 0xf0, 0xbd, 0x6d, 0x95, 0x00, 0x00, 0x5d, 0x5f, 0x05, 0x00, 0x4d, 0x96, 
	0x01, 0x00, 0x7b, 0x5e, 0x02, 0x00, 0x69, 0x53, 0x08, 0x00, 0x45, 0x10, 0x08, 0x00, 0x54, 0x24, 
	0x08, 0x00, 0x07, 0x0e, 0x00, 0x00, 0x2b, 0xf0, 0x04, 0x00, 0xff, 0xff, 0xff, 0x01, 0x5b, 0x32, 
	0x08, 0x00, 0x8f, 0x02, 0x00, 0x00, 0xcd, 0x92, 0x01, 0x00, 0x6f, 0x4b, 0x02, 0x00, 0x50, 0x66, 
	0x08, 0x00, 0x43, 0x1c, 0x03, 0x00, 0xf5, 0x70, 0x00, 0x00, 0x21, 0x37, 0x05, 0x00, 0xa9, 0x36, 
	0x05, 0x00, 0xc3, 0x6c, 0x05, 0x00, 0x01, 0x05, 0xff, 0xb9, 0x70, 0x75, 0x08, 0x00, 0xb4, 0xa5, 
	0x6c, 0x05, 0x00, 0xd1, 0x1c, 0x05, 0x00, 0xb1, 0x73, 0x08, 0x00, 0x91, 0x48, 0x05, 0x00, 0x47, 
	0x37, 0x05, 0x00, 0xff, 0xb5, 0x68, 0x46, 0xff, 0xf7, 0x7a, 0xfa, 0xff, 0xbd, 0xff, 0xb5, 0x68, 
	0x46, 0xff, 0xf7, 0xaf, 0xfa, 0xff, 0xbd, 0xff, 0xb5, 0x68, 0x46, 0xff, 0xf7, 0x46, 0xfb, 0xff, 
	0xbd, 0xff, 0xb5, 0x68, 0x46, 0xff, 0xf7, 0x69, 0xfb, 0xff, 0xbd, 0xff, 0xb5, 0x68, 0x46, 0xff, 
	0xf7, 0xc2, 0xfb, 0xff, 0xbd, 0xff, 0xb5, 0x68, 0x46, 0xff, 0xf7, 0x9d, 0xfb, 0xff, 0xbd, 0xff, 
	0xb5, 0x68, 0x46, 0xff, 0xf7, 0x7a, 0xfb, 0xff, 0xbd, 0xff, 0xb5, 0x68, 0x46, 0xff, 0xf7, 0x56, 
	0xf8, 0xff, 0xbd, 0xff, 0xb5, 0x68, 0x46, 0xff, 0xf7, 0x6f, 0xf8, 0xff, 0xbd, 0xff, 0xb5, 0x68, 
	0x46, 0xff, 0xf7, 0x37, 0xf8, 0xff, 0xbd, 0xff, 0xb5, 0x68, 0x46, 0xff, 0xf7, 0x40, 0xf8, 0xff, 
	0xbd, 0xff, 0xb5, 0x68, 0x46, 0xff, 0xf7, 0x97, 0xf8, 0xff, 0xbd, 0xff, 0xb5, 0x68, 0x46, 0xff, 
	0xf7, 0x0d, 0xf9, 0xff, 0xbd, 0xff, 0xb5, 0x68, 0x46, 0xff, 0xf7, 0xd5, 0xf9, 0xff, 0xbd, 0xff, 
	0xb5, 0x68, 0x46, 0xff, 0xf7, 0xb3, 0xf8, 0xff, 0xbd, 0xff, 0xb5, 0x68, 0x46, 0xff, 0xf7, 0xd4, 
	0xf8, 0xff, 0xbd, 0x01, 0x05, 0xff, 0x8d, 0x78, 0x7b, 0x08, 0x00, 0x88, 0x00, 0xb5, 0xf8, 0xf0, 
	0xd5, 0xfa, 0x00, 0xbd, 0x03, 0x6e, 0x08, 0x00, 0x8d, 0x6e, 0x08, 0x00, 0xd5, 0x65, 0x08, 0x00, 
	0xa5, 0x6e, 0x08, 0x00, 0x11, 0x6f, 0x08, 0x00, 0x2d, 0x6f, 0x08, 0x00, 0x45, 0x6f, 0x08, 0x00, 
	0x5b, 0x6f, 0x08, 0x00, 0x69, 0x6f, 0x08, 0x00, 0x99, 0x6f, 0x08, 0x00, 0xad, 0x6f, 0x08, 0x00, 
	0xc5, 0x6f, 0x08, 0x00, 0x55, 0x70, 0x08, 0x00, 0x59, 0x72, 0x08, 0x00, 0xa5, 0x72, 0x08, 0x00, 
	0x59, 0x73, 0x08, 0x00, 0xa9, 0x74, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xf1, 0x6e, 0x08, 0x00, 0x01, 0x05, 0xff, 0x85, 0x04, 0xf3, 0x1a, 0x00, 0x80, 0xce, 0xfb, 0x04, 
	0x00, 0xde, 0x33, 0x02, 0x00, 0x14, 0x29, 0x02, 0x00, 0xf4, 0x4b, 0x04, 0x00, 0x60, 0x13, 0x02, 
	0x00, 0xd6, 0x28, 0x05, 0x00, 0x74, 0x39, 0x02, 0x00, 0x88, 0xac, 0x00, 0x00, 0x0c, 0x76, 0x00, 
	0x00, 0x06, 0x0a, 0x02, 0x00, 0x50, 0x98, 0x00, 0x00, 0xb4, 0xc6, 0x01, 0x00, 0x7a, 0x95, 0x00, 
	0x00, 0xb8, 0x5e, 0x02, 0x00, 0x58, 0x95, 0x01, 0x00, 0xe8, 0x72, 0x00, 0x00, 0x2e, 0x49, 0x05, 
	0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x52, 0x48, 0x00, 0x00, 0x01, 0x05, 0xff, 
	0xff, 0x00, 0x00, 0x18, 0x00, 0xfa, 0x70, 0xb5, 0x35, 0x4d, 0xae, 0x7f, 0x01, 0x24, 0xa6, 0x46, 
	0x31, 0x48, 0xfe, 0x44, 0x00, 0x47, 0xb0, 0x42, 0xf8, 0xd1, 0x03, 0x20, 0x17, 0x21, 0x89, 0x01, 
	0xa6, 0x46, 0x2d, 0x4a, 0xfe, 0x44, 0x10, 0x47, 0xad, 0x7f, 0xa6, 0x46, 0x2a, 0x48, 0xfe, 0x44, 
	0x00, 0x47, 0xa8, 0x42, 0xf9, 0xd1, 0xfe, 0xe7, 0x10, 0xb5, 0x29, 0x49, 0x08, 0x1f, 0x29, 0x4a, 
	0x10, 0x60, 0x2c, 0x48, 0x02, 0x1c, 0x71, 0x3a, 0x93, 0x24, 0x01, 0x23, 0xa3, 0x54, 0x26, 0x4b, 
	0x0b, 0x60, 0x02, 0x23, 0x13, 0x71, 0x25, 0x4b, 0x4b, 0x60, 0x03, 0x23, 0x53, 0x71, 0x24, 0x4b, 
	0x8b, 0x60, 0x04, 0x23, 0x03, 0x70, 0x24, 0x4b, 0xcb, 0x60, 0x05, 0x23, 0x83, 0x73, 0x23, 0x4b, 
	0x0b, 0x61, 0x06, 0x23, 0x03, 0x73, 0x22, 0x4b, 0x4b, 0x61, 0x07, 0x23, 0x43, 0x71, 0x21, 0x4b, 
	0x8b, 0x61, 0x08, 0x24, 0x03, 0x1c, 0x33, 0x3b, 0x1c, 0x70, 0x1f, 0x4b, 0xcb, 0x61, 0x09, 0x23, 
	0xd3, 0x74, 0x1e, 0x4b, 0x0b, 0x62, 0x0a, 0x23, 0xd3, 0x71, 0x1d, 0x4a, 0x4a, 0x62, 0x0b, 0x22, 
	0x42, 0x77, 0x1c, 0x4a, 0x8a, 0x62, 0x0c, 0x23, 0x02, 0x1c, 0x3d, 0x3a, 0x13, 0x70, 0x1a, 0x4a, 
	0xca, 0x62, 0x0f, 0x38, 0x0d, 0x22, 0x02, 0x70, 0x19, 0x4a, 0x0a, 0x63, 0x0e, 0x22, 0x02, 0x73, 
	0x18, 0x4a, 0x4a, 0x63, 0x0f, 0x22, 0x42, 0x73, 0x17, 0x4a, 0x8a, 0x63, 0x10, 0x22, 0x02, 0x77, 
	0x16, 0x48, 0xc8, 0x63, 0x10, 0xbd, 0x25, 0x86, 0x04, 0x00, 0x1b, 0x90, 0x04, 0x00, 0x6c, 0x52, 
	0x08, 0x00, 0x08, 0x66, 0x08, 0x00, 0x20, 0x55, 0x08, 0x00, 0xdf, 0x75, 0x08, 0x00, 0xf3, 0x75, 
	0x08, 0x00, 0xd5, 0x75, 0x08, 0x00, 0x95, 0x55, 0x08, 0x00, 0x11, 0x76, 0x08, 0x00, 0xe9, 0x75, 
	0x01, 0x05, 0xff, 0xff, 0xfa, 0x00, 0x18, 0x00, 0xfa, 0x08, 0x00, 0xcb, 0x75, 0x08, 0x00, 0x1b, 
	0x76, 0x08, 0x00, 0xfd, 0x75, 0x08, 0x00, 0x07, 0x76, 0x08, 0x00, 0x8f, 0x75, 0x08, 0x00, 0x99, 
	0x75, 0x08, 0x00, 0x85, 0x75, 0x08, 0x00, 0xa3, 0x75, 0x08, 0x00, 0xc1, 0x75, 0x08, 0x00, 0xb7, 
	0x75, 0x08, 0x00, 0xad, 0x75, 0x08, 0x00, 0xf0, 0xb5, 0x28, 0x4e, 0x0c, 0x22, 0x32, 0x70, 0x1a, 
	0x23, 0x73, 0x70, 0x09, 0x20, 0xb0, 0x70, 0x10, 0x20, 0xf0, 0x70, 0x03, 0x20, 0x25, 0x4d, 0x29, 
	0x1c, 0x01, 0x39, 0x01, 0x24, 0xa6, 0x46, 0x21, 0x4f, 0xfe, 0x44, 0x38, 0x47, 0xb2, 0x78, 0xf3, 
	0x78, 0x03, 0x20, 0x29, 0x1c, 0xa6, 0x46, 0x1d, 0x4e, 0xfe, 0x44, 0x30, 0x47, 0x03, 0x20, 0x29, 
	0x1c, 0x01, 0x31, 0xa6, 0x46, 0x1c, 0x4a, 0xfe, 0x44, 0x10, 0x47, 0xa6, 0x46, 0x1b, 0x48, 0xfe, 
	0x44, 0x00, 0x47, 0x1a, 0x4b, 0x00, 0x21, 0x08, 0x1c, 0x1a, 0x68, 0x00, 0x2a, 0x04, 0xd0, 0x02, 
	0x07, 0x15, 0x0f, 0x22, 0x1c, 0xaa, 0x40, 0x11, 0x43, 0x02, 0x07, 0x12, 0x0f, 0x0f, 0x2a, 0x05, 
	0xd1, 0xc5, 0x08, 0x06, 0x22, 0x2a, 0x40, 0x12, 0x4d, 0xa9, 0x52, 0x00, 0x21, 0x04, 0x33, 0x01, 
	0x30, 0x20, 0x28, 0xe9, 0xd3, 0x11, 0x48, 0x01, 0x1c, 0x50, 0x31, 0x0c, 0x70, 0x0a, 0x21, 0x0d, 
	0x4a, 0x11, 0x70, 0x2a, 0x21, 0x01, 0x70, 0x00, 0x20, 0x0d, 0x49, 0x08, 0x60, 0xa6, 0x46, 0x0d, 
	0x49, 0xfe, 0x44, 0x08, 0x47, 0x0b, 0x49, 0x08, 0x60, 0xf0, 0xbd, 0xfc, 0x53, 0x08, 0x00, 0x31, 
	0x90, 0x04, 0x00, 0xc6, 0x05, 0x00, 0x00, 0x1b, 0x90, 0x04, 0x00, 0x33, 0x00, 0x18, 0x00, 0x80, 
	0x7b, 0x08, 0x00, 0x84, 0xf3, 0x1a, 0x00, 0x6d, 0x22, 0x08, 0x00, 0x69, 0x53, 0x08, 0x00, 0x48, 
	0x66, 0x08, 0x00, 0x01, 0x05, 0xff, 0x0d, 0xf4, 0x01, 0x18, 0x00, 0x08, 0x4c, 0x66, 0x08, 0x00, 
	0xd1, 0xa1, 0x04, 0x00, 0x01, 0x83, 0xff, 0x14, 0x79, 0x7b, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0c, 0xfd, 0x09, 
	0x01, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x64, 0x00, 0x01, 0x09, 0xfd, 0x08, 0x58, 0x60, 0x1a, 
	0x00, 0x00, 0x10, 0x00, 0x10, 0x01, 0x09, 0xfd, 0x08, 0x10, 0x60, 0x1a, 0x00, 0x10, 0x00, 0x10, 
	0x00, 0x01, 0x1c, 0xfd, 0x14, 0xff, 0x88, 0x13, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0x00, 0xfa, 0x00, 0xff, 0xff, 0x00, 0x01, 0x76, 0xfd, 0x31, 0x01, 0x21, 0x54, 
	0x00, 0x00, 0x61, 0x57, 0x00, 0x00, 0x14, 0x05, 0x0a, 0x05, 0x00, 0x07, 0x06, 0x0a, 0x04, 0x05, 
	0x08, 0x09, 0x0b, 0x0c, 0x0d, 0x0e, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 
	0xfd, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x80, 0xfd, 0x06, 0x3c, 0xf0, 0x5f, 0x00, 
	0x00, 0x00, 0x01, 0x38, 0xfe, 0x00

#ifdef __SUPPORT_LOW_ENERGY__
	/* Add LE enable, just for Hosts that asks the controller if supporting LE, before loading the LE */
	,0x01, 0x5b, 0xfd, 0x02, 0x01, 0x01
#endif //__SUPPORT_LOW_ENERGY__
};


static BTPSCONST unsigned int BasePatchLength = sizeof(BasePatch);

#ifdef __SUPPORT_LOW_ENERGY__
   /* The following is the binary representation of the Low Energy Patch*/
   /* Data.                                                             */
static BTPSCONST unsigned char LowEnergyPatch[] =
{
	0x01, 0x5b, 0xfd, 0x02, 0x01, 0x01, 0x01, 0xdd, 0xfd, 0x01, 0x01
};

static BTPSCONST unsigned int LowEnergyPatchLength = sizeof(LowEnergyPatch);
#endif //__SUPPORT_LOW_ENERGY__

 /* The following is the binary representation of the AVPR Patch Data.*/
static BTPSCONST unsigned char AvprPatch[] =
{
	0x01, 0x92, 0xfd, 0x05, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0xa8, 0xfd, 0x01, 0x01, 0x01, 0x09, 
	0xfd, 0x08, 0x00, 0xc0, 0x1b, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x05, 0xff, 0x09, 0x78, 0x27, 
	0x1b, 0x00, 0x04, 0xf4, 0x75, 0x00, 0x00, 0x01, 0x05, 0xff, 0x09, 0x48, 0x3b, 0x1b, 0x00, 0x04, 
	0xf4, 0x75, 0x00, 0x00, 0x01, 0x05, 0xff, 0x25, 0x44, 0x3d, 0x1b, 0x00, 0x20, 0xd4, 0x1f, 0x00, 
	0x20, 0xd8, 0x1f, 0x00, 0x20, 0xdc, 0x1f, 0x00, 0x20, 0xe0, 0x1f, 0x00, 0x20, 0xcc, 0x1f, 0x00, 
	0x20, 0xce, 0x1f, 0x00, 0x20, 0xd0, 0x1f, 0x00, 0x20, 0xd2, 0x1f, 0x00, 0x20, 0x01, 0x05, 0xff, 
	0x09, 0x1c, 0x5a, 0x1b, 0x00, 0x04, 0xf4, 0x75, 0x00, 0x00, 0x01, 0x05, 0xff, 0x09, 0xc4, 0x60, 
	0x1b, 0x00, 0x04, 0xf4, 0x75, 0x00, 0x00, 0x01, 0x05, 0xff, 0x09, 0xbc, 0x67, 0x1b, 0x00, 0x04, 
	0xf7, 0x75, 0x00, 0x00, 0x01, 0x05, 0xff, 0x09, 0xa8, 0x6a, 0x1b, 0x00, 0x04, 0xf8, 0x75, 0x00, 
	0x00, 0x01, 0x05, 0xff, 0x09, 0x14, 0x6d, 0x1b, 0x00, 0x04, 0xf7, 0x75, 0x00, 0x00, 0x01, 0x05, 
	0xff, 0x09, 0x80, 0x73, 0x1b, 0x00, 0x04, 0xf7, 0x75, 0x00, 0x00, 0x01, 0x01, 0xff, 0x06, 0xca, 
	0xbf, 0x1b, 0x00, 0x00, 0x00, 0x01, 0x09, 0xfd, 0x08, 0x00, 0xc0, 0x1b, 0x00, 0x00, 0x00, 0x01, 
	0x00, 0x01, 0xa8, 0xfd, 0x01, 0x00
};

static BTPSCONST unsigned int AvprPatchLength = sizeof(AvprPatch);


#endif //_CC256XC_H_
