#include <bits/stdc++.h>

using namespace std;
//template begins
#define int long long int
#define pb push_back
#define mp make_pair
#define all(v) v.begin(),v.end()
typedef pair<int,int> pii;
//template ends
const int inf=999999;
const int binf=1e15;
vector<vector<pii>> adj,tadj;
vector<int> h;
vector<vector<int>> ans;

//array based dijkstra

void djarray(int sc)
{
    int n=tadj.size()-1;
    vector<int> path(n,inf);    //final dijkstra array
    path[sc]=0;
    vector<int> check(n,0);     //if its in solution set or not
    for(int i=0;i<n;i++)        //normal dijkstra loop, implemented using array
    {
        int now=-1,mn=inf;
        for(int i=0;i<n;i++)
        {
            if(path[i]<mn&&!check[i])
            {
                mn=path[i];
                now=i;
            }
        }
        if(now==-1)
            break;
        check[now]=1;
        for(pii x:tadj[now])
        {
            if(path[x.first]>path[now]+x.second)
                path[x.first]=path[now]+x.second;
        }
    }
    for(int i=0;i<n;i++){       //putting it in final answer array
        if(path[i]!=inf)
            ans[sc][i]=path[i]+h[i]-h[sc];
        else
            ans[sc][i]=path[i];
    }
}

//binary heap based dijsktra and required functions

pii extractminbinaryheap(vector<pii> &heap,vector<int> &heapind,int mx) //binary heap extract min  O(logn)
{
    pii ret=heap[0];        //heap[0] is minimum value,
    swap(heap[0],heap[mx-1]);   //putting it in last of the array (mx is the size)
    heapind[heap[0].second]=0;      //changing heapindices
    heapind[heap[mx-1].second]=mx-1;
    int i=0;
    while(i<mx-1)       //percolate down
    {
        int left=2*i+1,right=2*i+2;
        if(left>=mx-1)
            break;
        if(right>=mx-1)
        {
            if(heap[left].first<heap[i].first)
            {
                swap(heap[left],heap[i]);
                heapind[heap[left].second]=left;    //changing heapindices
                heapind[heap[i].second]=i;
                i=left;
            }
            else{
                break;
            }
        }
        else{
            if(heap[left].first<heap[right].first)
            {
                if(heap[left].first<heap[i].first)
                {
                    swap(heap[left],heap[i]);
                    heapind[heap[left].second]=left;    //changing heap indices
                    heapind[heap[i].second]=i;
                    i=left;
                }
                else{
                    break;
                }
            }
            else{
                if(heap[right].first<heap[i].first)
                {
                    swap(heap[right],heap[i]);
                    heapind[heap[right].second]=right;
                    heapind[heap[i].second]=i;
                    i=right;
                }
                else{
                    break;
                }
            }
        }
    }
    return ret; //returning the min node
}

void decreasekeybinaryheap(int which,int vl,vector<pii> &heap,vector<int> &heapind) //decrese key of binary heap O(logn)
{
    int pos=heapind[which];
    heap[pos].first=vl; //changing value
    int i=pos;
    while(i>0)      //percolate up
    {
        int par=(i-1)/2;
        if(heap[par].first>heap[i].first)
        {
            swap(heap[par],heap[i]);
            heapind[heap[par].second]=par;
            heapind[heap[i].second]=i;
            i=par;
        }
        else{
            break;
        }
    }
}

void djbinaryheap(int sc)       //binary heap dijkstra
{
    int n=tadj.size()-1;
    vector<int> path(n,inf);        //path vector
    path[sc]=0;
    vector<pii> heap(n);        //binart heap
    vector<int> heapind(n,0);       //heapindices
    heap[0]={0,sc};
    heapind[sc]=0;
    int tptr=1;
    for(int i=0;i<n;i++)        //putting all the infinity in heap
    {
        if(i==sc)
            continue;
        heap[tptr]={inf,i};
        heapind[i]=tptr;
        tptr++;
    }
    for(int i=0;i<n;i++)
    {
        int now=extractminbinaryheap(heap,heapind,n-i).second;      //extract min
        for(pii x:tadj[now])
        {
            if(path[x.first]>path[now]+x.second)
            {
                path[x.first]=path[now]+x.second;
                decreasekeybinaryheap(x.first,path[x.first],heap,heapind);  //decrease key
            }
        }
    }
    for(int i=0;i<n;i++){       //changing main answer array
        if(path[i]!=inf)
            ans[sc][i]=path[i]+h[i]-h[sc];
        else
            ans[sc][i]=path[i];
    }
}
//binomial heap based dijkstra

