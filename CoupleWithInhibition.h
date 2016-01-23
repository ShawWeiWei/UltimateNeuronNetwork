#pragma once

#pragma once
#include <vector>
using namespace std;


template <typename Node>
class CoupleWithInhibition{
public:
	CoupleWithInhibition(int nNeuron,int pML1,int pML2);
	~CoupleWithInhibition();
	void createNode();
	void setExcInh(const vector<vector<int>> &vec);
	void createConnection(char *type);
	void createConnection(char *type,double _rewiring);
	void setCouple(double _gc_exc,double _gc_inh,double _threshold,double _Vsyn_exc,double _Vsyn_inh);
	void updateCouple(double *pCouple);
	void outputDegreeDistribution(vector<int> &degree);

	//Verify
	void OutputConn();
	
	int getNum(){
		return nNode;
	}
	Node* getPNode(){
		return pNode;
	}
	void makeFileComps(char *sCouple,char *sCon,char *sComposition,char *sSpecification);
private:	
	enum NodeSyn {Excitory,Inhibitory,NoCoupling};
	const int nNode;
	const int pTypeI;
	const int pTypeII;
	Node *pNode;
	NodeSyn *pNodeSyn;
	bool * isCoupled;
	char * conn;
	double gc_exc;
	double gc_inh;
	double threshold;
	double Vsyn_exc;
	double Vsyn_inh;
	vector<vector<int>> aExc;
	vector<vector<int>> aInh;
};