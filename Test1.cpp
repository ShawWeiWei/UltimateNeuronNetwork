// Test1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ML.h"
#include "ExcitatoryCouple.h"
#include "ExcitatoryCouple.cpp"
#include "CoupleWithInhibition.h"
#include "CoupleWithInhibition.cpp"
#include "ExcitatoryCoupleWithNoise.h"
#include "ExcitatoryCoupleWithNoise.cpp"
#include "Network.h"
#include "Network.cpp"
bool isTest = true;
void HeterExcitatorySparser(int *aML1,int ML1_size,double *aGc,int GC_size,double *aP,int sizeP,bool isSpiking,bool isSpiralWave){
	printf("HeterExcitatorySparser\n");
	for(int i=0;i<ML1_size;++i){
		ExcitatoryCouple<MorrisLecar> ex(16384,aML1[i],true);
		for(int iP=0;iP<sizeP;++iP){
			ex.createConnection("Sparser",aP[iP]);
			for(int j=0;j<GC_size;++j){
				ex.setCouple(aGc[j],-25,36);
				Network<MorrisLecar,ExcitatoryCouple> net(&ex);
				net.logInfo();
				if(isSpiking&&!isTest){
					net.OutputSpikingIndex();
					net.OutputAverISI();
					net.OutputCV();
					net.OutputPhaseAmplitude();
					net.OutputAverISIForOneAndTwo();
					net.OutputCVForOneAndTwo();
				}
				if(isSpiralWave&&!isTest){
					net.SpiralWave();
				}
			}
		}
	}
}

void HeterExcitatorySmallWorld(int *aML1,int ML1_size,double *aGc,int GC_size,bool isSpiking,bool isSpiralWave){
	printf("HeterExcitatorySmallWorld\n");
	for(int i=0;i<ML1_size;++i){
		ExcitatoryCouple<MorrisLecar> ex(16384,aML1[i],true);
		double aP[]={0,0.1,0.2,0.3,0.4,0.5,0.6};
		int sizeP=sizeof(aP)/sizeof(double);
		for(int iP=0;iP<sizeP;++iP){
			ex.createConnection("SmallWorld",aP[iP]);
			for(int j=0;j<GC_size;++j){
				ex.setCouple(aGc[j],-25,36);
				Network<MorrisLecar,ExcitatoryCouple> net(&ex);
				net.logInfo();
				if(isSpiking&&!isTest){
					net.OutputSpikingIndex();
					net.OutputAverISI();
					net.OutputCV();
					net.OutputPhaseAmplitude();
					net.OutputAverISIForOneAndTwo();
					net.OutputCVForOneAndTwo();
				}
				if(isSpiralWave&&!isTest){
					net.SpiralWave();
				}
			}
		}
	}
}

void HeterExcitatorySquare(int *aML1,int ML1_size,double *aGc,int GC_size,bool isSpiking,bool isSpiralWave){
	printf("HeterExcitatory\n");
	for(int i=0;i<ML1_size;++i){
		ExcitatoryCouple<MorrisLecar> ex(16384,aML1[i],true);
		ex.createConnection("Square");
			for(int j=0;j<GC_size;++j){

				ex.setCouple(aGc[j],-25,36);
				Network<MorrisLecar,ExcitatoryCouple> net(&ex);
				net.logInfo();
				if(isSpiking&&!isTest){
					net.OutputSpikingIndex();
					net.OutputAverISI();
					net.OutputCV();
					net.OutputPhaseAmplitude();
					net.OutputAverISIForOneAndTwo();
					net.OutputCVForOneAndTwo();
				}
				if(isSpiralWave&&!isTest){
					net.SpiralWave();
				}
			}
		
	}
}

void HeterExcitatoryWithNoise(int *aML1,int ML1_size,double*aGc,int GC_size,double *aNoise,int NOISE_size,bool isSpiking,bool isSpiralWave){
	for(int i=0;i<ML1_size;++i){
		ExcitatoryCoupleWithNoise<MorrisLecar> ex(16384,aML1[i],true);
	/*	double aP[]={0.2,0.3,0.4,0.5,0.6};
		int sizeP=sizeof(aP)/sizeof(double);
		for(int iP=0;iP<sizeP;++iP){*/
			ex.createConnection("Square");
			for(int j=0;j<GC_size;++j){
				ex.setCouple(aGc[j],-25,36);
				for(int iN=0;iN<NOISE_size;++iN){
					ex.setNoise(aNoise[iN]);
					Network<MorrisLecar,ExcitatoryCoupleWithNoise> net(&ex);
					net.logInfo();
					if(isSpiking&&!isTest){
						net.OutputSpikingIndex();
						net.OutputAverISI();
						net.OutputCV();
						net.OutputPhaseAmplitude();
						net.OutputAverISIForOneAndTwo();
						net.OutputCVForOneAndTwo();
					}
					if(isSpiralWave&&!isTest){
						net.SpiralWave();
					}
				}
			}
		//}
	}
}

