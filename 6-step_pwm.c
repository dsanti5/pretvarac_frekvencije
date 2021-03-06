#include "stm32f401xe.h"
#include "math.h"

int cnt1=0;
int cnt2=120;
int cnt3=240;
int faza1 [360];
int faza2 [360];
int faza3 [360];


void TIM1_UP_TIM10_IRQHandler(void){
	if(cnt1==360){
		cnt1=0;
	}
	if(cnt2==360){
		cnt2=0;
	}
	if(cnt3==360){
		cnt3=0;
	}

	if(cnt1>=180){
		TIM1-> CCER |= TIM_CCER_CC1E;
		TIM1-> CCER |= TIM_CCER_CC1P;
		TIM1-> CCER &= ~TIM_CCER_CC1NE;
		//GPIOA-> BSRR |= GPIO_BSRR_BS_3;
		GPIOA->ODR |= (1 << 3);
	}
	else{
		TIM1-> CCER &= ~TIM_CCER_CC1E;
		TIM1-> CCER |= (TIM_CCER_CC1NE);
		TIM1-> CCER &= ~TIM_CCER_CC1P;
		//GPIOA-> BSRR |= GPIO_BSRR_BR_3;
		GPIOA->ODR &= ~(1 << 3);
	}

	if(cnt2==180){
		TIM1-> CCER |= TIM_CCER_CC2E;
		TIM1-> CCER |= TIM_CCER_CC2P;
		TIM1-> CCER &= ~TIM_CCER_CC2NE;
	}
	if(cnt2==0){
		TIM1-> CCER &= ~TIM_CCER_CC2E;
		TIM1-> CCER |= (TIM_CCER_CC2NE);
		TIM1-> CCER &= ~TIM_CCER_CC2P;
	}
	if(cnt3==180){
		TIM1-> CCER |= TIM_CCER_CC3E;
		TIM1-> CCER |= TIM_CCER_CC3P;
		TIM1-> CCER &= ~TIM_CCER_CC3NE;
	}
	if(cnt3==0){
		TIM1-> CCER &= ~TIM_CCER_CC3E;
		TIM1-> CCER |= (TIM_CCER_CC3NE);
		TIM1-> CCER &= ~TIM_CCER_CC3P;
	}



	//TIM1-> CCR1 = cnt1;
	//TIM1-> CCR2 = cnt2;
	//TIM1-> CCR3 = cnt3;

	cnt1++;
	cnt2++;
	cnt3++;

	//TIM1-> CCR3 = faza1[cnt1];
	//Potrebno je ocistiti status registar
	TIM1-> SR &= ~(TIM_SR_UIF);
	//Ocisti update interrupt - to je trenutni interrupt
	TIM1 -> DIER &= ~(TIM_DIER_UIE);
	//ponovno ukljuci interrupte
	TIM1 -> DIER |= TIM_DIER_UIE;

}

// Funkcija za generiranje sinusa
void generateSin(){
	for (int d = 0; d < 360; ++d) {
		faza1 [d] = 100*((1+sin(d*(M_PI/180)))/2);

	}
}
void generateSin2(){
	for (int d = 120; d < 360; ++d) {
		if(d >= 240){
			d=d+120-360;
		}
		else {
			d=d+120;
		}
		faza2 [d] = 100*((1+sin(d*(M_PI/180)))/2);
	}
}
void generateSin3(){
	for (int d = 0; d < 360; ++d) {
		/*if(d >= 240){
			d=d+240-360;
		}
		else {
			d=d+240;
		}*/
		faza3 [d] = 100*((1+sin(((d%360)+240)*(M_PI/180)))/2);
	}
}
int main(void){

	generateSin();
	generateSin2();
	generateSin3();
	// Enable TIM1
	RCC-> APB2ENR |= RCC_APB2ENR_TIM1EN;
	// Enable GPIOA i GPIOB
	RCC-> AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;

	//GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7_0 | GPIO_OSPEEDER_OSPEEDR7_1 | GPIO_OSPEEDER_OSPEEDR8_0 | GPIO_OSPEEDER_OSPEEDR8_1 | GPIO_OSPEEDER_OSPEEDR9_0 | GPIO_OSPEEDER_OSPEEDR9_1 | GPIO_OSPEEDER_OSPEEDR10_0 | GPIO_OSPEEDER_OSPEEDR10_1;
	//GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0_0 | GPIO_OSPEEDER_OSPEEDR0_1 | GPIO_OSPEEDER_OSPEEDR1_0 | GPIO_OSPEEDER_OSPEEDR1_1;

	// CH1
	GPIOA-> MODER &= ~(GPIO_MODER_MODER8_0);
	GPIOA-> MODER |= GPIO_MODER_MODER8_1;
	// CH1 N
	GPIOA-> MODER &= ~(GPIO_MODER_MODER7_0);
	GPIOA-> MODER |= GPIO_MODER_MODER7_1;
	// CH2
	GPIOA-> MODER &= ~(GPIO_MODER_MODER9_0);
	GPIOA-> MODER |= GPIO_MODER_MODER9_1;
	// CH2 N
	GPIOB-> MODER &= ~(GPIO_MODER_MODER0_0);
	GPIOB-> MODER |= GPIO_MODER_MODER0_1;
	// CH3
	GPIOA-> MODER &= ~(GPIO_MODER_MODER10_0);
	GPIOA-> MODER |= GPIO_MODER_MODER10_1;
	// CH3 N
	GPIOB-> MODER &= ~(GPIO_MODER_MODER1_0);
	GPIOB-> MODER |= GPIO_MODER_MODER1_1;

	GPIOA-> MODER |= GPIO_MODER_MODER3_0;
	GPIOA-> MODER &= ~(GPIO_MODER_MODER3_1);

	GPIOA-> AFR[1] = 0x111;
	GPIOA-> AFR[0] = 0x10000000;
	GPIOB-> AFR[0] = 0x11;


	TIM1-> PSC = 0;
	TIM1-> ARR = 3600;
	// Capture compare registar za svaki kanal
	TIM1-> CCR1 = 1800;
	TIM1-> CCR2 = 1800;
	TIM1-> CCR3 = 1800;

	// Capture compare mode register 1
	TIM1-> CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1PE | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2PE;
	// Capture compare mode register 2
	TIM1-> CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3PE;

	// Capture compare enable registar
	// CH1E CH2E CH3E CH3NE CH4E
	//TIM1-> CCER |= TIM_CCER_CC1E | TIM_CCER_CC1NP | TIM_CCER_CC1NE | TIM_CCER_CC2E | TIM_CCER_CC2NP | TIM_CCER_CC2NE | TIM_CCER_CC3E | TIM_CCER_CC3P | TIM_CCER_CC3NP | TIM_CCER_CC3NE;
	//TIM1-> CCER |= TIM_CCER_CC1NE;
	// Master out enable
	TIM1-> BDTR |= TIM_BDTR_MOE;
	// Counter enable
	TIM1-> CR1 |= TIM_CR1_CEN;

	TIM1-> EGR |= TIM_EGR_UG;
	//TIM1 -> DIER |= TIM_DIER_CC4IE;
	TIM1 -> DIER |= TIM_DIER_UIE;

	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
	NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0);


    while(1)
    {
    }
}

