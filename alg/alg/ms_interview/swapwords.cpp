/*
第10题（字符串）
翻转句子中单词的顺序。
题目：输入一个英文句子，翻转句子中单词的顺序，但单词内字符的顺序不变。

句子中单词以空格符隔开。为简单起见，标点符号和普通字母一样处理。
例如输入“I am a student.”，则输出“student. a am I”。
*/
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <assert.h>

void reverse(char const s[], char d[])
{
	char* pd=d+strlen(s);
	const char *p1=s;
	char const	*p2=strchr(p1,' ');
	
	*pd=0;
	for(;;)
	{
		int t;
		if(!p2)
		{
			t=strlen(p1);
			assert(pd-t>=0);
			memcpy(pd-t, p1, t);
			break;
		}
		else
		{
			t=p2-p1;
			assert(pd-t>=0);
			memcpy(pd-t, p1, t);
			pd -=t;
			
			//printf("COpy s %s n %d d %s pd %s dx %d\n", p1, t, d, pd, pd-d);
			
			p1=p2+1, p2=strchr(p1,' ');
			pd--;*pd=' ';
		}
			
		
	}
}

void reverse2(char const s[], char d[])
{
	int n=strlen(s);
	int s1=0,s2=0;	// start and end pointer
	int d1=n;
	
	d[d1]=0;
	for(;s2<n;)
	{
		if(s[s2]==' ')
		{
			//copy s1..s2 out
			d1 -=s2-s1;
			assert(d1>=0);
			memcpy(d+d1, s+s1,s2-s1);
			s1 =s2+1;
			
			d1--;
			d[d1]=' ';
			
			s2++;
		}
		else
			s2++;
	}
	
	if(s1<s2)
	{
		d1 -=s2-s1;
		assert(d1>=0);
		memcpy(d+d1, s+s1,s2-s1);
	}
}

void main()
{
	char a[]="I am a student.";
	char b[100];
	std::fill(b, b+100,' ');
	reverse2(a,b);
	printf("%s ", b);
}
