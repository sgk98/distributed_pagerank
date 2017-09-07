#include<bits/stdc++.h>
using namespace std;
vector <int> adj[281905];
vector <int> rev[281905];
int outdeg[281905];
int N,M;
double d=0.15;
double threshold=0.00000001;
double pr[281905];
double prev[281905];
void compute(){
    double error=10000000000.0;
    int count=0;
    
    for(int i=1;i<=N;i++)
        prev[i]=d/N;
    while(error>threshold){
        count+=1;
        
        //cout<<error<<"\n";
        #pragma omp parallel for  
        for(int i=1;i<=N;i++){
            pr[i]=d/(N*1.0);
            for(int v=0;v<rev[i].size();v++){
                int ver=rev[i][v];
                pr[i]+=((prev[ver]*1.0)/(outdeg[ver]*1.0))*(1.0-d);
            }
        }
        double nerr=0.0;
        for(int i=1;i<=N;i++){
            nerr=max(nerr,abs(prev[i]-pr[i]));
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
        adj[u].push_back(v);
        rev[v].push_back(u);
        outdeg[u]++;
    }
    compute();
    return 0;
}



