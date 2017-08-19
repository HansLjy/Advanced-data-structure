// poj 2761
// 离线第K大
#include <cstdio>
#include <algorithm>
#include <ctime>
#define rep(i,a,b) for(int i = (a); i <= (b); i++)
#define N 100005
using namespace std;
int n,m,root,tot,lastl,lastr,val[N],ans[N];
struct node{
	int l,r,w,v,rnd,size;
	void set(){
		l = r = w = v = rnd = size = 0;
	}
}tr[N];
struct ques{
	int l,r,k,rnk;
}Q[N];
void updata(int k){
	tr[k].size = tr[tr[k].l].size+tr[tr[k].r].size+tr[k].w;
}
void lturn(int& k){
	int lc = tr[k].l;
	tr[k].l = tr[lc].r;
	tr[lc].r = k;
	tr[lc].size = tr[k].size;
	updata(k);
	k = lc;
}
void rturn(int& k){
	int rc = tr[k].r;
	tr[k].r = tr[rc].l;
	tr[rc].l = k;
	tr[rc].size = tr[k].size;
	updata(k);
	k = rc;
}
void insert(int& k, int x){
	if(k == 0){
		k = ++tot;
		tr[k].l = tr[k].r = 0;
		tr[k].w = tr[k].size = 1;
		tr[k].rnd = rand();
		tr[k].v = x;
		return;
	}
	tr[k].size++;
	if(x > tr[k].v){
		insert(tr[k].r,x);
		if(tr[tr[k].r].rnd < tr[k].rnd) rturn(k);
	}
	else if(x < tr[k].v){
		insert(tr[k].l,x);
		if(tr[tr[k].l].rnd < tr[k].rnd) lturn(k);
	}
	else tr[k].w++;
} 
void del(int& k,int x){
	if(k == 0) return;
	if(x > tr[k].v) {
		tr[k].size--;
		del(tr[k].r,x);
	}
	else if(x < tr[k].v){
		tr[k].size--;
		del(tr[k].l,x);
	}
	else {
		if(tr[k].w > 1){
			tr[k].w--;
			tr[k].size--;
			return;
		}
		if(tr[k].l*tr[k].r == 0) k = tr[k].l+tr[k].r;
		else if(tr[tr[k].l].rnd < tr[tr[k].r].rnd)
			lturn(k),del(k,x);
		else rturn(k),del(k,x);
	}
}
int Kth(int k,int pos){
	if(tr[tr[k].l].size >= pos)
		return Kth(tr[k].l,pos);
	else if(pos > tr[tr[k].l].size+tr[k].w)
		return Kth(tr[k].r,pos-tr[tr[k].l].size-tr[k].w);
	else return tr[k].v;
}
bool cmp(const ques& a,const ques& b){
	if (a.l < b.l) return true;
	else if(a.l == b.l && a.r < b.r) return true;
	return false;
}
int main(){
	srand(time(NULL));
	scanf("%d%d",&n,&m);
	rep(i,1,n) scanf("%d",&val[i]);
	rep(i,1,m) {
		scanf("%d%d%d",&Q[i].l,&Q[i].r,&Q[i].k);
		Q[i].rnk = i;
	}
	sort(Q+1,Q+1+m,cmp);
	rep(i,1,m){
		if(lastl == 0 && lastr == 0)
			rep(j,Q[i].l,Q[i].r) insert(root,val[j]);
		else {
			if(lastl < Q[i].l)	rep(j,lastl,Q[i].l-1) 
				del(root,val[j]);
			else rep(j,Q[i].l,lastl-1) 
				insert(root,val[j]);
			if(lastr > Q[i].r) rep(j,Q[i].r+1,lastr)
				del(root,val[j]);
			else rep(j,lastr+1,Q[i].r)
				insert(root,val[j]);
		}
		lastl = Q[i].l;lastr = Q[i].r;
		ans[Q[i].rnk] = Kth(root,Q[i].k);
	}
	rep(i,1,m) printf("%d\n",ans[i]);
	return 0;
} 
