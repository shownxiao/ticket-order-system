/*
这是票类，用于展示给用户车票信息，以及管理这张车票的中途车站的信息 
Ticket					ticket构造函数，根据选出的路线初始化
printTicket				打印车票
printStation			打印途径站点 
*/
#include<iostream>
#include<cstring>
#include<string>
using namespace std;
 
class Ticket {
	public:
		string start;    			//出发地
		string destination;			//目的地
		string start_time;			//出发时间 
		string end_time;			//到达时间 
		int money;					//花费金钱
		string car_number;			//车次号 
		int seat_number;			//座位号
		int rest_number;			//余票
		int init_number;			//额定票数 
	 	Ticket(string start,string destination,string start_time,string end_time,int money,string car_number,int seat_number,int rest_number,int init_number):
		 start(start),destination(destination),start_time(start_time),end_time(end_time),money(money),car_number(car_number),seat_number(seat_number),rest_number(rest_number),init_number(init_number){
		 }
		 		 
		Ticket(){				//ticket构造函数，根据选出的路线初始化,暂时用测试数据 
			this->start = "ChangSha";    			
			this->destination = "BeiJing";			
			this->start_time = "2019-1-14 9:55";			 
			this->end_time = "2019-1-15 7:40";			
			this->money = 2018;				
			this->car_number = "K205";		
			this->seat_number = 38;
			//下面是初始化中途站点，目前为测试数据
			this->station[0] = "ChangSha";
			this->station[1] = "Wuhan";
			this->station[2] = "ZhengZhou";
			this->station[3] = "BeiJing"; 
		}	
		void printTicket(){
			//cout<<"这是最终为你筛选的车票"<<endl;
			cout<<"-------------------------------------------"<<endl;
			cout<<"               "+this->car_number<<endl;
			cout<<"    额定票数"<<this->init_number;
			cout<<"       已定票人数"<<(this->init_number-this->rest_number)<<endl; 
			cout<<"               "<<this->seat_number<<"座位"<<endl;
			cout<<this->toStringPlace()<<endl;
			cout<<this->toStringtime()<<endl;
			cout<<"￥"<<this->money<<endl;
			cout<<"-------------------------------------------"<<endl;
		}
		void printStation(){
			cout<<this->toStringStation()<<endl;
		}
	private: 
		string station[20];		//一张车票途径站点最多为20个 
		string toStringPlace(){
			return this->start+"       ------->       "+this->destination;
		}
		string toStringtime(){
			return this->start_time+"           "+this->end_time;
		} 
		string toStringStation(){
			string all_station = "";
			for(int i = 0; i < 20; i++){
				all_station = all_station + "  " + this->station[i];
			}
			return all_station;
		}
};

