/*****************************************************************************************/
/* This Program is a Supervised Neural Network to solve that solves the XOR              */
/* the inputs come from the ADC ch0 and ch1                                              */
/*                                                      2012/8/11  Jose Gonzalez         */ 
/*****************************************************************************************/

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h" 
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"   // Defines and macros for GPIO API of DriverLib (GPIOPinTypePWM)
#include "driverlib/adc.h"
#include "supervisedNN.h"
#include "drivers/rit128x96x4.h" // Defines and macros for the OLED Display. 
#include "stdio.h"


/* In case that there is an incorrect parameter or library function in the API */
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif


/* Variable storing 4 samples of the Temperature reading */
unsigned long ulADC0Value[4];
/* Variable storing the average of the Temperature, is volatile so is not
* optimezed by the compiler and will be available to the watch or loca window */
volatile unsigned long ulTempAvg;
/* Variable storing the Temp in Celsius */
volatile unsigned long ulTempValueC;
/* Variable storing the Temp in Fahrenheit*/
	volatile unsigned long ulTempValueF;
	
	
	
unsigned long _500msec_counter;			/*500msecタイマカウンタ*/
unsigned long ADCValue[2] = {0,0};
short debug=1;
short count=0;

int flag2=0;

	/* Neural Network Variables Declaration */

float Inputs[NumIn+1];
float Outputs[NumOut+1];
float Hidden[NumHid+1];

float InWeights[NumIn+1][NumHid+1];
float HidWeights[NumHid+1][NumOut+1];

float eta = 0.1;

float target[NumOut+1];
float Bias[2]={-1, -1};

float XORTarget[4] = {1.0, 0.1, 0.1, 1.0};
float XORInputs[4][2] = {0.1,0.1,0.1,1.0,1.0,0.1,1.0,1.0};

float array1[3] = {1,2,3};
float array2[3][3] = {1,2,3,4,5,6,7,8,9};

short targetFlag = 0;

/******************************************************************************/
/**** Erase the specified row in the OLED                                     */
void RIT128x96x4StringErase(int row)
{
	RIT128x96x4StringDraw("                           ", 0,  row, 15);
}	


/******************************************************************************/
/**** Erase the all the screen                                                */
void RIT128x96x4ScreenErase()
{
	int i;
	for(i=0;i<60;i++)
	{
		RIT128x96x4StringErase(i);
	}

}
/******************************************************************************/
/**** ADC Interruption Handler                                                */
void ADC1IntHandler(void)
{
		/* Clear conversion complete flag */
	ADCIntClear(ADC0_BASE, 1);
	
	/* Read the ADC value */
	ADCSequenceDataGet(ADC0_BASE, 1, ulADC0Value);
		
	   /* Trigger ADC Conversion */
	ADCProcessorTrigger(ADC0_BASE,1);
}


