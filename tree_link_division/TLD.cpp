/*
	Name: POJ 2763
	Copyright:EZOI
	Author:ezoiljy
	Date :2017/8/22
	Description: 树链剖分
	要求支持单边修改与区间查询
	<del>这难道不是LCA练习题吗?</del>
*/

#include <cstdio>
#include <algorithm>
#include <cstring>
#define rep(i, a, b) for(int i = (a); i <= (b); i++)
using std::memset;
using std::sort; 
using std::max;
using std::min;
using std::swap;

namespace graph
{
	const int N = 100005;
	int cp = 1;
	int head[N], nxt[N << 1], to[N << 1], val[N << 1];
	void addline(int u, int v, int weight)
	{
		to[++cp] = v; nxt[cp] = head[u]; head[u] = cp; val[cp] = weight;
		to[++cp] = u; nxt[cp] = head[v]; head[v] = cp; val[cp] = weight;
	}
}
using namespace graph;

namespace chain_division
{
	const int N = 100005;
	int tot = -1;
	int size[N], pos[N], fa[N], dep[N], son[N], top[N], w[N];

	namespace segment_tree
	{
		// 线段树长度：tot, 根节点：1
		struct node{
			int sum, to_add;
		}tr[N << 2];

		void build(int rt, int l, int r)
		{
			if(l == r)
			{
				tr[rt].sum = w[l];
				return;
			}
			int mid = (l + r >> 1), lc = rt << 1, rc = lc + 1;
			build(lc, l, mid);
			build(rc, mid + 1, r);
			tr[rt].sum = tr[lc].sum + tr[rc].sum;
		}

		void modify(int rt, int l_lim, int r_lim, int l, int r, int new_val)
		{
			if(l_lim == r_lim)
			{
				tr[rt].sum = new_val;
				return;
			}
			int mid = (l_lim + r_lim >> 1), lc = rt << 1, rc = lc + 1;
			if(r <= mid) modify(lc, l_lim, mid, l, r, new_val);
			else if(l > mid) modify(rc, mid + 1, r_lim, l, r, new_val);
			else
			{
				modify(lc, l_lim, mid, l, mid, new_val);
				modify(rc, mid + 1, r_lim, mid + 1, r ,new_val);
			}
			tr[rt].sum = tr[lc].sum + tr[rc].sum;
		}

		int query(int rt, int l_lim, int r_lim, int l, int r)
		{
			if(l_lim == l && r_lim == r)
				return tr[rt].sum;
			int mid = (l_lim + r_lim >> 1), lc = rt << 1, rc = lc + 1;
			if(r <= mid) return query(lc, l_lim, mid, l, r);
			else if(l > mid) return query(rc, mid + 1, r_lim, l ,r);
			else return query(lc, l_lim, mid, l, mid) + query(rc, mid + 1, r_lim, mid + 1, r);
		}
	}
	
	void reset()
	{
		tot = -1;
	}
	
	void dfs_1(int u, int father)
	{
		fa[u] = father;
		size[u] = 1;
		son[u] = 0;
		int v;
		for(int i = head[u]; i != -1; i = nxt[i])
		{
			v = to[i];
			if(v == father) continue;
			dep[v] = dep[u] + 1;
			dfs_1(v, u);
			if(son[u] == 0 || size[v] > size[son[u]])
				son[u] = v;
			size[u] += size[v];
		}
	}

	void dfs_2(int u, int tp)
	{
		int v;
		pos[u] = ++tot;
		top[u] = tp;
		if(son[u]) dfs_2(son[u], tp);
		for(int i = head[u]; i != -1; i = nxt[i])
		{
			v = to[i];
			if(v == fa[u]) continue;
			if(son[u] != v) dfs_2(v, v);
		}
	}

	void build(int rt) //指定根节点建树
	{
		dep[rt] = 0;
		dfs_1(rt, -1);
		dfs_2(rt, rt);
		for(int i = 2; i <= cp; i += 2)
		{
			if(dep[to[i]] > dep[to[i ^ 1]])
				w[pos[to[i]]] = val[i];
			else w[pos[to[i ^ 1]]] = val[i];
		}
		segment_tree::build(1, 1, tot);
	}

	void modify(int k, int new_val)
	{
		k = 2 * k; 
		if(dep[to[k]] < dep[to[k ^ 1]])
			segment_tree::modify(1, 1, tot, pos[to[k ^ 1]], pos[to[k ^ 1]], new_val);
		else segment_tree::modify(1, 1, tot, pos[to[k]], pos[to[k]], new_val);
	}

	int query(int u, int v)
	{
		int f1 = top[u], f2 = top[v], ans = 0;
		while(f1 != f2)
		{
			if(dep[f1] < dep[f2])
			{
				swap(f1, f2);
				swap(u, v);
			}
			ans += segment_tree::query(1, 1, tot, pos[f1], pos[u]);
			u = fa[f1];
			f1 = top[u];
		}
		if(u == v) return ans;
		if(dep[u] < dep[v]) swap(u, v);
		ans += segment_tree::query(1, 1, tot, pos[son[v]], pos[u]);
		return ans;
	}

	void print(int n)
	{
		rep(i, 1, n)
		{
			printf("%d : size = %d, dep = %d, top = %d, son = %d, pos = %d\n",
				i, size[i], dep[i], top[i], son[i], pos[i]);
		}
	}
}

int main()
{
	int n, q, s, a, b, weight, k, new_val, opt;
	while(scanf("%d%d%d", &n, &q, &s) == 3)
	{
		cp = 1;
		memset(head, -1, sizeof(head));
		chain_division::reset();
		int cur = s;
		rep(i, 1, n - 1)
		{
			scanf("%d%d%d", &a, &b, &weight);
			addline(a, b, weight);
		}
		chain_division::build(1);
		rep(i, 1, q)
		{
			scanf("%d", &opt);
			switch(opt)
			{
				case 0 :
					scanf("%d", &a);
					printf("%d\n", chain_division::query(cur, a));
					cur = a;
					break;
				case 1 :
					scanf("%d%d", &k, &new_val);
					chain_division::modify(k, new_val);
					break;
			}
		}
	} 
	return 0;
}
