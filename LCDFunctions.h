#ifndef LCDFunctionsHeader

#include <stdio.h>

#define LCDFunctionsHeader

#define LCDD0Pin 0
#define LCDD0Port GPIOC
#define LCDD1Pin 1
#define LCDD1Port GPIOC
#define LCDD2Pin 2
#define LCDD2Port GPIOC
#define LCDD3Pin 3
#define LCDD3Port GPIOC
#define LCDD4Pin 9
#define LCDD4Port GPIOB
#define LCDD5Pin 8
#define LCDD5Port GPIOB
#define LCDD6Pin 9
#define LCDD6Port GPIOC
#define LCDD7Pin 8
#define LCDD7Port GPIOC

#define LCDEnablePin 5
#define LCDEnablePort GPIOC

#define LCDReadWritePin 13
#define LCDReadWritePort GPIOB

#define LCDRegisterSelectPin 6
#define LCDRegisterSelectPort GPIOC

#define TimeDelayBeforeEnable 400
#define TimeDelayBeforeDisable 800

#define LCDInstructions_GoToNextLine 0b11000000
#define LCDInstructions_Set8bitMode_2LineDisplay 0b00111000
#define LCDInstructions_DisplayOn_CursorOn_CursorBlinkOff 0b00001110
#define LCDInstructions_IncrementPositionByOne 0b00000110
#define LCDInstructions_ClearDisplay 0b00000001
#define LCDTurnOffCursor 0b00001100
#define LCDGoHome 0b00000010

void notExactTimeDelay(int timeDelay)
{
	volatile int i;
	for (i = 0; i < timeDelay; i++)
	{

	}
}

void SetPortAndPinForOutput(GPIO_TypeDef *port, int pinNumber)
{
	if (port == GPIOA)
	{
		RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	}
	if (port == GPIOB)
	{
		RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	}
	if (port == GPIOC)
	{
		RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	}
	if (port == GPIOD)
	{
		RCC -> AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	}

	port->MODER &= ~(1 << ((pinNumber*2)+1));
	port->MODER |= (1 << (pinNumber*2));

	port->OTYPER &= ~(1<< pinNumber);

	port->OSPEEDR |= (1 << ((pinNumber*2)+1)) | (1 << (pinNumber*2));

	port->PUPDR &= ~(1 << pinNumber);
}

void InitializePortsForLCD()
{
	SetPortAndPinForOutput(LCDD0Port, LCDD0Pin);
	SetPortAndPinForOutput(LCDD1Port, LCDD1Pin);
	SetPortAndPinForOutput(LCDD2Port, LCDD2Pin);
	SetPortAndPinForOutput(LCDD3Port, LCDD3Pin);
	SetPortAndPinForOutput(LCDD4Port, LCDD4Pin);
	SetPortAndPinForOutput(LCDD5Port, LCDD5Pin);
	SetPortAndPinForOutput(LCDD6Port, LCDD6Pin);
	SetPortAndPinForOutput(LCDD7Port, LCDD7Pin);
	SetPortAndPinForOutput(LCDEnablePort, LCDEnablePin);
	SetPortAndPinForOutput(LCDReadWritePort, LCDReadWritePin);
	SetPortAndPinForOutput(LCDRegisterSelectPort, LCDRegisterSelectPin);
}

void SendBitToPortAndPin(GPIO_TypeDef *port, int pinNumber, uint8_t bitState )
{
	if(bitState) {
		port->BSRR |= (1 << pinNumber);

	} else {
		port->BSRR |= (1 << (pinNumber+16));

	}
}

void LCDEnable()
{
	notExactTimeDelay(TimeDelayBeforeEnable);
	SendBitToPortAndPin(LCDEnablePort, LCDEnablePin, 1);
}

void SetToWrite()
{
	SendBitToPortAndPin(LCDReadWritePort, LCDReadWritePin, 0);
}

void LCDSetToRead()
{
	SendBitToPortAndPin(LCDReadWritePort, LCDReadWritePin, 1);
}

void LCDInstructionMode()
{
	SendBitToPortAndPin(LCDRegisterSelectPort, LCDRegisterSelectPin, 0);
}

void CommandMode()
{
	SendBitToPortAndPin(LCDRegisterSelectPort, LCDRegisterSelectPin, 0);
}

void LCDCharacterMode()
{
	SendBitToPortAndPin(LCDRegisterSelectPort, LCDRegisterSelectPin, 1);
}

void LCDSendAByteToTheLCDDataPins(char character)
{
	SendBitToPortAndPin(LCDD0Port, LCDD0Pin, character & 0b00000001 );
	SendBitToPortAndPin(LCDD1Port, LCDD1Pin, character & 0b00000010 );
	SendBitToPortAndPin(LCDD2Port, LCDD2Pin, character & 0b00000100 );
	SendBitToPortAndPin(LCDD3Port, LCDD3Pin, character & 0b00001000 );
	SendBitToPortAndPin(LCDD4Port, LCDD4Pin, character & 0b00010000 );
	SendBitToPortAndPin(LCDD5Port, LCDD5Pin, character & 0b00100000 );
	SendBitToPortAndPin(LCDD6Port, LCDD6Pin, character & 0b01000000 );
	SendBitToPortAndPin(LCDD7Port, LCDD7Pin, character & 0b10000000 );
	notExactTimeDelay(TimeDelayBeforeDisable);
	SendBitToPortAndPin(LCDEnablePort, LCDEnablePin, 0);
}

void LCDSendACharacter(char character)
{
	SetToWrite();
	LCDCharacterMode();
	LCDEnable();
	LCDSendAByteToTheLCDDataPins(character);
}

void LCDSendAnInstruction (char character)
{
	SetToWrite();
	LCDInstructionMode();
	LCDEnable();
	LCDSendAByteToTheLCDDataPins(character);
}

void LCDSendAString(char *StringOfCharacters)
{
	while(*StringOfCharacters)
	{
		LCDSendACharacter(*StringOfCharacters++);
	}
}

void LCDSetupDisplay()
{
	LCDSendAnInstruction(LCDInstructions_Set8bitMode_2LineDisplay);
	LCDSendAnInstruction(LCDInstructions_DisplayOn_CursorOn_CursorBlinkOff);
	LCDSendAnInstruction(LCDInstructions_IncrementPositionByOne);
	LCDSendAnInstruction(LCDInstructions_ClearDisplay);
}

void LCDSend2LinesOfStringWithDelay(int delay, char *StringOfCharactersLine1, char *StringOfCharactersLine2)
{
	notExactTimeDelay(delay);
	LCDSendAnInstruction(LCDInstructions_ClearDisplay);
	LCDSendAString(StringOfCharactersLine1);
	LCDSendAnInstruction(LCDInstructions_GoToNextLine);
	LCDSendAString(StringOfCharactersLine2);
}

void LCDSendAnInteger(int IntegerToBedisplayed)
{
	uint8_t LengthOfDigits=1;
	while(IntegerToBedisplayed>1000){ LengthOfDigits++; IntegerToBedisplayed/=10; }

	char StringNumber[LengthOfDigits];
	sprintf(StringNumber, "%d", IntegerToBedisplayed);

	LCDSendAString(StringNumber);
}

void LCDSendAnFloatingPointNumber(float FloatingPointNumberToBedisplayed, uint8_t MaxLengthOfDigits)
{
	char StringNumber[MaxLengthOfDigits];
	sprintf(StringNumber, "%e", FloatingPointNumberToBedisplayed);

	LCDSendAString(StringNumber);
}

#endif
