#include<iostream>
#include<fstream> 
#include<map>
#include<stdlib.h>
#include"Graph.h"
#include"Ticket.h" 
#include<cstdio>
#include<vector>
#include<malloc.h>
#include<set>
#include<cmath>
#include<cstdlib>
#include<algorithm>
#define maxn 100
using namespace std;
enum Type {PLANE, TRAIN, CAR};
const double kplane = 2;
const double ktrain = 0.5;
const double kcar =0.25;
set<string> s;
Ticket tickets[maxn];
/*������
bool is_low(Date d)    ��ǰ���ڱ� dС���磩���� true�����򷵻� false
string toString()      �������ڵĸ�ʽ���ַ�  ��: 2018-01-11 16:03 
string toSequence()    �������ڵ��������У������ļ�д�룬�磺201801111603 
*/
class Date {
public:
    int year;
    int month;
    int day;
    int hour;
    int minute;
    
public:
    Date(){}
    Date(int year, int month, int day, int hour, int minute) :
        year(year), month(month), day(day), hour(hour), minute(minute) {}
    //���ַ������г�ʼ�����磺201901122212 
    Date(string date_string){
        this->year = atoi(date_string.substr(0,4).c_str());
        this->month = atoi(date_string.substr(4,2).c_str());
        this->day = atoi(date_string.substr(6,2).c_str());
        this->hour = atoi(date_string.substr(8,2).c_str());
        this->minute = atoi(date_string.substr(10,2).c_str());
    }
    
    //is_low(Date d) ��ǰ���ڱ� dС���磩���� true�����򷵻� false
    bool is_low(Date d) {
        return this->toSequence().compare(d.toSequence()) < 0 ? true:false;
    }
    
    //������ת��Ϊ��ӡ��ʽ���磺2019-01-13 22:13 
    string toString() {
        char c[16];
        sprintf(c, "%4d-%02d-%02d %02d:%02d", this->year, this->month,
            this->day, this->hour, this->minute);
        return string(c);
    }
    
    //������ת��Ϊ�ļ�д���ʽ���磺201901132213
    string  toSequence(){
        char c[12];
        sprintf(c, "%4d%02d%02d%02d%02d", this->year, this->month,
            this->day, this->hour, this->minute);
        return string(c);
    }
};

/*
string toSequence()    �����ļ�д��ĸ�ʽ���ַ���
*/ 
class Path {
public:
    string car_number; //���κ� 
    Type path_type;    //��ͨ���� PLANE��TRAIN��CAR 
    Date start_time;   //����ʱ�� 
    Date end_time;     //��վʱ�� 
    bool avilable;     //ɾ����ǣ�true��ʾδɾ����false��ʾ��ɾ�� 
    int from;          //�������� 
    int start_city;    //�������� 
    int end_city;      //������� 
    int price;         //�۸� 
    int distance;      //�г̾��� 
    int rest_num;      //��Ʊ����   
    int init_num;      //�Ʊ�� 
    
public:
    Path(){}
    //���ļ���ȡ���г�ʼ�� Path
    //ʾ��:
    //car_number avilable start_time from start_city end_city distance path_type rest_num
    //K1234 1 201901132213 -1 1 2 50 0 50 
    Path(string car_number, int avilable, string start_time, string end_time, int from, int start_city, int end_city, 
         int distance, int path_type, int rest_num, int price, int init_num): 
         car_number(car_number), from(from), start_city(start_city), end_city(end_city), 
         distance(distance), rest_num(rest_num), price(price), init_num(init_num){
            Date d(start_time);
            Date d_1(end_time);
            this->end_time = d_1;
            this->start_time = d;
            this->path_type = (Type)path_type;
            this->avilable = (bool)avilable; 
    }
     
    //����д���ļ��õĸ�ʽ���ַ��� 
    string toSequence(){
        char c[500];
        sprintf(c, "%s %d %s %s %d %d %d %d %d %d %d %d", 
                this->car_number.c_str(), (int)this->avilable, this->start_time.toSequence().c_str(), 
                this->end_time.toSequence().c_str(), this->from, this->start_city, this->end_city, 
                this->distance, (int)this->path_type, this->rest_num, this->price, this->init_num); 
        return string(c);
    }
};



/*
path                   ʱ�̱����������·�������� 
path_num               ʱ�̱����������·����������ѭ��ʱ�õ� 

bool check(string car_number)   ���ʱ�̱����Ƿ����� car_number��Ӧ��·�������򷵻� true�����򷵻� false  
void addPath(Path p)            ��ʱ�̱������һ��·�� 
bool empty()                    path_numΪ 0���� true�����򷵻� false
bool deletePath(string, bool)   ���� car_numberɾ��һ��·���� truely_deleteΪ falseʱִ���߼�ɾ����Ϊ true����ɾ�� 
Path getPathByCarNumber(string) ���� car_number����һ�� path 

*/
class TimeTable {
public:
	Path* path;      //ʱ�̱����������·�������� 
	int path_num;    //ʱ�̱���������п���·����������ѭ��ʱ�õ�  
    int true_num;    //ʱ�̱�������·���������������߼�ɾ����·�����ļ�д��ʱ�õ��ò���������·��д���ļ� 

public:
    TimeTable():path_num(0), path(NULL), true_num(0){}
    
    //���ʱ�̱����Ƿ����� car_number��Ӧ��·�������򷵻� true�����򷵻� false 
    bool check(string car_number){
        for(int i=0; i<this->true_num; i++){
            if(this->path[i].car_number.compare(car_number) == 0){
                return true;
            }
        }
        return false;
    }
    
