#pragma once
#include <vector>
using namespace std;


void buildSquare(int nNode,vector<vector<int>> &aCon);

void buildSmallWorld(int nNode,double _rewiring,vector<vector<int>> &aCon);

void buildSparser(int nNode,double _rewiring,vector<vector<int>> &aCon);