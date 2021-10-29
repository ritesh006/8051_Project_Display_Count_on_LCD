/*----------------------------------------------------------------------------------

	Project: Count number of times switch pressed and display count number on LCD.
	
	LCD data pins are connected to Port 0 and control pins to Port 2.0 to 2.2 .
	Switch is connected to Port 3.4 (counter 0 pin).

----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------

	You can’t display two digits or more directly on LCD. Suppose number is 15, then 1st

	you have to send 1 and then 5 to LCD. We know that while sending numbers to LCD, we 
	
	have to send it in ASCII format.  

	ASCII value of 0 is 0x30,  

	ASCII value of 1 is 0x31  

	and similarly ASCII value of 9 is 0x39  

	Hence if you want to display 1 you have to send (1 + 0x30) i.e 0x31 to LCD and then to 
	
	display 5, you have to send (5 + 0x30) i.e 0x35 to LCD.  

	Now somehow we have to segregate digits 1 and 5 from number 15.  

	Displayed number on LCD is 5  

-------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------
	Here is the logic.  

	Let’s say you have collected number in count_val variable of type unsigned int and i is temporary variable of type unsigned char.  

	count_val = 15;        

	To get the Ten's place digit you have to divide number by 10.  Here in our case 15/10 result in 1 as quotient    

	i = count_val/10;   // i= 1  

	now to convert digit into ASCII add 0x30  i.e (i + 0x30) and then send it  to LCD as a data.  

	send_data(i + 0x30);  // send_data(0x31)  // Displayed number on LCD is 1  

	To get the unit place digit you have to take mod 10 of number.  Here in our case 15%10 result in 5 as remainder. MOD operation gives you remainder.

	i = count_val%10 ;  // i= 5  

	ow to convert digit into ASCII add 0x30 i.e  (i + 0x30) and then send it to LCD as a data.  

	send_data(i + 0x30);  // send_data(0x35)  // Displayed number on LCD is 5  
---------------------------------------------------------------------------------------------*/


#include<reg52.h>												/* Register address header file */

sbit counter=P3^4;											/* Rename input pin as counter */

sbit RS=P2^0;														/* Rename register select pin as a RS */														
sbit RW=P2^1;														/* Rename read/write pin as a RW */
sbit E=P2^2;														/* Rename enable pin as a E */

void send_command(unsigned int command_value ) ;			/* command function prototype */
void send_data(unsigned int data_value) ;							/* data function prototype */
void ms_delay( unsigned int time);										/* delay function prototype */
unsigned char i;																			/* global variable i declare */
unsigned int count_val;																/* global variable count_val declare */			
void main()
{
        
/* LCD INITIALIZATION  */

/* 2 lines and 5x7 matrix */
send_command(0x38);

/* display on cursor blink */
send_command(0x0E);
    
/* clear display screen */
send_command(0X01);

/* counter initialization */
	
TMOD=0X06; /* counter 0, mode 2, 8 bit mode */
counter=1; /* p3.4 input pin */
    
while(1)
{
    
TH0=0x00;
TL0=0x00;    /* count value initialize */
    
TR0=1; /* counter 0 on */

while(!TF0) /* monitoring TF0 flag */
{
    send_command(0x80)    ; /* force cursor on 1st line and 1st col */   
    
    count_val = TL0;    
 
    i = count_val/10; /* Get the Ten's place */
  /*     i + 0x30;             Convert it to ASCII */
  send_data(i + 0x30);            /* Display Tens place */
      
    i = count_val%10 ;            /* Get the unit place */
  //     i + 0x30;             /* Convert it to ASCII */
  send_data(i + 0x30);    /* Display unit place */
    
    if (count_val == 100)   /* restricting count 00 to 99 */
    {
        break;
    }
}

TF0=0;  /* reset flag */
TR0=0; /* off counter */
}

}

void send_command(unsigned int command_value ) /* send command to LCD */
{
    P1=command_value;
    RW=0;
    RS=0;
    E=1;
    ms_delay(10);
    E=0;
}    

void send_data(unsigned int data_value) /* send data to LCD */
{
    P1=data_value;
    RW=0;
    RS=1;
    E=1;
    ms_delay(10);
    E=0;
}

void ms_delay( unsigned int time)   /* delay generation */
{
    unsigned int i,j;
    /* time X 1ms */
    for(i=0;i<time;i++)
    {
        for(j=0;j<113;j++);  /* 1 ms */
    }
}



   