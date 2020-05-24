
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
	// init crap
	consoleDemoInit();
	
	//turn interrupts off
	REG_IME = 0;

	
	// acknowledge every interrupt
	REG_IF = 0xffff;

	// now get the current line 
	// and wait till we are not in hblank or vblank
	// after that write the current line to lyc and waste a few cycles
	// see if we get a lyc set


	while((REG_DISPSTAT & 0x3) != 0)
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
	

	
	// enable vcount if
	REG_DISPSTAT |= 1 << 5;
	
	int ly = REG_VCOUNT;
	
	volatile char *lyc = (char*)0x4000005;
	
	// start time
	volatile uint16_t start = *tm0cnt_l;
	
	// write vcount and pull dispstat
	*lyc = ly;
	
	// pull dispstat
	volatile uint16_t dispstat = REG_DISPSTAT;
	
	// end
	volatile uint16_t end = *tm0cnt_l;
	
	
	// if pulled after but so its not part of the timing but just pulled for arguements sake
	volatile uint16_t interrupt_flags = REG_IF;
	
	
	
	printf("start %x\n",start);
	printf("end %x\n",end);
	printf("dispstat: %x\n",dispstat & (1 << 2));
	printf("if: %x\n",interrupt_flags);
	
	// yeah i know i should use a proper wait here but allow it
	for(;;) {}
	
}