

#include "stdafx.h"
#include "Network.h"
#include "ML.h"
#include "Constants.h"
#include <stdio.h>
#include <direct.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;



template <typename Node, template<typename> class CoupleType>
Network<Node,CoupleType>::Network(CoupleType<Node> *_couple):nNode(_couple->getNum()),pCouple(_couple),pNode(_couple->getPNode()){
	pCouplingCurrents=new double[nNode];
	sCouple=new char [30];
	sCon=new char [30];
	sComposition=new char[30];
	sSpecification=new char[100];
	sPre=new char[250];
}

//Preprocess 3:Specify neural currents
template <typename Node, template<typename> class CoupleType>
Network<Node,CoupleType>::~Network(){
	delete []pCouplingCurrents;
	delete []sCouple;
	delete []sCon;
	delete []sComposition;
	delete []sSpecification;
	delete []sPre;
}


//Preprocess 4:Set the initial values of variables to sychronization...
template <typename Node, template<typename> class CoupleType>
void Network<Node,CoupleType>::__AlSyncVar(){
	t=0.0;
	_SetDt(0.1);
	printf("Initial value is randomized.\n");
	srand((unsigned int)100);               //random generator seed is 100
	for(int i=0;i<nNode;++i)
		pNode[i].RandVar();
	srand(100);
}

template <typename Node, template<typename> class CoupleType>
void Network<Node,CoupleType>::_SetDt(double _dt){
	dt=_dt;
	for(int i=0;i<nNode;++i){
		pNode[i].SetDt(_dt);
	}
}

template <typename Node, template<typename> class CoupleType>
void Network<Node,CoupleType>::_EulerIterate(){
	for(int i=0;i<nNode;++i){
		pNode[i].euler(pCouplingCurrents[i]);
	}
	t+=dt;
}


template <typename Node, template<typename> class CoupleType>
bool Network<Node,CoupleType>::__IsML1(int No){
	
	if(0==strcmp(pNode[No].neuron_model,"MorrisLecar1")){
		return true;
	}
	return false;
}

template <typename Node, template<typename> class CoupleType>
bool Network<Node,CoupleType>::__IsML2(int No){
	if(0==strcmp(pNode[No].neuron_model,"MorrisLecar2")){
		return true;
	}
	return false;
}

template <typename Node, template<typename> class CoupleType>
bool Network<Node,CoupleType>::__IsInner(int No){
	int dimension=sqrt(nNode)+0.5;
	int row=No/dimension;
	int column=No%dimension;
	if(row>=16&&row<=111&&column>=16&&column<=111){
		return true;
	}
	return false;
}

template <typename Node, template<typename> class CoupleType>
void Network<Node,CoupleType>::_MakePrefix(){
	pCouple->makeFileComps(sCouple,sCon,sComposition,sSpecification);
	char direct1[100];
	sprintf_s(direct1,"F:\\output\\%s",sCouple);
	_mkdir(direct1);
	char direct2[100];
	sprintf_s(direct2,100,"F:\\output\\%s\\%s",sCouple,sCon);
	_mkdir(direct2);
	char direct3[100];
	sprintf_s(direct3,100,"F:\\output\\%s\\%s\\%s",sCouple,sCon,sComposition);
	_mkdir(direct3);

	sprintf_s(sPre,250,"F:\\output\\%s\\%s\\%s\\%s",sCouple,sCon,sComposition,sSpecification);
}

template <typename Node,template <typename> class CoupleType>
void Network<Node,CoupleType>::_MakeFileName(char *filename,char *postfix){
	sprintf_s(filename,300,"%s_%s",sPre,postfix);
}

template <typename Node, template<typename> class CoupleType>
void Network<Node,CoupleType>::OutputI(){
	//Create directory
	char pre[50];
	sprintf_s(pre,"F:\\Sigmoidal_Square\\JustForTest%d",1);
//	_mkdir(pre);

	//Create a file pointer
	FILE *pOutputI;
	char filename[100];
	sprintf_s(filename,"%s_I.dat",pre);
	fopen_s(&pOutputI,filename,"w");
//	fprintf(pOutputI,"NeuronID,ML1_I,ML2_I\n");

	for(int i=0;i<nNode;++i){
//		if(__IsML1(i)){
				fprintf(pOutputI,"%d,%lf,\n",i,pNode[i].GetI());
//		}
//		if(__IsML2(i)){
//				fprintf(pOutputI,"%d,,%lf\n",i,pNode[i].I);
//		}
	}
}



template <typename Node, template<typename> class CoupleType>
void Network<Node,CoupleType>::OutputCoupleIdx(){
	FILE *pSquare;
	fopen_s(&pSquare,"F:\\Sigmoidal_Square\\ML1_%dML2_%d_CoupleIdx.dat","w");
	int iDimension=(int)(sqrt(nNode)+0.5);
	int nCount;
	for(int i=0;i<nNode;++i){
//		fprintf(pSquare,"neuron(%d)(Exc %d,Inh %d) ",i,aExcitation[i],aInhibition[i]);
		fprintf(pSquare,"neuron(%d) ",i);
		nCount=mpCoupleNum[i];//aExcitation[i]+aInhibition[i];
		for(int j=0;j<nCount;++j){
				fprintf(pSquare,"%d ",mpCoupleIdx[i][j]);
		}

		fprintf(pSquare,"\n");
	}
	fclose(pSquare);
}

