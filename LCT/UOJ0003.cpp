/*
	http://www.lydsy.com/JudgeOnline/problem.php?id=3669
	Name : BZOJ3669
	Copyright : EZOI
	Author : ezoiljy
	Date : 2017/9/6
	Description : LCT
	维护两点之间min动态加边
*/
#include <bits/stdc++.h>
#define rep(i, a, b) for(int i = (a); i <= (b); i++)
using std::sort;
using std::max;
using std::min;
using std::swap;
const int INF = (1 << 30);

namespace graph
{
	const int N = 100005;
	struct Edge
	{
		int x, y, a, b;
		bool operator < (const Edge B) const
		{
			return a < B.a;
		}
	}E[N];
}
using namespace graph;

namespace LCT
{
	const int N = 300005;
	struct node{
		int mx, fa, ch[2], val;
		bool rev;
	}tr[N];
	int stk[N];

	bool is_root(int x)
	{
		return tr[x].fa == 0 || (tr[tr[x].fa].ch[0] != x && tr[tr[x].fa].ch[1] != x);
	}

	void pushdown(int x)
	{
		if(!x) return;
		if(tr[x].rev)
		{
			swap(tr[x].ch[0], tr[x].ch[1]);
			if(tr[x].ch[0]) tr[tr[x].ch[0]].rev ^= 1;
			if(tr[x].ch[1]) tr[tr[x].ch[1]].rev ^= 1;
			tr[x].rev = 0;
		}
	}

	void update(int x)
	{
		if(!x) return;
		pushdown(tr[x].ch[0]);
		pushdown(tr[x].ch[1]);
		tr[x].mx = x;
		if(tr[x].ch[0] && tr[tr[x].mx].val < tr[tr[tr[x].ch[0]].mx].val)
			tr[x].mx = tr[tr[x].ch[0]].mx;
		if(tr[x].ch[1] && tr[tr[x].mx].val < tr[tr[tr[x].ch[1]].mx].val)
			tr[x].mx = tr[tr[x].ch[1]].mx;
	}

	void rotate(int x, int id)
	{
		int y = tr[x].fa, z = tr[y].fa;
		if(!is_root(y))
		{
			if(tr[z].ch[0] == y) tr[z].ch[0] = x;
			else tr[z].ch[1] = x;
		}
		tr[y].ch[id] = tr[x].ch[id ^ 1];
		if(tr[x].ch[id ^ 1])
			tr[tr[x].ch[id ^ 1]].fa = y;
		tr[x].ch[id ^ 1] = y;
		tr[x].fa = z;
		tr[y].fa = x;
		update(y);
	}

	void splay(int x)
	{
		int pos = x, tot = 0;
		while(!is_root(pos))
		{
			stk[++tot] = pos;
			pos = tr[pos].fa;
		}
		stk[++tot] = pos;
		while(tot)
			pushdown(stk[tot--]);
		while(!is_root(x))
		{
			int y = tr[x].fa, z = tr[y].fa;
			if(tr[y].ch[0] == x)
			{
				if(tr[z].ch[0] == y) rotate(y, 0);
				rotate(x, 0);
			}
			else
			{
				if(tr[z].ch[1] == y) rotate(y, 1);
				rotate(x, 1);
			}
		}
		update(x);
	}

	void access(int x)
	{
		int tmp = 0;
		while(x)
		{
			splay(x);
			tr[x].ch[1] = tmp;
			update(x);
			tmp = x;
			x = tr[x].fa;
		}
	}

	void make_root(int x)
	{
		access(x);
		splay(x);
		tr[x].rev ^= 1;
	}

	void link(int x, int y)
	{
		make_root(x);
		tr[x].fa = y;
	}

	void cut(int x, int y)
	{
		make_root(x);
		access(y);
		splay(y);
		tr[y].ch[0] = tr[x].fa = 0;
		update(y);
	}

	int find_root(int x)
	{
		access(x);
		splay(x);
		while(tr[x].ch[0])
			x = tr[x].ch[0];
		return x;
	}

	bool query(int x, int y)
	{
		return find_root(x) == find_root(y);
	}

	int get_max(int x, int y)
	{
		make_root(x);
		access(y);
		splay(y);
		return tr[y].mx;
	}
}

int main()
{
	int n, m, x, y, a, b;
	scanf("%d%d", &n, &m);
	rep(i, 1, m)
		scanf("%d%d%d%d", &E[i].x, &E[i].y, &E[i].a, &E[i].b);
	sort(E + 1, E + 1 + m);
	int ans = INF;
	{
		using namespace LCT;
		rep(i, 1, n)
		{
			tr[i].val = 0;
			tr[i].mx = i;
		}
		rep(i, 1, m)
		{
			a = E[i].a; b = E[i].b;
			x = E[i].x;	y = E[i].y;
			bool flag = true;
			if(query(x, y))
			{
				int ct = get_max(x, y);
				if(E[ct - n].b <= b)
					flag = false;
				else
				{
					cut(ct, E[ct - n].x);
					cut(ct, E[ct - n].y);
				}
			}
			if(flag)
			{
				tr[n + i].val = b;
				tr[n + i].mx = n + i;
				link(x, n + i);
				link(y, n + i);
				if(query(1, n))
					ans = min(ans, a + tr[get_max(1, n)].val);
			}
		}
	}
	if(ans == INF) printf("-1\n");
	else printf("%d\n", ans);
	return 0;
}