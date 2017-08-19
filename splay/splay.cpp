/* LJOJ 1332 */ 
#include <cstdio>
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
void zig(int k)
{
    int fa = tr[k].fa;
    tr[fa].l = tr[k].r;
    if(tr[k].r) tr[tr[k].r].fa = fa;
    tr[k].fa = tr[fa].fa;
    if(tr[fa].fa)
    {
        if(tr[tr[fa].fa].l == fa)
            tr[tr[fa].fa].l = k;
        else tr[tr[fa].fa].r = k;
    }
    tr[k].r = fa;
    tr[fa].fa = k;
    tr[k].size = tr[fa].size;
    update(fa);
    if(fa == root) root = k;
}

// 右旋
void zag(int k)
{
    int fa = tr[k].fa;
    tr[fa].r = tr[k].l;
    if(tr[k].l) tr[tr[k].l].fa = fa;
    tr[k].fa = tr[fa].fa;
    if(tr[fa].fa != 0)
    {
        if(tr[tr[fa].fa].l == fa)
            tr[tr[fa].fa].l = k;
        else tr[tr[fa].fa].r = k;
    }
    tr[k].l = fa;
    tr[fa].fa = k;
    tr[k].size = tr[fa].size;
    update(fa);
    if(fa == root) root = k;
}

// 旋转到根
void splay(int k,int end)
{
    bool tmp1,tmp2;
    int fa = tr[k].fa;
    int des_fa = tr[end].fa;
    while(fa != des_fa && fa != end)
    {
        tmp1 = (tr[tr[fa].fa].l == fa);
        tmp2 = (tr[fa].l == k);
        if(tmp1)
        {
            if(tmp2) {zig(fa);zig(k);}
            else {zag(k);zig(k);}
        }
        else
        {
            if(tmp2) {zig(k);zag(k);}
            else {zag(fa);zag(k);}
        }
        fa = tr[k].fa;
    }
    if(fa != des_fa)
    {
        if(tr[fa].l == k) zig(k);
        else zag(k);
    }
    if(root == end) root = k;
}

//新生成一个节点
void set(int& rt,int x,int fa)
{
    rt = ++tot;
    tr[rt].val = x;
    tr[rt].fa = fa;
    tr[rt].l = tr[rt].r = 0;
    tr[rt].size = tr[rt].count = 1;
    splay(rt,root);
}

//插入一个数
void insert(int& rt,int x,int fa)
{
    if(rt == 0)
    {
        set(rt,x,fa);
        return;
    }
    tr[rt].size++;
    if(tr[rt].val < x)
        insert(tr[rt].r,x,rt);
    else if(tr[rt].val > x)
        insert(tr[rt].l,x,rt);
    else tr[rt].count++;
    return;
}

// 查询第k大，返回树上编号
int query_kth(int rt,int k)
{
    if(tr[rt].l != 0 && tr[tr[rt].l].size >= k)
        return query_kth(tr[rt].l,k);
    else if(((tr[rt].l ? tr[tr[rt].l].size : 0) + tr[rt].count) < k && tr[rt].r)
        return query_kth(tr[rt].r, k - ((tr[rt].l ? tr[tr[rt].l].size : 0) + tr[rt].count));
    else return rt;
}

//按值删除
void del(int x)
{
	int pos = root;
    while(tr[pos].val != x)
    {
        if(x < tr[pos].val) pos = tr[pos].l;
        if(x > tr[pos].val) pos = tr[pos].r;
    }
    splay(pos,root);
    pos = root;
    if(tr[pos].count > 1)
    {
        tr[pos].size--;
        tr[pos].count--;
        return;
    }
    if(tr[root].l)
    {
        splay(query_kth(tr[root].l,tr[tr[root].l].size),tr[root].l);
        tr[tr[root].l].r = tr[root].r;
        tr[tr[root].l].size += (tr[root].r ? tr[tr[root].r].size : 0);
        tr[tr[root].r].fa = tr[root].l;
        tr[tr[root].l].fa = 0;
        root = tr[root].l;
    }
    else root = tr[root].r;
    return;
}

//询问排名，返回排名
int query_rnk(int rt, int more_number, int x)
{
    if(tr[rt].val > x)
        return query_rnk(tr[rt].l, more_number, x);
    else if(tr[rt].val < x)
        return query_rnk(tr[rt].r, more_number + tr[rt].count
            + (tr[rt].l ? tr[tr[rt].l].size : 0), x);
    else return (tr[rt].l ? tr[tr[rt].l].size : 0)+ more_number + 1;
}

int lower_bound(int rt, int x)
{
    int pos = rt;
    int ans;
    while(true)
    {
        bool flag = false;
        if(tr[pos].val >= x)
        {
            if(tr[pos].l != 0)
            {
                pos = tr[pos].l;
                flag = true;
            }
        }
        else
        {
            ans = tr[pos].val;
            if(tr[pos].r)
            {
                pos = tr[pos].r;
                flag = true;
            }
        }
        if(flag == false)
            break;
    }
    return ans;
}

int upper_bound(int rt, int x)
{
    int pos = rt;
    int ans;
    while(true)
    {
        bool flag = false;
        if(tr[pos].val <= x)
        {
            if(tr[pos].r != 0)
            {
                pos = tr[pos].r;
                flag = true;
            }
        }
        else
        {
            ans = tr[pos].val;
            if(tr[pos].l)
            {
                pos = tr[pos].l;
                flag = true;
            }
        }
        if(flag == false)
            break;
    }
    return ans;
}

#ifdef DEBUG
void print(int step,int rt)
{
    if(tr[rt].r) print(step+2,tr[rt].r);
    rep(i,1,step) printf("-");
    printf(" %d \n",tr[rt].val);
    if(tr[rt].l) print(step+2,tr[rt].l);
}
#endif

int main()
{
    scanf("%d", &n);
    memset(tr, 0, sizeof(tr));
    int tmp_cnt = 0;
    rep(i,1,n)
    {
        scanf("%d%d", &opt, &x);
        switch(opt)
        {
            case 1: insert(root, x, 0); break;
            case 2: del(x); break;
            case 3: printf("%d\n", query_rnk(root, 0, x));  tmp_cnt++; break;
            case 4: printf("%d\n", tr[query_kth(root, x)].val); tmp_cnt++; break;
            case 5: printf("%d\n",lower_bound(root, x)); tmp_cnt++; break;
            case 6: printf("%d\n",upper_bound(root, x)); tmp_cnt++; break;
#ifdef DEBUG
            case 7: print(2, root); break;
#endif
        }
#ifdef DEBUG
   	printf("******************root = %d******************\n",root);
   	rep(i,1,tot) printf("%d : val = %d, fa = %d, l = %d, r = %d, size = %d, cnt = %d\n",
			i, tr[i].val, tr[i].fa, tr[i].l, tr[i].r, tr[i].size, tr[i].count);
#endif
    }
    return 0;
}
