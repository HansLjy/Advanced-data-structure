/*
	Name : POJ2104
	Copyright : EZOI
	Author : ezoiljy
	Date : 2017/9/7
	Description : 可持久化线段树
	在线区间第K大
*/

#include <cstdio>
#include <algorithm>
#include <cstring>
#define rep(i, a, b) for(int i = (a); i <= (b); i++)
const int N = 100005;
using std::memset;
using std::sort;
using std::swap;
using std::max;
using std::min;

namespace Sustainable_Segment_tree
{
	// const int N = 100005;
	int rt[N];
	int tot = 0;
	struct node{
		int l, r, ch[2];
		int size;
	}tr[N * 20];

	void build(int& u, int l, int r)
	{
		u = ++tot;
		tr[u].l = l;
		tr[u].r = r;
		if(l == r) return;
		int mid = (l + r >> 1);
		build(tr[u].ch[0], l, mid);
		build(tr[u].ch[1], mid + 1, r);
	}

	void insert(int pre, int& u, int x)
	{
		u = ++tot;
		tr[u] = tr[pre];
		tr[u].size++;
		int mid = (tr[u].l + tr[u].r) >> 1;
		if(tr[u].l == tr[u].r) return;
		if(x <= mid) insert(tr[pre].ch[0], tr[u].ch[0], x);
		else insert(tr[pre].ch[1], tr[u].ch[1], x);
	}

	int query(int pre, int u, int x)
	{
		if(tr[u].l == tr[u].r)
			return tr[u].l;
		int sz = tr[tr[u].ch[0]].size - tr[tr[pre].ch[0]].size;
		if(sz >= x)	query(tr[pre].ch[0], tr[u].ch[0], x);
		else query(tr[pre].ch[1], tr[u].ch[1], x - sz);
	}

	void init()
	{
		memset(tr, 0, sizeof(tr));
	}
}
using namespace Sustainable_Segment_tree;

struct p{
	int id, x;
	bool operator < (const p& b) const{
		return x < b.x || (x == b.x && id < b.id);
	}
}S[N];

int main()
{
	int n, m, x, y, k;
	int a[N], rnk[N];
	scanf("%d%d", &n, &m);
	rep(i, 1, n)
	{
		scanf("%d", a + i);
		S[i].id = i;
		S[i].x = a[i];
	}
	sort(S + 1, S + 1 + n);
	rep(i, 1, n) rnk[S[i].id] = i;
	init();
	build(rt[0], 1, n);
	rep(i, 1, n) insert(rt[i - 1], rt[i], rnk[i]);
	rep(i, 1, m)
	{
		scanf("%d%d%d", &x, &y, &k);
		if(x > y) swap(x, y);
		printf("%d\n", S[query(rt[x - 1], rt[y], k)].x);
	}
	return 0;
}