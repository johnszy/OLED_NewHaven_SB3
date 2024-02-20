/*
 *
 *     SB3 NewHaven Display driver for 16(L)F1619
 * 
 *     NHD-0216MW-SB3
 *     https://www.mouser.com/ProductDetail/Newhaven-Display/NHD-0216MW-SB3?qs=P0I6NS6hZyT6mMekoc0ebw%3D%3D
 * 
 *     author: john szymanski
 *     johnszy@icloud.com
 * 
 * 
 *     IDE:  MPLAB X IDE v5.40  (www.microchip.com)
 *     C Compiler:   XC8 v2.30
 *     
 * 
 *     Pinouts:     Pin  9  = RC7 = /RES reset for OLED
 *                  Pin 10  = RB7 = /CS for OLED
 *                  Pin 11  = RB6 = CLK
 *                  Pin 12  = RB5 = SDO  --> wire to SDI of Device to drive
 *                  Pin 13  = RB4 = SDI  -->>wire to SDO of Device to drive
 * 
 * 
 * 
 *                  SSPCLKPPS = 0x0E;   //RB6->MSSP:SCK;
 *                  SSPDATPPS = 0x0C;   //RB4->MSSP:SDI;
 *                  RB6PPS = 0x10;   //RB6->MSSP:SCK;
 *                  RB5PPS = 0x11;   //RB5->MSSP:SDO;
 *   

 * 
 *  */


#include "mcc_generated_files/mcc.h"
unsigned char dummy;
unsigned long milliTime=0;

unsigned long millis(void);
void spi_write(unsigned char );
void command(unsigned char );
void dataCMD(unsigned char );
void initOLED(void);
void printRegValue(char *);

unsigned char text1[] = {"    johnszy     "};
unsigned char text2[] = {"  Engineering   "};
unsigned char text3[] = {"   Warming  Up  "};
unsigned char text4[] = {" One Moment...  "};

unsigned char text5[] = {" OSCCON =       "};
unsigned char text6[] = {"     howdy      "};
unsigned char text7[] = {"  Then press    "};
unsigned char text8[] = {"  Green Button  "};


void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    
    __delay_ms(1000);
    
    initOLED();
    
     __delay_ms(1000);
    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    int i;
    
    while (1)
    {
      

        command(0x01);
        __delay_ms(2);
        for(i=0;i<16;i++){
          dataCMD(text1[i]);
        }
        
        command(0xA0);
        for(i=0;i<16;i++){
          dataCMD(text2[i]);
        }

        __delay_ms(3000);

        command(0x01);
        __delay_ms(2);
        for (i=0;i<16;i++){
		dataCMD(text3[i]);
        }
	
        command(0xA0);
        for (i=0;i<16;i++){
		dataCMD(text4[i]);
        }

        __delay_ms(3500);
        
        printRegValue( text5 );
        
        __delay_ms(3000);

        command(0x01);
        __delay_ms(2);
        for (i=0;i<16;i++){
		dataCMD(text7[i]);
        }
	
        command(0xA0);
        for (i=0;i<16;i++){
		dataCMD(text8[i]);
        }
        __delay_ms(3000);
    }
}

void printRegValue(char * s)
{

   unsigned char tempReg = '\x00' ;
   int i;
   
   
   tempReg = OSCCON;  // INSERT REGISTER ASSIGNMENT HERE
   
   unsigned char lowerBits = tempReg & '\x0F';
   unsigned char upperBits =  (tempReg >> 4) & '\x0F';
   
   
   if (lowerBits< 10)
       s[11]= lowerBits | '\x30';
   else
       s[11] = ((lowerBits-1)&'\x07') | '\x40';
   
     
   if (upperBits< 10)
       s[10]= upperBits | '\x30';
   else
       s[10] = ((upperBits- 1)&'\x07') | '\x40';
   
   command(0x01);
   __delay_ms(2);
   for(i=0;i<16;i++){
      dataCMD(s[i]);
   }
        
  // for(i=0;i<16;i++){
  //    text6[i]= ' ';
  // }
 
   
   command(0xA0);
   for(i=0;i<16;i++){
      dataCMD( text6[i] );
   }

}