/******************************************************************************/
/**** This is the handler for INT_GPIOC.  It triggers INT_GPIOB and saves the */
/**** interrupt sequence number. 											  */
void IntGPIOg(void)
{	
	char	str[256];
	int epoch=0;
	float error=100;
	short	i,j,k,cent,v,n;
	
	long int Status = GPIOPinIntStatus(GPIO_PORTG_BASE, true);
	
		// test the multiplication for the arrays. 
  		if ( Status == 0x80) // select 
  		{
  			test(array1,array2,4);
  			RIT128x96x4ScreenErase();
			for (i=1;i<3;i++){
				for (j=0;j<3;j++){
					// convert the numbers to string. 
					sprintf(str, "%.4f", array2[i][j]);
					// display the number 
					RIT128x96x4StringDraw(str, 10,  i*10, 15);
				}
			}
		}
		
		// Changes the output target to a AND
		if (Status == 0x20)//left button
		{
			switch  (targetFlag) 
			{
				case 0: 
				{
				XORTarget[0] = 0.1;
				XORTarget[1] =0.1;
				XORTarget[2] =0.1;
				XORTarget[3] =1.0;
				epoch=0;					
				targetFlag = 1;
				break;
				}
		
		// Changes the output target to an OR		
				case 1 :
				{
				epoch=0;	
				XORTarget[0] = 0.1;
				XORTarget[1] =1.0;
				XORTarget[2] =1.0;
				XORTarget[3] =1.0;
				targetFlag = 2;
				break;
				}
					
		// Changes the output target to a XOR			
				case 2: 
				{
				epoch=0;	
				XORTarget[0] = 1.0;
				XORTarget[1] =0.1;
				XORTarget[2] =0.1;
				XORTarget[3] =1.0;
				targetFlag = 0;
				break;
				}
			}
		}
		
		// display the weights				
		if (Status == 0x40) {	
			RIT128x96x4ScreenErase();		
			// display title
			RIT128x96x4StringDraw("Input Weights", 2,  0, 15);
			
			for(j=0;j<=NumHid;j++){
				for(i=0;i<=NumIn;i++){	
					
					sprintf( str, "%.4f", InWeights[i][j] );
					// display the number 
					RIT128x96x4StringDraw(str, (i*8), (j+1)*8, 10);
				}
			}	
			
			// display title
			RIT128x96x4StringDraw("Hidden Weights", 2,  0, 15);	
					
			for(k=0;k<=NumOut;k++){
				for(j=0;j<=NumHid;j++){
					sprintf( str, "%.4f", HidWeights[j][k] );
					// display the number 
					RIT128x96x4StringDraw(str, (i*8), (j+NumHid)*8, 10);
				}
			}
		}
			
		// Start Training. 		
		if (Status == 0x08) // up
		{	
			RIT128x96x4ScreenErase();	
			/*	training();*/
		    /*******************************************************************************/		
			// display title
			RIT128x96x4StringDraw("Training...", 2,  0, 15);
			error=100;  // error init;
			n=1;
			cent=0;
			//for (n=0;n<1000;n++){
			while ((error>0.05) || (epoch>20000)) {   /* do the loop until error< 0.05 or epoch > 5000*/
				//if (epoch>1000){
				//	n=0;
				//}
				//if (error<0.05){
				//	n=0;
				//} 
				//else {	
				epoch++;  // new epoch	
				error=0;
				for (i=0;i<NumPat;i++)
				{  // present the patterns
					j=i;
					Inputs[0]= Bias[0];
					Inputs[1]= XORInputs[j][0];
					Inputs[2]= XORInputs[j][1];
					Hidden[0]= Bias[1];
					target[1]= XORTarget[j];			
					Forward(Inputs, InWeights, Hidden, HidWeights, Outputs);		
					error += 0.5*(target[1]-Outputs[1])*(target[1]-Outputs[1]);  // calculates the total error for the epoch
					if (epoch>=cent)
					{
						v=j/1;

					}		
					BackPropagation(target,Inputs,InWeights,Hidden,HidWeights, Outputs, eta);	
				}
				if (epoch>=cent)
				{
					// display title
					//RIT128x96x4StringDraw("Final Error: ", 2,  10, 15);			
					v=epoch;
					//sprintf( str, "%.4f", error );
					//RIT128x96x4StringDraw(str, 20,  10, 15);
					//RIT128x96x4StringDraw(":", 2,  10, 15);
					
					//sprintf( str, "%.4f", v );	
			        //RIT128x96x4StringDraw(str, 30,  10, 15);
					cent+=500;
				}	
			
	//	}
			}
			RIT128x96x4StringDraw("Final Error: ", 2,  10, 10);			
			v=epoch;
			sprintf( str, "%.4f", error );
			RIT128x96x4StringDraw(str, 20,  10, 10);
			RIT128x96x4StringDraw(":", 25,  10, 10);
					
			sprintf( str, "%.4f", v );	
	        RIT128x96x4StringDraw(str, 30,  10, 10);
	        
	        RIT128x96x4StringDraw("Training Finished", 30,  50, 15);
	
		}
		
		// Run the Neural Network
		if (Status == 0x10) // down 
		{
			RIT128x96x4ScreenErase();
			for (i=0;i<NumPat;i++)
			{
				Inputs[0]= Bias[0];
				Inputs[1]= XORInputs[i][0];
				Inputs[2]= XORInputs[i][1];
				target[1]=XORTarget[i];

		        RIT128x96x4StringDraw("Results", 60,  0, 15);
				RIT128x96x4StringDraw("Inputs", 10,  0, 15);
						
				sprintf( str, "%.2f", Inputs[1]);
			    RIT128x96x4StringDraw(str, 0,  10*i+10, 15);
				
				sprintf( str, "%.2f", Inputs[2]);
				RIT128x96x4StringDraw(str, 30,  10*i+10, 15);
				
				RIT128x96x4StringDraw("=", 55,  10*i+10, 15);
					
				Hidden[0]= Bias[1];
						
				Forward(Inputs, InWeights, Hidden, HidWeights, Outputs);
											
				sprintf( str, "%.2f", Outputs[1]);
				RIT128x96x4StringDraw(str, 65,  10*i+10, 15);
				
				sprintf( str, "%.2f", target[1]);
			    RIT128x96x4StringDraw(str, 95,  10*i+10, 15);
			}
		}
	
 	GPIOPinIntClear(GPIO_PORTG_BASE, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
}



/******************************************************************************/
/**** Main 																	  */
int main(void)
{	
	float v;
	unsigned short data;
	short	ans;
	short	cnt;
	short	i,k;
	long	para;
	 char	str[256];
	unsigned long tmp;

	long int outputint=0;
	
	int epoch=0;
	int num=0;
	float error=100;
	int errorint=0;
	short n=1;  // main training loop
	//short i=0;  // pattern loop
	short t=0;  // counter for the random pattern selector.
	short j=0;  // general counter
	//char str[10];
	short cent=0;
	double x;
	
	/* Set the system Clock from the PLL to 20MHz (SYSCTL_SYSDIV_10) */
	SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
	
	/***************************** IO Configuration */
	
	/* The clock should be enabled for the peripheral RCGCn register */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	
	/* Configure the GPIO as an Output. IN this case is PORT G PIN_2 */
	GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2);
	GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_0);

	/* Configure the GPIO as an Input. IN this case is PORT G PIN_3 to PIN_7 */
	GPIOPinTypeGPIOInput(GPIO_PORTG_BASE, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPadConfigSet(GPIO_PORTG_BASE, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	
	/***************************** ADC Configuration */
	/* Enable the ADC0 */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	
	/* ADC sample rate at 1MHz*/
	SysCtlADCSpeedSet(SYSCTL_ADCSPEED_1MSPS);
	
	/* Disable the sequencer to configure its settings */
	ADCSequenceDisable(ADC0_BASE, 1);
	
	/* Set the ADC to trigger after a processor event for sequencer 1 with priority level 0 */
	ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
	
	/* Configure step 0-2 on sequencer 3 to sample the temperature sensor (ADC_CTL_TS) */
	//ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_TS);
	//ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_TS);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH0);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH1 | ADC_CTL_IE | ADC_CTL_END);
	
	/* Reload the sequencer */
	ADCSequenceEnable(ADC0_BASE, 1);
	
	/***************************** Interruption Configuration*/
	/* General Enable Interruptions */
	IntMasterEnable();
	
	/* Enable ADC interruption */
	IntEnable(INT_ADC0SS1);
	
	/* Register the interrupt handler for the Interruption */
	ADCIntRegister(ADC0_BASE, 1, ADC1IntHandler);
	
	/* Enable ADC sequencer 1 to generate the interruption*/
	ADCIntEnable(ADC0_BASE, 1);
	
	/* Clear conversion complete flag */
	ADCIntClear(ADC0_BASE, 1);
	
	/* Enable the IO interrupt. */
	IntEnable(INT_GPIOG);
	
	/* Register the IO interruption handler */   
	GPIOPortIntRegister(GPIO_PORTG_BASE,IntGPIOg);
	
	/* Enable Port G.3 and Port G.4 for interruption */
	GPIOPinIntEnable(GPIO_PORTG_BASE, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
	
	/* Trigger ADC Conversion */
	ADCProcessorTrigger(ADC0_BASE,1);
	
	/* Init the OLED screen */
	RIT128x96x4Init(1000000);
	
	/* Initialize the weights  */
	InWeightsInit(InWeights);// init weights
	HidWeightsInit(HidWeights);	
	
	while (1)
	{
  		
	}
}



