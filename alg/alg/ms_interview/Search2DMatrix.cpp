/*
第17题（字符串）：
题目：在一个字符串中找到第一个只出现一次的字符。如输入abaccdeff，则输出b。 
分析：这道题是2006年google的一道笔试题。

*/
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <assert.h>

void 	init()
{
	
}

template <class T, class V>
T findLastLessthan(T begin, T end, V v)
{
	if(v
}

class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
		int h=matrix.size();
		if(h==0)return false;
		int w=matrix[0].size();
		if(w==0)return false;
       vector<int> itx1= findLastLessthan( matrix[0].begin(), matrix[0].end(), target)
	   if(*itx1==target) return true;
	   vector<int> itx2= findFirstLargerthan( matrix[n-1].begin(), matrix[n-1].end(), target);
	   if(*itx2==target) return true;
	   if(itx1>iter
    }
};

void main()
{
	init();
	
	printf("%c",c);
}