unsigned long millis(){
    return milliTime;
}


void spi_write(unsigned char spiByte)
{
     SSPBUF=spiByte;	//write byte to sspbuf, which starts transfer
     
     while(!BF );	//wait until transfer complete
     dummy= SSPBUF;	//read out the data to clear flags
}
void command(unsigned char s)
{
   
    LATB7=0;   //  /CS chip select
    spi_write('\xF8');  // denotes command mode to U2066 controller
    
	unsigned char bit0 = '\x00' ;
	unsigned char sendByte = '\x00';
	
	for(int i=0; i< 8; i++)
	{   
		bit0 = s & 1;
		//printChar(bit0);
		s = s >> 1;
		sendByte = sendByte << 1;
		if (bit0 == 1){
			//printf("here\n");
			sendByte= sendByte | 1;}
		if (i==3)
		{	
		  sendByte = sendByte << 4;
		  spi_write(sendByte);
		  sendByte = '\x00';
	    }
	}
      sendByte = sendByte << 4;
	  spi_write(sendByte);
	 // sendByte = '\x00';	
     // __delay_ms(30); // delay 10 ms
	  LATB7=1;
}

void dataCMD(unsigned char s)
{
   
    LATB7 =0;  // /CS chip select
    spi_write('\xFA');  // denotes data mode to U2066 controller
    
	unsigned char bit0 = '\x00' ;
	unsigned char sendByte = '\x00';
	
	for(int i=0; i< 8; i++)
	{   
		bit0 = s & 1;
		//printChar(bit0);
		s = s >>1;
		sendByte = sendByte <<1;
		if (bit0 == 1){
			//printf("here\n");
			sendByte= sendByte | 1;}
		if (i==3)
		{	
		  sendByte = sendByte << 4;
		  spi_write(sendByte);
		  sendByte = '\x00';
	    }
	}
      sendByte = sendByte << 4;
	  spi_write(sendByte);
	 // sendByte = '\x00';	
     // __delay_ms(30); // delay 100 ms
	  LATB7=1;
}

void initOLED()
{
    //SPI.begin();
     //digitalWrite(RES, HIGH);  // set /RES pin high
     LATC7 = 1;   // set /RES pin high
     
    __delay_ms(100); // delay 10 ms
    
    command(0x2A);  //function set (extended command set)
	command(0x71);  //function selection A, disable internal Vdd regualtor
	dataCMD(0x00);
	command(0x28);  //function set (fundamental command set)
	command(0x08);  //display off, cursor off, blink off
	command(0x2A);  //function set (extended command set)
	command(0x79);  //OLED command set enabled
	command(0xD5);  //set display clock divide ratio/oscillator frequency
	command(0x70);  //set display clock divide ratio/oscillator frequency
	command(0x78);  //OLED command set disabled
	command(0x09);  //extended function set (4-lines)
    command(0x06);  //COM SEG direction
	command(0x72);  //function selection B, disable internal Vdd regualtor
	dataCMD(0x00);     //ROM CGRAM selection
	command(0x2A);  //function set (extended command set)
	command(0x79);  //OLED command set enabled
	command(0xDA);  //set SEG pins hardware configuration
//	command(0x10);  //set SEG pins ... NOTE: When using NHD-0216AW-XB3 or NHD_0216MW_XB3 change to (0x00)
	command(0x00);  //set SEG pins ... NOTE: When using NHD-0216AW-XB3 or NHD_0216MW_XB3 change to (0x00)
	command(0xDC);  //function selection C
	command(0x00);  //function selection C
	command(0x81);  //set contrast control
	command(0x7F);  //set contrast control
	command(0xD9);  //set phase length
	command(0xF1);  //set phase length
	command(0xDB);  //set VCOMH deselect level
	command(0x40);  //set VCOMH deselect level
	command(0x78);  //OLED command set disabled
	command(0x28);  //function set (fundamental command set)
	command(0x01);  //clear display
	command(0x80);  //set DDRAM address to 0x00
	command(0x0C);  //display ON
      
    __delay_ms(100); // delay 100 ms
}
/**
 End of File
*/