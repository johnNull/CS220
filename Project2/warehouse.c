#include "slots.h"
#include <stdio.h>

int main(int argc, char ** argv) {

	int bin;
	int slotTrack = 0;
	//Just changed the slots into a queue data structure
	initSlots();
	while(1==scanf("%d",&bin)) {
		if (-1==findSlot(bin)) {
			if(slotTrack == 4) slotTrack = 0;
			getBin(bin,slotTrack);
			slotTrack++;
		}
		getWidget(bin);
	}
	printEarnings();
	return 0;
}