    //���� car_numberɾ��һ��·���� truely_deleteΪ falseʱִ���߼�ɾ����Ϊ true����ɾ�� 
    bool deletePath(string car_number, bool truely_delete){
        for(int i=0; i<this->true_num; i++){
            if(this->path[i].car_number.compare(car_number) == 0){
                if(truely_delete){//����ɾ�� 
                    for(int k=i; k<this->true_num; k++){
                        if(k != this->true_num - 1){
                            this->path[k] = this->path[k+1];
                        }
                    }
                    if(i < this->path_num){
                        this->path_num--;
                    }
                    this->true_num--;
                }
                else if(i < this->path_num){//�߼�ɾ�� ����ɾ����·���Ƶ�������棬���� path_num�� 1 
                    Path temp = this->path[i];
                    temp.avilable = false;
                    for(int k=i; k<this->true_num; k++){
                        if(k != this->true_num - 1){
                            this->path[k] = this->path[k+1];
                        }
                    }
                    this->path_num--;
                    this->path[this->true_num - 1] = temp;
                }
                else{
                    return false;
                }
                return true;
            }
        }
        return false;
    }
    
    //���� car_number��Ӧ��·������Ʊ 
    int updatePathRestNum(string car_number, int offset){
        if(this->check(car_number)){
            for(int i=0; i<this->path_num; i++){
                if(this->path[i].car_number.compare(car_number) == 0){
                    this->path[i].rest_num += offset;
                    return this->path[i].rest_num;
                }
            }
        } 
        return -1;
    }
    
    //�ָ�һ�����߼�ɾ����·����������ɾ����·���޷��ָ�
    bool reservePath(string car_number){
        for(int i=this->path_num; i<this->true_num; i++){
            if(this->path[i].car_number.compare(car_number) == 0 && !this->path[i].avilable){
                Path temp = this->path[i];
                temp.avilable = true;
                for(int k=i; k>this->path_num; k--){
                    this->path[k] = this->path[k - 1];
                }
                this->path[this->path_num] = temp;
                this->path_num++;
                return true;
            }
        }
        return false;
    } 
    
    //��ʱ�̱������һ��·�� 
    void addPath(Path p){
        if(this->path == NULL){
        	
            this->path = new Path[this->path_num + 1];
            this->path[this->path_num] = p;
        }
        else{
            //���� car_number���·���Ƿ��Ѵ��ڣ���·���Ѵ����򷵻� 
            for(int i=0; i<this->true_num; i++){
                if(this->path[i].car_number.compare(p.car_number) == 0){
                    return;
                }
            } 
            
            Path* temp = new Path[this->true_num + 1];
            if(p.avilable){
                for(int i=0; i<this->true_num; i++){
                    temp[i] = this->path[i];
                }
                for(int i=this->true_num; i>this->path_num; i--){
                    temp[i] = temp[i - 1];
                }
                temp[this->path_num] = p;
            }
            else{
                for(int i=0; i<this->true_num; i++){
                    temp[i] = this->path[i];
                }
                temp[this->true_num] = p;
            }
            delete[] this->path;
            this->path = temp;
        }
        this->true_num++;
        if(p.avilable){
            this->path_num++;
        }
    }
    
    //���� car_number����һ�� path 
    Path getPathByCarNumber(string car_number){
        for(int i=0; i<this->true_num; i++){
            if(this->path[i].car_number.compare(car_number) == 0){
                return this->path[i];
            }
        }
    }
    
    //path_numΪ 0���� true�����򷵻� false
    bool empty(){
        return this->path_num == 0;
    } 
};

/*
id          ���� id��ÿ�����е� idΨһ 
name        �������֣�һ�㲻������غϵģ�������ǿ��Ҫ��Ψһ 
time_table  ÿ�����еķ���ʱ�̱� 
*/
class City {
public:
    int id;
    string name;
    TimeTable time_table;
};

/*
Map()                          ���ļ���ȡ���ݳ�ʼ�� Map 
void dump()                    �� cities��path����ȫ��д���ļ�
bool deletePath(string, bool)  ���� car_numberɾ��һ��·���� truely_deleteΪ falseʱִ���߼�ɾ����Ϊ true����ɾ��  
void displayPathByCity(int)    ��ӡ city_id��Ӧ�ĳ��е�����·��
void printPath(int, string)    ���� city_id�� car_number��ӡһ��·��
void printReserveablePath()    ��ӡ�ɻָ���·��, int����ʼ���� id 
bool reservePath(string)       �ָ�һ�����߼�ɾ����·����������ɾ����·���޷��ָ� 
int updatePathRestNum(string, int, int) 
                               �޸��� start_cityΪ����� car_number��Ӧ��·���ϵ���Ʊ 
*/
class Map {                                                                                                                                                                                                                                                                                                                        
public:
    City* cities;
    int** matrix;
    int n;
    
