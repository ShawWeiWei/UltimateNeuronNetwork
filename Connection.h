#pragma once
#include <vector>
using namespace std;

void createRegularConnection(vector<vector<int>> &conVec,char *type);

void createRandomConnection(vector<vector<int>> &conVec,char *type,double _rewiring);

void buildSquare(int nNode,vector<vector<int>> &conVec);

void _buildSquareMatrix(bool ** relationMatrix,int node);

void buildSmallWorld(int nNode,double _rewiring,vector<vector<int>> &conVec);

void buildSparser(int nNode,double _rewiring,vector<vector<int>> &conVec);