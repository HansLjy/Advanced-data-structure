/*
	Name : LJOJ2494
	Copyright : EZOI
	Author : ezoiljy
	Date : 2017/9/6
	Description :
		单点修改加区间查询
		用zkw实现
*/

#include <bits/stdc++.h>
#define rep(i, a, b) for(int i = (a); i <= (b); i++)
#define per(i, a, b) for(int i = (a); i >= (b); i--)
using std::sort;
using std::max;
using std::min;

int n, m;
int a[100005];

namespace zkw
{
	#define lc (s << 1)
	#define rc ((s << 1) + 1)
	const int N = 100005;
	const int INF = (1 << 30);
	int M;
	struct node{
		int mn;
	}tr[N << 2];

	void init(int tot)
	{
		M = 1;
		while(M <= tot + 2)
			M <<= 1;
		memset(tr, 0, sizeof(tr));
		tr[M].mn = INF;
		rep(s, M + 1, M + n) tr[s].mn = a[s - M];
		rep(s, M + n + 1, M + M - 1) tr[s].mn = INF;
		per(s, M - 1, 1) tr[s].mn = min(tr[lc].mn, tr[rc].mn);
	}

	int query(int s, int t)
	{
		s = s - 1 + M;
		t = t + 1 + M;
		int res = INF;
		while((s ^ t) != 1)
		{
			if((s & 1) == 0)
				res = min(res, tr[s + 1].mn);
			if((t & 1) == 1)
				res = min(res, tr[t - 1].mn);
			s >>= 1; t >>= 1;
		}
		return res;
	}

	void modify(int s, int mn)
	{
		s = s + M;
		tr[s].mn = mn;
		while(s > 1)
		{
			s >>= 1;
			tr[s].mn = min(tr[lc].mn, tr[rc].mn);
		}
		return;
	}
	#undef lc
	#undef rc
}
using namespace zkw;

int main()
{
	int p, x, y;
	scanf("%d%d", &n, &m);
	rep(i, 1, n) scanf("%d", a + i);
	init(n);
	rep(i, 1, m)
	{
		scanf("%d%d%d", &p, &x, &y);
		switch (p)
		{
			case 1 : printf("%d ", query(x, y)); break;
			case 2 : modify(x, y); break;
		}
	}
}