struct node{        //binomial heap node
    int val,id,deg;
    struct node *parent,*child,*sibling;
};

node *addnewbinomialnode(int val,int id)    //adding new nodes
{
    node *nz=(node *)malloc(sizeof(node));
    nz->val=val;
    nz->id=id;
    nz->deg=0;
    nz->parent=NULL;
    nz->sibling=NULL;
    nz->child=NULL;
    return nz;
}

node *reversell(node *head)     //reversing linked list, used in union
{
    node *curr=head,*prev=NULL,*next=NULL;
    while(curr!=NULL)
    {
        next=curr->sibling;
        curr->sibling=prev;
        prev=curr;
        curr=next;
    }
    return prev;
}

node *unionbinomialheap(node *n1,node *n2)   //union of binomial heap, n1 is main, n2 is children linked list
{
    node *final=NULL;
    n2=reversell(n2);       //reversing n2 because children head is marked from max deg to least deg
    node *pt1=n1,*pt2=n2,*pt3=NULL;    
    while(pt1!=NULL||pt2!=NULL)     //merging in final linked list
    {
        if(pt2==NULL)
        {
            node *temp=pt1->sibling;
            if(pt3==NULL)
            {
                final=pt1;
                pt1->sibling=NULL;
                pt3=final;
            }
            else{
                pt3->sibling=pt1;
                pt3=pt1;
                pt1->sibling=NULL;
            }
            pt1=temp;
        }
        else if(pt1==NULL)
        {
            node *temp=pt2->sibling;
            if(pt3==NULL)
            {
                final=pt2;
                pt2->sibling=NULL;
                pt3=final;
            }
            else{
                pt3->sibling=pt2;
                pt3=pt2;
                pt2->sibling=NULL;
            }
            pt2=temp;
        }
        else{
            if((pt1->deg)<(pt2->deg))
            {
                node *temp=pt1->sibling;
                if(pt3==NULL)
                {
                    final=pt1;
                    pt1->sibling=NULL;
                    pt3=final;
                }
                else{
                    pt3->sibling=pt1;
                    pt3=pt1;
                    pt1->sibling=NULL;
                }
                pt1=temp;
            }
            else{
                node *temp=pt2->sibling;
                if(pt3==NULL)
                {
                    final=pt2;
                    pt2->sibling=NULL;
                    pt3=final;
                }
                else{
                    pt3->sibling=pt2;
                    pt3=pt2;
                    pt2->sibling=NULL;
                }
                pt2=temp;
            }
        }
    }
    pt3=final;
    node *prev=pt3;
    while(pt3!=NULL)        //merging,prev pointer is the previous pointer
    {
        pt3->parent=NULL;
        if(pt3->sibling==NULL)
            break;
        if((pt3->deg)!=(pt3->sibling->deg))     //if siblings are not of same degree, move ahead
        {
            prev=pt3;
            pt3=pt3->sibling;
            continue;
        }
        if((pt3->sibling->sibling)==NULL||(pt3->sibling->sibling->deg!=pt3->deg))       //if deg of sibling of sibling is not same, we merge
        {
            if((pt3->val)>(pt3->sibling->val))
            {
                node *temp=pt3->sibling;
                pt3->parent=temp;
                pt3->sibling=temp->child;
                temp->child=pt3;
                (temp->deg)++;
                if(pt3==final)
                {
                    final=temp;
                }
                else{
                    prev->sibling=temp;
                }
                pt3=temp;
            }
            else{
                node *temp=pt3->sibling,*temp2=pt3->sibling->sibling;
                temp->parent=pt3;
                temp->sibling=pt3->child;
                pt3->child=temp;
                (pt3->deg)++;
                pt3->sibling=temp2;
                prev=pt3;
                pt3=temp2;
            }
        }
        else{       //else we move ahead
            prev=pt3;
            pt3=pt3->sibling;
            continue;
        }
    }
    return final;
}

void insbinomialheap(int val,int id,node **head,vector<node*> &heapind)     //inserting in binomial heap using union
{
    node *nz=addnewbinomialnode(val,id);
    heapind[id]=nz;
    *head=unionbinomialheap(*head,nz);
}

