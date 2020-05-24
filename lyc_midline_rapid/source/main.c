
#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>





int main(void) 
{
	// init stuff
	consoleDemoInit();
	
	volatile uint16_t * ime = (uint16_t*)0x04000208;
	
	//turn interrupts off
	*ime = 0;

	volatile uint16_t *disp_stat = (uint16_t*)0x04000004;
	
	// turn off vcount intr
	*disp_stat &= ~(1 << 5);
	
	
	volatile uint16_t *interrupt_flag = (uint16_t*)0x04000202;
	
	*interrupt_flag = 0xffff;

	// now get the current line 
	// and wait till we are not in hblank or vblank
	// after that write the current line to lyc 
	// and an invalid one over and over


	volatile uint16_t *vcount = (uint16_t*)0x04000006;
	
	// sync to 0x10 visible drawing
	while((REG_DISPSTAT & 0x3) != 0 && *vcount != 0x10)
	{

	}
	
	volatile uint16_t *tm0cnt_l = (uint16_t*)4000100;
	
	volatile uint16_t *tm0cnt_h = (uint16_t*)4000102;
	
	// disable timer and set reload to zero
	*tm0cnt_h = 0;
	*tm0cnt_l = 0;
	
	asm volatile("nop");
	
	// 1 cycle prescaler selection
	// count up and eanble
	*tm0cnt_h = (1 << 7); 
	

	
	// enable vcount intr
	*disp_stat |= 1 << 5;
	
	
	
	volatile uint16_t ly = REG_VCOUNT;
	
	volatile char *lyc = (char*)0x4000005;
	
	// start time
	volatile uint16_t start = *tm0cnt_l;

	

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
			// write vcount and pull dispstat
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