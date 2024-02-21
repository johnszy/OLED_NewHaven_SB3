/*
 *     SB3 NewHaven Display driver for 16(L)F1619
 * 
 *     author: john szymanski
 *     johnszy@icloud.com
 * 
 *     IDE:  MPLAB X IDE v5.40  (www.microchip.com)
 *     C Compiler:   XC8 v2.30
 *     
 *     Pinouts:     Pin  9  = RC7 = /RES reset for OLED
 *                  Pin 10  = RB7 = /CS for OLED
 *                  Pin 11  = RB6 = CLK
 *                  Pin 12  = RB5 = SDO  --> wire to SDI of Device to drive
 *                  Pin 13  = RB4 = SDI  -->>wire to SDO of Device to drive
 * 
 *                  SSPCLKPPS = 0x0E;   //RB6->MSSP:SCK;
 *                  SSPDATPPS = 0x0C;   //RB4->MSSP:SDI;
 *                  RB6PPS = 0x10;   //RB6->MSSP:SCK;
 *                  RB5PPS = 0x11;   //RB5->MSSP:SDO;
 *   
 *     Mouser Part #: NHD-0216MW-SB3
 *     https://www.mouser.com/ProductDetail/Newhaven-Display/NHD-0216MW-SB3?qs=P0I6NS6hZyT6mMekoc0ebw%3D%3D
 * 
 *  */
