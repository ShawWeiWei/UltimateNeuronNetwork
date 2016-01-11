#include "stdafx.h"
#include "ExcitatoryCoupleWithNoise.h"
#include "Connection.h"
#include "utils.h"
#include <algorithm>
#include "stdlib.h"
#include "Constants.h"
template <typename Node>
ExcitatoryCoupleWithNoise<Node>::ExcitatoryCoupleWithNoise(int nNeuron,int pML1,bool isRestory):nNode(nNeuron),pTypeI(pML1){
	
	pNode=new Node[nNode];
	noise=new double[nNode];
	Con=new char[20];
	vector<int> vec_loc;
	//打乱
	shuffleNumber(vec_loc,nNeuron);

	int nML1=nNode*((double)pML1/100.0);
	for(int i=0;i<nML1;++i){
		pNode[vec_loc[i]].set_class1();
		pNode[vec_loc[i]].SetI(isRestory?(Node::RESTORY_CURRENT_FOR_CLASS_I):(Node::FIRING_CURRENT_FOR_CLASS_I));
	}
	for(int i=nML1;i<nNode;++i){
		pNode[vec_loc[i]].set_class2();
		pNode[vec_loc[i]].SetI(isRestory?(Node::RESTORY_CURRENT_FOR_CLASS_II):(Node::FIRING_CURRENT_FOR_CLASS_II));
	}	
}

//template <typename Node>
//ExcitatoryCoupleWithNoise<Node>::ExcitatoryCoupleWithNoise(int nNeuron,int pML1):nNode(nNeuron),pTypeI(pML1){
//	
//	pNode=new Node[nNode];
//	Con=new char[20];
//	//vector<int> vec_loc(nNode);
//	//for(int i=0;i<nNode;++i){
//	//	vec_loc[i]=i;
//	//}
//	//random_shuffle(vec_loc.begin(),vec_loc.end());
//
//	int *IsFilled=new int[nNode];
//	memset(IsFilled,0,sizeof(int)*nNode);
//	int index;
//	srand((unsigned int)100);
//	int part1=nNode*((double)pML1/100.0);
//	
//	for(int i=0;i<part1;++i){
//		index=rand()%nNode;
//		while(IsFilled[index])
//			index=rand()%nNode;
//		IsFilled[index]=1;
//		pNode[index].set_class1();
//		pNode[index].SetI(40);
//	}
//	for(int i=0;i<nNode;++i){
//		if(!IsFilled[i]){
//			pNode[i].set_class2();		
//			pNode[i].SetI(90);
//		}
//	}
//}

template <typename Node>
ExcitatoryCoupleWithNoise<Node>::~ExcitatoryCoupleWithNoise(){
	delete [] pNode;
	delete [] noise;
	delete [] Con;
}

template <typename Node>
void ExcitatoryCoupleWithNoise<Node>::createConnection(char *type){
	createRegularConnection(nNode,aExc,Con,type);
}

template <typename Node>
void ExcitatoryCoupleWithNoise<Node>::createConnection(char *type,double _rewiring){
	createRandomConnection(nNode,aExc,Con,type,_rewiring);
}
	

template <typename Node>
void ExcitatoryCoupleWithNoise<Node>::setCouple(double _gc,double _threshold,double _V_syn){
	gc=_gc;
	threshold=_threshold;
	V_syn=_V_syn;
}

template <typename Node>
void ExcitatoryCoupleWithNoise<Node>::makeFileComps(char *sCouple,char *sCon,char *sComposition,char *sSpecification){
	const char *coupletype="ExcitatoryCoupleWithNoise";
	sprintf_s(sCouple,30,"%s",coupletype);
	sprintf_s(sCon,30,"%s",Con);
	sprintf_s(sComposition,30,"pML1=%d%%",pTypeI);
	sprintf_s(sSpecification,100,"gc=%.5lf_Vsyn=%.5lf_threshold=%.5lf_noise_intensity=%.5lf",gc,V_syn,threshold,noise_intensity);
}


template <typename Node>
void ExcitatoryCoupleWithNoise<Node>::updateCouple(double *pCouple){
	double sum;
	int nExc,nTotal,iIndex;
	//更新耦合电流
	for(int i=0;i<nNode;++i){
			sum=0.0;
			nExc=aExc[i].size();
			for(int j=0;j<nExc;++j){
				iIndex=aExc[i][j];
				sum+=1.0/(1.0+exp(-(pNode[iIndex].V-threshold)));
			}
			pCouple[i]=gc*(V_syn-pNode[i].V)*sum;
		/*	if(_isnan(pCouple[i])||!_finite(pCouple[i])){
				printf("What the fuck!\n");
			}*/
	}
	
	//更新噪声
	updateNoise(noise,nNode,noise_intensity);
//	updatenoise();
	for(int i=0;i<nNode;++i){
		//double coupleOld=pCouple[i];
		//double noiseOld=noise[i];
		pCouple[i]+=noise[i]*Node::C;
		/*if(_isnan(pCouple[i])||!_finite(pCouple[i])){
				printf("What the fuck!\n");
		}*/
	}
}



