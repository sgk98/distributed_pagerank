#include<bits/stdc++.h>
using namespace std;
vector <int> adj[325731];
vector <int> rev[325731];
int vis[325731],scc[325731];
int level[325731];
vector <int> LEVEL[200];
vector <int> SCC[325731];
deque <int> L;
int outdeg[325731];
int N,M;
double d=0.15;
double threshold=0.00000001;
int SCC_MAX;
double pr[325731];
double prev[325731];
int dead[325731];
int block_indeg[325731];
int MAX_LL;
vector <int> final_adj[325731];
deque <int> block_zero;
vector <int> block_topo;
vector <int> block_adj[325731];

void dfs1(int u){
    if(vis[u]==0){
        vis[u]=1;
        for(int i=0;i<adj[u].size();i++)
        {
            dfs1(adj[u][i]);
        }
        L.push_front(u);
    }
}
void assign(int u,int root){

    if(scc[u]==0){
        scc[u]=root;
        SCC[root].push_back(u);
      
        for(int i=0;i<rev[u].size();i++){
            assign(rev[u][i],root);
            
        }
    }
}
void SCC_MAIN(){
    
    for(int i=1;i<=N;i++){
        if(!vis[i])
            dfs1(i);
    }
    
    //cout<<L.size()<<endl;
    int SCC_count=1;
    deque<int>::iterator it = L.begin();
    while(it!=L.end()){
        if(!scc[*it]){
         
            assign(*it,SCC_count++);
            
        }
        else{
            ;
        }
        it++;
    }
    SCC_MAX=SCC_count;
    //cout<<"SCC count"<<SCC_count<<endl;

}

void dfs2(int u){
    if(vis[u]==2)
        return ;
    vis[u]=2;
    for(int i=0;i<adj[u].size();i++){
        if(scc[u]!=scc[adj[u][i]]){
            block_adj[scc[u]].push_back(scc[adj[u][i]]);
            block_indeg[scc[adj[u][i]]]++;
        }
        if(vis[adj[u][i]]!=2)
            dfs2(adj[u][i]);
    }
}
void make_block(){
    //cout<<"MAKE BLOCK START"<<endl;
    for(int i=1;i<=N;i++){
        if(vis[i]!=2)
            dfs2(i);
    }
    //cout<<"DFS Done"<<endl;
    int count=0;
    for(int i=1;i<=SCC_MAX;i++)
    {
        if(block_indeg[i]==0){
            block_zero.push_back(i);
            level[i]=1;
            ++count;
            LEVEL[1].push_back(i);
        }
    }
    //cout<<"COUNT with 0"<<count<<endl;
    //cout<<"MAKE BLOCK DONE"<<endl;
}
void topo(){
   int max_lev=1;
    while(!block_zero.empty()){
        int u=block_zero.front();
        block_zero.pop_front();
        block_topo.push_back(u);
        for(int i=0;i<block_adj[u].size();i++){
            block_indeg[block_adj[u][i]]--;
            if(block_indeg[block_adj[u][i]]==0){
                block_zero.push_back(block_adj[u][i]);
                level[block_adj[u][i]]=1+level[u];
                LEVEL[1+level[u]].push_back(block_adj[u][i]);
                max_lev=max(max_lev,1+level[u]);
            }
        }
    }
    //cout<<block_topo.size()<<endl;
    //cout<<"LEVELS"<<max_lev<<endl;
    MAX_LL=max_lev;
}
void compute_SCC(int sc){
    double error=10000000000.0;
    double threshold2=threshold/N;
    for(int i=0;i<SCC[sc].size();i++){
        prev[SCC[sc][i]]=d/N;
    }
    //cerr<<"MADE IT HERE"<<endl;
    while(error>threshold){
        #pragma omp parallel for
        for(int i=0;i<SCC[sc].size();i++){
            if(dead[SCC[sc][i]]==1)
                continue;
            pr[SCC[sc][i]]=d/(N*1.0);
            for(int v=0;v<rev[SCC[sc][i]].size();v++){
                int ver=rev[SCC[sc][i]][v];
                pr[SCC[sc][i]]+=((prev[ver]*1.0))/(outdeg[ver]*1.0)*(1.0-d);
            }

        }
        //cerr<<"HERE"<<endl;
        double nerr=0.0;
        for(int i=0;i<SCC[sc].size();i++){
        	/*cerr<<SCC[sc][i]<<"DEBUG"<<endl;
        	cerr<<"HERE FIRST"<<endl;
        	cerr<<prev[SCC[sc][i]]<<endl;
        	cerr<<"FIRST DONE"<<endl;
        	cerr<<pr[SCC[sc][i]]<<endl;
        	cerr<<"2 DONE"<<endl;*/
            nerr=max(nerr,abs(prev[SCC[sc][i]]-pr[SCC[sc][i]]));
            //cerr<<"HERE TOO"<<endl;
            if(abs(prev[SCC[sc][i]]-pr[SCC[sc][i]]<=threshold2))
                dead[SCC[sc][i]]=1;

            prev[SCC[sc][i]]=pr[SCC[sc][i]];
        }
        error=nerr;
    }
}
void compute_new(int level){
    
    #pragma omp parallel for
    for(int i=1;i<LEVEL[level].size();i++){
    	//cerr<<i<<endl;
        compute_SCC(LEVEL[level][i]);
    }
}
void pagerank(){
    for(int i=1;i<MAX_LL;i++){
    	//cerr<<i<<endl;
        compute_new(i);
    }
}

void compute(){
    double threshold2=threshold/N;
    double error=10000000000.0;
    int count=0;
    
    for(int i=0;i<=N;i++)
        prev[i]=d/N;
    while(error>threshold){
        count+=1;
        
        #pragma omp parallel for  
        for(int i=0;i<=N;i++){
            if(dead[i]==1)
                continue;
            pr[i]=d/(N*1.0);
            for(int v=0;v<rev[i].size();v++){
                int ver=rev[i][v];
                pr[i]+=((prev[ver]*1.0)/(outdeg[ver]*1.0))*(1.0-d);
            }
        }
        double nerr=0.0;
        for(int i=0;i<=N;i++){
            nerr=max(nerr,abs(prev[i]-pr[i]));
            if(abs(prev[i]-pr[i])<=threshold2)
                dead[i]=1;
            prev[i]=pr[i];
        }
        error=nerr;

    }
}

int main(){
   
    cin>>N>>M;
    //cout<<d/N<<endl;
    for(int i=0;i<M;i++)
    {
        int u,v;
        cin>>u>>v;
        if(u!=v){
            adj[u].push_back(v);
            rev[v].push_back(u);
            outdeg[u]++;
        }
    }
    //compute();
    SCC_MAIN();
    make_block();
    topo();
    pagerank();
    return 0;
}



