#include "stdafx.h"
#include "ArrangeNumber.h"

void shuffleNumber(vector<int> &nums, int size){
	nums.resize(size);
	for(int i=0;i<size;++i){
		nums[i]=i;
	}
	random_shuffle(nums.begin(),nums.end());
}