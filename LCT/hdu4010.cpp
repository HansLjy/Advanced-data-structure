/*
	Name : hdu4010
	Copyright : EZOI
	Author : ezoiljy
	Date : 2017/9/2
	Description : LCT
	要求拆树连树以及区间修改区间查询
*/

#include <bits/stdc++.h>
#define rep(i, a, b) for(int i = (a); i <= (b); i++)
using std::memset;
using std::swap;
using std::max;
using std::min;

namespace LCT
{
	const int N = 300005;
	const int INF = (1 << 30);
	struct node{
		int mx, fa, to_add, val;
		int ch[2];
		bool rev;
	}tr[N];
	int stk[N];

	bool is_root(int x)
	{
		return tr[x].fa == 0 || (tr[tr[x].fa].ch[0] != x && tr[tr[x].fa].ch[1] != x);
	}

	int get_max(int x)
	{
		if(x == 0) return -INF;
		else return tr[x].mx;
	}

	void pushdown(int x)
	{
		if(x == 0) return;
		if(tr[x].rev)
		{
			swap(tr[x].ch[0], tr[x].ch[1]);
			if(tr[x].ch[0]) tr[tr[x].ch[0]].rev ^= 1;
			if(tr[x].ch[1]) tr[tr[x].ch[1]].rev ^= 1;
			tr[x].rev = 0;
		}
		if(tr[x].to_add)
		{
			tr[x].mx += tr[x].to_add;
			tr[x].val += tr[x].to_add;
			if(tr[x].ch[0]) tr[tr[x].ch[0]].to_add += tr[x].to_add;
			if(tr[x].ch[1]) tr[tr[x].ch[1]].to_add += tr[x].to_add;
			tr[x].to_add = 0;
		}
	}
	void update(int x)
	{
		if(!x) return;
		pushdown(x); pushdown(tr[x].ch[0]); pushdown(tr[x].ch[1]);
		tr[x].mx = max(tr[x].val, max(get_max(tr[x].ch[0]), get_max(tr[x].ch[1])));
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
		tr[y].fa = x;
		tr[x].fa = z;
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

	void access(int root)
	{
		int tmp = 0;
		while(root)
		{
			splay(root);
			tr[root].ch[1] = tmp;
			update(root);
			tmp = root;
			root = tr[root].fa;
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

	void cut_fa(int x, int y)
	{
		make_root(x);
		splay(y);
		if(tr[y].ch[0])
		{
			tr[tr[y].ch[0]].fa = tr[y].fa;
			tr[y].ch[0] = tr[y]. fa = 0;
		}
		else tr[y].fa = 0;
	}

	void modify(int x, int y, int w)
	{
		make_root(x);
		access(y);
		splay(y);
		tr[y].to_add += w;
	}

	int get_max(int x, int y)
	{
		make_root(x);
		access(y);
		splay(y);
		return tr[y].mx;
	}

	bool query(int x, int y)
	{
		return find_root(x) == find_root(y);
	}
}

int main()
{
	int n, x, y, id, z, Q, cnt = 0;
	while(scanf("%d", &n) == 1)
	{
		memset(LCT::tr, 0, sizeof(LCT::tr));
		rep(i, 1, n - 1)
		{
			scanf("%d%d", &x, &y);
			LCT::link(x, y);
		}
		rep(i, 1, n)
		{
			scanf("%d", &LCT::tr[i].val);
			LCT::tr[i].mx = LCT::tr[i].val;
		}
		scanf("%d", &Q);
		rep(i, 1, Q)
		{
			scanf("%d%d%d", &id, &x, &y);
			switch (id)
			{
				case 1 :
					if(LCT::query(x, y)) printf("-1\n");
					else LCT::link(x, y); break;
				case 2 :
					if(x == y || !LCT::query(x, y)) printf("-1\n");
					else LCT::cut_fa(x, y);
					break;
				case 3 :
					scanf("%d", &z);
					if(!LCT::query(y, z)) printf("-1\n");
					else {LCT::modify(y, z, x);}
					break;
				case 4 :
					if(!LCT::query(x, y)) printf("-1\n");
					else printf("%d\n", LCT::get_max(x, y));
					break;
			}
		}
		printf("\n");
	}
}