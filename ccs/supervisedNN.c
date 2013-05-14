/*****************************************************************************************/
/* Neural Network with Backpropagation                                                   */
/*                                                                                       */
/*                                                      2012/8/11  Jose Gonzalez         */ 
/*****************************************************************************************/

#include "supervisedNN.h"
#include <math.h>
#include <stdlib.h>

/*******************************************************/
/*  Sigmoid Funtion                                    */
/*******************************************************/ 

double sigmoid(float x) {
	double result=0;
	char	str[256];
	//if (x<0) {result = 0;}
	//else {result = 1;}
	result=exp(-1*x);
	result+=1;
	result=1/result;
		
	return result;
	}

float expo(float x) {
	float result=0;
	
	//if (x<0) {result = 0;}
	//else {result = 1;}
	
	result = exp(-1*x);
	
	return result;
	}
	
float linear(float x) {
	float result=0;
	
	if (x<0) {result = 0.1;}
	else {result = 1;}
	
	return result;	
}

/*******************************************************/
/***********  Forward Algorithm                        */

void Forward(float inputs[NumIn+1], float InWeights[][NumHid+1], float hidden[NumHid+1], float HidWeights[][NumOut+1], float outputs[NumOut+1]){
	
	short i=0;  /* Input layer counter */
	short j=0;	/* Hidden layer counter */
	short k=0;	/* Output layer counter */
	
	char	str[256];
	int outputint=0;

	/**** compute the hidden layer activation ******/
	for (j=1;j<=NumHid;j++){
	hidden[j]=0;   		
		for (i=0;i<=NumIn;i++) {
			hidden[j]= hidden[j]+inputs[i]*InWeights[i][j]; 				
		}
		hidden[j]= sigmoid(hidden[j]);	

	}
	
	/**** compute the output layer activation ******/
	for (k=1;k<=NumOut;k++){  
	outputs[k]=0;   
		for (j=0;j<=NumHid;j++){
			outputs[k]= outputs[k]+hidden[j]*HidWeights[j][k]; 
		}
		outputs[k]= sigmoid(outputs[k]);
	}
	
	}

/*******************************************************/
/*  Back Propagation Algorithm                         */
/*******************************************************/

void BackPropagation (float target[NumOut+1], float inputs[NumIn+1], float InWeights[][NumHid+1], float hidden[NumHid+1], float HidWeights[][NumOut+1], float outputs[NumOut+1], float eta){
	short i=0;  /* Input layer counter */
	short j=0;	/* Hidden layer counter */
	short k=0;	/* Output layer counter */
	float DeltaOH[NumOut+1]={0.0};  /* Error from Hidden to Output */
	float DeltaHI[NumHid+1]={0.0};  /* Error from Input to Hidden */
	char	str[256];
	int outputint=0;
	float temp;

	for (k=1;k<=NumOut;k++){ 
		DeltaOH[k] = (target[k]-outputs[k]);//*outputs[k]*(1.0-outputs[k]); /* Calculate the Error from Hidden to Output */
		for (j=0;j<=NumHid;j++){
			HidWeights[j][k] = HidWeights[j][k] + eta*DeltaOH[k]*hidden[j];/* Update the Hidden Layer Weights*/
				
		}
	}
					
				
	for (j=1;j<=NumHid;j++){
		for (k=1;k<=NumOut;k++){						
			DeltaHI[j]=DeltaHI[j]+HidWeights[j][k]*DeltaOH[k];		/* Backpropagate the Error */ 
		}
		DeltaHI[j]= DeltaHI[j]*hidden[j]*(1-hidden[j]);			/* Calculate the Error from Input to Hidden */				
		for (i=0;i<=NumIn;i++) {
			InWeights[i][j] =InWeights[i][j] + eta*DeltaHI[j]*inputs[i]; /* Update the Input Layer Weights */
			  
		}				
	}	
	}

/*******************************************************/
/*  Input Weights Initialization                       */
/*******************************************************/

void InWeightsInit(float InWeights[][NumHid+1]){
	short i=0;  /* Input layer counter */
	short j=0;	/* Hidden layer counter */
	
	for(j=0;j<=NumHid;j++){
		for(i=0;i<=NumIn;i++){
			InWeights[i][j]=getrandom_f(-1.0,1.0);
		}
	}
	
	}

/*******************************************************/
/*  Hidden Weights Initialization                      */
/*******************************************************/

void HidWeightsInit(float HidWeights[][NumOut+1]){
	short j=0;	/* Hidden layer counter */
	short k=0;	/* Output layer counter */
	for(k=0;k<=NumOut;k++){
		for(j=0;j<=NumHid;j++){
			HidWeights[j][k]=getrandom_f(-1.0,1.0);
		}
	}
	}

/*******************************************************/
/*  Random Number Generator                            */
/*******************************************************/

float getrandom_f(float min,float max){
	return min + (rand()*(max-min)/(RAND_MAX + 1));
}

void test(float array1[3], float array2[][3], float in){
	float x;
	short i,j;
	char	str[256];

	for (i=1;i<3;i++){
		for (j=0;j<3;j++){
			x = in*array2[i][j];
			array2[i][j]=x;

		}
	}
	}
	
