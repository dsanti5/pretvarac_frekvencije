#include "stm32f401xe.h"

volatile int cnt=0;
volatile int nesto=0;
volatile int sadasnja=0;
volatile int prosla=0;
volatile int aLastState=0;
volatile int aState=0;
volatile int bState=0;
volatile int encoder=0;

void EXTI15_10_IRQHandler(){
	if(EXTI->PR & EXTI_PR_PR15) {
		EXTI->PR |= EXTI_PR_PR15;
	}
	sadasnja++;
}

int main(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	//sklopka
	GPIOA->MODER &= ~GPIO_MODER_MODER15;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR15;
	//A i B encodera
	GPIOA->MODER &= ~GPIO_MODER_MODER6;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR6;

	GPIOA->MODER &= ~GPIO_MODER_MODER5;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR5;

	SYSCFG->EXTICR[3]=SYSCFG_EXTICR2_EXTI5_PA;
	EXTI->IMR |= EXTI_IMR_MR15;
	EXTI->RTSR |= EXTI_RTSR_TR15;

	NVIC_EnableIRQ(EXTI15_10_IRQn);
	NVIC_SetPriority(EXTI15_10_IRQn, 0);
	if(GPIOA->IDR & GPIO_IDR_IDR_5){
		aLastState++;
	}

	while(1)
    {

		if(GPIOA->IDR & GPIO_IDR_IDR_5){
			aState=1;
		}
		else {
			aState=0;
		}

		if(aState != aLastState) {
			if(GPIOA->IDR & GPIO_IDR_IDR_6){
				bState=1;
			}
			else {
				bState=0;
			}
			if(aState != bState){
				encoder++;
			}
			else{
				encoder--;
			}
		}
		aLastState = aState;

		if(prosla!=sadasnja){
			prosla=sadasnja;
			cnt++;
		}
    }
}
