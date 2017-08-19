#include <cstring>
#define rep(i,a,b) for(int i = (a); i <= (b); i++)
#define N 200105
// #define DEBUG
using namespace std;

int root,n,x,opt,tot,ans;
int tmp[N];
struct node{
    int l,r,fa,val,size,count;
}tr[N];

void update(int k)
{
    tr[k].size = 0;
    if(tr[k].l) tr[k].size += tr[tr[k].l].size;
    if(tr[k].r) tr[k].size += tr[tr[k].r].size;
    tr[k].size += tr[k].count;
    tr[k].size = tr[tr[k].l].size + tr[tr[k].r].size + tr[k].count;
}

// 左旋