template <typename Node, template<typename> class CoupleType>
void Network<Node,CoupleType>::PrintTypeNum(){
//	FILE *pSquare;
//	fopen_s(&pSquare,"F:\\Sigmoidal_Square\\ML1_%dML2_%d_Type.dat","w");
//	int iDimension=(int)(sqrt(nNode)+0.5);
//	int nCount;
	int nType1=0,nType2=0;
	long nAdd1=0,nAdd2=0;
	for(int i=0;i<nNode;++i){
		if(/*0==strcmp(pNode[i].neuron_model,"MorrisLecar1")*/__IsML1(i)){
			nType1++;
			nAdd1+=i;
		}
		else{
			nType2++;
			nAdd2+=i;
		}
	}


	printf("%d %d: %d %d\n",m_nML1,nType1,(nAdd1/nType1),(nAdd2/nType2));

}

template <typename Node, template<typename> class CoupleType>
void Network<Node,CoupleType>::OutputNoForOneAndTwo(){
	srand(100);
	FILE *fp;
	char filename[100];
	pCouple->makeFileComps(sCouple,sCon,sComposition,sSpecification);
	sprintf_s(filename,"%s\\%s_NO.dat",CONFIG_DIRECT,sComposition);
	fopen_s(&fp,filename,"w");
	int one[5],two[5];
	int indexForOne=0,indexForTwo=0;
	int randNo;
	const int num=5;
	//if(m_nML2==0){
	//	while(indexForOne<num){
	//		randNo=rand()%nNode;
	//		if(__IsInner(randNo)){
	//			if(__IsML1(randNo)){
	//				one[indexForOne]=randNo;
	//				indexForOne++;
	//			}
	//		}	
	//	}
	//	for(int i=0;i<num;++i){
	//		fprintf(fp,"%d ",one[i]);
	//	}
	//	fprintf(fp,"\n");
	//}
	//else if(m_nML1==0){
	//	while(indexForTwo<num){
	//		randNo=rand()%nNode;
	//		if(__IsInner(randNo)){
	//			if(__IsML2(randNo)){
	//				two[indexForTwo]=randNo;
	//				indexForTwo++;
	//			}
	//		}		
	//	}
	//	for(int i=0;i<num;++i){
	//		fprintf(fp,"%d ",two[i]);
	//	}
	//	fprintf(fp,"\n");
	//}
//	else{
		while(indexForOne<num&&indexForTwo<num){
			randNo=rand()%nNode;
			if(__IsInner(randNo)){
				if(__IsML1(randNo)){
					one[indexForOne]=randNo;
					indexForOne++;
				}
				if(__IsML2(randNo)){
					two[indexForTwo]=randNo;
					indexForTwo++;
				}
			}
		}
		while(indexForOne<num){
			randNo=rand()%nNode;
			if(__IsInner(randNo)){
				if(__IsML1(randNo)){
					one[indexForOne]=randNo;
					indexForOne++;
				}
			}	
		}
		while(indexForTwo<num){
			randNo=rand()%nNode;
			if(__IsInner(randNo)){
				if(__IsML2(randNo)){
					two[indexForTwo]=randNo;
					indexForTwo++;
				}
			}		
		}
		for(int i=0;i<num;++i){
			fprintf(fp,"%d ",one[i]);
		}
		fprintf(fp,"\n");
		for(int i=0;i<num;++i){
			fprintf(fp,"%d ",two[i]);
		}
		fprintf(fp,"\n");
		
//	}
	fclose(fp);
}


//process
template <typename Node, template<typename> class CoupleType>
void Network<Node,CoupleType>::OutputTimeSeries(){
	//Initialize
	__AlSyncVar();
	//Create directory
	char *timeSeriesFileName=new char[300];
	_MakePrefix();
	_MakeFileName(timeSeriesFileName,"TimeSeries.dat");

	/*Create file pointer...*/
	FILE* pOutputTimeSeries;
	fopen_s(&pOutputTimeSeries,timeSeriesFileName,"w");
//	fopen_s(&pOutputCouple,coupleFileName,"w");
	int iter_trans,iter_end;
	
	iter_trans=(int)(time_begin/dt+0.5);
	iter_end=(int)(time_end/dt+0.5);
	//Calculate

	char inFilename[100];
	pCouple->makeFileComps(sCouple,sCon,sComposition,sSpecification);
	sprintf_s(inFilename,"%s\\%s_NO.dat",CONFIG_DIRECT,sComposition);
	std::ifstream No(inFilename,std::ios::in);
	std::vector<int> vec;
	int inputNo;
	while(!No.eof()){
		No>>inputNo;
		if(No.fail())
			break;
		vec.push_back(inputNo);
	}
	No.close();
	int n=vec.size();
	for(int i=1;i<=iter_trans;++i){
		pCouple->updateCouple(pCouplingCurrents);
		//__UpdateNoise();
		_EulerIterate();
	}
	for(int i=iter_trans+1;i<=iter_end;++i){
		pCouple->updateCouple(pCouplingCurrents);
		//__UpdateNoise();
		_EulerIterate();
		fprintf(pOutputTimeSeries,"%lf ",t);
		for(int j=0;j<n;++j){
			fprintf(pOutputTimeSeries,"%lf ",pNode[vec[j]].V);
		}
		fprintf(pOutputTimeSeries,"\n");
		//printf("%d\n",i);
	}
	fclose(pOutputTimeSeries);

	delete []timeSeriesFileName;
}

