#include "stdafx.h"
#include "utils.h"
#include <math.h>

template<typename T> 
void create2dvector(T **2dVector,int v1,int v2){
	T **2dVector=new T*[v1];
	2dVector[0]=new T[v1*v2];
	for(int i=1;i<v1;++i){
		2dVector[i]=2dVector[i-1]+v2;
	}
	memset(2dVector[0],0,sizeof(T)*v1*v2);
}

template<template T>
void delete2dvector(T **2dVector){
	delete [] 2dVector[0];
	delete [] 2dVector;
}

//return -1 if not exists
int sqrt_int(int num){
	if(num<1){
		return -1;
	}
	else{
		int a=sqrt((double)num);
		if((a*a)==num){
			return a;
		}else if(((a-1)*(a-1))==num){
			return a-1;
		}else if(((a+1)*(a+1))==num){
			return a+1;
		}else{
			return -1;
		}
	}
}