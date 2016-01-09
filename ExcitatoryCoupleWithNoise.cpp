#include "stdafx.h"
#include "ExcitatoryCoupleWithNoise.h"
#include "Connection.h"
#include "ArrangeNumber.h"
#include <algorithm>
#include "stdlib.h"
template <typename Node>
ExcitatoryCoupleWithNoise<Node>::ExcitatoryCoupleWithNoise(int nNeuron,int pML1,bool isRestory):nNode(nNeuron),pTypeI(pML1){
	
	pNode=new Node[nNode];
	Con=new char[20];
	vector<int> vec_loc;
	//´òÂÒ
	shuffleNumber(vec_loc,nNeuron);

	int nML1=nNode*((double)pML1/100.0);
	for(int i=0;i<nML1;++i){
		pNode[vec_loc[i]].set_class1();
		pNode[vec_loc[i]].SetI(40);
	}
	for(int i=nML1;i<nNode;++i){
		pNode[vec_loc[i]].set_class2();
		pNode[vec_loc[i]].SetI(90);
	}	
}

template <typename Node>
ExcitatoryCouple<Node>::ExcitatoryCouple(int nNeuron,int pML1):nNode(nNeuron),pTypeI(pML1){
	
	pNode=new Node[nNode];
	Con=new char[20];
	//vector<int> vec_loc(nNode);
	//for(int i=0;i<nNode;++i){
	//	vec_loc[i]=i;
	//}
	//random_shuffle(vec_loc.begin(),vec_loc.end());

	int *IsFilled=new int[nNode];
	memset(IsFilled,0,sizeof(int)*nNode);
	int index;
	srand((unsigned int)100);
	int part1=nNode*((double)pML1/100.0);
	
	for(int i=0;i<part1;++i){
		index=rand()%nNode;
		while(IsFilled[index])
			index=rand()%nNode;
		IsFilled[index]=1;
		pNode[index].set_class1();
		pNode[index].SetI(40);
	}
	for(int i=0;i<nNode;++i){
		if(!IsFilled[i]){
			pNode[i].set_class2();		
			pNode[i].SetI(90);
		}
	}
}

template <typename Node>
ExcitatoryCouple<Node>::~ExcitatoryCouple(){
	delete [] pNode;
	delete [] Con;
}

template <typename Node>
void ExcitatoryCouple<Node>::createConnection(char *type){
	if(0==strcmp("Square",type)){
		buildSquare(nNode,aExc);
		sprintf_s(Con,20,"%s","Square");
	}
	else{
		throw("wrong input");
	}
}

template <typename Node>
void ExcitatoryCouple<Node>::createConnection(char *type,double _rewiring){
    if(0==strcmp("SmallWorld",type)){
		buildSmallWorld(nNode,_rewiring,aExc);
		sprintf_s(Con,20,"%s_%.5lf",type,_rewiring);
	}
	else if(0==strcmp("Sparser",type)){
		buildSparser(nNode,_rewiring,aExc);
		sprintf_s(Con,20,"%s_%.5lf",type,_rewiring);
	}
	else{
		throw("Invalid input!");
	}
}
	

template <typename Node>
void ExcitatoryCouple<Node>::setCouple(double _gc,double _threshold,double _V_syn){
	gc=_gc;
	threshold=_threshold;
	V_syn=_V_syn;
}

template <typename Node>
void ExcitatoryCouple<Node>::makeFileComps(char *sCouple,char *sCon,char *sComposition,char *sSpecification){
	const char *coupletype="ExcitatoryCouple";
	sprintf_s(sCouple,30,"%s",coupletype);
	sprintf_s(sCon,30,"%s",Con);
	sprintf_s(sComposition,30,"pML1=%d%%",pTypeI);
	sprintf_s(sSpecification,100,"gc=%.5lf_Vsyn=%.5lf_threshold=%.5lf",gc,V_syn,threshold);
}


template <typename Node>
void ExcitatoryCouple<Node>::updateCouple(double *pCouple){
//	memset(pCouple,0,sizeof(double)*nNeuron);
	double sum;
	int nExc,nTotal,iIndex;
	for(int i=0;i<nNode;++i){
			sum=0.0;
			nExc=aExc[i].size();
			for(int j=0;j<nExc;++j){
				iIndex=aExc[i][j];
				sum+=1.0/(1.0+exp(-(pNode[iIndex].V-threshold)));
			}
			pCouple[i]=gc*(V_syn-pNode[i].V)*sum;
	}
}