template <typename Node, template<typename> class CoupleType>
void Network<Node,CoupleType>::OutputCouple(){
	//Initialize
	__AlSyncVar();
	//Create directory
	char coupleSeriesFileName=new char[300];
	_MakePrefix();
	_MakeFileName(coupleSeriesFileName,"CoupleSeries.dat");

	/*Create file pointer...*/
	FILE* pOutputCoupleSeries;
	fopen_s(&pOutputCoupleSeries,coupleSeriesFileName,"w");
//	fopen_s(&pOutputCouple,coupleFileName,"w");
	int iter_trans,iter_end;
	iter_trans=(int)(time_begin/dt+0.5);
	iter_end=(int)(time_end/dt+0.5);
	//Calculate

	char inFilename[100];
	pCouple->makeFileComps(sCouple,sCon,sComposition,sSpecification);
	sprintf_s(inFilename,"%s\\%s_NO.dat",CONFIG_DIRECT,sComposition);
	std::ifstream No(inFilename,std::ios::in);
	std::vector<int> vec;
	int inputNo;
	while(!No.eof()){
		No>>inputNo;
		if(No.fail())
			break;
		vec.push_back(inputNo);
	}
	No.close();
	int n=vec.size();
	for(int i=1;i<=iter_trans;++i){
		pCouple->updateCouple(pCouplingCurrents);
		//__UpdateNoise();
		_EulerIterate();
	}
	for(int i=iter_trans+1;i<=iter_end;++i){
		pCouple->updateCouple(pCouplingCurrents);
		//__UpdateNoise();
		_EulerIterate();
		fprintf(pOutputCoupleSeries,"%lf ",t);
		for(int j=0;j<n;++j){
			fprintf(pOutputCoupleSeries,"%lf ",sm_gCouple[vec[j]]);
		}
		fprintf(pOutputCoupleSeries,"\n");
		//printf("%d\n",i);
	}
	fclose(pOutputCoupleSeries);
	

}

template <typename Node, template<typename> class CoupleType>
void Network<Node,CoupleType>::OutputCoupleAndPotential(){
	//Initialize
	__AlSyncVar();
	//Create directory
	char *coupleSeriesFileName=new char[300];
	_MakePrefix();
	_MakeFileName(coupleSeriesFileName,"CoupleAndPotentialSeries.dat");

	/*Create file pointer...*/
	FILE* pOutputCoupleSeries;
	fopen_s(&pOutputCoupleSeries,coupleSeriesFileName,"w");
//	fopen_s(&pOutputCouple,coupleFileName,"w");
	int iter_trans,iter_end;
	iter_trans=(int)(time_begin/dt+0.5);
	iter_end=(int)(time_end/dt+0.5);

	for(int i=0;i<=iter_end;++i){
		pCouple->updateCouple(pCouplingCurrents);
		//__UpdateNoise();
		_EulerIterate();
		fprintf(pOutputCoupleSeries,"%lf ",t);
		for(int j=100;j<101;++j){
			fprintf(pOutputCoupleSeries,"%lf %lf",pCouplingCurrents[j],pNode[j].V);
		}
		fprintf(pOutputCoupleSeries,"\n");
		//printf("%d\n",i);
	}
	fclose(pOutputCoupleSeries);
}

template <typename Node, template<typename> class CoupleType>
void Network<Node,CoupleType>::SpiralWave(){
	/*Initialize...*/
	__AlSyncVar();
	/*Create directory...*/

	_MakePrefix();

	char *spiralWaveFileName=new char[300];


	int iter_trans,iter_end;

	iter_trans=(int)(time_begin/dt+0.5);
	iter_end=(int)(time_end/dt+0.5);

//	int length=strlen(specification);
	int iColumn=int(sqrt(nNode)+0.5);
	/*Create file pointer...*/
	FILE* pOutputSpiralWave;

	int iDimension=int(sqrt((double)nNode)+0.5);
	printf("%d\n",iDimension);
	for(int i=1;i<=iter_trans;++i){
		pCouple->updateCouple(pCouplingCurrents);
		//__UpdateNoise();
		_EulerIterate();
	}

	for(int i=iter_trans+1;i<iter_end;++i){
		pCouple->updateCouple(pCouplingCurrents);
		//__UpdateNoise();
		_EulerIterate();

		if(i%200==0){
			sprintf_s(spiralWaveFileName,300,"%s_t=%.5lf.dat",sPre,t);
		    fopen_s(&pOutputSpiralWave,spiralWaveFileName,"w");	

			for(int j=0;j<nNode;++j){
				fprintf(pOutputSpiralWave,"%lf ",pNode[j].V);
				if(j%iDimension==iDimension-1){
					fprintf(pOutputSpiralWave,"\n");

				}
			}
			fprintf(pOutputSpiralWave,"\n");
			fclose(pOutputSpiralWave);
		}
	}	
	delete []spiralWaveFileName;
}

template <typename Node, template<typename> class CoupleType>
void Network<Node,CoupleType>::SpiralWave(double _begin,double _end,double _dt){
	/*Initialize...*/
	__AlSyncVar();
	/*Create directory...*/
	char *spiralWaveFileName=new char[300];
	_MakePrefix();

	int iColumn=int(sqrt(nNode)+0.5);


	int iDimension=int(sqrt((double)nNode)+0.5);
	int iBegin=int(_begin/dt+0.5);
	int iEnd=int(_end/dt+0.5);
	int iDt=int(_dt/dt+0.5);
	for(int i=0;i<=iEnd;++i){
		if(i>=iBegin&&(i-iBegin)%iDt==0){
			/*Create file pointer...*/
			FILE* pOutputSpiralWave;
			sprintf_s(spiralWaveFileName,300,"%s_t=%.5lf.dat",sPre,t);
			//sprintf_s(filename,"%s\\%s_noise=%.5lf_t=%.5lf.dat",direct,specification,t);
			fopen_s(&pOutputSpiralWave,filename,"w");
			for(int j=0;j<nNode;++j){
				fprintf(pOutputSpiralWave,"%lf ",pNode[j].V);
				if(j%iDimension==iColumn-1) fprintf(pOutputSpiralWave,"\n");
			}
			fprintf(pOutputSpiralWave,"\n");
			fclose(pOutputSpiralWave);
		}
	
		//Calculate...
		pCouple->updateCouple(pCouplingCurrents);
     	//__UpdateNoise();
		_EulerIterate();
	}
	delete []spiralWaveFileName;
	
}

