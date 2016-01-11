#include "stdafx.h"
#include "utils.h"
#include "Constants.h"

#include <algorithm>
#include <math.h>
#include <stdlib.h>



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
		return (double(rand()+1.0))/(double(RAND_MAX+1.0));
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
		if(_isnan(noise[i])||!_finite(noise[i])){
				printf("What the fuck!\n");
		}
		if(_isnan(noise[i+1])||!_finite(noise[i+1])){
				printf("What the fuck!\n");
		}

	}
	printf("%lf",noise[100]);
}


void shuffleNumber(vector<int> &nums, int size){
	nums.resize(size);
	for(int i=0;i<size;++i){
		nums[i]=i;
	}
	random_shuffle(nums.begin(),nums.end());
}