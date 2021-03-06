/*
第11题（树）
求二叉树中节点的最大距离...

如果我们把二叉树看成一个图，父子节点之间的连线看成是双向的，
我们姑且定义"距离"为两节点之间边的个数。
写一个程序，
求一棵二叉树中相距最远的两个节点之间的距离。
   10
  / /
  6  14
 / / / /
4  8 12 16

*/
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <assert.h>


struct TreeNode
{
	int val;
	TreeNode *lc, *rc;
};

TreeNode	g_pool[100];
int			g_nUsed =0;

void 	init()
{
	memset(g_pool, 0, sizeof(g_pool));
}

TreeNode	*getNode(int v)
{
	// 0 for NULL node
	if(v==0)
		return NULL;
		
	for(int i=0; i<g_nUsed; i++)
		if(v==g_pool[i].val)
			return &g_pool[i];
			
	if(g_nUsed>=100)
		return NULL;
		
	TreeNode *p= &g_pool[g_nUsed++];
	p->val =v;
	return p;
}

TreeNode	*connect(int parent, int lc, int rc)
{
	TreeNode *p =getNode(parent);
	TreeNode *pl =getNode(lc);
	TreeNode *pr =getNode(rc);
	p->lc =pl;
	p->rc=pr;
	return p;
}

void initTree(TreeNode **p)
{
	*p=connect(1,2,3);
	connect(2,4,5);
	connect(4,6,0);
	connect(5,0,7);
	connect(6,8,0);
	connect(7,0,9);
};

int max(int a, int b)
{
	return a>b?a:b;
}

int maxDist(TreeNode const *p, int *deptho)
{
	if(!p) {*deptho=0; return 0;}
	if(!p->lc && !p->rc){*deptho=1; return 1;}
	
	int depth[2]={0};
	int d1=maxDist(p->lc, depth);
	int d2=maxDist(p->rc, depth+1);
	*deptho =max(depth[0], depth[1])+1;
	return max(
		max(d1, d2), depth[0]+depth[1]
	);
}

void main()
{
	init();
	
	TreeNode *p=0;
	initTree(&p);
	
	int depth=0;
	int dist=0;
	dist =maxDist(p,&depth);
	printf("%d %d", dist, depth);
}