//template <typename Node, template<typename> class CoupleType>
//void Network<Node,CoupleType>::SpiralWaveAndTimeSeries(){
//	/*Initialize...*/
//	__AlSyncVar();
//	/*Create directory...*/
//	char timeSeriesFileName=new char[300];
//	char spiralWaveFileName=new char[300];
//	const char *postfix="TimeSeries.dat";
//	_MakePrefix();
//	_MakeFileName(TimeSeriesFileName,postfix);
//
//	/*Create file pointer...*/
//	FILE *pOutputTimeSeries,* pOutputCoupleSeries;
//	
//	fopen_s(&pOutputTimeSeries,timeSeriesFileName,"w");
//	sprintf_s(filename,"%s\\%s_CoupleSeries.dat",direct,specification);
//	fopen_s(&pOutputCoupleSeries,filename,"w");
//	int iDimension=int(sqrt((double)nNode)+0.5);
//	int iter_trans,iter_end;
//	if(percentageOfML1>95){
//	    iter_trans=(int)(2000/dt+0.5);
//	    iter_end=(int)(8000/dt+0.5);
//	}
//	else{
//		iter_trans=(int)(2000/dt+0.5);
//		iter_end=(int)(5000/dt+0.5);
//	}
//	//Calculate
//
//	char inFilename[100];
//	sprintf_s(inFilename,"F:\\config\\ML1=%d_ML2=%d_NO.dat",m_nML1,m_nML2);
//	std::ifstream No(inFilename,std::ios::in);
//	std::vector<int> vec;
//	int inputNo;
//	while(!No.eof()){
//		No>>inputNo;
//		if(No.fail())
//			break;
//		vec.push_back(inputNo);
//	}
//	No.close();
//	int n=vec.size();
//	for(int i=1;i<=iter_trans;++i){
//		pCouple->updateCouple(pCouplingCurrents);
////		//__UpdateNoise();
//		_EulerIterate();
//	}
//
//	for(int i=iter_trans+1;i<iter_end;++i){
//		pCouple->updateCouple(pCouplingCurrents);
////		//__UpdateNoise();
//		_EulerIterate();
//		fprintf(pOutputTimeSeries,"%lf ",t);
//		fprintf(pOutputCoupleSeries,"%lf ",t);
//		for(int j=0;j<n;++j){
//			fprintf(pOutputTimeSeries,"%lf ",pNode[vec[j]].V);
//			fprintf(pOutputCoupleSeries,"%lf ",sm_gCouple[vec[j]]);
//		}
//		fprintf(pOutputTimeSeries,"\n");
//		fprintf(pOutputCoupleSeries,"\n");
//		if(i%200==0){
//			FILE* pOutputSpiralWave;
//			sprintf_s(filename,"%s\\%s_t=%.5lf.dat",direct,specification,t);
//		    fopen_s(&pOutputSpiralWave,filename,"w");	
//
//			for(int j=0;j<nNode;++j){
//				fprintf(pOutputSpiralWave,"%lf ",pNode[j].V);
//				if(j%iDimension==iDimension-1) fprintf(pOutputSpiralWave,"\n");
//			}
//			fprintf(pOutputSpiralWave,"\n");
//			fclose(pOutputSpiralWave);
//		}
//	}
//	
//	fclose(pOutputCoupleSeries);
//	fclose(pOutputTimeSeries);
//	delete []direct;
//	delete []specification;
//}

template <typename Node, template<typename> class CoupleType>
void Network<Node,CoupleType>::OutputSpikingIndex(){
	//Initialize
	__AlSyncVar();
	//Create directory
	_MakePrefix();
	char *sSpikingIndex=new char[300];
	_MakeFileName(sSpikingIndex,"SpikingIndex.dat");

	/*Create file pointer...*/
	FILE* pOutputSpikingIndex;
	fopen_s(&pOutputSpikingIndex,sSpikingIndex,"w");
	int iter_trans,iter_end;

	iter_trans=(int)(time_begin/dt+0.5);
	iter_end=(int)(time_end/dt+0.5);

	//Calculate
	for(int i=1;i<=iter_trans;++i){
		pCouple->updateCouple(pCouplingCurrents);
		_EulerIterate();
	}
	double *V1=new double[nNode];
	double *V2=new double[nNode];
	vector<int> spikingindex;
	
	vector<vector<int> > vecforspikingindex(nNode);
	for(int i=0;i<nNode;i++){
		vecforspikingindex[i].push_back(iter_trans+1);
	}
	for(int i=0;i<nNode;i++){
		V1[i]=pNode[i].V;
		V2[i]=pNode[i].V;
	}
	for(int i=iter_trans+1;i<=iter_end;++i){
		pCouple->updateCouple(pCouplingCurrents);
		_EulerIterate();
		
		for(int j=0;j<nNode;++j){
			if(V2[j]>20&&V2[j]>pNode[j].V&&V2[j]>V1[j]){
				vecforspikingindex[j].push_back(i);
			}
			V1[j]=V2[j];
			V2[j]=pNode[j].V;
		}
	}
	
	for(int i=0;i<nNode;i++){
		vecforspikingindex[i].push_back(iter_end);
	}

//	int dimension=sqrt(nNode)+0.5;
//	int row,column;
	for(int i=0;i<nNode;++i){
//		row=nNode/dimension;
//		column=nNode%dimension;
//		if((row!=0)&&(row!=dimension-1)&&(column!=0)&&(column!=dimension-1)){
			for(auto it=vecforspikingindex[i].begin();it!=vecforspikingindex[i].end();it++){
				fprintf(pOutputSpikingIndex,"%d ",*it);
			}
			fprintf(pOutputSpikingIndex,"\n");
//		}
	}
	fclose(pOutputSpikingIndex);
	delete []sSpikingIndex;
}

