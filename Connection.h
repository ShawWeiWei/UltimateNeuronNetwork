#pragma once
#include <vector>
using namespace std;

void createRegularConnection(int node,vector<vector<int>> &conVec,char *conSpec,char *type);

void createRandomConnection(int node,vector<vector<int>> &conVec,char *conSpec,char *type,double _rewiring);

void buildSquare(int nNode,vector<vector<int>> &conVec);

void _buildSquareMatrix(bool ** relationMatrix,int node);

void buildSmallWorld(int nNode,double _rewiring,vector<vector<int>> &conVec);

void buildSparser(int nNode,double _rewiring,vector<vector<int>> &conVec);