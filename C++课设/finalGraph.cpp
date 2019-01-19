#include"Graph.h"
#include"Ticket.h" 
#include<cstdio>
#include<vector>
#include<malloc.h>
#include<set>
#include<cmath>
#include<cstdlib>
#define maxn 100

using namespace std;
const double kplane = 2;
const double ktrain = 0.5;
const double kcar =0.25;
set<string> s;

struct Gpath{
			int size;
			int nodes[maxn];
		};
struct HugeTicket{
	int money;
	int time;
	int trans;
	int stations;
	int stationmoney[maxn];
	string endtime[maxn];
	string starttime[maxn];
	string startcity[maxn];
	string totalpath[maxn];
	string carnumber[maxn];
	HugeTicket(){
		memset(this,0,sizeof(HugeTicket));
	}
}hugetickets[maxn],tempticket;

//三种比较方式，按时间，花费，中转次数进行比较 
int cmp1(const struct HugeTicket nodea,const struct HugeTicket nodeb);
int cmp2(const struct HugeTicket nodea,const struct HugeTicket nodeb);
int cmp3(const struct HugeTicket nodea,const struct HugeTicket nodeb);
/*
G为 Graph类的对象
cur为现在的站点编号
end为目的站点的编号
paths为路径数组
pathsNum为路径条数 
*/
void DFS(Graph &G,int cur,int end,struct Gpath paths[],int *pathsNum); 
void find_all();
bool selectCheck(Map &mp,struct Gpath paths[],int len,int priority);
bool PathCheck(Map &mp,struct Gpath paths,int &flag);

int main(){
	find_all();
	Ticket tickets[maxn];
	return 0;
}

int cmp1(const struct HugeTicket nodea,const struct HugeTicket nodeb){
	if(nodea.time==nodeb.time){
		if(nodea.money==nodeb.money){
			return nodea.trans<nodeb.trans;
		}
		return nodea.money<nodeb.money;
	}
	return nodea.time<nodeb.time;
}

int cmp2(const struct HugeTicket nodea,const struct HugeTicket nodeb){
	if(nodea.money==nodeb.money){
		if(nodea.time==nodeb.time){
			return nodea.trans<nodeb.trans;
		}
		return nodea.time<nodeb.time;
	}
	return nodea.money<nodeb.money;
}

int cmp3(const struct HugeTicket nodea,const struct HugeTicket nodeb){
	if(nodea.trans==nodeb.trans){
		if(nodea.money==nodeb.money){
			return nodea.time<nodeb.time;
		}
		return nodea.money<nodeb.money;
	}
	return nodea.trans<nodeb.trans;
}
 
void find_all(){
	int n,priority;
	int **a,start,end;
	struct Gpath * paths = (struct Gpath*)calloc(10000,sizeof(struct Gpath));
	for(int i=0;i<maxn;i++){
		paths[i].size=0;
		for(int z=0;z<maxn;z++){
			paths[i].nodes[z]=0;
		}
	}
	int pathsNum=0;
	cout<<"输入n"<<endl;
	cin>>n;
	a=(int **)new int *[maxn]; 
	for(int i=0;i<maxn;i++){
		a[i]=new int [maxn];
	}
	cout<<"输入一个邻接矩阵"<<endl;
	int temp;
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			cin>>temp;
			*((int *)a+i*n+j)=temp;
		}
	}
	Graph g(a,n);
	g.init();
	cout<<"输入起点和终点"<<endl;
	cin>>start>>end; 
	DFS(g,start,end,paths,&pathsNum);
	if(pathsNum==0){
		cout<<"在您输入的两点之间不存在可达路径,请重新输入"<<endl;
	}
	//
	cout<<"请输入你希望的优先级：1.时间最短2.旅费最少3.中转次数最少(仅输入数字)"<<endl; 
	cin>>priority;
	if(selectCheck(mp,paths,pathsNum,priority)){
		selecttickets();
	}
	else{
		cout<<"当前无余票充足且可达的路线"<<endl;
	}
	//
	int y;
	for(int x=0;x<pathsNum;x++){
		for(y=paths[x].size-1;y>0;y--){
			cout<<paths[x].nodes[y]<<"->";
		}
		cout<<(paths[x].nodes[y])<<endl;
	}
	delete [] paths;
}

bool selectCheck(Map &mp,struct Gpath paths[],int len,int priority){
	int flag=0;
	int cnt=0;
	for(int i=0;i<len;i++){
		if(!PathCheck(mp,paths[i],flag)){
			cnt++;
		}
	}
	if(cnt==len){
		return false;
	}
	else{
		if(priority==1){//按时间短排序
			 sort(hugetickets,hugetickets+flag,cmp1);
		}
		else if(priority == 2) sort(hugetickets,hugetickets+flag,cmp2);//按旅费低排序
		else(priority == 3) sort(hugetickets,hugetickets+flag,cmp3); //按中转次数低排序 
	}
	return true; 
}

void selecttickets(){
	HugeTicket temp=hugetickets[0];
	int seat = rand()%50;
	int ticketcnt=0;
	for(int i=0;i<temp.stations;i++){
		myflag=i;
		while(i<temp.stations-1&&temp.carnumber[i]==temp.carnumber[i+1]){
			temp.stationmoney[i+1]+=temp.money[i];
			i++;
		}
		tickets[ticketcnt++]=Ticket(temp.startcity[myflag],temp.totalpath[i],temp.starttime[myflag],temp.endtime[i],temp.stationmoney[i],temp.carnumber[i],seat); 
	}
	for(int i=0;i<ticketcnt;i++){
		tickets[i].printTicket();
		cout<<endl;
	}
}

