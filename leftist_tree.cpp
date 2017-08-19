// poj 2887 
#include <cstdio>
#include <cstring>
#include <cmath>
#define rep(i,a,b) for(int i = (a); i <= (b); i++)
#define N 2005
using namespace std;
char Str[N*N>>2];
int lim,Len;
struct block_list{
	int head,tot;
	struct node{
		int nxt,len;
		char str[N];
		void clr()
		{
			memset(str,0,sizeof(str));
			nxt = -1;
			len = 0;
		}
	}list[N];
	void Build(char* s)
	{
		int pos = 1;
		head = tot = 1;
		list[tot].clr();
		for(int i = head; i != -1; i = list[i].nxt)
		{
			for(int j = 1; j <= lim && s[pos]; j++)
			{
				list[i].str[j] = s[pos];
				list[i].len++;
				pos++;
			}
//			printf("%d",pos);
			if(s[pos])
			{
				tot++;
				list[tot-1].nxt = tot;
				list[tot].clr();
			}
		}
//		printf("log1");
		for (int cur = head; cur != -1; cur = list[cur].nxt)
		{
			if(list[cur].len == lim) split(cur);
//			printf("%d",cur);
		}
	}
	void split(int x)
	{
		int pos = lim/2;
		list[++tot].clr();
		rep(i,pos+1,lim)
		{
			list[tot].str[i-pos] = list[x].str[i];
			list[x].str[i] = 0;
			list[tot].len++; list[x].len--;
		}
		list[tot].nxt = list[x].nxt;
		list[x].nxt = tot;
	}
	void insert(char ch,int x)
	{
		int pos = x;
		int cur = head;
		while(pos - list[cur].len > 0 && list[cur].nxt != -1)
		{
			pos -= list[cur].len;
			cur = list[cur].nxt;
		}
		if(pos > list[cur].len)
			list[cur].str[++list[cur].len] = ch;
		else {
			for(int i = list[cur].len+1; i >= pos; i--)
			{
				list[cur].str[i] = list[cur].str[i-1];
			}
			list[cur].str[pos] = ch;
			list[cur].len++;
		}
		if(list[cur].len == lim)
			split(cur);
	}
	char query(int x)
	{
		int pos = x;
		int cur = head;
		while(pos - list[cur].len > 0)
		{
			pos -= list[cur].len;
			cur = list[cur].nxt;
		}
		return list[cur].str[pos];
	}
	void print()
	{
		for(int cur = head; cur != -1; cur = list[cur].nxt)
		{
			rep(i,1,list[cur].len)
				printf ("%c",list[cur].str[i]);
			printf("\t");
		} 
		printf("\n");
	} 
}BL;
int n,tmp;
char ord,ch;
int main()
{
	scanf("%s",Str+1);
	Len = strlen(Str+1);
	lim = 1000;
	BL.Build(Str);
	scanf("%d\n",&n);
//	printf("##\t%d\t##\n",n); 
	rep(i,1,n)
	{
		scanf("%c",&ord);
// 		printf("##\t%c\t##\n",ord);
//		printf("###");
//		BL.print();
		getchar();
		if(ord == 'Q') 
		{
			scanf("%d\n",&tmp);
//			printf("##%d##\n",tmp);
			printf("%c\n",BL.query(tmp));
//			printf("############%c %d\n",ord,tmp);
		}
		if(ord == 'I')
		{
			scanf("%c%d\n",&ch,&tmp);
			BL.insert(ch,tmp);
//			printf("############%c %c %d\n",ord,ch,tmp);
		}
	}
	return 0;
} 
