#include<bits/stdc++.h>
#include<omp.h>
#include <parallel/algorithm>
using namespace std;
typedef long long ll;
#define N_MAX 1000000
vector <int> adj[N_MAX];
int vis[N_MAX];
int outdeg[N_MAX];
int ran_unsort[N_MAX];
long double b_min;
vector < pair<long double,int> > ran(N_MAX);
vector <int> q;
int N,M;
bool sortinrev(const pair<int,int> &a,const pair<int,int> &b)
{
    return (a.first > b.first);
}
long double max(int N)
{
    long double max_val;
    int idx;
    int n=N;
    #pragma omp parallel for reduction(max:max_val)
    for(idx=0;idx<N;idx++)
    {
        max_val = max_val > ran[idx].first ? max_val : ran[idx].first;
    }
    return max_val;
}
void BFS()
{
    omp_set_nested(1);
    int idx=1;
    cerr<<ran[0].second;
    q.push_back(ran[0].second);
    vis[ran[0].second]=ran[0].second;
    int prev=0;
    int time=1;
    
    while(1)
    {
        int flag=0;
        int curn=q.size();
        #pragma omp parallel for
        for(int i=prev;i<curn;i++)
        {
        //    cerr<<i<<endl;
            int v=q[i];
            #pragma omp parallel for
            for(int j=0;j<adj[v].size();j++)
            {
                int ver=adj[v][j]; 
                #pragma omp crtical
                {
                    if(vis[ver]==0)
                    {
                        vis[ver]=v;
                        q.push_back(ver);
                        flag=1;
                    }
                }
            }
        }
        
        ++time;
        prev=curn;
        if(prev==N)
            break;
        if(q.size()>=N)
            break;
        while(1)
        {
            if(idx==N+1)
                break;
            if(b_min-ran[idx].first<time)
            {
                ++idx;
                if(vis[ran[idx-1].second]==0)
                    q.push_back(ran[idx-1].second);
            }
            else
                break;
        }
    }
    return ;
}
int main(int argc,char* argv[])
{
    omp_set_num_threads(40);
    string bet=argv[1];
    int threads=stoi(argv[2]);
    omp_set_num_threads(threads);
    long double beta = ::atof(bet.c_str());
    random_device rd;
    mt19937 gen(rd());
    exponential_distribution<> d(beta);
    
    cin>>N>>M;
    int n,m;
    n=N;
    m=M;
    for(int i=0;i<m;i++)
    {
        int u,v;
        cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    cout<<"IO Done"<<endl;
    //int start=clock();
    double time1=omp_get_wtime();
    #pragma omp parallel for
    for(int i=0;i<=n;i++)
    {
    	ran[i].first=d(gen);
        ran[i].second=i;
        ran_unsort[i]=ran[i].first;
    }
    
    b_min=max(n);
    __gnu_parallel::sort(ran.begin(),ran.end(),sortinrev);
    BFS();
    cout<<omp_get_wtime()-time1<<endl;
    

    return 0;
}

