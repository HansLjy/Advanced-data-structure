/*
	Name: BZOJ 2049
	License: GPL
	Copyright: EZOI
	Author: ezoiljy
	Date : 2017/8/25
	Description: LCT练习题
	要求支持合并树， 拆分树
*/
#include <bits/stdc++.h>
#define rep(i, a, b) for(int i = (a); i <= (b); i++)
using std::max;
using std::min;
using std::swap;

namespace LCT
{
	const int N = 2000005;
	struct node{
		int fa, ch[2];
		bool rev;
	}tr[N];
	int stk[N];

	void pushdown(int x)
	{
		if(tr[x].rev)
		{
			swap(tr[x].ch[0], tr[x].ch[1]);
			if(tr[x].ch[0]) tr[tr[x].ch[0]].rev ^= 1;
			if(tr[x].ch[1]) tr[tr[x].ch[1]].rev ^= 1;
			tr[x].rev ^= 1;
		}
	}

	bool is_root(int x)
	{
		return tr[x].fa == 0 || (tr[tr[x].fa].ch[0] != x && tr[tr[x].fa].ch[1] != x);
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
	}

	void splay(int x)
	{
		int pos = x;
		int tot = 0;
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
	}

	int access(int root)
	{
		int tmp = 0;
		while(root)
		{
			splay(root);
			tr[root].ch[1] = tmp;
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
		make_root(x); access(y); splay(y);
		tr[x].fa = tr[y].ch[0] = 0;
	}

	int find_root(int x)
	{
		access(x);
		splay(x);
		while(tr[x].ch[0]) x = tr[x].ch[0];
		return x;
	}
	bool query(int x, int y)
	{
		return find_root(x) == find_root(y);
	}
}

int main()
{
	int n, m, x, y;
	char opt[10];
	scanf("%d%d", &n, &m);
	rep(i, 1, m)
	{
		scanf("%s%d%d", opt, &x, &y);
		if(opt[0] == 'Q')
			printf(LCT::query(x, y) ? "Yes\n" : "No\n");
		else if(opt[0] == 'D')
			LCT::cut(x, y);
		else LCT::link(x, y);
	}
	return 0;
}