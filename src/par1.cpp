#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define N_MAX 1000000
vector <int> adj[N_MAX];
int vis[N_MAX];
int outdeg[N_MAX];
long double ran[N_MAX];
long double max(int N)
{
    long double max_val;
    int idx;
    int n=N;
    #pragma omp parallel for reduction(max:max_val)
    for(idx=0;idx<N;idx++)
    {
        max_val = max_val > ran[idx] ? max_val : ran[idx];
    }
    return max_val;
}

int main(int argc,char* argv[])
{
	string bet=argv[1];
	long double beta = ::atof(bet.c_str());
    random_device rd;
    mt19937 gen(rd());
    exponential_distribution<> d(beta);
    int n,m;
    n=1000000;
    //cin>>n>>m;
    int start=clock();
    #pragma omp parallel for
    for(int i=0;i<n;i++)
    {
    	ran[i]=d(gen);

    }
    /*for(int i=0;i<n;i++)cout<<ran[i]<<endl;
    cout<<"ans is"<<endl;*/
    max(n);
    int stop=clock();
    cout << "time: " << (stop-start)/double(CLOCKS_PER_SEC)*1000 << endl;



    return 0;
}