bool PathCheck(Map &mp,struct Gpath paths,int &flag){
	double starttime,endtime,costmoney=0,speed;
	int costtime,k;
	City curCity;
	hugetickets[flag].stations=0;
	//假设在邻接矩阵可达的点，在数据表里一定能联通且时间表不会冲突 
	for(int i=paths.size-1;i>0;i--){
		//从起点到临终点
		curCity=mp.cities[paths.nodes[i]];
		for(k=0;k<curCity.time_table.path_num;k++)
		{//该城市的某条路线的下一个结点为该路径上的下一个城市
			if(curCity.time_table.path[k].end_city==mp.cities[paths.nodes[i-1]]);
			break;
		}
		//没有到该城市的路线或者没有余票 
		if(k==curCity.time_table.path_num||curCity.time_table.path[k].rest_num==0) 
			return false;
		if(mp.cities[paths.nodes[i]].time_table.path[k].path_type==PLANE)
		{
			speed=kplane;
			costmoney+=curCity.time_table.path[k].distance*kplane;
			hugetickets[flag].stationmoney[stations]=curCity.time_table.path[k].distance*kplane;
		}
		else if(mp.cities[paths.nodes[i]].time_table.path[k].path_type==TRAIN){
			speed=ktrain;
			costmoney+=curCity.time_table.path[k].distance*ktrain;
			hugetickets[flag].stationmoney[stations]=curCity.time_table.path[k].distance*ktrain;
		}
		else{
			speed=kcar;
			costmoney+=curCity.time_table.path[k].distance*kcar;
			hugetickets[flag].stationmoney[stations]=curCity.time_table.path[k].distance*kcar;
		}
		//每站的到达城市及出发时间和出发城市
		hugetickets[flag].startcity[stations]=curCity.name; 
		hugetickets[flag].starttime[stations]=curCity.time_table.path[k].start_time.toString();
		hugetickets[flag].totalpath[stations]=mp.cities[curCity.time_table.path[k].end_city].name;
		//该车票的总路线		 
		if(i==paths.size-1){//起点 
		starttime=mp.cities[paths.nodes[i]].time_table.path[k].start_time.hour*60+mp.cities[paths.nodes[i]].time_table.path[k].start_time.minute;
		//hugetickets[flag].starttime=mp.cities[paths.nodes[paths.size-1]].time_table.path[k].start_time.toString();
		}
		endtime=mp.cities[paths.nodes[i]].time_table.path[k].start_time.hour*60+mp.cities[paths.nodes[i]].time_table.path[k].start_time.minute;
			if(mp.cities[paths.nodes[i]].time_table.path[k].path_type==PLANE){ 
				endtime+=mp.cities[paths.nodes[i]].time_table.path[k].distance*1.0*60/600; 
			}
			else if(mp.cities[paths.nodes[i]].time_table.path[k].path_type==TRAIN){
				endtime+=mp.cities[paths.nodes[i]].time_table.path[k].distance*1.0*60/300;
			}
			else{
				endtime+=mp.cities[paths.nodes[i]].time_table.path[k].distance*1.0*60/50;
			}
			minute=endtime%60;
			hour=(endtime/60)%24;
			day=(endtime/(60*24)%30);
			hugetickets[flag].endtime[stations]=Date(mp.cities[paths.nodes[i]].time_table.path[k].start_time.year,mp.cities[paths.nodes[i]].time_table.path[k].start_time.month,day,hour,minute).toString();
		hugetickets[flag].carnumber[stations]=curCity.time_table.path[k].car_number;
		s.insert(curCity.time_table.path[k].car_number);
		stations++;
	}
	
	hugetickets[flag].trans=s.size()-1;//转车站数 
//	for(set<int>::iterator it=s.begin();it!=s.end();it++){
//		hugetickets[flag].carnumber+=*it;
//		hugetickets[flag].carnumber+="  ";
//	}
	costtime=(int)(endtime-starttime);
	hugetickets[flag].time=costtime;
	hugetickets[flag].money=(int)costmoney;
	//hugetickets[flag].startCity=mp.cities[paths.nodes[paths.size-1]].name;
	//hugetickets[flag].endCity=mp.cities[paths.nodes[0]].name;
	//把到达后的时间转为日期格式
	flag++;
	return true;
}

void DFS(Graph &G,int cur,int end,struct Gpath paths[],int *pathsNum){
	G.visited[cur]=1;
	struct Gpath * tempPaths = (struct Gpath*) calloc(100000,sizeof(struct Gpath));
	int tempPathsNum;
	for(int i=0;i<G.getn();i++){
		if(!G.visited[i]&&(*((int *)G.getMatrix()+cur*G.getn()+i))){
			if(i==end){
				paths[(*pathsNum)].size=2;
				paths[(*pathsNum)].nodes[0]=end;
				paths[(*pathsNum)].nodes[1]=cur;
				(*pathsNum)++;
			}
			else{
				tempPathsNum=0;
				DFS(G,i,end,tempPaths,&tempPathsNum);
				for(int j=0;j<tempPathsNum;j++){
					tempPaths[j].nodes[tempPaths[j].size]=cur;
					tempPaths[j].size++;
					paths[(*pathsNum)]=tempPaths[j];
					(*pathsNum)++;
				}
			}
		}
	}
	G.visited[cur]=false;
	delete [] tempPaths;
}
