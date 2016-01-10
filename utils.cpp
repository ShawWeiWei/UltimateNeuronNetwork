#include "stdafx.h"
#include "utils.h"
#include "Constants.h"

#include <math.h>
#include <stdlib.h>
//template<typename T> 
//void create2dvector(T **_2dVector,int v1,int v2){
//	_2dVector=new T*[v1];
//	_2dVector[0]=new T[v1*v2];
//	for(int i=1;i<v1;++i){
//		_2dVector[i]=_2dVector[i-1]+v2;
//	}
//	memset(_2dVector[0],0,sizeof(T)*v1*v2);
//}
//
//template<typename T>
//void delete2dvector(T **_2dVector){
//	delete [] _2dVector[0];
//	delete [] _2dVector;
//}

//return -1 if not exists
int sqrt_int(int num){
	int retVal;
	if(num<1){
		retVal=-1;
	}
	else{
		int a=sqrt((double)num);
		if((a*a)==num){
			retVal=a;
		}else if(((a-1)*(a-1))==num){
			retVal=a-1;
		}else if(((a+1)*(a+1))==num){
			retVal=a+1;
		}else{
			retVal=-1;
		}
	}
	if(retVal==-1){
		throw("Not A Square Of Integer!");
	}else{
		return retVal;
	}
}

double Uniform_01(){
		return (double(rand()))/(double(RAND_MAX));
}

void updateNoise(double *noise,int node,double noise_intensity){
	double ran1,ran2;
	double log_temp,angle_temp;
	for(int i=0;i<node;i+=2){  //m_nNeuron must be even.
		ran1=Uniform_01();
		ran2=Uniform_01();
		log_temp=log(ran1);
		angle_temp=2.0*PI*ran2;

		noise[i]=sqrt(-4.0*noise_intensity*log_temp)*cos(angle_temp);

		noise[i+1]=sqrt(-4.0*noise_intensity*log_temp)*sin(angle_temp);
	}
}