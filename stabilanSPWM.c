#include "stm32f401xe.h"
#include "math.h"

int cnt1=0;
int cnt2;
int cnt3;

int frekvencija=50;
int refFrekvencija=50;
int brojSampleova;
int reload;
int bS;

/*int sinek[37] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180,
                 170, 160, 150, 140, 130, 120, 110, 100, 90, 80, 70, 60, 50, 40, 30, 20, 10, 0};*/

int sinek[];

void TIM1_UP_TIM10_IRQHandler(void){
	//Potrebno je ocistiti status registar
	TIM1-> SR &= ~(TIM_SR_UIF);

	TIM1->CCR1 = sinek[cnt1];
	if(cnt1 >= reload){
		cnt1=0;
	}
	cnt1++;
	TIM1->CCR2 = sinek[cnt2];
	if(cnt2 >= reload){
		cnt2=0;
	}
	cnt2++;
	TIM1->CCR3 = sinek[cnt3];
	if(cnt3 >= reload){
		cnt3=0;
	}
	cnt3++;
}

void racunajMe(){
		frekvencija=refFrekvencija;
		bS=round(sqrt((16000000/frekvencija)));
		reload=round(sqrt((16000000/frekvencija)));
		sinek[reload];
		for (int a = 0; a < reload; ++a) {
			sinek [a] = (int)round(reload*((1+sin(2*M_PI*(float)a/reload))/2));
		}
		TIM1-> ARR = reload;
		cnt1=0;
		cnt2=bS/3;
		cnt3=2*(bS/3);
}

int main(void){
	racunajMe();
	// Enable TIM1
	RCC-> APB2ENR |= RCC_APB2ENR_TIM1EN;
	//RCC-> APB1ENR |= RCC_APB1ENR_TIM2EN;
	// Enable GPIOA i GPIOB
	RCC-> AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;

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

	GPIOA-> AFR[1] = 0x111;
	GPIOA-> AFR[0] = 0x10000000;
	GPIOB-> AFR[0] = 0x11;

	TIM1-> PSC = 0;
	TIM1-> ARR = 0;

	// Capture compare mode register 1
	TIM1-> CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1PE | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2PE;
	// Capture compare mode register 2
	TIM1-> CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3PE;

	TIM1-> CCER |= TIM_CCER_CC1E | TIM_CCER_CC1NE | TIM_CCER_CC2E | TIM_CCER_CC2NE | TIM_CCER_CC3E | TIM_CCER_CC3NE;
	TIM1-> BDTR |= TIM_BDTR_DTG_3  | TIM_BDTR_DTG_2 | TIM_BDTR_DTG_1 | TIM_BDTR_DTG_0;

	// Master out enable
	TIM1-> BDTR |= TIM_BDTR_MOE;
	TIM1-> EGR |= TIM_EGR_UG;
	TIM1-> DIER |= TIM_DIER_UIE;

	// Counter enable
	TIM1-> CR1 |= TIM_CR1_CEN;
	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);

    while(1){
    	if (frekvencija!=refFrekvencija) {
			racunajMe();
		}
    }
}
