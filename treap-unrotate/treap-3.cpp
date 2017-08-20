/*
	Name:treap-非旋转
	Copyright: EZOI
	Author: ezoiljy
	Date : 2017-8-20
	Description: POJ 3580
	需要支持的操作：
	1.按位置插入 INSERT
	2.区间加 ADD
	3.区间取min MIN
	4.区间翻转 REVERSE
	5.区间循环位移 REVOLVE
	6.删除Ak DELETE
*/

#include <cstdio>
#include <algorithm>
#define rep(i, a, b) for(int i = (a); i <= (b); i++)
#define N 200005
// #define Debug
using std::pair;
using std::make_pair;
using std::sort;
using std::max;
using std::min;
using std::swap;
typedef pair<int,int> pii;

int n, x, y, num, t, m;
int a[N];
char opt[10];

namespace treap_unrotate
{
	const int INF = 1073741824;
	int root = 0, tot = 0;

	struct node{
		int val, mn, fix, to_add, l, r, size;
		bool rev;
	}tr[N];

	int new_node(int value)
	{
		tr[++tot].val = value;
		tr[tot].mn = value;
		tr[tot].fix = rand();
		tr[tot].to_add = tr[tot].l = tr[tot].r = 0;
		tr[tot].size = 1;
		tr[tot].rev = 0;
		return tot;
	}

	void pushdown(int rt)
	{
		int add = tr[rt].to_add, lc = tr[rt].l, rc = tr[rt].r, r = tr[rt].rev;
		if(lc)
		{
			tr[lc].val += add;
			tr[lc].mn += add;
			tr[lc].to_add += add;
			if(r) swap(tr[lc].l, tr[lc].r), tr[lc].rev ^= 1;
		}
		if(rc)
		{
			tr[rc].val += add;
			tr[rc].mn += add;
			tr[rc].to_add += add;
			if(r) swap(tr[rc].l, tr[rc].r), tr[rc].rev ^= 1;
		}
		tr[rt].to_add = 0;
		tr[rt].rev = 0;
	}

	int get_size(int u)	{return u ? tr[u].size : 0;}

	int get_min(int u) {return u ? tr[u].mn : INF;}

	void update(int x) // 维护根节点信息
	{
		tr[x].size = get_size(tr[x].l) + get_size(tr[x].r) + 1;
		tr[x].mn = min(min(get_min(tr[x].l), get_min(tr[x].r)), tr[x].val);
	}

	int merge(int a, int b) // 合并两棵子树， 返回合并后的根节点(注意此时不能交换顺序)
	{
		if(a == 0) return b;
		if(b == 0) return a;
		pushdown(a);
		pushdown(b);
		if(tr[a].fix < tr[b].fix)
		{
			tr[a].r = merge(tr[a].r, b);
			update(a);
			return a;
		}
		else
		{
			tr[b].l = merge(a, tr[b].l);
			update(b);
			return b;
		}
	}

	pii split(int rt, int k) // 分成 k 个与 tot - k 个两棵子树， 返回两个子树的根节点
	{
		pushdown(rt);
		if(rt == 0) return make_pair(0, 0);
		int lsize = get_size(tr[rt].l);
		pii ret = make_pair(0, 0);
		if(lsize >= k)
		{
			ret = split(tr[rt].l, k);
			tr[rt].l = ret.second;
			ret.second = rt;
			update(rt);
		}
		else
		{
			ret = split(tr[rt].r, k - lsize - 1);
			tr[rt].r = ret.first;
			ret.first = rt;
			update(rt);
		}
		return ret;
	}

	void insert(int pos, int value) // 在指定位置插入元素
	{
		pii sub = split(root, pos);
		int cur = new_node(value);
		cur = merge(sub.first, cur);
		root = merge(cur, sub.second);
	}

	void del(int pos) // 删除指定位置的元素
	{
		pii spl = split(root, pos - 1);
		int cur = spl.second, root1 = spl.first, root2;
		spl = split(cur, 1);
		root2 = spl.second;
		root = merge(root1, root2);
	}

	int query_min(int pos1, int pos2)
	{
		pii spl = split(root, pos1 - 1);
		int cur = spl.second, left = spl.first, res;
		spl = split(cur, pos2 - pos1 + 1);
		res = tr[spl.first].mn;
		cur = merge(spl.first, spl.second);
		root = merge(left, cur);
		return res;
	}

	void modify(int pos1, int pos2, int num)
	{
		pii spl = split(root, pos1 - 1);
		int left = spl.first;
		spl = split(spl.second, pos2 - pos1 + 1);
		int cur = spl.first;
		tr[cur].to_add = num;
		tr[cur].val += num;
		tr[cur].mn += num;
		int right = merge(spl.first, spl.second);
		root = merge(left, right);
	}
	void reverse(int pos1, int pos2)
	{
		pii spl = split(root, pos1 - 1);
		int left = spl.first;
		spl = split(spl.second, pos2 - pos1 + 1);
		int cur = spl.first;
		tr[cur].rev ^= 1;
		swap(tr[cur].l, tr[cur].r);
		int right = merge(spl.first, spl.second);
		root = merge(left, right);
	}
	void revolve(int pos1, int pos2, int c)
	{
		pii spl = split(root, pos1 - 1);
		int left = spl.first;
		spl = split(spl.second, c);
		int a = spl.first;
		spl = split(spl.second, pos2 - pos1 + 1 - c);
		int b = spl.first;
		int right = spl.second;
		int cur = merge(b, a);
		cur = merge(left, cur);
		root = merge(cur, right);
	}
#ifdef Debug
	void print(int rt, int step)
	{
		pushdown(rt);
		if(tr[rt].l) print(tr[rt].l, step + 2);
		// rep(i, 1, step) printf("-");
		printf("%d ", tr[rt].val);
		if(tr[rt].r) print(tr[rt].r, step + 2);
	}
#endif
}
using namespace treap_unrotate;

int main()
{
	scanf("%d", &n);
	rep(i, 1, n)
	{
		scanf("%d", a + i);
		insert(i, a[i]);
	}
	scanf("%d", &m);
	rep(i, 1, m)
	{
		#ifdef Debug
				print(root, 0);
				printf("\n");
		#endif
		scanf("%s", opt + 1);
		if(opt[1] == 'A') // ADD
		{
			scanf("%d%d%d", &x, &y, &num);
			modify(x, y, num);
		}
		else if(opt[1] == 'M') // MIN
		{
			scanf("%d%d", &x, &y);
			printf("%d\n", query_min(x, y));
		}
		else if(opt[1] == 'I') // INSERT
		{
			scanf("%d%d", &x, &num);
			insert(x, num);
		}
		else if(opt[1] == 'D') // DELETE
		{
			scanf("%d", &x);
			del(x);
		}
		else if(opt[4] == 'E') // REVERSE
		{
			scanf("%d%d", &x, &y);
			reverse(x, y);
		}
		else //REVOLVE
		{
			scanf("%d%d%d", &x, &y, &t);
			revolve(x, y, y - x + 1 - t % (y - x + 1));
		}
	}
	return 0;
}