    const string city_file_name;
    const string path_file_name;
    
public:
    Map():cities(NULL), matrix(NULL), n(0), city_file_name("city.txt"), path_file_name("path.txt"){
        //��ʼ�� cities
        ifstream city_file(this->city_file_name.c_str());
        City* cities;
        if(city_file.good()){
            int count = 0;
            int id[100];
            string name[100];
            string last = "";
            while(!city_file.eof()){
                city_file >> id[count] >> name[count];
                if(last.compare(name[count]) != 0){
                	count++;
				}
            }
            city_file.close();
            this->n = count;
            this->cities = new City[count];
            for(int i=0; i<count; i++){
                this->cities[i].id = id[i];
                this->cities[i].name = name[i];
            }
        }
        else{
            throw string("�ļ� city.txt�����ڡ�");
        }
        
        //��ʼ��ÿ�����е�ʱ�̱�
        //�ļ�����ʾ��
        //car_number avilable start_time from start_city end_city distance path_type rest_num
        //K1234 1 201901132213 -1 1 2 50 0 50 
        ifstream  path_file(this->path_file_name.c_str());
        if(path_file.good()){
            string car_number;
            string start_time;
            string end_time;
            int avilable;
            int from;
            int start_city;
            int end_city;
            int distance;
            int price;
            int path_type;
            int rest_num;
            int init_num;
            
            while(!path_file.eof()){
                path_file >> car_number >> avilable >> start_time >> end_time >> from >> start_city 
                          >> end_city >> distance >> path_type >> rest_num >> price >> init_num;

                Path p(car_number, avilable, start_time, end_time, from, start_city, end_city, 
                       distance, path_type, rest_num, price, init_num);
                this->cities[start_city].time_table.addPath(p);
            }
            path_file.close(); 
        }
        else{
            throw string("�ļ� path.txt�����ڡ�");
        }
        
        //��ʼ�� cities_map
        int** matrix = new int*[this->n];
        for(int i=0; i<this->n; i++){
            matrix[i] = new int[this->n];
        }
        for(int i=0; i<this->n; i++){
            int path_num = this->cities[i].time_table.path_num;
            for(int k=0; k<path_num; k++){
                int end_city = this->cities[i].time_table.path[k].end_city;
                matrix[i][end_city] = 1;
                matrix[end_city][i] = 1;
            }
        } 
        for(int i=0; i<this->n; i++){
            for(int k=0; k<this->n; k++){
                if(matrix[i][k] != 1){
                    matrix[i][k] = 0;
                }
            }
        }
        this->matrix = matrix;
    }

    ~Map(){
        if(this->matrix != NULL){
            for(int i=0; i<this->n; i++){
                delete[] this->matrix[i];
            }
            delete[] this->matrix;
        }
        if(this->cities != NULL){
            delete[] this->cities;
        }
    }
    
    //�� citis��path����ȫ��д���ļ� 
    void dump(){
        if(this->cities != NULL){
            //д�� cities 
            ofstream city_file(this->city_file_name.c_str());
            for(int i=0; i<this->n; i++){
                city_file << this->cities[i].id << " "
                          << this->cities[i].name << "\n";
            } 
            city_file.close(); 
            
            //д�� path 
            ofstream path_file(this->path_file_name.c_str());
            for(int i=0; i<this->n; i++){
                int path_num = this->cities[i].time_table.true_num;
                for(int k=0; k<path_num; k++){
                    path_file << this->cities[i].time_table.path[k].toSequence() << "\n";
                }
            }
            path_file.close();
        }
    }
    
    
    //���� car_numberɾ��һ��·���� truely_deleteΪ falseʱִ���߼�ɾ����Ϊ true����ɾ�� 
    bool deletePath(string car_number, bool truely_delete=false){
        bool status = false;
        for(int i=0; i<this->n; i++){
            bool t = this->cities[i].time_table.deletePath(car_number, truely_delete);
            status = status || t;
        }
        //�����ļ� 
        if(status){
            this->dump();
        }
        return status;
    }
    
    //�ָ�һ�����߼�ɾ����·����������ɾ����·���޷��ָ� 
    bool reservePath(string car_number){
        bool status = false;
        for(int i=0; i<this->n; i++){
            bool t = this->cities[i].time_table.reservePath(car_number);
            status = status || t;
        } 
        if(status){
            this->dump();
        }
        return status;
    }
    
    //�޸��� start_cityΪ����� car_number��Ӧ��·���ϵ���Ʊ 
    int updatePathRestNum(string car_number, int start_city, int offset){
        if(start_city >= 0 && start_city < this->n && this->cities[start_city].time_table.check(car_number)){
            int t_city = start_city;
            int rest_num = 10000;
            while(this->cities[t_city].time_table.check(car_number)){
                int t = this->cities[t_city].time_table.updatePathRestNum(car_number, offset);
                t_city = this->cities[t_city].time_table.getPathByCarNumber(car_number).end_city;
                if(t < rest_num){
                    rest_num = t;
                }
            }
            this->dump();
            return rest_num;
        }
        else{
            return -1;
        }
    }
    
    //����һ�� Path���飬�� map�����һ��·��
    void addPath(Path path[], int n){
        for(int i=0; i<n; i++){
            int city_id = path[i].start_city;
            this->cities[city_id].time_table.addPath(path[i]);
        }
        this->dump();
    }
    
    //��ӡ�ɻָ���·��, int����ʼ���� id 
    bool printReserveablePath(){
        map<string, int> path_map;
        for(int i=0; i<this->n; i++){
            int path_num = this->cities[i].time_table.path_num;
            int true_num = this->cities[i].time_table.true_num;
            for(int k=path_num; k<true_num; k++){
                string car_number = this->cities[i].time_table.path[k].car_number;
                if(this->cities[i].time_table.path[k].from == -1){
                    path_map[car_number] = i;
                }
            }
        } 
        map<string, int>::iterator iter;
        if(path_map.begin() == path_map.end()){
            cout << "û�пɻָ���·��" << endl; 
            return false;
        }
        cout << "+----------------------------------------------\n";
        cout << "| ����  | ʱ��             | ״̬  |  ·��\n";
        for(iter = path_map.begin(); iter != path_map.end(); iter++){
            char c[200]; 
            Path p = this->cities[iter->second].time_table.getPathByCarNumber(iter->first);
            string status;
            if(p.avilable){
                status = "����";
            }
            else{
                status = "ɾ��";
            }
            cout << "+-------+------------------+-------+------------\n";
            sprintf(c, "| %-5s | %16s | %5s | ", 
                        iter->first.c_str(), p.start_time.toString().c_str(), status.c_str());
            cout << string(c);
            this->printPath(iter->second, iter->first);
        }
        cout << "+--------------------------------------------\n";
        return true;
    }
    
