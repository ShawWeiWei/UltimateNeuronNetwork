#include "stdafx.h"
#include "Connection.h"
#include "utils.h"
#include <vector>
using namespace std;

void createRegularConnection(int node,vector<vector<int>> &conVec,char *conSpec,char *type){
	if(0==strcmp("Square",type)){
		buildSquare(node,conVec);
		sprintf_s(conSpec,20,"%s","Square");
	}
	else{
		throw("wrong input");
	}
}

void createRandomConnection(int node,vector<vector<int>> &conVec,char *conSpec,char *type,double _rewiring){
	if(0==strcmp("SmallWorld",type)){
		buildSmallWorld(node,_rewiring,conVec);
		sprintf_s(conSpec,20,"%s_%.5lf",type,_rewiring);
	}
	else if(0==strcmp("Sparser",type)){
		buildSparser(node,_rewiring,conVec);
		sprintf_s(conSpec,20,"%s_%.5lf",type,_rewiring);
	}
	else{
		throw("Invalid input!");
	}
}

void buildSquare(int nNode,vector<vector<int>> &aCon){
	aCon.resize(nNode);
	int iRow,iColumn;
	int iDimension=sqrt_int(nNode);

	int nTotal;

	for(int i=0;i<nNode;++i){
		iRow=i/iDimension;
		iColumn=i%iDimension;
        //up
		if((iRow-1)>=0){
			int index=(iRow-1)*iDimension+iColumn;
			aCon[i].push_back(index);
		}

		//left
		if((iColumn-1)>=0){
			int index=iRow*iDimension+iColumn-1;
			aCon[i].push_back(index);
		}

		//down
		if((iRow+1)<iDimension){
			int index=(iRow+1)*iDimension+iColumn;
			aCon[i].push_back(index);
		}

		//right
		if((iColumn+1)<iDimension){
			int index=iRow*iDimension+iColumn+1;
			aCon[i].push_back(index);
		}
	}
}

void _buildSquareMatrix(bool ** RelationMatrix,int node){
	int dimension=sqrt_int(node);
	int row,column;
	for(int i=0;i<node;++i){
		row=i/dimension;
		column=i%dimension;
		//up
		if(row>0){
			RelationMatrix[i][(row-1)*dimension+column]=true;
			RelationMatrix[(row-1)*dimension+column][i]=true;
		}

		//left
		if(column>0){
			RelationMatrix[i][row*dimension+column-1]=true;
			RelationMatrix[row*dimension+column-1][i]=true;
		}

		//right
		if(column<dimension-1){
			RelationMatrix[i][row*dimension+column+1]=true;
			RelationMatrix[row*dimension+column+1][i]=true;
		}

		//down
		if(row<dimension-1){
			RelationMatrix[i][(row+1)*dimension+column]=true;
			RelationMatrix[(row+1)*dimension+column][i]=true;
		}
	}
}

void buildSmallWorld(int nNode,double _rewiring,vector<vector<int>> &aCon){
	aCon.resize(nNode);
	int nDimension=sqrt_int(nNode);
	
	bool **RelationMatrix=NULL;
	create2dvector(&RelationMatrix,nNode,nNode);
	_buildSquareMatrix(RelationMatrix,nNode);
	
	srand((unsigned int)100);
	int iElement;
	for(int i=0;i<nNode;++i){
		for(int j=i+1;j<nNode;++j){
			if(RelationMatrix[i][j]&&(_rewiring>Uniform_01())){
				RelationMatrix[i][j]=false;
				RelationMatrix[j][i]=false;
				while(1){
					iElement=rand()%nNode;
					if(RelationMatrix[i][iElement]==false){
						if((iElement!=i)&&(iElement!=j)){
							break;
						}
					}
				}
				RelationMatrix[i][iElement]=true;
				RelationMatrix[iElement][i]=true;
			}
		}
	}
	
	for(int i=0;i<nNode;++i){
		for(int j=0;j<nNode;++j){
			if(RelationMatrix[i][j]){
				aCon[i].push_back(j);
			}
		}
	}
	delete2dvector(RelationMatrix);
}

void buildSparser(int nNode,double _rewiring,vector<vector<int>> &aCon){
	aCon.resize(nNode);
	int nDimension=sqrt_int(nNode);

	bool **RelationMatrix=NULL;
	create2dvector(&RelationMatrix,nNode,nNode);
	_buildSquareMatrix(RelationMatrix,nNode);

	srand((unsigned int)100);

	for(int i=0;i<nNode;++i){
		for(int j=i+1;j<nNode;++j){
			if(RelationMatrix[i][j]&&(_rewiring>Uniform_01())){
				RelationMatrix[i][j]=false;
				RelationMatrix[j][i]=false;
			}
		}
	}
	
	for(int i=0;i<nNode;++i){
		for(int j=0;j<nNode;++j){
			if(RelationMatrix[i][j]){
				aCon[i].push_back(j);
			}
		}
	}
	delete2dvector(RelationMatrix);
}