int extractminbinomialheap(node **head)     //extract min
{
    node *prev=*head,*now=*head,*temp=*head,*tempprev=*head;
    while(temp!=NULL)       //finding the minimum
    {
        if((temp->val)<(now->val))
        {
            now=temp;
            prev=tempprev;
        }
        tempprev=temp;
        temp=temp->sibling;
    }
    node *tempnew=now->child;
    int ret=now->id;
    if(now==(*head))        //removing it
    {
        (*head)=now->sibling;
        now->sibling=NULL;
        now->child=NULL;
        free(now);
    }
    else{
        prev->sibling=now->sibling;
        now->sibling=NULL;
        now->child=NULL;
        free(now);
    }
    *head=unionbinomialheap(*head,tempnew);     //union of now linked list and children linked list
    return ret;
}

void decreasekeybinomialheap(int id,int val,vector<node*> &heapind)
{
    node *now=heapind[id];
    now->val=val;       //changing value
    while((now->parent)!=NULL)          //percolate up
    {
        if((now->val)<(now->parent->val))
        {
            int tmp1=now->parent->val,tmp2=now->parent->id;
            now->parent->val=now->val;
            now->parent->id=now->id;
            now->val=tmp1;
            now->id=tmp2;
            heapind[now->id]=now;
            heapind[now->parent->id]=now->parent;
        }
        else{
            break;
        }
        now=now->parent;
    }
}

void djbinomialheap(int sc)
{
    int n=tadj.size()-1;
    vector<int> path(n,inf);
    path[sc]=0;
    vector<node *> heapind(n,NULL);
    node *head=NULL;
    for(int i=0;i<n;i++)
    {
        insbinomialheap(path[i],i,&head,heapind);
    }
    for(int i=0;i<n;i++)
    {
        int now=extractminbinomialheap(&head);
        for(pii x:tadj[now])
        {
            if(path[x.first]>path[now]+x.second)
            {
                path[x.first]=path[now]+x.second;
                decreasekeybinomialheap(x.first,path[x.first],heapind);
            }
        }
    }
    for(int i=0;i<n;i++){
        if(path[i]!=inf)
            ans[sc][i]=path[i]+h[i]-h[sc];
        else
            ans[sc][i]=path[i];
    }
}

//fibonacci heap based dijkstra
struct fnode{       //fibonacci heap node
    int val,id,deg;
    fnode *left,*right,*parent,*child;
    bool mark;
};

int numnode=0;

fnode *addnewfibnode(int val,int id)    //adding new nodes
{
    fnode *nz=(fnode *)malloc(sizeof(fnode));
    nz->val=val;
    nz->id=id;
    nz->left=nz;
    nz->right=nz;
    nz->parent=NULL;
    nz->child=NULL;
    nz->deg=0;
    nz->mark=false;
    return nz;
}

void insertinfibheap(int val,int id,fnode **min,vector<fnode*> &heapind)    //inserting a node in fib heap
{
    fnode *nz=addnewfibnode(val,id);
    heapind[id]=nz;
    if((*min)==NULL)
    {
        (*min)=nz;
    }
    else{       //inserting to the right of min pointer
        (*min)->right->left=nz;
        nz->right=(*min)->right;
        nz->left=(*min);
        (*min)->right=nz;
        if(((*min)->val)>(nz->val))
            *min=nz;
    }
    numnode++;
}

void bindtwo(fnode *a1,fnode *a2)       //binding two nodes, a2 becoming parent of a1
{
    a1->right->left=a1->left;
    a1->left->right=a1->right;
    a1->parent=a2;
    if(a2->child==NULL){
        a1->right=a1;
        a1->left=a1;
        a2->child=a1;
    }
    else{
        fnode *ch=a2->child;
        ch->right->left=a1;
        a1->right=ch->right;
        a1->left=ch;
        ch->right=a1;
    }
    if((a2->child->val)>(a1->val))
        a2->child=a1;
    (a2->deg)++;
}

