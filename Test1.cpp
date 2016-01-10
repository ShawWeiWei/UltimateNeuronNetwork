// Test1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ML.h"
#include "ExcitatoryCouple.h"
#include "ExcitatoryCouple.cpp"
#include "CoupleWithInhibition.h"
#include "CoupleWithInhibition.cpp"
#include "Network.h"
#include "Network.cpp"

void HeterExcitatory(int *aML1,int ML1_size,double *aGc,int GC_size){
	for(int i=0;i<ML1_size;++i){
		ExcitatoryCouple<MorrisLecar> ex(16384,aML1[i]);
		double aP[]={0.2,0.3,0.4,0.5,0.6};
		int sizeP=sizeof(aP)/sizeof(double);
		for(int iP=0;iP<sizeP;++iP){
			ex.createConnection("Sparser",aP[iP]);
			for(int j=0;j<GC_size;++j){

				ex.setCouple(aGc[j],-25,36);
				Network<MorrisLecar,ExcitatoryCouple> net(&ex);
				//net.OutputSpikingIndex();
				//net.OutputAverISI();
				//net.OutputCV();
				//net.OutputPhaseAmplitude();
				//net.OutputAverISIForOneAndTwo();
				//net.OutputCVForOneAndTwo();
				net.SpiralWave();
			}
		}
	}
}

void HeterInhibitory5(int *aML1,int ML1_size,double*aGc,int GC_size){
	for(int i=0;i<ML1_size;++i){
		CoupleWithInhibition<MorrisLecar> in(16384,aML1[i],95-aML1[i]);
		in.createConnection("Square");
		for(int j=0;j<GC_size;++j){

			in.setCouple(aGc[j],aGc[j],-25,36,-45);
			Network<MorrisLecar,CoupleWithInhibition> net(&in);
			net.SpiralWave();
		}
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	int aML1[]={1,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,95,99};
	int nML1=sizeof(aML1)/sizeof(int);
	vector<vector<int>> vec(12);

	vec.resize(9);
//	double aGc[]={0.27};
//	int nGc=sizeof(aGc)/sizeof(double);
//	HeterInhibitory5(aML1,nML1,aGc,nGc);
//	HeterExcitatory(aML1,nML1,aGc,nGc);
	return 0;
}

