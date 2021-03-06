/*判断整数序列是不是二元查找树的后序遍历结果
题目：输入一个整数数组，判断该数组是不是某二元查找树的后序遍历的结果。
如果是返回true，否则返回false。

例如输入5,7,6,9,11,10,8，由于这一整数序列是如下树的后序遍历结果：

         8
      /  /
     6    10
    / /  / /
   5  7 9  11
因此返回true。
如果输入7,4,6,5，没有哪棵树的后序遍历的结果是这个序列，因此返回false。*/
//#include <algorithm>
#include <stdio.h>
#include <assert.h>

bool isPostOrder(int n, const int a[])
{
	if(n==0 || n==1) return true;
	if(n==2) return true;
	if(n==3) return a[0]<a[2] && a[2]<a[1];
	
	int t=a[n-1];
	int i=0;
	for(; i<n-1 && a[i]<t; i++);
	
	return isPostOrder(i, a) && isPostOrder(n-i-1, a+i);
}

void main()
{
	{
	int a[]={5,7,6,9,11,10,8};
	int n=sizeof(a)/sizeof(a[0]);
	printf("%d ", isPostOrder(n,a));
	}
	
	{
	int a[]={7,4,6,5};
	int n=sizeof(a)/sizeof(a[0]);
	printf("%d ", isPostOrder(n,a));
	}
}