void consolidate(fnode **min)       //consolidating so that no two trees of same degree remain
{
    int mxdeg=(int)log2(numnode);
    vector<fnode *> what(mxdeg+2,NULL);     //degree of nodes
    fnode *curr=*min,*temp=*min;
    int totallistnodes=0;
    do{     //finding the true min and calculating number of trees
        totallistnodes++;
        curr->parent=NULL;
        curr->mark=false;
        if((temp->val)>(curr->val))
            temp=curr;
        curr=curr->right;
    }while(curr!=(*min));
    *min=temp;
    curr=*min;
    curr=curr->right;       //starting from right of min so that min pointer never changes
    int cntnow=0;
    while(cntnow<totallistnodes)        //running all the nodes have been seen once
    {
        cntnow++;
        int degg=curr->deg;
        curr->parent=NULL;
        curr->mark=false;
        if(what[degg]==NULL)        //if this is the first one of this degree
        {
            what[degg]=curr;
        }
        else{
            int ff=1;
            while(what[curr->deg]!=NULL)
            {
                degg=curr->deg;
                fnode *samedeg=what[degg];
                if((samedeg->val)>=(curr->val))
                {
                    bindtwo(samedeg,curr);      //binding the two
                    what[degg]=NULL;
                }
                else{
                    if((samedeg->right==curr)&&(samedeg->left==curr))       //if only two trees left
                    {
                        bindtwo(curr,samedeg);
                        *min=samedeg;
                        what[degg]=NULL;
                        ff=0;
                        break;
                    }
                    else{           //shifting the previous one to the right of the current one and then binding the two
                        samedeg->left->right=samedeg->right;
                        samedeg->right->left=samedeg->left;
                        samedeg->right=samedeg;
                        samedeg->left=samedeg;

                        curr->right->left=samedeg;
                        samedeg->right=curr->right;
                        samedeg->left=curr;
                        curr->right=samedeg;

                        samedeg=curr;
                        curr=curr->right;
                        bindtwo(samedeg,curr);
                        what[degg]=NULL;
                    }
                }
            }
            what[curr->deg]=curr;
            if(!ff)
                break;
        }
        curr=curr->right;
    }
}

int extractminfibheap(fnode **min,vector<fnode *> &heapind)     //extract min of fib heap
{
    int ret=(*min)->id;
    if(numnode==1)      //if only one node
    {
        fnode *temp=(*min);
        *min=NULL;
        temp->right=NULL;
        temp->left=NULL;
        temp->parent=NULL;
        temp->child=NULL;
        numnode--;
        free(temp);
        return ret;
    }
    fnode *tempchild=(*min)->child;
    if(tempchild!=NULL)     //merging children ll and main ll
    {
        fnode *temp=(*min);
        temp->right->left=tempchild->left;
        tempchild->left->right=temp->right;
        temp->right=tempchild;
        tempchild->left=temp;
    }
    fnode *temp=(*min);     //deleting min, making a random pointer
    fnode *randh=temp->right;
    temp->right->left=temp->left;
    temp->left->right=temp->right;
    heapind[temp->id]=NULL;
    temp->right=NULL;
    temp->left=NULL;
    temp->parent=NULL;
    temp->child=NULL;
    free(temp);
    *min=randh;
    consolidate(min);   //consolidating
    numnode--;
    return ret;
}

void delchild(fnode *now,fnode **min)       //deleting the child and merging it to the right of min
{
    fnode *par=now->parent;
    if(now->right==now)
        par->child=NULL;
    else
        par->child=now->right;
    (par->deg)--;
    now->right->left=now->left;
    now->left->right=now->right;        //changing the child linked list
    now->right=now;
    now->left=now;
    (*min)->right->left=now;        //merging to the right of min
    now->right=(*min)->right;
    now->left=(*min);
    (*min)->right=now;
    now->parent=NULL;
    now->mark=false;
    if(((*min)->val)>(now->val))        //updating min pointer
        *min=now; 
}

void cutagain(fnode *now,fnode **min)       //recursive cutting, if parent is marked
{
    fnode *par=now->parent;
    if(par==NULL){
        now->mark=false;
        return;
    }
    delchild(now,min);
    if((par->parent)==NULL)
    {
        par->mark=false;
        return;
    }
    if(par->mark)       //mark only changes outside the function delchild, so mark means it is previously marked
        cutagain(par,min);
    else
    {
        par->mark=true;
        return;
    }
    
}

void decreasekeyfibheap(int ind,int val,fnode **min,vector<fnode *> &heapind)   //decrease key
{
    fnode *temp=heapind[ind];
    if(temp==NULL)
    {
        return;
    }
    temp->val=val;
    if(temp->parent==NULL)
    {
        temp->mark=false;
        if((temp->val)<((*min)->val))
        {
            *min=temp;
        }
        return;
    }
    if((temp->val)>=(temp->parent->val))
    {
        return;
    }
    fnode *par=temp->parent;
    delchild(temp,min);
    if(par->mark==false)
    {
        par->mark=true;
    }
    else{
        if(par->parent!=NULL)
            cutagain(par,min);
        else
            par->mark=false;
    }
}

