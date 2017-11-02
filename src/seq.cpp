#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
vector <int> adj[500000];
int vis[500000];
int outdeg[500000];
set<int> s;
void BFS(int u,double beta)
{
    queue<int>q;
    vis[u]=u;
    q.push(u);
    s.erase(u);
    int outside=outdeg[u];
    int inside=0;
    int break_flag=0;
    while(!q.empty())
    {
        int v=q.front();
        q.pop();
        
        #pragma omp parallel for
        for(int i=0;i<adj[v].size();i++)
        {
           
            int ver=adj[v][i];
            if(vis[ver]==0)
            {
                vis[ver]=u;
                #pragma omp critical
                {
                    s.erase(ver);
                    q.push(ver);
                    outside+=outdeg[ver];
                    inside++;
                    if(inside<(outside/beta))
                    {
                        break_flag=1;
                    }
                }
                  
            }
        }
        if(break_flag)
            break;
           


    }
    return ;
}
int main()
{
    int n,m;
    double beta=0.01;
    cin>>n>>m;
    for(int i=1;i<=n;i++)
        s.insert(i);
    cout<<"set made"<<endl;
    time_t t1;
    time_t t2;
    for(int i=0;i<m;i++)
    {
        int u,v;
        cin>>u>>v;
        outdeg[u]++;
        outdeg[v]++;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    cout<<"IO Done"<<endl;
    int count=0;
    int start=clock();
    while(!s.empty())
    {
        int ver=*s.begin();
        BFS(ver,beta);
        count++;
    }
    int stop=clock();
    cout<<count<<endl;
    cout << "time: " << (stop-start)/double(CLOCKS_PER_SEC)*1000 << endl;
    return 0;
}

