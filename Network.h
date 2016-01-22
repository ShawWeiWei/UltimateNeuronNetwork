#pragma once

template <typename Node, template<typename> class CoupleType>
class Network{
//	friend class MorrisLecar;
public:
	explicit Network(CoupleType<Node> *_couple);
	/*Destructor*/
	virtual ~Network();    
	//Output network for verify
	void OutputCouple();
	void OutputCoupleAndPotential();
	void OutputCoupleIdx();
	void OutputI();
	void OutputNoByOneAndTwo();
	//	void OutputNoise();
	void OutputNoForOneAndTwo();
	void PrintTypeNum();
	//log
	void logInfo();
	//process
	void OutputTimeSeries();
	void SpiralWave(double _begin,double _end,double _dt);
	void SpiralWave();
	void SpiralWaveAndTimeSeries();
	void OutputSpikingIndex();
	void OutputPhaseAmplitude();
	void OutputAverISI();
	void OutputCV();
	void OutputAverISIForOneAndTwo();
	void OutputCVForOneAndTwo();
	//dive in
	void OutputCouplePerSpike();	
protected:
	
	void _EulerIterate();
	void _Rungekutta();
	void _SetDt(double _dt);

	void _MakePrefix();
	void _MakeFileName(char *filename,char *postfix);

private:
	const int nNode;
	CoupleType<Node> *pCouple;
	Node *pNode;
	double *pCouplingCurrents;
	double t;
	double dt;
	char *sCouple;
	char *sCon;
	char *sComposition;
	char *sSpecification;
	char *sPre;
	void __AlSyncVar();

	bool __IsInner(int _No);
	bool __IsML1(int _No);
	bool __IsML2(int _No);
};