    //��ӡ���г��� 
    void printAllCities(){
        char c[50];
        for(int i=0; i<this->n; i++){
            if(i%5==0){
                cout << endl;
            }
            sprintf(c, "%3d:%7s ", this->cities[i].id, this->cities[i].name.c_str());
            cout << string(c);
        } 
        cout << endl;
    }
    
    //��Ӧ�ĳ����Ƿ���� 
    bool pathIsExist(string car_number){
    	map<string, int> path_map;
        for(int i=0; i<this->n; i++){
            int path_num = this->cities[i].time_table.path_num;
            int true_num = this->cities[i].time_table.true_num;
            for(int k=0; k<true_num; k++){
                string car_number = this->cities[i].time_table.path[k].car_number;
                if(this->cities[i].time_table.path[k].from == -1){
                    path_map[car_number] = i;
                }
            }
        } 
        map<string, int>::iterator iter;
        for(iter = path_map.begin(); iter != path_map.end(); iter++){
        	if(iter->first.compare(car_number) == 0 && 
			   this->cities[iter->second].time_table.getPathByCarNumber(iter->first).avilable == 1){
        		return true;
			}
		}
		return false;
	} 
    
    //��ӡ����·��, int����ʼ���� id 
    void printAllPath(){
        map<string, int> path_map;
        for(int i=0; i<this->n; i++){
            int path_num = this->cities[i].time_table.path_num;
            int true_num = this->cities[i].time_table.true_num;
            for(int k=0; k<true_num; k++){
                string car_number = this->cities[i].time_table.path[k].car_number;
                if(this->cities[i].time_table.path[k].from == -1){
                    path_map[car_number] = i;
                }
            }
        } 
        map<string, int>::iterator iter;
        if(path_map.begin() == path_map.end()){
            cout << "û��·��" << endl; 
            return;
        }
        cout << "+---------------------------------------------------\n";
        cout << "| ����  | ʱ��             | ��Ʊ | ״̬ |  ·��\n";
        for(iter = path_map.begin(); iter != path_map.end(); iter++){
            char c[200]; 
            Path p = this->cities[iter->second].time_table.getPathByCarNumber(iter->first);
            string status;
            if(p.avilable){
                status = "����";
            }
            else{
                status = "ɾ��";
            }
            cout << "+-------+------------------+------+------+------------\n";
            sprintf(c, "| %-5s | %16s | %-4d | %-4s | ", 
                        iter->first.c_str(), p.start_time.toString().c_str(), p.rest_num, status.c_str());
            cout << string(c);
            this->printPath(iter->second, iter->first);
        }
        cout << "+-----------------------------------------------\n";
    } 
    
    //��ӡ״̬������·��, int����ʼ���� id 
    void printExistPath(){
        map<string, int> path_map;
        for(int i=0; i<this->n; i++){
            int path_num = this->cities[i].time_table.path_num;
            int true_num = this->cities[i].time_table.true_num;
            for(int k=0; k<true_num; k++){
                string car_number = this->cities[i].time_table.path[k].car_number;
                if(this->cities[i].time_table.path[k].from == -1){
                    path_map[car_number] = i;
                }
            }
        } 
        map<string, int>::iterator iter;
        if(path_map.begin() == path_map.end()){
            cout << "û��·��" << endl; 
            return;
        }
        cout << "+---------------------------------------------------\n";
        cout << "| ����  | ʱ��             | ��Ʊ | ״̬ |  ·��\n";
        for(iter = path_map.begin(); iter != path_map.end(); iter++){
            char c[200]; 
            Path p = this->cities[iter->second].time_table.getPathByCarNumber(iter->first);
            string status;
            if(p.avilable){
                status = "����";
            }
            else{
                status = "ɾ��";
            }
            cout << "+-------+------------------+------+------+------------\n";
            sprintf(c, "| %-5s | %16s | %-4d | %-4s | ", 
                        iter->first.c_str(), p.start_time.toString().c_str(), p.rest_num, status.c_str());
            cout << string(c);
            this->printPath(iter->second, iter->first);
        }
        cout << "+-----------------------------------------------\n";
    } 

    
    //��ӡ city_id��Ӧ�ĳ��е�����·�� 
    void displayPathByCity(int city_id){
        if(city_id >= 0 || city_id < this->n){
            if(this->cities[city_id].time_table.empty()){
                cout << this->cities[city_id].name << ": �ó���û��·��" << endl;
            }
            else{
                int path_num = this->cities[city_id].time_table.path_num;
                cout << endl << this->cities[city_id].name << ":" << endl;
                for(int i=0; i<path_num; i++){
                    string car_number = this->cities[city_id].time_table.path[i].car_number;
                    this->printPath(city_id, car_number);
                }
                cout << endl;
            }
        }
        else{
            cout << "��ѡ��ĳ��в�����" << endl; 
        }
    }
    
    //���� city_id�� car_number��ӡһ��·�� 
    void printPath(int city_id, string car_number){
        if(city_id >= 0 || city_id < this->n){            
            cout << this->cities[city_id].name;
            int t_city_id = this->cities[city_id].time_table.getPathByCarNumber(car_number).end_city;
            while(this->cities[t_city_id].time_table.check(car_number)){
                cout << " -->" << this->cities[t_city_id].name;
                t_city_id = this->cities[t_city_id].time_table.getPathByCarNumber(car_number).end_city; 
            }
            cout << " -->" << this->cities[t_city_id].name << endl;
        }
        else{
            cout << "��ѡ��ĳ��л�·��������" << endl;
        } 
    } 
};

struct Gpath{
			int size;
			int nodes[maxn];
		};
struct HugeTicket{
	int money;
	int time;
	int trans;
	int stations;
	int initticket[maxn];
	int restticket[maxn];
	int stationmoney[maxn];
	string endtime[maxn];
	string starttime[maxn];
	string startcity[maxn];
	string totalpath[maxn];
	string carnumber[maxn];
}hugetickets[maxn];

