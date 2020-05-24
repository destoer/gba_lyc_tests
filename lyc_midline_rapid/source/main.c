
#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>



volatile uint16_t *tm0cnt_l = (uint16_t*)0x04000100;
volatile uint16_t *disp_stat = (uint16_t*)0x04000004;
volatile uint16_t *tm0cnt_h = (uint16_t*)0x04000102;
volatile uint16_t * ime = (uint16_t*)0x04000208;
volatile uint16_t *interrupt_flag = (uint16_t*)0x04000202;
volatile uint8_t *vcount = (uint8_t*)0x04000006;
volatile uint8_t *lyc = (uint8_t*)0x4000005;


int main(void) 
{
	// init stuff
	consoleDemoInit();
	
	
	
	//turn interrupts off
	*ime = 0;


	
	// turn off vcount intr
	*disp_stat &= ~(1 << 5);
	
	
	
	*interrupt_flag = 0xffff;




	
	// sync to 0x10 visible drawing
	while((*disp_stat & 0x3) != 0 && *vcount != 0x10)
	{

	}
	

	
	// disable timer and set reload to zero
	*tm0cnt_h = 0;
	*tm0cnt_l = 0;
	
	
	// 1 cycle prescaler selection
	// count up and enable
	*tm0cnt_h = (1 << 7); 
	

	
	// enable vcount intr
	*disp_stat |= 1 << 5;
	
	
	
	volatile uint16_t ly = *vcount;
	

	
	// start time
	volatile uint16_t start = *tm0cnt_l;


	
	// write the current line to lyc 
	// and an invalid one over and over
	// to trigger interrupts

	int count = 0;
	while(*vcount == ly)
	{
		// vcount fired
		if((*interrupt_flag & (1 << 2)))
		{
			count++;
			// clear intr
			*interrupt_flag = 0xffff;
			*lyc = 255; // set lyc to a invalid value
		}
		
		else
		{
			// set compare to current vcount
			*lyc = ly;
		}
	}
	
	// end time
	volatile uint16_t end = *tm0cnt_l;
	
	
	
	
	
	printf("start %x\n",start);
	printf("end %x\n",end);
	printf("count %x\n",count);
	
	// yeah i know i should use a proper wait here but allow it
	for(;;) {}
	
}