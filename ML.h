#pragma once
#include "Network.h"
#include "ExcitatoryCouple.h"
class MorrisLecar{
//friend class Network<MorrisLecar,ExcitatoryCouple<MorrisLecar>>;
public:

	MorrisLecar(); 

	~MorrisLecar();

	void SetI(double _I){
		I=_I;
	}
	double GetI() const{
		return I;
	}
	void SetDt(double _dt){
		dt=_dt;
	}
//	void equation_s();
	void euler_s();
	void rungekutta_s();

	void equation();
	void euler(double _couplingcurrent);
	void rungekutta(double _couplingcurrent);

    void InitVar();
	void RandVar();

	void set_class1();
	void set_class2();
public:
	double V;
	char neuron_model[15];
	static const double RESTORY_CURRENT_FOR_CLASS_I;
	static const double RESTORY_CURRENT_FOR_CLASS_II;
	static const double FIRING_CURRENT_FOR_CLASS_I;
	static const double FIRING_CURRENT_FOR_CLASS_II;
private:	

	double n;
	/*parameters*/
	double gCa;
	double phi;
	double V3;
	double V4;
	double I;

	/*intermediate variables for equations*/
	double DV;
	double Dn;
	
	/*intermediate variables for updating V and n*/
	double V0;
	double n0;

	/*intermediate variables for rungekutta method*/


	/*variables to compare to determine spike time*/
	/*double V_pre;
	double V_pre1;*/

	double dt;
	
};