void djfibheap(int sc)  //dijkstra fib heap
{
    int n=tadj.size()-1;
    vector<int> path(n,inf); 
    path[sc]=0;
    vector<fnode *> heapind(n,NULL);
    numnode=0;
    fnode * min=NULL;
    for(int i=0;i<n;i++)
    {
        insertinfibheap(path[i],i,&min,heapind);
    }
    for(int i=0;i<n;i++)
    {
        int now=extractminfibheap(&min,heapind);
        for(pii x:tadj[now])
        {
            if(path[x.first]>path[now]+x.second)
            {
                path[x.first]=path[now]+x.second;
                decreasekeyfibheap(x.first,path[x.first],&min,heapind);
            }
        } 
    }
    for(int i=0;i<n;i++){
        if(path[i]!=inf)
            ans[sc][i]=path[i]+h[i]-h[sc];
        else
            ans[sc][i]=path[i];
    }
}

//bellman ford 
int bellmanford(int n)      //normal bellman ford for johnsons algo
{
    vector<int> path(n+1,binf);
    path[n]=0;
    for(int i=0;i<n;i++)        //belman ford
    {
        int ff=0;
        for(int j=0;j<=n;j++)
        {
            for(pii x:adj[j])
            {
                if(path[x.first]>path[j]+x.second)
                {
                    path[x.first]=path[j]+x.second;
                    ff=1;
                }
            }
        }
        if(!ff)
            break;
    }
    int ff=0;
    for(int j=0;j<=n;j++)       //checking for negative cycle
    {
        for(pii x:adj[j])
        {
            if(path[x.first]>path[j]+x.second)
            {
                path[x.first]=path[j]+x.second;
                ff=1;
            }
        }
    }
    for(int i=0;i<n;i++)
        h[i]=path[i];       //h array (updating the weights using this)
    return ff;
}

signed main(signed argc,char *argv[]){
    vector<double> caltime;
    int t;
    cin>>t;
    clock_t time;
    while(t--)
    {
        int n,d;
        cin>>n>>d;
        adj.assign(n+1,{});
        tadj.assign(n+1,{});
        h.assign(n+1,0);
        ans.assign(n+1,vector<int>(n+1,0));
        int flag=0;
        for(int i=0;i<n;i++)        //input
        {
            for(int j=0,x;j<n;j++)
            {
                cin>>x;
                if(x==inf)
                    continue;
                if(x<0&&!d)
                {
                    flag=1;
                }
                adj[i].pb({j,x});
            }
        }
        time=clock();
        if(flag){
            cout<<-1<<endl;     //if negative in undirected graph
            time=clock()-time;
            caltime.pb((double)((double)time/(double)(CLOCKS_PER_SEC)));
            continue;
        }
        if(d)
        {
            for(int i=0;i<n;i++)
                adj[n].pb({i,0});
            int neg=bellmanford(n); //presence of a negative cycle
            if(neg)
            {
                time=clock()-time;
                cout<<-1<<endl;
                caltime.pb((double)((double)time/(double)(CLOCKS_PER_SEC)));
                continue;
            }
            for(int i=0;i<n;i++)
            {
                for(pii x:adj[i])
                {
                    tadj[i].pb({x.first,x.second+h[i]-h[x.first]});     //updating weights
                }
            }
        }
        else
            tadj=adj;       //no bellman ford as all the edges are already positive
        int cc=-1;
        if(argc!=1)
            cc=atoi(argv[1]);       //running dijkstra n times (keeping all vertices as source)
        if(argc==1||cc==4)
        {
            for(int i=0;i<n;i++)
                djfibheap(i);
        }
        else if(cc==3)
        {
            for(int i=0;i<n;i++)
                djbinomialheap(i);
        }
        else if(cc==2)
        {
            for(int i=0;i<n;i++)
                djbinaryheap(i);
        }
        else if(cc==1)
        {
            for(int i=0;i<n;i++)
                djarray(i);
        }
        else{
            for(int i=0;i<n;i++)
                djfibheap(i);
        }
        time=clock()-time;
        caltime.pb((double)((double)time/(double)(CLOCKS_PER_SEC)));
        for(int i=0;i<n;i++)        //final answer
        {
            for(int j=0;j<n;j++)
            {
                cout<<ans[i][j]<<" ";
            }
            cout<<endl;
        } 
    }
    for(auto x:caltime)
        cout<<fixed<<setprecision(7)<<x<<" ";
    cout<<endl;        
}