void HeterInhibitorySquare(int *aML1,int ML1_size,double*aGc,int GC_size,bool isSpiking,bool isSpiralWave){
	printf("HeterInhibitory5\n");
	for(int i=0;i<ML1_size;++i){
		CoupleWithInhibition<MorrisLecar> in(16384,aML1[i],95-aML1[i]);
		in.createConnection("Square");
		for(int j=0;j<GC_size;++j){
			in.setCouple(aGc[j],aGc[j],-25,36,-45);
			Network<MorrisLecar,CoupleWithInhibition> net(&in);
			net.logInfo();
			if(isSpiking&&!isTest){
				net.OutputSpikingIndex();
				net.OutputAverISI();
				net.OutputCV();
				net.OutputPhaseAmplitude();
				net.OutputAverISIForOneAndTwo();
				net.OutputCVForOneAndTwo();
			}
			if(isSpiralWave&&!isTest){
				net.SpiralWave();
			}
		}
	}
}

int conf[][21]={
	{1,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,99},
	{1,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,98},
	{1,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,97},
	{1,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,96},
	{1,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95}
};

int conf_size[]={
	21,
	21,
	21,
	21,
	20
};
void HeterInhibitorySquare(int pInh,double*aGc,int GC_size,bool isSpiking,bool isSpiralWave){
	printf("HeterInhibitory%s\n",pInh);
	for(int i=0;i<conf_size[pInh];++i){
		CoupleWithInhibition<MorrisLecar> in(16384,conf[pInh][i],100-conf[pInh][i]-pInh);
		in.createConnection("Square");
		for(int j=0;j<GC_size;++j){
			in.setCouple(aGc[j],aGc[j],-25,36,-45);
			Network<MorrisLecar,CoupleWithInhibition> net(&in);
			net.logInfo();
			if(isSpiking&&!isTest){
				net.OutputSpikingIndex();
				net.OutputAverISI();
				net.OutputCV();
				net.OutputPhaseAmplitude();
				net.OutputAverISIForOneAndTwo();
				net.OutputCVForOneAndTwo();
			}
			if(isSpiralWave&&!isTest){
				net.SpiralWave();
			}
			
		}
	}
}

void HeterInhibitorySparser(int pInh,double*aGc,int GC_size,double *aP,int sizeP,bool isSpiking,bool isSpiralWave){
	printf("HeterInhibitory%s\n",pInh);
	for(int i=0;i<conf_size[pInh];++i){
		CoupleWithInhibition<MorrisLecar> in(16384,conf[pInh][i],100-conf[pInh][i]-pInh);
		for(int iP=0;iP<sizeP;++iP){
			in.createConnection("Sparser",aP[iP]);
			for(int j=0;j<GC_size;++j){
				in.setCouple(aGc[j],aGc[j],-25,36,-45);
				Network<MorrisLecar,CoupleWithInhibition> net(&in);
				net.logInfo();
				if(isSpiking&&!isTest){
					net.OutputSpikingIndex();
					net.OutputAverISI();
					net.OutputCV();
					net.OutputPhaseAmplitude();
					net.OutputAverISIForOneAndTwo();
					net.OutputCVForOneAndTwo();
				}
				if(isSpiralWave&&!isTest){
					net.SpiralWave();
				}
			}
		}
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	int aML1[]={1,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,95,99};
	int nML1=sizeof(aML1)/sizeof(int);

	double aGc[]={0.2,0.21,0.22,0.23,0.24,0.25,0.26,0.27,0.28,0.29,0.3,0.31,0.32,0.33,0.34,0.35};
	int nGc=sizeof(aGc)/sizeof(double);

	double aNoise[]={0.5,1,1.5,2};
	int nNoise=sizeof(aNoise)/sizeof(double);

	double aP[]={0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5};
	int sizeP=sizeof(aP)/sizeof(double);

//	HeterExcitatoryWithNoise(aML1,nML1,aGc,nGc,aNoise,nNoise);
//	HeterInhibitory5(aML1,nML1,aGc,nGc);
//	HeterExcitatorySparser(aML1,nML1,aGc,nGc);
//	HeterExcitatorySmallWorld(aML1,nML1,aGc,nGc);
	HeterExcitatorySquare(aML1,nML1,aGc,nGc,true,true);
	
	system("PAUSE");
	return 0;
}

