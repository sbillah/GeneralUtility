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

int min(int a, int b)
{
	return a<b?a:b;
}

int find(int n, char const a[])
{
	const int ARRAYSZ='z'-'a';
	int hist[ARRAYSZ]={0};
	int a1sttime[ARRAYSZ];
	std::fill(a1sttime, a1sttime+ARRAYSZ,-1);
	
	for(int i=0; i<n; i++)
	{
		int idx=a[i]-'a';
		if(hist[idx]==0)
		{
			assert(a1sttime[idx]<0);
			a1sttime[idx] =i;
		}
		hist[idx]++;
	}
	
	int minIdx=ARRAYSZ;
	for(int i=0; i<n; i++)
	{
		if(hist[i]==1)
		{
			minIdx =min(minIdx, a1sttime[i]);
		}
	}
	return minIdx+'a';
}

void main()
{
	init();
	char a[]="abaccdeff";
	
	char c=find(sizeof(a)/sizeof(a[0]),a);
	printf("%c",c);
}
