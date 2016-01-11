#include "stdafx.h"
#include "ML.h"
#include "Network.h"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <float.h>

class SquareNetwork;
const double MorrisLecar::C=20.0;
const double MorrisLecar::gK=8.0;
const double MorrisLecar::gL=2.0;
const double MorrisLecar::VK=-84.0;
const double MorrisLecar::VCa=120.0;
const double MorrisLecar::VL=-60.0;
const double MorrisLecar::V1=-1.2;
const double MorrisLecar::V2=18.0;

const double MorrisLecar::RESTORY_CURRENT_FOR_CLASS_I = 39.7;
const double MorrisLecar::FIRING_CURRENT_FOR_CLASS_I = 40;
const double MorrisLecar::RESTORY_CURRENT_FOR_CLASS_II = 88.1;
const double MorrisLecar::FIRING_CURRENT_FOR_CLASS_II = 90;

MorrisLecar::MorrisLecar(){
	SetDt(0.01);
}

MorrisLecar::~MorrisLecar(){}

void MorrisLecar::equation(){
		DV=(I-gL*(V-VL)-gCa*0.5*(1+tanh((V-V1)/V2))*(V-VCa)-gK*n*(V-VK))/C;
	    Dn=(0.5*(1+tanh((V-V3)/V4))-n)*phi*cosh((V-V3)/(2*V4));
}

/*void MorrisLecar::equation(){
	DV=(I-gL*(V-VL)-gCa*0.5*(1+tanh((V-V1)/V2))*(V-VCa)-gK*n*(V-VK)+Network::CoupleVector[No])/C+Network::NoiseVector[No];
	Dn=(0.5*(1+tanh((V-V3)/V4))-n)*phi*cosh((V-V3)/(2*V4));
}*/

void MorrisLecar::euler_s(){
	equation();
	V+=DV*dt;
	n+=Dn*dt;
}

void MorrisLecar::euler(double _couplingcurrent){
	/*if(_isnan(_couplingcurrent)||isfinite()){
			printf("What the fuck!\n");
	}*/
	/*if(_isnan(_couplingcurrent)||!_finite(_couplingcurrent)){
		printf("What the fuck!\n");
	}*/

	equation();
	//if(_isnan(DV)||_isnan(Dn)){
	//		printf("What the fuck!\n");
	//}
	V+=(DV+(_couplingcurrent/C))*dt;
	n+=Dn*dt;
	//if(_isnan(V)){
	//		printf("What the fuck!\n");
	//}
}


void MorrisLecar::rungekutta_s(){
		double DV1;
	double Dn1;
	double DV2;
	double Dn2;
	double DV3;
	double Dn3;
	double DV4;
	double Dn4;
		V0=V;
		n0=n;
		equation();
		DV1=DV*dt;
		Dn1=Dn*dt;
		V=V0+DV1/2.0;
		n=n0+Dn1/2.0;
		equation();
		DV2=DV*dt;
		Dn2=Dn*dt;
		V=V0+DV2/2.0;
		n=n0+Dn2/2.0;
		equation();
		DV3=DV*dt;
		Dn3=Dn*dt;
		V=V0+DV3;
		n=n0+Dn3;
		equation();
		DV4=DV*dt;
		Dn4=Dn*dt;
		V=V0+(DV1+2.0*DV2+2.0*DV3+DV4)/6.0;
		n=n0+(Dn1+2.0*Dn2+2.0*Dn3+Dn4)/6.0;
}

void MorrisLecar::rungekutta(double _couplingcurrent){
		double DV1;
	double Dn1;
	double DV2;
	double Dn2;
	double DV3;
	double Dn3;
	double DV4;
	double Dn4;
	 V0=V;
	n0=n;
	double CouplePlusNoise=_couplingcurrent/C;//+SquareNetwork::sm_gNoise[No];
	equation();
	DV1=(DV+CouplePlusNoise)*dt;
	Dn1=Dn*dt;
	V=V0+DV1/2.0;
	n=n0+Dn1/2.0;
	equation();
	DV2=(DV+CouplePlusNoise)*dt;
	Dn2=Dn*dt;
	V=V0+DV2/2.0;
	n=n0+Dn2/2.0;
	equation();
	DV3=(DV+CouplePlusNoise)*dt;
	Dn3=Dn*dt;
	V=V0+DV3;
	n=n0+Dn3;
	equation();
	DV4=(DV+CouplePlusNoise)*dt;
	Dn4=Dn*dt;
	V=V0+(DV1+2.0*DV2+2.0*DV3+DV4)/6.0;
	n=n0+(Dn1+2.0*Dn2+2.0*Dn3+Dn4)/6.0;
}


//void MorrisLecar::InitVar(){
//	V=4.583345;//-40.0;
//	n=0.29891089;//0.0;
//}

void MorrisLecar::RandVar(){
	V=-10+30.0*rand()/(double)RAND_MAX;
	n=0.1+0.5*rand()/(double)RAND_MAX;
}

void MorrisLecar::set_class1(){
	gCa=4.0;
	V3=12.0;
	V4=17.40;
	phi=1.0/15.0;
	strcpy_s(neuron_model,"MorrisLecar1");
}

void MorrisLecar::set_class2(){
	 gCa=4.4;
	 V3=2;
	 V4=30;
	 phi=0.04;
	 strcpy_s(neuron_model,"MorrisLecar2");
}


