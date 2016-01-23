#include "stdafx.h"
#include "ExcitatoryCouple.h"
#include "Connection.h"
#include <algorithm>
#include "stdlib.h"
#include "utils.h"
#include "Constants.h"

template <typename Node>
ExcitatoryCouple<Node>::ExcitatoryCouple(int nNeuron,int pML1,bool isRestory):nNode(nNeuron),pTypeI(pML1){
	
	pNode=new Node[nNode];
	Con=new char[20];
	vector<int> vec_loc;
	srand((unsigned int)SEED_FOR_RANDOM_SHUFFLE);
	shuffleNumber(vec_loc,nNode);

	int nML1=nNode*((double)pML1/100.0)+0.5;
	for(int i=0;i<nML1;++i){
		pNode[vec_loc[i]].set_class1();
		pNode[vec_loc[i]].SetI(isRestory?(Node::RESTORY_CURRENT_FOR_CLASS_I):(Node::FIRING_CURRENT_FOR_CLASS_I));
	}
	for(int i=nML1;i<nNode;++i){
		pNode[vec_loc[i]].set_class2();
		pNode[vec_loc[i]].SetI(isRestory?(Node::RESTORY_CURRENT_FOR_CLASS_II):(Node::FIRING_CURRENT_FOR_CLASS_II));
	}	
}


template <typename Node>
ExcitatoryCouple<Node>::~ExcitatoryCouple(){
	delete [] pNode;
	delete [] Con;
}

template <typename Node>
void ExcitatoryCouple<Node>::createConnection(char *type){
	aExc.clear();
	createRegularConnection(nNode,aExc,Con,type);
}

template <typename Node>
void ExcitatoryCouple<Node>::createConnection(char *type,double _rewiring){
	aExc.clear();
	createRandomConnection(nNode,aExc,Con,type,_rewiring);
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
	sprintf_s(sSpecification,100,"gc_exc=%.5lf_v_exc=%.5lf_threshold=%.5lf",gc,V_syn,threshold);
}


template <typename Node>
void ExcitatoryCouple<Node>::updateCouple(double *pCouple){
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

template <typename Node>
void ExcitatoryCouple<Node>::outputDegreeDistribution(vector<int> &degree){
	degree.resize(nNode);
	for(int i=0;i<nNode;++i){
			degree[i]=aExc[i].size();
	}
}