#include "stm32f401xe.h"
#include "math.h"

int var;
int cnt;
int isr;
int cnt=0;
int counter=0;

int sinus [3600];


void TIM1_UP_TIM10_IRQHandler(void){
	//isr=1;
	/*for (int i = 0; i < 500000; ++i) {
		i++;
	}*/
	//isr=0;

	// Postavi OCCR na slijedecu vrij. iz tablice
	// PROMIJENI NA 360 STUPNJEVA KAD IMAS DVA KANALA!
	if(cnt==3600){
		cnt=0;
	}
	//isr=1;
	//var=sinus[cnt];
	isr=2;
	TIM1-> CCR4 = sinus[cnt];
	isr=3;
	cnt++;

	//Potrebno je ocistiti status registar
	TIM1-> SR &= ~(TIM_SR_UIF);
	//Ocisti update interrupt - to je trenutni interrupt
	TIM1 -> DIER &= ~(TIM_DIER_UIE);
	//ponovno ukljuci interrupte
	TIM1 -> DIER |= TIM_DIER_UIE;

}

//Funkcija za generiranje sinusa
void generateSin(){
	for (int d = 0; d < 3600; ++d) {
		/*
		 * 	             		sin(d)+1
		 *sinus [d] = ARR_MAX* ----------
		 * 			            	2
		 */
		sinus [d] = 1000*((1+sin(d*(M_PI/180)))/2);
	}
}


int main(void)
{
	generateSin();
	RCC-> APB2ENR |= RCC_APB2ENR_TIM1EN;
	RCC-> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA-> MODER &= ~(GPIO_MODER_MODER11_0);
	GPIOA-> MODER |= GPIO_MODER_MODER11_1;

	GPIOA-> AFR[1] = 0x1000;

	TIM1-> PSC = 0;
	TIM1-> ARR = 1000;
	TIM1-> CCR4 = 0;
	TIM1-> CCMR2 |= TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4PE;
	TIM1-> CCER |= TIM_CCER_CC4E;
	TIM1-> BDTR |= TIM_BDTR_MOE;
	TIM1-> CR1 |= TIM_CR1_CEN;
	TIM1-> EGR |= TIM_EGR_UG;

	//TIM1 -> DIER |= TIM_DIER_CC4IE;
	TIM1 -> DIER |= TIM_DIER_UIE;

	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
	NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0);


    while(1)
    {

    	//var=GPIOA ->IDR & GPIO_IDR_IDR_11;
    	//counter=TIM1->CNT;

    	/*for (int i = 0; i < 65000; ++i) {
    		TIM1-> CCR4 = i;
		}*/
    }
}

