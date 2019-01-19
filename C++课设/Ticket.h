/*
����Ʊ�࣬����չʾ���û���Ʊ��Ϣ���Լ��������ų�Ʊ����;��վ����Ϣ 
Ticket					ticket���캯��������ѡ����·�߳�ʼ��
printTicket				��ӡ��Ʊ
printStation			��ӡ;��վ�� 
*/
#include<iostream>
#include<cstring>
#include<string>
using namespace std;
 
class Ticket {
	public:
		string start;    			//������
		string destination;			//Ŀ�ĵ�
		string start_time;			//����ʱ�� 
		string end_time;			//����ʱ�� 
		int money;					//���ѽ�Ǯ
		string car_number;			//���κ� 
		int seat_number;			//��λ��
		int rest_number;			//��Ʊ
		int init_number;			//�Ʊ�� 
	 	Ticket(string start,string destination,string start_time,string end_time,int money,string car_number,int seat_number,int rest_number,int init_number):
		 start(start),destination(destination),start_time(start_time),end_time(end_time),money(money),car_number(car_number),seat_number(seat_number),rest_number(rest_number),init_number(init_number){
		 }
		 		 
		Ticket(){				//ticket���캯��������ѡ����·�߳�ʼ��,��ʱ�ò������� 
			this->start = "ChangSha";    			
			this->destination = "BeiJing";			
			this->start_time = "2019-1-14 9:55";			 
			this->end_time = "2019-1-15 7:40";			
			this->money = 2018;				
			this->car_number = "K205";		
			this->seat_number = 38;
			//�����ǳ�ʼ����;վ�㣬ĿǰΪ��������
			this->station[0] = "ChangSha";
			this->station[1] = "Wuhan";
			this->station[2] = "ZhengZhou";
			this->station[3] = "BeiJing"; 
		}	
		void printTicket(){
			//cout<<"��������Ϊ��ɸѡ�ĳ�Ʊ"<<endl;
			cout<<"-------------------------------------------"<<endl;
			cout<<"               "+this->car_number<<endl;
			cout<<"    �Ʊ��"<<this->init_number;
			cout<<"       �Ѷ�Ʊ����"<<(this->init_number-this->rest_number)<<endl; 
			cout<<"               "<<this->seat_number<<"��λ"<<endl;
			cout<<this->toStringPlace()<<endl;
			cout<<this->toStringtime()<<endl;
			cout<<"��"<<this->money<<endl;
			cout<<"-------------------------------------------"<<endl;
		}
		void printStation(){
			cout<<this->toStringStation()<<endl;
		}
	private: 
		string station[20];		//һ�ų�Ʊ;��վ�����Ϊ20�� 
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

