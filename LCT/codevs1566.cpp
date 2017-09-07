/*
	Name : codevs 1566
	Copyright : EZOI
	Author : ezoiljy
	Date : 2017/9/4
	Description : LCT
	要求：区间染色 区间数颜色段
*/

#include <bits/stdc++.h>
#define rep(i, a, b) for(int i = (a); i <= (b); i++)
using namespace std;

namespace LCT
{
	const int N = 300005;
	const int INF = (1 << 30);
	struct node{
		int cnt, fa, col, block_col, lx, rx;
		int ch[2];
		bool rev;
	}tr[N];
	int stk[N];

	bool is_root(int x)
	{
		return tr[x].fa == 0 || (tr[tr[x].fa].ch[0] != x && tr[tr[x].fa].ch[1] != x);
	}

	void pushdown(int x)
	{
		if(x == 0) return;
		if(tr[x].rev)
		{
			swap(tr[x].ch[0], tr[x].ch[1]);
			swap(tr[x].lx, tr[x].rx);
			if(tr[x].ch[0]) tr[tr[x].ch[0]].rev ^= 1;
			if(tr[x].ch[1]) tr[tr[x].ch[1]].rev ^= 1;
			tr[x].rev = 0;
		}
		if(tr[x].block_col)
		{
			tr[x].col = tr[x].block_col;
			tr[x].lx = tr[x].rx = tr[x].col;
			tr[x].cnt = 1;
			if(tr[x].ch[0]) tr[tr[x].ch[0]].block_col = tr[x].block_col;
			if(tr[x].ch[1]) tr[tr[x].ch[1]].block_col = tr[x].block_col;
			tr[x].block_col = 0;
		}
	}
	void update(int x)
	{
		if(!x) return;
		pushdown(tr[x].ch[0]); pushdown(tr[x].ch[1]);
		tr[x].cnt = 1;
		if(tr[x].ch[0])
		{
			tr[x].cnt += tr[tr[x].ch[0]].cnt;
			if(tr[tr[x].ch[0]].rx == tr[x].col) tr[x].cnt--;
		}
		if(tr[x].ch[1])
		{
			tr[x].cnt += tr[tr[x].ch[1]].cnt;
			if(tr[tr[x].ch[1]].lx == tr[x].col) tr[x].cnt--;
		}
		tr[x].lx = tr[x].ch[0] ? tr[tr[x].ch[0]].lx : tr[x].col;
		tr[x].rx = tr[x].ch[1] ? tr[tr[x].ch[1]].rx : tr[x].col;
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

	void print_color(int x, int y, int c)
	{
		make_root(x);
		access(y);
		splay(y);
		tr[y].block_col = c;
	}

	int count_color(int x, int y)
	{
		make_root(x);
		access(y);
		splay(y);
		return tr[y].cnt;
	}
}

void read(char& c)
{
	c = getchar();
	while(isspace(c)) c = getchar();
}

int main()
{
	int n, m, x, y, c;
	char opt;
	scanf("%d%d", &n, &m);
	rep(i, 1, n)
	{
		scanf("%d", &LCT::tr[i].col);
		LCT::tr[i].cnt = 1;
		LCT::tr[i].lx = LCT::tr[i].rx = LCT::tr[i].col;
	}
	rep(i, 1, n - 1)
	{
		scanf("%d%d", &x, &y);
		LCT::link(x, y);
	}
	rep(i, 1, m)
	{
		read(opt);
		switch (opt)
		{
			case 'C' :
				scanf("%d%d%d", &x, &y, &c);
				LCT::print_color(x, y, c);
				break;
			case 'Q' :
				scanf("%d%d", &x, &y);
				printf("%d\n", LCT::count_color(x, y));
				break;
		}
	}
}