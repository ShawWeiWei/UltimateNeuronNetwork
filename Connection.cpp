#include "stdafx.h"
#include "Connection.h"
#include <vector>
using namespace std;


void buildSquare(int nNode,vector<vector<int>> &aCon){
//	sprintf_s(m_sName,40,"Square");
	aCon.clear();
	vector<int> vec;
	for(int i=0;i<nNode;++i){
		aCon.push_back(vec);
	}
	int iRow,iColumn;
	int iDimension=sqrt(nNode)+0.5;
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

void buildSmallWorld(int nNode,double _rewiring,vector<vector<int>> &aCon){
//	sprintf_s(m_sName,40,"SmallWorld_%.2f",_rewiring);
	aCon.clear();
	vector<int> vec;
	for(int i=0;i<nNode;++i){
		aCon.push_back(vec);
	}

	int nDimension=sqrt(nNode)+0.5;
	bool **RelationMatrix=new bool*[nNode];
	RelationMatrix[0]=new bool[nNode*nNode];
	for(int i=1;i<nNode;++i){
		RelationMatrix[i]=RelationMatrix[i-1]+nNode;
	}
	memset(RelationMatrix[0],0,sizeof(bool)*nNode*nNode);
	int iRow,iColumn;
	for(int i=0;i<nNode;++i){
		iRow=i/nDimension;
		iColumn=i%nDimension;
		//up
		if(iRow>0){
			RelationMatrix[i][(iRow-1)*nDimension+iColumn]=true;
			RelationMatrix[(iRow-1)*nDimension+iColumn][i]=true;
		}

		//left
		if(iColumn>0){
			RelationMatrix[i][iRow*nDimension+iColumn-1]=true;
			RelationMatrix[iRow*nDimension+iColumn-1][i]=true;
		}

		//right
		if(iColumn<nDimension-1){
			RelationMatrix[i][iRow*nDimension+iColumn+1]=true;
			RelationMatrix[iRow*nDimension+iColumn+1][i]=true;
		}

		//down
		if(iRow<nDimension-1){
			RelationMatrix[i][(iRow+1)*nDimension+iColumn]=true;
			RelationMatrix[(iRow+1)*nDimension+iColumn][i]=true;
		}
	}

	srand((unsigned int)100);
	int iElement;
	for(int i=0;i<nNode;++i){
		for(int j=i+1;j<nNode;++j){
			if(RelationMatrix[i][j]&&(rand()/(double)RAND_MAX)<_rewiring){
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
	delete []RelationMatrix[0];
	delete []RelationMatrix;
}

void buildSparser(int nNode,double _rewiring,vector<vector<int>> &aCon){
//	sprintf_s(m_sName,40,"SmallWorld_%.2f",_rewiring);
	aCon.clear();
	vector<int> vec;
	for(int i=0;i<nNode;++i){
		aCon.push_back(vec);
	}

	int nDimension=sqrt(nNode)+0.5;
	bool **RelationMatrix=new bool*[nNode];
	RelationMatrix[0]=new bool[nNode*nNode];
	for(int i=1;i<nNode;++i){
		RelationMatrix[i]=RelationMatrix[i-1]+nNode;
	}
	memset(RelationMatrix[0],0,sizeof(bool)*nNode*nNode);
	int iRow,iColumn;
	for(int i=0;i<nNode;++i){
		iRow=i/nDimension;
		iColumn=i%nDimension;
		//up
		if(iRow>0){
			RelationMatrix[i][(iRow-1)*nDimension+iColumn]=true;
			RelationMatrix[(iRow-1)*nDimension+iColumn][i]=true;
		}

		//left
		if(iColumn>0){
			RelationMatrix[i][iRow*nDimension+iColumn-1]=true;
			RelationMatrix[iRow*nDimension+iColumn-1][i]=true;
		}

		//right
		if(iColumn<nDimension-1){
			RelationMatrix[i][iRow*nDimension+iColumn+1]=true;
			RelationMatrix[iRow*nDimension+iColumn+1][i]=true;
		}

		//down
		if(iRow<nDimension-1){
			RelationMatrix[i][(iRow+1)*nDimension+iColumn]=true;
			RelationMatrix[(iRow+1)*nDimension+iColumn][i]=true;
		}
	}

	srand((unsigned int)100);
	int iElement;
	for(int i=0;i<nNode;++i){
		for(int j=i+1;j<nNode;++j){
			if(RelationMatrix[i][j]&&(rand()/(double)RAND_MAX)<_rewiring){
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
	delete []RelationMatrix[0];
	delete []RelationMatrix;
}