template <typename Node, template<typename> class CoupleType>
void Network<Node,CoupleType>::OutputPhaseAmplitude(){
	//Create directory
	_MakePrefix();
	char *sSpikingIndex=new char [300];
	char *sPhaseAmplitude=new char[300];
	_MakeFileName(sSpikingIndex,"SpikingIndex.dat");
	_MakeFileName(sPhaseAmplitude,"PhaseAmplitude.dat");

	ifstream inputSpikingIndex(sSpikingIndex,ios::in);
	string line;
	int iSpikingIndex;
	getline(inputSpikingIndex,line);
	vector<int> maxiList; 
	int maxiListCount;
	istringstream findbeginandend(line);
	while(!findbeginandend.eof()){
		findbeginandend>>iSpikingIndex;
		if(!findbeginandend.fail()){
			maxiList.push_back(iSpikingIndex);
		}
	}
	maxiListCount=maxiList.size();
	int begin=maxiList[0];
	int end=maxiList[maxiListCount-1];
	const int count=end-begin+1;
	int nodecount=0;
	double *cosphase=new double[count];
	double *sinphase=new double[count];
	double *order=new double[count];
	for(int i=0;i<count;++i){
		cosphase[i]=0.0;
		sinphase[i]=0.0;
		order[i]=0.0;
	}

	double deltat,aver_deltat;
	int leftindex,rightindex;
	double phaseangle;
	
	inputSpikingIndex.seekg(0);
	while(!inputSpikingIndex.eof()){
		getline(inputSpikingIndex,line);
		maxiList.clear();
		if(!inputSpikingIndex.fail()){
			nodecount++;
			istringstream instring(line);
			while(!instring.eof()){
				instring>>iSpikingIndex;
				if(!instring.fail()){
					maxiList.push_back(iSpikingIndex);
				}
			}
			maxiListCount=maxiList.size();
			if(maxiListCount<4){
				printf("There are not spikes!\n");
				for(int i=0;i<count;++i){
					cosphase[i]=cosphase[i]+1;
				}
//				return;
				//exit(-1);
			}
			else{
				aver_deltat=double(maxiList[maxiListCount-2]-maxiList[1])/(maxiListCount-3);
				leftindex=maxiList[0];
				rightindex=maxiList[1];
				for(int i=leftindex;i!=rightindex;i++){
					phaseangle=2.0*PI*(aver_deltat+i-rightindex)/aver_deltat;
					cosphase[i-begin]+=cos(phaseangle);
					sinphase[i-begin]+=sin(phaseangle);
				}
 
				for(int m=1;m!=maxiListCount-2;++m){
					leftindex=maxiList[m];
					rightindex=maxiList[m+1];
					deltat=double(rightindex-leftindex);
					for(int i=leftindex;i!=rightindex;++i){
						phaseangle=2.0*PI*(i-leftindex)/deltat;
						cosphase[i-begin]+=cos(phaseangle);
						sinphase[i-begin]+=sin(phaseangle);
					}
				}


                
				leftindex=maxiList[maxiListCount-2];
				rightindex=maxiList[maxiListCount-1];
				for(int i=leftindex;i<=rightindex;++i){
					phaseangle=2.0*PI*(i-leftindex)/aver_deltat;
					cosphase[i-begin]+=cos(phaseangle);
					sinphase[i-begin]+=sin(phaseangle);
				}
			}
            
		}
	}
	for(int i=0;i<count;++i){
		order[i]=sqrt(pow(cosphase[i],2)+pow(sinphase[i],2))/nodecount;
	}

	FILE *pPhaseAmplitude;
	fopen_s(&pPhaseAmplitude,sPhaseAmplitude,"w");
	for(int i=0;i<count;++i){
		fprintf(pPhaseAmplitude,"%lf\n",order[i]);
	}
	fclose(pPhaseAmplitude);

	delete []sSpikingIndex;
	delete []sPhaseAmplitude;
}

template <typename Node, template<typename> class CoupleType>
void Network<Node,CoupleType>::OutputAverISI(){
	//Create directory
	_MakePrefix();
	char *sSpikingIndex=new char[300];
	char *sAverISI=new char[300];
	_MakeFileName(sSpikingIndex,"SpikingIndex.dat");
	_MakeFileName(sAverISI,"AverISI.dat");

	FILE *pAverISI;
	fopen_s(&pAverISI,sAverISI,"w");
	ifstream inputSpikingIndex(sSpikingIndex,ios::in);
	string line;
	int iSpikingIndex;
	int iNodeIndex=0;
	double fAverISI;
	vector<int> maxiList; 
	int maxiListCount;
	vector<double> vAverISI;

	
	while(!inputSpikingIndex.eof()){
		getline(inputSpikingIndex,line);
		maxiList.clear();
		if(!inputSpikingIndex.fail()){
			istringstream instring(line);
			while(!instring.eof()){
				instring>>iSpikingIndex;
				if(!instring.fail()){
					maxiList.push_back(iSpikingIndex);
				}
			}
			maxiListCount=maxiList.size();
			if(maxiListCount<4){
				printf("There are not spikes!\n");
				vAverISI.push_back(DBL_MAX);
				continue;
				//exit(-1);
			}
			fAverISI=(double)(maxiList[maxiListCount-2]-maxiList[1])*dt/(maxiListCount-3);
			vAverISI.push_back(fAverISI);      
		}
	}
	for(auto it=vAverISI.begin();it!=vAverISI.end();++it){
		fprintf(pAverISI,"%lf\n",*it);
	}
	fclose(pAverISI);

	delete []sSpikingIndex;
	delete []sAverISI;
}

