/*
第14题（数组）：
题目：输入一个已经按升序排序过的数组和一个数字，
在数组中查找两个数，使得它们的和正好是输入的那个数字。
要求时间复杂度是O(n)。如果有多对数字的和等于输入的数字，输出任意一对即可。
例如输入数组1,2,4,7,11,15和数字15。由于4+11=15，因此输出4和11。

*/
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <assert.h>

void 	init()
{
	
}

void find(int n, int const a[], int sum)
{
	for(int p1=0,p2=n-1; p1<p2; )
	{
		int t=a[p1]+a[p2];
		if(t==sum)
		{
			printf("%d %d",a[p1],a[p2]);
			return;
		}
		else if(t<sum)
			p1++;
		else
		{
			p2--;
		}
	}
}

void main()
{
	init();
	int a[]={2,4,7,11,15};
	
	find(sizeof(a)/sizeof(a[0]),a,15);
}