//���ֱȽϷ�ʽ����ʱ�䣬���ѣ���ת�������бȽ� 
int cmp1(const struct HugeTicket nodea,const struct HugeTicket nodeb);
int cmp2(const struct HugeTicket nodea,const struct HugeTicket nodeb);
int cmp3(const struct HugeTicket nodea,const struct HugeTicket nodeb);
void hugeticketsinit(); 
/*
GΪ Graph��Ķ���
curΪ���ڵ�վ����
endΪĿ��վ��ı��
pathsΪ·������
pathsNumΪ·������ 
*/
void DFS(Graph &G,int cur,int end,struct Gpath paths[],int *pathsNum); 
void find_all();
bool selectCheck(Map &mp,struct Gpath paths[],int len,int priority,Date &startdate);
bool PathCheck(Map &mp,struct Gpath paths,int &flag,Date &startdate);
void selecttickets();
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
 
void find_all(Map &mp){
	int year;					//����ʱ����
	int month;					//����ʱ����
	int day;					//����ʱ����
	int hour;					//����ʱ��Сʱ 
	int minute; 				//����ʱ����� 
	char year_input[5];			//���ճ���ʱ����
	char month_input[5];		//���ճ���ʱ����
	char day_input[5];			//���ճ���ʱ����	
	char hour_input[5];			//���ճ���ʱ��Сʱ
	char minute_input[5];		//���ճ���ʱ����� 
	int path_type;				//��ͨ��ʽ
	char path_type_input[20];	//��ͨ��ʽ�������� 
	char infor_is_right;		//������Ϣ�Ƿ���ȷ
	
	cout << "\n1-��ѯ·��\n\n";
	cout<<"\n��������ĳ���ʱ��(�ÿո����ÿ������ �� �� Сʱ ����):"<<endl;
	cout<<" >";
	cin>>month_input>>day_input>>hour_input>>minute_input;
	month = atoi(month_input);
	day = atoi(day_input);
	hour = atoi(hour_input);
	minute = atoi(minute_input);
	while(month <= 0 || month > 12 || day <= 0 || day > 31 || hour < 0 || hour > 24 || minute < 0 || minute >= 60){
		cout<<"�����������������: ";
		cin>>month_input>>day_input>>hour_input>>minute_input;
		month = atoi(month_input);
		day = atoi(day_input);
		hour = atoi(hour_input);
		minute = atoi(minute_input);
	}
	Date start_time(2019, month, day, hour, minute);	//����date��
	
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
	n=mp.n;
	a=(int **)new int *[maxn]; 
	for(int i=0;i<maxn;i++){
		a[i]=new int [maxn];
	}
	
	int temp;
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			temp=mp.matrix[i][j];
			*((int *)a+i*n+j)=temp;
		}
	}
	Graph g(a,n);
	g.init();
	
	cout<<"\n�����б����£�\n";
	mp.printAllCities();
	cout << "\n��������������ţ�";
	cin>>start; 
	cout << "�������յ������ţ�";
    cin>>end; 
    while(start < 0 || start >= mp.n || end < 0 || end >= mp.n){
        cout << "��������������ţ�";
        cin>>start; 
        cout << "�������յ������ţ�";
        cin>>end;
    }
    
	DFS(g,start,end,paths,&pathsNum);
	if(pathsNum==0){
		cout<<"\n�������������֮��û��·��, ����������\n"<<endl; 
		cout<<"���� q�˳�, ����������";
		getchar();
		char c = getchar();
		system("cls");
		if(c != 'q'){
            find_all(mp);
        }
	}
	else{
	    getchar();
	    char c;
	    while(true){
//	    	int y;
//		for(int x=0;x<pathsNum;x++){
//			for(y=paths[x].size-1;y>0;y--){
//				cout<<paths[x].nodes[y]<<"->";
//			}
//			cout<<(paths[x].nodes[y])<<endl;
//		}
	        cout<<"\n��ѡ����Ҫ�ķ�����1.ʱ����� 2.�÷����� 3.��ת��������(���� q�˳���ѯ)\n"; 
        	c = getchar();
        	if(c >= '1' && c <= '3'){
        	    if(selectCheck(mp, paths, pathsNum, c-'0',start_time)){
            		selecttickets();
            	}
            	else{
            		cout<<"��ǰ����Ʊ�����ҿɴ��·��"<<endl;
            	}
            }
            else if(c == 'q'){
                system("cls");
                break; 
            }
            getchar();
        }
         
    }
	
	delete [] paths;
}

bool selectCheck(Map &mp,struct Gpath paths[],int len,int priority, Date &startdate){
	int flag=0;
	int cnt=0;
	for(int i=0;i<len;i++){
		if(!PathCheck(mp,paths[i],flag,startdate)){
			cnt++;
		}
	}

	if(cnt==len){
		//cout<<"selectCheck ʧ����"<<endl;
		return false;
	}
	else{
		if(priority==1){//��ʱ�������
			 sort(hugetickets,hugetickets+flag,cmp1);
		}
		else if(priority == 2) sort(hugetickets,hugetickets+flag,cmp2);//���÷ѵ�����
		else sort(hugetickets,hugetickets+flag,cmp3); //����ת���������� 
	}
	//cout<<"selectCheck�ɹ���"<<endl;
	return true; 
}

