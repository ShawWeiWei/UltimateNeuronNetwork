#pragma once
#include <vector>
using namespace std;

template <typename Node>
class ExcitatoryCoupleWithNoise{
public:
	ExcitatoryCoupleWithNoise(int nNeuron,int pML1,bool isRest);
	~ExcitatoryCoupleWithNoise();
	void createConnection(char *type);
	void createConnection(char *type,double _rewiring);
	void setCouple(double _gc,double _threshold,double _V_syn);
	void setNoise(double _noise_intensity){
		noise_intensity=_noise_intensity;
	}
	void makeFileComps(char *sCouple,char *sCon,char *sComposition,char *sSpecification);
	void updateCouple(double *pCouple);
	void updatenoise();
	void outputDegreeDistribution(vector<int> &degree);


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
	double noise_intensity;
	double *noise;
};