// poj 2559 
#include <cstdio>
#include <cstring>
#define rep(i,a,b) for(int i = (a); i <= (b); i++)
#define per(i,a,b) for(int i = (a); i >= (b); i--)
#define N 100005
#define max(a,b) ((a)>(b)?(a):(b))
using namespace std;
int n,s[N],st[N],root = 1,f,l;
long long ans; 
struct node{
	int v,l,fa,r;
	void clr(int x)
		{v = x;l = r = fa = 0;}
}tr[N];
void insert(int i)
{
	tr[i].clr(s[i]);
	if(s[i] < tr[root].v) 
	{
		st[l = 1] = i;
		tr[i].l = root;
		tr[root].fa = i;
		root = i;
		tr[root].fa = -1;
		return;
	}
	if(s[i] >= s[st[l]])
	{
		tr[i].fa = st[l];
		tr[st[l]].r = i;
		st[++l] = i;
		return;
	}
	while(l != 0 && s[st[l]] > s[i]) l--;
	tr[st[l]].r = i;
	tr[i].fa = st[l];
	tr[i].l = st[l+1];
	tr[st[l+1]].fa = i;
	st[++l] = i;
//	rep(i,1,l)
//	{
//		printf("%d\t",s[st[i]]);
//	}
//	printf("\n");
}
int pre_pos(int pos)
{
	int x = pos;
//	if(tr[x].fa == -1) return 0;
	while(x >= pos)
	{
		x = tr[x].fa;
//		printf("#%d\n",x);
		if(x == -1) return 0;
	}
	return x;
}
int after_pos(int pos)
{
	int x = pos;
//	if(tr[x].fa == -1) return n+1;
	while(x <= pos)
	{
		x = tr[x].fa;
//		printf("*%d\n",x);
		if(x == -1) return n+1;
	}
	return x;
}
void set()
{
	memset(s,0,sizeof(s));
	memset(tr,0,sizeof(tr));
	memset(st,0,sizeof(st));
	f = l = 1;
	root = 1;
	ans = 0;
}
void print(int root,int depth)
{
	if(tr[root].r) print(tr[root].r,depth+2);
	else 
	{
		rep(i,1,depth+2) printf("-");
		printf("null\n");
	}
	rep(i,1,depth)
		printf("-");
	printf("%d\n",tr[root].v);
	if(tr[root].l) print(tr[root].l,depth+2);
	else 
	{
		rep(i,1,depth+2) printf("-");
		printf("null\n");
	}
}
int main()
{
	scanf("%d",&n);
	while(n != 0)
	{
		set();
		rep(i,1,n)	scanf("%d",&s[i]);
		st[1] = 1;
		tr[root].v = s[1];
		tr[root].fa = -1;
		rep(i,2,n) 
		{
			insert(i);
//			printf("#%d\n",i);
//			print(root,2);
//			printf("----------------------------------\n");
		}
//		print(root,2); 
		rep(i,1,n)
		{
			int ll = pre_pos(i);
			int rr = after_pos(i);
			ll++;rr--;
//			printf("l = %d\tr = %d\n",ll,rr);
			ans = max(1ll*s[i]*(rr-ll+1),ans);
		}
		printf("%lld\n",ans);
		scanf("%d",&n);
	}
	return 0;
} 