void selecttickets(){
	HugeTicket temp=hugetickets[0];
	int seat=0,myflag=0;
	int ticketcnt=0;
	for(int i=0;i<temp.stations;i++){
		myflag=i;
		seat= rand()%50+1;
		while(i<temp.stations-1&&temp.carnumber[i]==temp.carnumber[i+1]){
			//��ƱΪͬһ���ε���Сֵ 
			temp.restticket[i+1]=min(temp.restticket[i+1],temp.restticket[i]);
			temp.stationmoney[i+1]+=temp.stationmoney[i];
			i++;
		}
		tickets[ticketcnt++]=Ticket(temp.startcity[myflag],temp.totalpath[i],temp.starttime[myflag],temp.endtime[i],temp.stationmoney[i],temp.carnumber[i],seat,temp.restticket[i],temp.initticket[i]); 
	}
	for(int i=0;i<ticketcnt;i++){
		tickets[i].printTicket();
		cout<<endl;
	}
	//�ǵ��Ż� 
	cout<<"��������Ϊ:"<<temp.money<<"Ԫ"<<endl;
	cout<<"����ʱ��Ϊ:"<<temp.time<<"����"<<endl;
	cout<<"��ת����Ϊ:"<<temp.trans<<"��"<<endl;
	hugeticketsinit();
}

