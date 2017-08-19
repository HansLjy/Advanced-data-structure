// hdu 1512 
#include <cstdio>
#include <cstring>
#include <algorithm>
#define rep(i,a,b) for(int i = (a); i <= (b); i++)
#define max(a,b) ((a)>(b)?(a):(b))
#define N 100050
using namespace std;

int n,m,x,y,root1,root2,left,right,tmp;

struct Node{
    int lc,rc,val,dist,fa;
}tr[N];
int Merge(int h1,int h2)
{
    if(h2 == 0) return h1;
    if(h1 == 0) return h2;
    int x = h1,y = h2;
    if(tr[x].val < tr[y].val) swap(x,y);
    tr[x].rc = Merge(tr[x].rc,y);
    int &l = tr[x].lc,&r = tr[x].rc; // & is for the swap
    tr[l].fa = tr[r].fa = x;
    if(tr[l].dist < tr[r].dist) swap(l,r);
    if(r == 0) tr[x].dist = 0;
	else tr[x].dist = tr[r].dist+1;
    return x;
}
int Del(int root)
{
    int l = tr[root].lc;
    int r = tr[root].rc;
    tr[l].fa = l;
    tr[r].fa = r;  
    tr[root].lc = tr[root].rc = tr[root].dist = 0;
    return Merge(l,r);
}
int find(int x)
{
    int pos = x;
    while(tr[pos].fa != pos) 
    {
        pos = tr[pos].fa;
    }
    return pos;
}
void Build(int pos,int va)
{
    tr[pos].fa = pos;
    tr[pos].val = va;
    tr[pos].lc = tr[pos].rc = 0;
    tr[pos].dist = pos == 0?-1:0;
}
int main()
{
    while(scanf("%d",&n) == 1)
    {
    	memset(tr,0,sizeof(tr));
	    rep(i,1,n) 
	    {
	        scanf("%d",&tmp);
	        Build(i,tmp);
	    }
	    Build(0,0);
	    scanf("%d",&m);
	    rep(i,1,m)
	    {
	        scanf("%d%d",&x,&y);
	        root1 = find(x);
	        root2 = find(y);
	        if(root1 == root2) printf("-1\n");
	        else
	        {
	            tr[root1].val >>= 1;
	            tr[root2].val >>= 1;
	            left = Del(root1);
	            right = Del(root2);
	            left = Merge(left,root1);
	            right = Merge(right,root2);
	            tmp = Merge(left,right);
	            printf("%d\n",tr[tmp].val);
	        }
	    }
    }
    return 0;
}
