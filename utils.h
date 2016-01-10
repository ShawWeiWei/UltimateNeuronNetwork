#pragma once
#include <vector>
using std::vector;

template<typename T> 
void create2dvector(T ***_2dVector,int v1,int v2){
	(*_2dVector)=new T*[v1];
	(*_2dVector)[0]=new T[v1*v2];
	for(int i=1;i<v1;++i){
		(*_2dVector)[i]=(*_2dVector)[i-1]+v2;
	}
	memset((*_2dVector)[0],0,sizeof(T)*v1*v2);
}

template<typename T>
void delete2dvector(T **_2dVector){
	delete [] _2dVector[0];
	delete [] _2dVector;
}

int sqrt_int(int num);

double Uniform_01();

void updateNoise(double *noise,int node,double noise_intensity);

void shuffleNumber(vector<int> &nums, int size);