template <typename Node, template<typename> class CoupleType>
void Network<Node,CoupleType>::OutputAverISIForOneAndTwo(){
	//Create directory
	_MakePrefix();
	char *sAverISIType1=new char[300];
	char *sAverISIType2=new char[300];
	char *sAverISI=new char[300];
	_MakeFileName(sAverISIType1,"AverISIType1.dat");
	_MakeFileName(sAverISIType2,"AverISIType2.dat");
	_MakeFileName(sAverISI,"AverISI.dat");


	FILE *pAverISIType1,*pAverISIType2,*pAverISI;
	fopen_s(&pAverISI,sAverISI,"r");
	fopen_s(&pAverISIType1,sAverISIType1,"w");
	fopen_s(&pAverISIType2,sAverISIType2,"w");
	double averISI;
	for(int i=0;i<nNode;++i){
		fscanf_s(pAverISI,"%lf \n",&averISI);
		if(/*0==strcmp(pNode[i].neuron_model,"MorrisLecar1")*/__IsML1(i)){
			fprintf(pAverISIType1,"%lf \n",averISI);
		}
		else{
			fprintf(pAverISIType2,"%lf \n",averISI);
		}
	}
	fclose(pAverISIType1);
	fclose(pAverISIType2);
	fclose(pAverISI);

	delete []sAverISI;
	delete []sAverISIType1;
	delete []sAverISIType2;
}
template <typename Node, template<typename> class CoupleType>
void Network<Node,CoupleType>::OutputCV(){
	//Create directory
	_MakePrefix();
	char *sSpikingIndex=new char[300];
	char *sCV=new char[300];
	_MakeFileName(sSpikingIndex,"SpikingIndex.dat");
	_MakeFileName(sCV,"CV.dat");

	FILE *pCV;
	fopen_s(&pCV,sCV,"w");
	ifstream inputSpikingIndex(sSpikingIndex,ios::in);
	string line;
	int iSpikingIndex;
	int iNodeIndex=0;
	double fCV;
	vector<int> maxiList; 
	int maxiListCount;
	vector<double> vAverOfSum,vAverOfSquareSum,vCV;

	
	while(!inputSpikingIndex.eof()){
		getline(inputSpikingIndex,line);
		maxiList.clear();
		if(!inputSpikingIndex.fail()){
			istringstream instring(line);
			while(!instring.eof()){
				instring>>iSpikingIndex;
				if(!instring.fail()){
					maxiList.push_back(iSpikingIndex);
				}
			}
			maxiListCount=maxiList.size();
			if(maxiListCount<4){
				printf("There are not spikes!\n");
				vAverOfSum.push_back(0);
				vAverOfSquareSum.push_back(0);
				vCV.push_back(0.0);
				continue;
				//exit(-1);
			}
			else{
				int nISI=maxiListCount-3;
				double squareSum=0.0,sum=0.0;
				for(int i=1;i<maxiListCount-2;++i){
					double isi=(double)(maxiList[i+1]-maxiList[i])*dt;
					squareSum+=isi*isi;
					sum+=isi;
				}
				double averofSum=sum/nISI;
				double averofSquareSum=squareSum/nISI;

				if((averofSquareSum-averofSum*averofSum)<0.00001){
					fCV=0.0;
				}
				else{
					fCV=sqrt(averofSquareSum-averofSum*averofSum)/averofSum;
				}
				vAverOfSum.push_back(averofSum);
				vAverOfSquareSum.push_back(averofSquareSum);
				vCV.push_back(fCV); 	
			}
		}
	}
	for(int i=0;i<nNode;++i){
		fprintf(pCV,"%lf\n",vCV[i]);
	}
	fclose(pCV);

	delete [] sSpikingIndex;
	delete [] sCV;
}

template <typename Node, template<typename> class CoupleType>
void Network<Node,CoupleType>::OutputCVForOneAndTwo(){
	//Create directory
	_MakePrefix();
	char *sCVType1=new char[300];
	char *sCVType2=new char[300];
	char *sCV=new char[300];
	_MakeFileName(sCVType1,"CVType1.dat");
	_MakeFileName(sCVType2,"CVType2.dat");
	_MakeFileName(sCV,"CV.dat");

	FILE *pCVType1,*pCVType2,*pCV;
	fopen_s(&pCV,sCV,"r");
	fopen_s(&pCVType1,sCVType1,"w");
	fopen_s(&pCVType2,sCVType2,"w");
	double CV;
	for(int i=0;i<nNode;++i){
		fscanf_s(pCV,"%lf \n",&CV);
		if(/*0==strcmp(pNode[i].neuron_model,"MorrisLecar1")*/__IsML1(i)){
			fprintf(pCVType1,"%lf \n",CV);
		}
		else{
			fprintf(pCVType2,"%lf \n",CV);
		}
	}
	fclose(pCVType1);
	fclose(pCVType2);
	fclose(pCV);

	delete []sCVType1;
	delete []sCVType2;
	delete []sCV;
}

