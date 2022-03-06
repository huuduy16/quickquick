#include<bits/stdc++.h>
#include"Sol.h"
#include"Board.h"

#define SIZE 362880+10

using namespace std;

int fac[]={0,1,2,6,24,120,720,5040,40320,362880};
int dst[SIZE];
int rankP[SIZE], pre[SIZE];
vector<int> avail;

void initListPer()
    {
        int cnt=0;
        rankP[0]=123456789;
        vector<int> V; V.clear();
        for (int i=1; i <= 9; i++)
            V.push_back(i);
        while (next_permutation(V.begin(),V.end())){
            int val=0;
            for (int i=0; i < 9; i++)
                val=val*10 + V[i];
            rankP[++cnt]=val;
        }
    }
int perToRank(int P)
    {
        int rs=0;
        vector<int> V; V.clear();
        while (P > 0) V.push_back(P%10), P/=10;
        for (int i=0; i < 9; i++){
            int ord=0;
            for (int j=0; j < i; j++)
                ord+=(V[j] < V[i]);
            rs+=ord*fac[i];
        }
        return rs;
    }
int rankToPer(int n){return rankP[n];}
void BFS()
    {
        initListPer();

        memset(dst,-1,sizeof dst);
        dst[0]=0;
        queue<int> Q; while (!Q.empty()) Q.pop();
        Q.push(0);
        while (!Q.empty()){
            int cur=Q.front(); Q.pop();
            int P=rankP[cur];
            avail.push_back(P);

            int newP=perToRank(Board(P).moveUp().getID());
            if (dst[newP] == -1){
                Q.push(newP);
                dst[newP]=dst[cur]+1;
                pre[newP]=1;
            }

            newP=perToRank(Board(P).moveDown().getID());
            if (dst[newP] == -1){
                Q.push(newP);
                dst[newP]=dst[cur]+1;
                pre[newP]=2;
            }

            newP=perToRank(Board(P).moveLeft().getID());
            if (dst[newP] == -1){
                Q.push(newP);
                dst[newP]=dst[cur]+1;
                pre[newP]=3;
            }

            newP=perToRank(Board(P).moveRigh().getID());
            if (dst[newP] == -1){
                Q.push(newP);
                dst[newP]=dst[cur]+1;
                pre[newP]=4;
            }
        }
    }
int trace(int p){return pre[perToRank(p)];}
int random(int L, int R){return L + rand()%(R-L+1);}
int rand(int level)
    {
        if (level == 1)
            return avail[random(1,1000)];
        if (level == 2)
            return avail[random(1000,10000)];
        if (level == 3)
            return avail[random(10000,50000)];
        if (level == 4)
            return avail[random(50000,100000)];
        if (level == 5)
            return avail[random(100000,181439)];
    }
