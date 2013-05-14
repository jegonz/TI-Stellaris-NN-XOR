#ifndef SUPERVISEDNN_H_
#define SUPERVISEDNN_H_

/************************************/
/*	Definitions       				*/
/************************************/
#define NumIn 2
#define NumHid 2
#define NumOut 1
#define NumLayers 3
#define NumPat 4

/************************************/
/*	Prototype       				*/
/************************************/

extern double sigmoid(float x);
extern float linear(float x);
extern float devsigmoid(float x);
extern void Forward(float inputs[NumIn+1], float InWeights[][NumHid+1], float hidden[NumHid+1], float HidWeights[][NumOut+1], float outputs[NumOut+1]);
extern void BackPropagation (float target[NumOut+1], float inputs[NumIn+1], float InWeights[][NumHid+1], float hidden[NumHid+1], float HidWeights[][NumOut+1], float outputs[NumOut+1], float eta);
extern void InWeightsInit(float InWeights[][NumHid+1]);
extern void HidWeightsInit(float HidWeights[][NumOut+1]);
extern float getrandom_f(float min,float max);
extern void test(float array1[3], float array2[][3], float in);

#endif /*SUPERVISEDNN_H_*/