bool PathCheck(Map &mp,struct Gpath paths,int &flag,Date &startdate){
	s.clear();
	double starttime=0,endtime=0,costmoney=0;
	hugetickets[flag].money=0;
	hugetickets[flag].time=0;
	hugetickets[flag].trans=0;
	int costtime=0,k=0,station;
	City curCity;//Ϊ��ʱ�ĳ��� 
	station=0;//ָ��flag��������Ʊ��������վ�����
	hugetickets[flag].stations=0; 								  
	for(int i=paths.size-1;i>0;i--){
		station=hugetickets[flag].stations;
		//����㵽���յ�(���յ�ǰһ��) 
		curCity=mp.cities[paths.nodes[i]];//Ϊ��ʱ�ĳ���
		for(k=0;k<curCity.time_table.path_num;k++)
		{
		//��һ���Ӹó��г����ģ�����һ�������ƥ���·�� 
			if(curCity.time_table.path[k].end_city==mp.cities[paths.nodes[i-1]].id){
				break;//�ҵ����˳�ѭ�� 
			}
		}
		
		//û�е��ó��е�·�߻���û����Ʊ 
		if(k==curCity.time_table.path_num||curCity.time_table.path[k].rest_num==0){
			//cout<<"û����Ʊ����·�߲�����"<<endl;
			return false;
		}
		if(curCity.time_table.path[k].start_time.is_low(startdate)){
		      return false;
	    }
		//����ÿվ�����ۼ��ܽ��
		//cout<< curCity.time_table.path[k].price<<endl;
		hugetickets[flag].initticket[station]=curCity.time_table.path[k].init_num;
		hugetickets[flag].restticket[station]=curCity.time_table.path[k].rest_num;
		hugetickets[flag].stationmoney[station]=curCity.time_table.path[k].price;
		hugetickets[flag].money+=hugetickets[flag].stationmoney[station];
		//cout<<hugetickets[flag].money<<endl;
		//ÿվ�ĵ�����м�����ʱ��ͳ�������
		hugetickets[flag].startcity[station]=curCity.name;
		hugetickets[flag].starttime[station]=curCity.time_table.path[k].start_time.toString();
		hugetickets[flag].totalpath[station]=mp.cities[curCity.time_table.path[k].end_city].name; 
		//�ó�Ʊ����·��		 
		if(i==paths.size-1){
		//��� 
		starttime=curCity.time_table.path[k].start_time.day*24*60+curCity.time_table.path[k].start_time.hour*60+curCity.time_table.path[k].start_time.minute;
		}
		//endtime=curCity.time_table.path[k].start_time.day*24*60+curCity.time_table.path[k].start_time.hour*60+curCity.time_table.path[k].start_time.minute;
		if(i==1) //�յ�ʱ�� 
			endtime=curCity.time_table.path[k].end_time.day*24*60+curCity.time_table.path[k].end_time.hour*60+curCity.time_table.path[k].end_time.minute;	
		
		hugetickets[flag].endtime[station]=curCity.time_table.path[k].end_time.toString();
		if(station!=0){
			if(hugetickets[flag].endtime[station-1].compare(hugetickets[flag].starttime[station])>0){
				//cout<<"ʱ���ͻ"<<endl; 
				return false;//˵������ô����е�ʱ��Ҫ���ڴӸ�·�� �ӵ�ǰ���г�����ʱ�䣬
							//����·��Ҫ�ų� 			
			}
		} 
		hugetickets[flag].carnumber[station]=curCity.time_table.path[k].car_number;
		//����STL��set���� 
		s.insert(curCity.time_table.path[k].car_number);
		//������վ����+1 
		hugetickets[flag].stations++;
	}
	
	hugetickets[flag].trans=s.size()-1;//ת��վ��
    s.clear();//���set����������� 
    //��·�߻��ѵ�ʱ�� 
	costtime=(int)(endtime-starttime);
	
	hugetickets[flag].time=costtime;
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
void hugeticketsinit(){
    for(int i=0;i<maxn;i++){
		hugetickets[i].money=0;
		hugetickets[i].time=0;
		hugetickets[i].trans=0;
		hugetickets[i].stations=0;
	
		for(int j=0;j<maxn;j++){
			hugetickets[i].initticket[j]=0;
			hugetickets[i].restticket[j]=0;
			hugetickets[i].carnumber[j]="";
			hugetickets[i].endtime[j]="";
			hugetickets[i].startcity[j]="";
			hugetickets[i].starttime[j]="";
			hugetickets[i].totalpath[j]="";
			hugetickets[i].stationmoney[j]=0;
		}
	}
}
/*
**����·�ߵ������� 
*/ 
void fun_searchPath(Map & map_test){ 
	hugeticketsinit();
    Map cities_map;
	find_all(cities_map);
}


void fun_addPath(Map &map_test){
    int n,i;
    char n_input[5];
    string car_number;
    int type;
    char type_input[5];			 
    int year;					//ʱ����
	int month;					//ʱ����
	int day;					//ʱ����
	int hour;					//ʱ��Сʱ 
	int minute; 				//ʱ����� 
	char year_input[5];			//����ʱ����
	char month_input[5];		//����ʱ����
	char day_input[5];			//����ʱ����	
	char hour_input[5];			//����ʱ��Сʱ
	char minute_input[5];		//����ʱ�����
	
	cout << "\n2-���·��\n\n";
	cout<<"������;��վ��ĸ���(������"<<map_test.n<<"��, �������)��";
    cin>>n_input;
    n = atoi(n_input);
    while(n <= 1 || n > map_test.n){
    	cout<<"�����������������:";
    	cin>>n_input;
    	n = atoi(n_input);
	}
    Path tmp_path[n - 1];  		//����һ�˳���ྭ����ʮ��վ
    cout<<"�����뵱������(�� ��): ";
	cin>>month_input>>day_input;
	month = atoi(month_input);
	day = atoi(day_input);
	while(month <= 0 || month > 12 || day <= 0 || day > 31){
		cout<<"�����������������(�� ��): ";
		cin>>month_input>>day_input;
		month = atoi(month_input);
		day = atoi(day_input);
	}
	
    cout<<"�����뽻ͨ����(1-�ɻ���2-�𳵣�3-����)��";
    cin>>type_input;
	type = atoi(type_input);
	while(type != 1 && type != 2 && type != 3){
		cout<<"�����������������(1-�ɻ���2-�𳵣�3-����)��";
		cin>>type_input;
		type = atoi(type_input);
	}
	type = type - 1;
    
    map_test.printAllPath();
    
    cout<<"�����복�κ�(�����������г���):";
    cin>>car_number;

    for(i=0;i<n-1;i++){
    	tmp_path[i].avilable = 1;
        tmp_path[i].start_time.year = 2019;
        tmp_path[i].start_time.month = month;
        tmp_path[i].end_time.year = 2019;
        tmp_path[i].end_time.month = month;
        tmp_path[i].start_time.day = day;
        tmp_path[i].end_time.day=day;
        tmp_path[i].path_type = (Type)type;
        tmp_path[i].car_number = car_number;
    }
    
    map_test.printAllCities();
    
    int city_id;
	char city_id_input[5];
    cout<<"\n��������ʼ����id:";
    cin>>city_id_input;
    city_id = atoi(city_id_input);
    while(city_id < 0 || city_id >= 100){
    	cout<<"�����������������:";
    	cin>>city_id_input;
    	city_id = atoi(city_id_input);
	}
	tmp_path[0].start_city = city_id;
	tmp_path[0].from = -1;

    for(i = 0; i < n - 1; i++){
    	
    	cout<<"�������"<<tmp_path[i].start_city<<"������ʱ��(Сʱ ����):";
		cin>>hour_input>>minute_input;
		hour = atoi(hour_input);
		minute = atoi(minute_input);
		while(hour < 0 || hour > 24 || minute < 0 || minute >= 60){
			cout<<"�����������������(Сʱ ����):";
			cin>>hour_input>>minute_input;
			hour = atoi(hour_input);
			minute = atoi(minute_input);
		}
		tmp_path[i].start_time.hour = hour;
		tmp_path[i].start_time.minute = minute;
		
        cout<<"�����뵽��ĵ�"<<i+1<<"�����е�id:";
        cin>>city_id_input;
    	city_id = atoi(city_id_input);
    	while(city_id < 0 || city_id >= 100 || city_id == tmp_path[i].start_city){
    		cout<<"�����������������:";
    		cin>>city_id_input;
    		city_id = atoi(city_id_input);
		}
		tmp_path[i].end_city = city_id;
		
		cout<<"������"<<tmp_path[i].end_city<<"�����ʱ��(Сʱ ����):";
		cin>>hour_input>>minute_input;
		hour = atoi(hour_input);
		minute = atoi(minute_input);
		while(hour < 0 || hour > 24 || minute < 0 || minute >= 60){
			cout<<"�����������������(Сʱ ����):";
			cin>>hour_input>>minute_input;
			hour = atoi(hour_input);
			minute = atoi(minute_input);
		}
		tmp_path[i].end_time.hour = hour;
		tmp_path[i].end_time.minute = minute;
		
		int distence;
		char distence_input[10];
        cout<<"�������"<<tmp_path[i].start_city<<"��"<<tmp_path[i].end_city<<"�ľ��룺";
        cin>>distence_input;
    	distence = atoi(distence_input);
    	while(distence <= 0){
    		cout<<"�����������������"<<endl;
    		cin>>distence_input;
    		distence = atoi(distence_input);
		}
		tmp_path[i].distance = distence;
		
		int money1;
		char money1_input[10];
        cout<<"�������"<<tmp_path[i].start_city<<"��"<<tmp_path[i].end_city<<"��Ʊ�ۣ�";
        cin>>money1_input;
    	money1 = atoi(money1_input);
    	while(money1 <= 0){
    		cout<<"�����������������:"<<endl;
    		cin>>money1_input;
    		money1 = atoi(money1_input);
		}
		tmp_path[i].price = money1;

		int rest_num;
		char rest_num_input[5];
        cout<<"�������"<<tmp_path[i].start_city<<"��"<<tmp_path[i].end_city<<"����Ʊ������";
        cin>>rest_num_input;
    	rest_num = atoi(rest_num_input);
    	while(rest_num <= 0){
    		cout<<"�����������������"<<endl;
    		cin>>rest_num_input;
    		rest_num = atoi(rest_num_input);
		}
		tmp_path[i].rest_num = rest_num;
		tmp_path[i].init_num=rest_num;
		//ǰһ��path��start_city�����path��from
		if(i != 0){
			tmp_path[i].from = tmp_path[i - 1].start_city; 
		}
		
        if(i != n - 1){
			//ǰһ��path�ĵ����������һ��path�ĳ�������
            tmp_path[i+1].start_city = tmp_path[i].end_city;
        }
    }
    map_test.addPath(tmp_path, n - 1);
    cout<<"\n���·�����!\n";
    map_test.printAllPath();
    cout<<"\n";
    system("pause");
    system("cls");
}
/*
**�޸�·����������
*/
void fun_adjustPath(Map & map_test){
    //��Ҫ��ʾ��¼��Ϣ  ����ȷ����ʾ
    //���복�κ� ������ �޸����path�Ķ��Ʊ�� 
    cout<<"\n3-�޸���Ʊ��\n\n";

    string car_number;
    int start_city;
    int rest_num;
    int adjust_rest_num;
    char start_city_input[5];
    char rest_num_input[5];
    
    map_test.printExistPath();
    
    cout<<"\n������Ҫ�޸ĵĳ��κ�:";
    cin>>car_number;
    while(!map_test.pathIsExist(car_number)){
    	cout<<"�����������ĳ��Σ�����������:";
    	cin>>car_number;
	}
    map_test.printAllCities();
    
    cout<<"\n������Ҫ�޸ĵ���ʼ�������:";
    cin>>start_city_input;
    start_city = atoi(start_city_input);
	int before_rest_num = map_test.updatePathRestNum(car_number, start_city, 0);
    while(start_city < 0 || start_city >= map_test.n || before_rest_num < 0){
    	cout<<"�����������������:";
    	cin>>start_city_input;
    	start_city = atoi(start_city_input);
    	before_rest_num = map_test.updatePathRestNum(car_number, start_city, 0);
	}
	cout<<"��ǰ�˶���Ʊ����Ϊ:"<<before_rest_num<<"Ʊ\n";
	cout<<"�������޸ĵ���Ʊ����:(�����������Ʊ����0����Ʊ�����䣬������������Ʊ��):";
   	cin>>rest_num_input;
   	rest_num = atoi(rest_num_input);
   	adjust_rest_num = map_test.updatePathRestNum(car_number, start_city, rest_num);
   	cout<<"�޸ĺ󣬸�·�λ�ʣ��"<<adjust_rest_num<<"��Ʊ"<<endl; 
   	cout<<"\n";
	system("pause");
	system("cls");
}
/*
**ɾ��·����������
*/
void fun_deletePath(Map & map_test){
   	int truely_delete;
    char truely_delete_input[5];
    string car_number;

    cout<<"\n4-ɾ��·��\n\n";
	
	map_test.printAllPath();
	
    cout<<"��ѡ��ɾ������ 1-�߼�ɾ�� 2-����ɾ�� 3-�������˵�:";
    cin>>truely_delete_input;
    truely_delete = atoi(truely_delete_input);

    while(truely_delete < 1||truely_delete>3){
        cout<<"�����������������:";
        cin>>truely_delete_input;
        truely_delete = atoi(truely_delete_input);
    }
    if(truely_delete == 3){
    	system("cls");
    	return;
	}
	truely_delete = truely_delete - 1;

    cout<<"������Ҫɾ���ĳ��κ�(�߼�ɾ��ֻ��ɾ��״̬�����ĳ���):";
    cin>>car_number;
    
    //����truely_delete����deletePath��ɾ������
    if(map_test.deletePath(car_number, truely_delete)) {
    	if(truely_delete){
    		cout<<"����ɾ���ɹ���"<<endl;
		}   
    	else {
    		cout<<"�߼�ɾ���ɹ���"<<endl;
		}
	}   
    else cout<<"û�иó��κ�!"<<endl;
    cout<<"\n";
	system("pause");
	system("cls"); 
}
/*
**�ָ��߼�ɾ����·����
*/
void fun_recoverPath(Map & map_test){
	cout<<"\n5-�ָ�·��\n\n";   
    if(map_test.printReserveablePath()){ 	//��ʾ���߼�ɾ����·��
    	string car_number;
    	cout<<"\n������Ҫ�ָ��ĳ��κ�:";
    	cin>>car_number;
		if(map_test.reservePath(car_number)) {
    		cout<<"�ָ���ɣ�"<<endl;
		}   
		else cout<<"û�иó��κ�!"<<endl;
	}
	cout<<"\n";
	system("pause");
	system("cls");
}

void chooseMenu(Map &map_test){
	char choose[20];
	char c[100];
	cout << "\n��ӭʹ�ó�Ʊ������ѯϵͳ��\n";
	cout << "\n+--------------------------------------------+\n";
	sprintf(c, "|%-14s %-14s %-14s|\n|%-14s %-14s %-14s|\n",
             "1-��ѯ��Ʊ��Ϣ", "2-���·��", "3-�޸���Ʊ��", "4-ɾ��·��", "5-�ָ�·��", "6-�˳�ϵͳ");
    cout << string(c);
    cout << "+--------------------------------------------+\n\n";
    cout<<"��ѡ������Ҫ�Ĺ���: ";
	cin>>choose;
	system("cls");
	int final_choose = atoi(choose);
	if(final_choose>=1 && final_choose<=6){
		switch(final_choose){
		case 1:
			fun_searchPath(map_test);
			break;
		case 2:
			fun_addPath(map_test);
			break;
		case 3:
			fun_adjustPath(map_test);
			break;
		case 4:
			fun_deletePath(map_test);
			break;
		case 5:
			fun_recoverPath(map_test);
			break;
		case 6:
			exit(1);
			break;
		}
	}
	else{
		cout<<"�����������������"<<endl;
		while(true){
			chooseMenu(map_test);
		}
	}
}

int main(){
	Map map_test;  		//��ʼ��map����
	while(true){
		chooseMenu(map_test);	//ִ�����˵� 
	}
	return 0;
}