template <typename Node, template<typename> class CoupleType>
void Network<Node,CoupleType>::OutputCouplePerSpike(){
	//Create directory
	_MakePrefix();

	char *sCouplingCurrents=new char[300];
	char *sSpikingIndex=new char[300];

	_MakeFileName(sCouplingCurrents,"CouplingCurrent.dat");
	_MakeFileName(sSpikingIndex,"SpikingIndex.dat");

	FILE *pCouplingCurrents;
	fopen_s(&pCouplingCurrents,sCouplingCurrents,"w");
	double *pSum=new double[nNode];
	for(int i=0;i<nNode;++i){
		pSum[i]=0.0;
	}
	double inDbl;
	while(fscanf_s(pCouplingCurrents,"%lf",&inDbl)!=EOF){
		for(int i=0;i<nNode;++i){
			fscanf_s(pCouplingCurrents,"%lf",&inDbl);
			pSum[i]+=inDbl;
		}
	}
	double *pAver=new double[nNode];
	ifstream inputSpikingIndex(sSpikingIndex,ios::in);
	string line;
	int iSpikingIndex;
	vector<int> maxiList; 
	int maxiListCount;
	int index=0;
	while(!inputSpikingIndex.eof()){
		getline(inputSpikingIndex,line);
		maxiList.clear();
		if(!inputSpikingIndex.fail()){
			istringstream instring(line);
			while(!instring.eof()){
				instring>>iSpikingIndex;
				if(!instring.fail()){
					maxiList.push_back(iSpikingIndex);
				}
			}
			maxiListCount=maxiList.size();
			if(maxiListCount==2){        
				pAver[index]=DBL_MAX;
			}
			else{
				pAver[index]=pSum[index]/(maxiListCount-2);
			}
		}
	}
	inputSpikingIndex.close();
	fclose(pCouplingCurrents);
	FILE *pOutputCouplingCurrentsPerSpike;
	char *sCouplingCurrentsPerSpike=new char[300];
	_MakeFileName(sCouplingCurrentsPerSpike,"CouplingCurrentsPerSpike.dat");	
	fopen_s(&pOutputCouplingCurrentsPerSpike,sCouplingCurrentsPerSpike,"w");
	for(int i=0;i<nNode;++i){
		fprintf(pOutputCouplingCurrentsPerSpike,"%lf\n",pAver[i]);
	}
	fclose(pOutputCouplingCurrentsPerSpike);
	delete []pAver;
	delete []pSum;

	delete []sCouplingCurrents;
	delete []sCouplingCurrentsPerSpike;
	delete []sSpikingIndex;
}
//double SquareNetwork::SyncFactor(){
//	double *sum_VikSquare=new double[nNode];
//	double *sum_Vik=new double[nNode];
//	double sum_averVkSquare=0.0,sum_averVk=0.0;
//
//	double sumVecField=0.0,averVecField=0.0;
//	double numerator=0.0,denominator=0.0,R=0.0;
//	const int iter_end=40001;
//	const int eval_begin=20000;
//	const int nTime=iter_end-eval_begin-1;
//	memset(sum_VikSquare,0,sizeof(double)*nNode);
//	memset(sum_Vik,0,sizeof(double)*nNode);
//	__AlSyncVar();
//	//Calculate
//	for(int iter=1;iter<iter_end;++iter){
//	 	pCouple->updateCouple(pCouplingCurrents);
//	//	//__UpdateNoise();
//
//		_EulerIterate();
//		if(iter>eval_begin){
//			sumVecField=0.0;
//			for(int index=0;index<nNode;++index){
//				sumVecField+=pNode[index].V;
//				sum_Vik[index]+=pNode[index].V;
//				sum_VikSquare[index]+=(pNode[index].V)*(pNode[index].V);
//			}
//			averVecField=sumVecField/nNode;
//			sum_averVk+=(averVecField);
//			sum_averVkSquare+=(averVecField*averVecField);
//		}
//	}
//
//	numerator=((sum_averVkSquare/nTime)-(sum_averVk/nTime)*(sum_averVk/nTime));
//	for(int index=0;index<nNode;++index){
//			denominator+=((sum_VikSquare[index]/nTime)-(sum_Vik[index]/nTime)*(sum_Vik[index]/nTime));
//	}
//	denominator/=nNode;
//	R=numerator/denominator;
//
//	delete []sum_Vik;
//	delete []sum_VikSquare;
//
//	return R;
//
//}

//void SquareNetwork::OutputPopulationFirings(){
//	//Create directory
//	char *direct=new char[100];
//	char *specification=new char[40];
//	char sSpikingIndex[200],sPopulationFirings[200];
//	pCouple->makeDirect(direct);
//	pCouple->makeFile(specification);
//
//	FILE *pPopulationFirings;
//	sprintf_s(sSpikingIndex,"%s\\%s_noise=%.5lf_SpikingIndex.dat",direct,specification,m_noiseintensity);	
//	sprintf_s(sPopulationFirings,"%s\\%s_noise=%.5lf_PopulationFirings.dat",direct,specification,m_noiseintensity);
//	fopen_s(&pPopulationFirings,sPopulationFirings,"w");
//	ifstream inputSpikingIndex(sSpikingIndex,ios::in);
//	string line;
//	int iSpikingIndex;
//	int iNodeIndex=0,iNodeRow,iNodeColumn;
//	int iDimension=(int)(sqrt(nNode)+0.5);
//
//	vector<int> maxiList; 
//	int maxiListCount;
//
//	while(!inputSpikingIndex.eof()){
//		getline(inputSpikingIndex,line);
//		maxiList.clear();
//		if(!inputSpikingIndex.fail()){
//			iNodeRow=iNodeIndex/iDimension;
//			iNodeColumn=iNodeIndex%iDimension;
//			iNodeIndex++;
//			istringstream instring(line);
//			while(!instring.eof()){
//				instring>>iSpikingIndex;
//				if(!instring.fail()){
//					maxiList.push_back(iSpikingIndex);
//				}
//			}
//			maxiListCount=maxiList.size();
//			if(maxiListCount<4){
//				printf("There are not spikes!\n");
//				continue;
//				//exit(-1);
//			}
//			double index2t;
//			for(int it=1;it!=maxiListCount-1;++it){
//				index2t=(double)maxiList[it]*dt;
//				fprintf(pPopulationFirings,"%d %d %lf\n",iNodeRow,iNodeColumn,index2t);
//			}    
//		}
//	}
//	
//	fclose(pPopulationFirings);
//}

