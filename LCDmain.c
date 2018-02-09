#include "stm32f401xe.h"
#include "LCDFunctions.h"

int main(void){
	InitializePortsForLCD();
int broj=1;
	LCDSendAnInstruction(0b00111000); //set to 8-bit mode and 2 line display
	LCDSendAnInstruction(0b00001110); //turn on display and cursor
	LCDSendAnInstruction(0b00000110); //set the mode to increment address by one
	LCDSendAnInstruction(0b00000001); //clears the display
	//LCDSendAnInstruction(0b11000000); // go to next line
	//LCDSendAnInstruction(0b11000000);
	//LCDSendAnInstruction(0b00000010);
	LCDSendACharacter(' ');
	LCDSendAString("Frekvencija:");

	LCDSendAnInstruction(0b11000000);
	LCDSendAnInteger(broj);
	LCDSendAString("Hz");
    LCDSendAnInstruction(0b00001100); //set cursor off
	while(1)
	{
	}
}
