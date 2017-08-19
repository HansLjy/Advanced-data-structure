// LJOJ 1451
#include <cstdio>
#include <ctime>
#include <algorithm>
#define rep(i,a,b) for(int i = (a); i <= (b); i++)
#define N 80005
#define MOD 1000000
using namespace std;
int n,tmp1,tmp2,ll,rr,tmp_ans,root1,root2,ans;
struct treap{
	int tot = 0;
	struct node{
		int l = 0,r = 0,v,w,size,rnd;
	}tr[N];
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
	void insert(int& k,int x){
		if(k == 0){
			k = ++tot;
			tr[k].l = tr[k].r = 0;
			tr[k].v = x;
			tr[k].w = tr[k].size = 1;
			tr[k].rnd = rand();
			return;
		} 
		tr[k].size++;
		if(x > tr[k].v){
			insert(tr[k].r,x);
			if(tr[tr[k].r].rnd < tr[k].rnd)	rturn(k);
		}
		else if(x < tr[k].v){
			insert(tr[k].l,x);
			if(tr[tr[k].l].rnd < tr[k].rnd) lturn(k);
		}
		else tr[k].w++;
	}
	void del(int& k,int x){
		if(k == 0) return;
		if(x > tr[k].v) tr[k].size--,del(tr[k].r,x);
		else if(x < tr[k].v) tr[k].size--,del(tr[k].l,x);
		else {
			if(tr[k].w > 1){
				tr[k].w--;
				tr[k].size--;
				return;
			}
			else {
				if(tr[k].l*tr[k].r == 0)
					k = tr[k].l+tr[k].r;
				else if(tr[tr[k].l].rnd < tr[tr[k].r].rnd){
					lturn(k);
					del(k,x);
				}
				else {
					rturn(k);
					del(k,x);
				}
			}
		}
	}
	void pre_pos(int k,int x,int& ans){
		if(k == 0) return;
		if(x < tr[k].v)
			pre_pos(tr[k].l,x,ans);
		else if(x > tr[k].v){
			ans = tr[k].v;
			pre_pos(tr[k].r,x,ans);
		} 
		else {
			ans = tr[k].v;
			return;
		}
	}
	void after_pos(int k,int x,int& ans){
		if(k == 0) return;
		if(x < tr[k].v){
			ans = tr[k].v;
			after_pos(tr[k].l,x,ans);
		}
		else if(x > tr[k].v)
			after_pos(tr[k].r,x,ans);
		else {
			ans = tr[k].v;
			return;
		}
	}
}T1,T2;
int main(){
	srand(time(NULL));
	scanf("%d",&n);
	rep(i,1,n){
		scanf("%d%d",&tmp1,&tmp2);
		if(tmp1 == 0) {
			if(T1.tot == 0 && T2.tot != 0){
				ll = rr = -1;
				T2.pre_pos(root2,tmp2,ll);
				T2.after_pos(root2,tmp2,rr);
				if(ll == -1 || rr == -1){
					tmp_ans = ll+rr+1;
				}
				else {
					if(ll+rr >= 2*tmp2)
						tmp_ans = ll;
					else tmp_ans = rr;
				}
				
				T2.del(root2,tmp_ans);
				T2.tot--;
				ans = (ans+abs(tmp_ans-tmp2))%MOD;
			}
			else T1.insert(root1,tmp2);
//			printf("%d\t",tmp_ans);
//			printf("%d\n",ans);
		}
		else {
			if(T2.tot == 0 && T1.tot != 0){
				ll = rr = -1;
				T1.pre_pos(root1,tmp2,ll);
				T1.after_pos(root1,tmp2,rr);
				if(ll == -1 || rr == -1){
					tmp_ans = ll+rr+1;
				}
				else {
					if(ll+rr >= 2*tmp2)
						tmp_ans = ll;
					else tmp_ans = rr;
				}
				T1.del(root1,tmp_ans);
				T1.tot--;
				ans = (ans+abs(tmp_ans-tmp2))%MOD;
			}
			else T2.insert(root2,tmp2);
//			printf("%d\t",tmp_ans);
//			printf("%d\n",ans);
		}
	}
	printf("%d",ans);
	return 0;
}