//void SquareNetwork::OutputPopulationFiringsOnce(){
//	//Create directory
//	char *direct=new char[100];
//	char *specification=new char[40];
//	char sSpikingIndex[200],sPopulationFirings[200];
//	pCouple->makeDirect(direct);
//	pCouple->makeFile(specification);
//
//	FILE *pPopulationFirings;
//	sprintf_s(sSpikingIndex,"%s\\%s_noise=%.5lf_SpikingIndex.dat",direct,specification,m_noiseintensity);	
//	sprintf_s(sPopulationFirings,"%s\\%s_noise=%.5lf_PopulationFiringsOnce.dat",direct,specification,m_noiseintensity);
//	fopen_s(&pPopulationFirings,sPopulationFirings,"w");
//	ifstream inputSpikingIndex(sSpikingIndex,ios::in);
//	string line;
//	int iSpikingIndex;
//	int iNodeIndex=0,iNodeRow,iNodeColumn;
//	int iDimension=(int)(sqrt(nNode)+0.5);
//
//	vector<int> maxiList; 
//	int maxiListCount;
//
//	while(!inputSpikingIndex.eof()){
//		getline(inputSpikingIndex,line);
//		maxiList.clear();
//		if(!inputSpikingIndex.fail()){
//			iNodeRow=iNodeIndex/iDimension;
//			iNodeColumn=iNodeIndex%iDimension;
//			iNodeIndex++;
//			istringstream instring(line);
//			while(!instring.eof()){
//				instring>>iSpikingIndex;
//				if(!instring.fail()){
//					maxiList.push_back(iSpikingIndex);
//				}
//			}
//			maxiListCount=maxiList.size();
//			if(maxiListCount<4){
//				printf("There are not spikes!\n");
//				continue;
//				//exit(-1);
//			}
//			double index2t=(double)maxiList[1]*dt;
//			fprintf(pPopulationFirings,"%d %d %lf\n",iNodeRow,iNodeColumn,index2t);
//	 
//		}
//	}	
//	fclose(pPopulationFirings);
//}
//void SquareNetwork::MaximumPotential(){
//	/*Initialize...*/
//	__AlSyncVar();
//	/*Create directory...*/
//
//	char *direct=new char[100];
//	char *specification=new char[40];
//	char filename[140];
//	pCouple->makeDirect(direct);
//	pCouple->makeFile(specification);
//	int iColumn=int(sqrt(nNode)+0.5);
//	/*Create file pointer...*/
//	FILE* pOutputSpiralWave;
//	double *aMaximumV=new double[nNode];
//	double *aMinimumV=new double[nNode];
//	for(int i=0;i<nNode;++i){
//		aMaximumV[i]=-DBL_MAX;
//	}
//	for(int i=0;i<nNode;++i){
//		aMinimumV[i]=DBL_MAX;
//	}
//	int iDimension=int(sqrt((double)nNode)+0.5);
//	for(int i=0;i<30000;++i){
//		if(i>20000&&i%20==0){
//			for(int j=0;j<nNode;++j){
//				if(aMaximumV[j]<pNode[j].V)
//					aMaximumV[j]=pNode[j].V;
//				if(aMinimumV[j]>pNode[j].V)
//					aMinimumV[j]=pNode[j].V;
//			}
//		}
//	
//		//Calculate...
//		pCouple->updateCouple(pCouplingCurrents);
//     	//__UpdateNoise();
//		_EulerIterate();
//	}
//	sprintf_s(filename,"%s\\%s_noise=%.5lf_MaximumV.dat",direct,specification,m_noiseintensity);
//	fopen_s(&pOutputSpiralWave,filename,"w");
//	for(int j=0;j<nNode;++j){
//		fprintf(pOutputSpiralWave,"%lf ",aMaximumV[j]);
//		if(j%iDimension==iColumn-1) fprintf(pOutputSpiralWave,"\n");
//	}
//	fprintf(pOutputSpiralWave,"\n");
//	fclose(pOutputSpiralWave);
//
//	sprintf_s(filename,"%s\\%s_noise=%.5lf_MinimumV.dat",direct,specification,m_noiseintensity);
//	fopen_s(&pOutputSpiralWave,filename,"w");
//	for(int j=0;j<nNode;++j){
//		fprintf(pOutputSpiralWave,"%lf ",aMinimumV[j]);
//		if(j%iDimension==iColumn-1) fprintf(pOutputSpiralWave,"\n");
//	}
//	fprintf(pOutputSpiralWave,"\n");
//	fclose(pOutputSpiralWave);
//
//	delete []direct;
//	delete []specification;
//	delete []aMaximumV;
//}



