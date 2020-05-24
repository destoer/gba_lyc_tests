
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
	// init crap
	consoleDemoInit();
	
	//turn interrupts off
	*ime = 0;

	
	// acknowledge every interrupt
	*interrupt_flag = 0xffff;


	// sync to 0x10 visible
	while((*disp_stat & 0x3) != 0 && *vcount != 0x10)
	{

	}

	
	// disable timer and set reload to zero
	*tm0cnt_h = 0;
	*tm0cnt_l = 0;
	
	asm volatile("nop");
	
	// 1 cycle prescaler selection
	// count up and eanble
	*tm0cnt_h = (1 << 7); 
	

	
	// enable vcount if
	REG_DISPSTAT |= 1 << 5;
	
	uint8_t ly = *vcount;
	

	// start time
	volatile uint16_t start = *tm0cnt_l;
	
	// write vcount and pull dispstat
	// we should see a interrupt triggered here
	*lyc = ly;
	
	// pull dispstat
	volatile uint16_t dispstat = *disp_stat;
	
	// end
	volatile uint16_t end = *tm0cnt_l;
	
	
	// if pulled after but so its not part of the timing but just pulled for arguements sake
	volatile uint16_t flags = *interrupt_flag;
	
	
	
	printf("start %x\n",start);
	printf("end %x\n",end);
	printf("dispstat: %x\n",dispstat & (1 << 2));
	printf("if: %x\n",flags);
	
	// yeah i know i should use a proper wait here but allow it
	for(;;) {}
	
}