#pragma once
#include <vector>
using namespace std;


template <typename Node>
class ExcitatoryCouple{
public:
	ExcitatoryCouple(int nNeuron,int pML1);
	~ExcitatoryCouple();
	void createConnection(char *type);
	void createConnection(char *type,double _rewiring);
	void setCouple(double _gc,double _threshold,double _V_syn);
	void makeFileComps(char *sCouple,char *sCon,char *sComposition,char *sSpecification);
	void updateCouple(double *pCouple);

	int getNum()  const{
		return nNode;
	}
	int getPTypeI() const{
		return pTypeI;
	}
	Node * getPNode ()const{
		return pNode;
	}
private:
	const int nNode;
	const int pTypeI;
	Node *pNode;
	char *Con;
	double gc;
	double threshold;
	double V_syn;
	vector<vector<int>> aExc;
};