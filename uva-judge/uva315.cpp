#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <cassert>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cstdio>
#include <vector>
#include <cmath>
#include <queue>
#include <deque>
#include <stack>
#include <map>
#include <set>

using namespace std;
#define mp make_pair 
#define All(v) v.begin(),v.end()
#define INF 1<<30
#define MAX 105

int low[MAX],d[MAX],pai[MAX],seen[MAX],art[MAX];
int childs,nds,root,critical;
vector<vector<int> > g;


void dfs(int u){
    seen[u] = true;
    d[u] = low[u] = nds++;
    for (int i = 0; i < g[u].size(); i++){
        int v = g[u][i];
        if (pai[u] != v){
            if(!seen[v]){
               pai[v] = u;
               dfs(v);
               if (u == root) childs++;
               if (d[u] <= low[v] && u != root){
                   art[u] = true;
               }
               if (d[u] <= low[v] && u == root && childs >= 2){
                   art[u] = true;
               }               
               low[u] = min(low[u], low[v]);
            }else{
               low[u] = min(low[u], d[v]);
            }
        }
    }
}

void solve(int n){
	memset(low,0,sizeof(low));
	memset(d,0,sizeof(d));
	memset(seen,0,sizeof(seen));	
	memset(pai,-1,sizeof(pai));
	memset(art,0,sizeof(art));
		
	critical = 0;
	nds = 0;
	for(int i = 0; i < n; i++){
		if (!seen[i]){
			nds = 0;
			root = i;
			childs = 0;
			dfs(i);
		}
	}
	for(int i = 0; i < n; i++){
		if (art[i]){
			critical++;
		}
	}
}
int main(){
	int n;
	while(scanf("%d",&n) && n){
		g = vector<vector<int> > (n+1);
		int u,v;
		while(scanf("%d",&u) && u){
			string s;
			getline(cin,s);
			istringstream is(s);
			while(is>>v){
				g[u-1].push_back(v-1);
				g[v-1].push_back(u-1);
			}
		}
		solve(n);
		printf("%d\n",critical);
	}
}
