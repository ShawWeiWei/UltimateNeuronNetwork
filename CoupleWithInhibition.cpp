#include "stdafx.h"
#include "CoupleWithInhibition.h"
#include "Connection.h"
#include <algorithm>

template <typename Node>
CoupleWithInhibition<Node>::CoupleWithInhibition(int nNeuron,int pML1,int pML2):nNode(nNeuron),pTypeI(pML1),pTypeII(pML2){
	pNode=new Node[nNode];
	pNodeSyn=new NodeSyn[nNode];
	isCoupled=new bool[nNode];
	conn=new char[20];
	vector<int> vec_loc(nNode);
	for(int i=0;i<nNode;++i){
		vec_loc[i]=i;
	}
	random_shuffle(vec_loc.begin(),vec_loc.end());

	int nML1=nNode*((double)pML1/100.0);
	for(int i=0;i<nML1;++i){
		pNode[vec_loc[i]].set_class1();
		pNode[vec_loc[i]].SetI(39.7);
		pNodeSyn[vec_loc[i]]=Excitory;
		isCoupled[vec_loc[i]]=true;
	}
	int nML2=nNode*((double)pML2/100.0);
	for(int i=nML1;i<nML1+nML2;++i){
		pNode[vec_loc[i]].set_class2();
		pNode[vec_loc[i]].SetI(88.1);
		pNodeSyn[vec_loc[i]]=Excitory;
		isCoupled[vec_loc[i]]=true;
	}	
	for(int i=nML1+nML2;i<nNode;++i){
		pNode[vec_loc[i]].set_class2();
		pNode[vec_loc[i]].SetI(100);
		pNodeSyn[vec_loc[i]]=Inhibitory;
		isCoupled[vec_loc[i]]=false;
	}

}

template <typename Node>
CoupleWithInhibition<Node>::~CoupleWithInhibition(){
	delete []pNode;
}

template <typename Node>
void CoupleWithInhibition<Node>::setExcInh(const vector<vector<int>>&vec){
	aExc.resize(nNode);
	aInh.resize(nNode);
	for(int i=0;i<nNode;++i){
		if(isCoupled[i]){
			int s=vec[i].size();
			for(int j=0;j<s;++j){
				int the_node=vec[i][j];
				if(pNodeSyn[the_node]==Excitory){
					aExc[i].push_back(the_node);
				}
				else if(pNodeSyn[the_node]==Inhibitory){
					aInh[i].push_back(the_node);
				}
			}
		}
	}
}
template <typename Node>
void CoupleWithInhibition<Node>::createConnection(char *type){

	vector<vector<int>> vec;
	createRegularConnection(nNode,vec,conn,type);

	setExcInh(vec);
}

template <typename Node>
void CoupleWithInhibition<Node>::createConnection(char *type,double _rewiring){

	vector<vector<int>> vec;
	createRandomConnection(nNode,vec,conn,type,_rewiring);
	setExcInh(vec);
}
	

template <typename Node>
void CoupleWithInhibition<Node>::setCouple(double _gc_exc,double _gc_inh,double _threshold,double _Vsyn_exc,double _Vsyn_inh){
	gc_exc=_gc_exc;
	gc_inh=_gc_inh;
	threshold=_threshold;
	Vsyn_exc=_Vsyn_exc;
	Vsyn_inh=_Vsyn_inh;
}

template <typename Node>
void CoupleWithInhibition<Node>::updateCouple(double *pCouple){
	double sumExc,sumInh;
	int nExc,nInh,iIndex;
	for(int i=0;i<nNode;++i){
		if(isCoupled[i]){
			sumExc=0.0;
			sumInh=0.0;
			nExc=aExc[i].size();
			nInh=aInh[i].size();
			for(int j=0;j<nExc;++j){
				iIndex=aExc[i][j];
				sumExc+=1.0/(1.0+exp(-(pNode[iIndex].V-threshold)));
			}
			for(int j=0;j<nInh;++j){
				iIndex=aInh[i][j];
				sumInh+=1.0/(1.0+exp(-(pNode[iIndex].V-threshold)));
			}
			pCouple[i]=gc_exc*(Vsyn_exc-pNode[i].V)*sumExc+gc_inh*(Vsyn_inh-pNode[i].V)*sumExc;
		}
		else{
			pCouple[i]=0.0;
		}
	}
}

template <typename Node>
void CoupleWithInhibition<Node>::OutputConn(){
	FILE *pConn;
	char filename[200];
	sprintf_s(filename,"F:/NetConn/CoupleWithInhibition/%s/pML1=%d_pML2=%d/conn.dat",conn,pType1,pType2);
	fopen_s(&pConn,filename,"w");
	for(int i=0;i<nNode;++i){
		int size_exc=aExc[i].size();
		int size_inh=aInh[i].size();
		for(int j=0;j<size_exc;++j){
			fprintf(pConn,"%d ",aExc[i][j]);
		}
		for(int j=0;j<size_inh;++j){
			fprintf(pConn,"%d ",aInh[i][j]);
		}
		fprintf(pConn,"\n");
	}
}

template <typename Node>
void CoupleWithInhibition<Node>::makeFileComps(char *sCouple,char *sCon,char *sComposition,char *sSpecification){
	const char *coupletype="CoupleWithInhibition";
	sprintf_s(sCouple,30,"%s",coupletype);
	sprintf_s(sCon,30,"%s",conn);
	sprintf_s(sComposition,30,"pML1=%d%%_pML2=%d%%",pTypeI,pTypeII);
	sprintf_s(sSpecification,100,"gc_exc=%.5lf_V_exc=%.5lf_gc_inh=%.5lf_V_inh=%.5lf_threshold=%.5lf",gc_exc,Vsyn_exc,gc_inh,Vsyn_inh,threshold);
}