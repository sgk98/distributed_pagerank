#include<bits/stdc++.h>
using namespace std;
vector <int> adj[325730];
vector <int> rev[325730];
int outdeg[325730];
int N,M;
double d=0.15;
double threshold=0.0000000001;
double pr[325730];
double prev[325730];
void compute(){
    double error=10000000000.0;
    int count=0;
    
    for(int i=0;i<=N;i++)
        prev[i]=d/N;
    while(error>threshold){
        count+=1;
        
        //cout<<error<<"\n";
        #pragma omp parallel for  
        for(int i=0;i<=N;i++){
            pr[i]=d/(N*1.0);
            for(int v=0;v<rev[i].size();v++){
                int ver=rev[i][v];
                pr[i]+=((prev[ver]*1.0)/(outdeg[ver]*1.0))*(1.0-d);
            }
        }
        double nerr=0.0;
        for(int i=0;i<=N;i++){
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



