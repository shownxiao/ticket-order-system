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
/*日期类
bool is_low(Date d)    当前日期比 d小（早）返回 true，否则返回 false
string toString()      返回日期的格式化字符  如: 2018-01-11 16:03 
string toSequence()    返回日期的数字序列，用于文件写入，如：201801111603 
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
    //从字符串序列初始化，如：201901122212 
    Date(string date_string){
        this->year = atoi(date_string.substr(0,4).c_str());
        this->month = atoi(date_string.substr(4,2).c_str());
        this->day = atoi(date_string.substr(6,2).c_str());
        this->hour = atoi(date_string.substr(8,2).c_str());
        this->minute = atoi(date_string.substr(10,2).c_str());
    }
    
    //is_low(Date d) 当前日期比 d小（早）返回 true，否则返回 false
    bool is_low(Date d) {
        return this->toSequence().compare(d.toSequence()) < 0 ? true:false;
    }
    
    //将日期转换为打印格式，如：2019-01-13 22:13 
    string toString() {
        char c[16];
        sprintf(c, "%4d-%02d-%02d %02d:%02d", this->year, this->month,
            this->day, this->hour, this->minute);
        return string(c);
    }
    
    //将日期转换为文件写入格式，如：201901132213
    string  toSequence(){
        char c[12];
        sprintf(c, "%4d%02d%02d%02d%02d", this->year, this->month,
            this->day, this->hour, this->minute);
        return string(c);
    }
};

/*
string toSequence()    返回文件写入的格式化字符串
*/ 
class Path {
public:
    string car_number; //车次号 
    Type path_type;    //交通类型 PLANE，TRAIN，CAR 
    Date start_time;   //发车时间 
    Date end_time;     //到站时间 
    bool avilable;     //删除标记，true表示未删除，false表示已删除 
    int from;          //发车城市 
    int start_city;    //出发城市 
    int end_city;      //到达城市 
    int price;         //价格 
    int distance;      //行程距离 
    int rest_num;      //余票数量   
    int init_num;      //额定票数 
    
public:
    Path(){}
    //从文件读取序列初始化 Path
    //示例:
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
     
    //返回写入文件用的格式化字符串 
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
path                   时刻表包含的所有路径的数组 
path_num               时刻表包含的所有路径的数量，循环时用到 

bool check(string car_number)   检查时刻表中是否有与 car_number对应的路径，有则返回 true，否则返回 false  
void addPath(Path p)            向时刻表中添加一条路径 
bool empty()                    path_num为 0返回 true，否则返回 false
bool deletePath(string, bool)   根据 car_number删除一条路径， truely_delete为 false时执行逻辑删除，为 true彻底删除 
Path getPathByCarNumber(string) 根据 car_number返回一个 path 

*/
class TimeTable {
public:
	Path* path;      //时刻表包含的所有路径的数组 
	int path_num;    //时刻表包含的所有可用路径的数量，循环时用到  
    int true_num;    //时刻表中所有路径的数量，包含逻辑删除的路径，文件写入时用到该参数将所有路径写入文件 

public:
    TimeTable():path_num(0), path(NULL), true_num(0){}
    
    //检查时刻表中是否有与 car_number对应的路径，有则返回 true，否则返回 false 
    bool check(string car_number){
        for(int i=0; i<this->true_num; i++){
            if(this->path[i].car_number.compare(car_number) == 0){
                return true;
            }
        }
        return false;
    }
    
    //根据 car_number删除一条路径， truely_delete为 false时执行逻辑删除，为 true彻底删除 
    bool deletePath(string car_number, bool truely_delete){
        for(int i=0; i<this->true_num; i++){
            if(this->path[i].car_number.compare(car_number) == 0){
                if(truely_delete){//物理删除 
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
                else if(i < this->path_num){//逻辑删除 将被删除的路径移到数组后面，并且 path_num减 1 
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
    
    //更新 car_number对应的路径的余票 
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
    
    //恢复一条被逻辑删除的路径，被物理删除的路径无法恢复
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
    
    //向时刻表中添加一条路径 
    void addPath(Path p){
        if(this->path == NULL){
        	
            this->path = new Path[this->path_num + 1];
            this->path[this->path_num] = p;
        }
        else{
            //根据 car_number检查路径是否已存在，若路径已存在则返回 
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
    
    //根据 car_number返回一个 path 
    Path getPathByCarNumber(string car_number){
        for(int i=0; i<this->true_num; i++){
            if(this->path[i].car_number.compare(car_number) == 0){
                return this->path[i];
            }
        }
    }
    
    //path_num为 0返回 true，否则返回 false
    bool empty(){
        return this->path_num == 0;
    } 
};

/*
id          城市 id，每个城市的 id唯一 
name        城市名字，一般不会出现重合的，但并不强制要求唯一 
time_table  每座城市的发车时刻表 
*/
class City {
public:
    int id;
    string name;
    TimeTable time_table;
};

/*
Map()                          从文件读取数据初始化 Map 
void dump()                    将 cities，path数据全部写入文件
bool deletePath(string, bool)  根据 car_number删除一条路径， truely_delete为 false时执行逻辑删除，为 true彻底删除  
void displayPathByCity(int)    打印 city_id对应的城市的所有路径
void printPath(int, string)    根据 city_id和 car_number打印一条路径
void printReserveablePath()    打印可恢复的路径, int是起始城市 id 
bool reservePath(string)       恢复一条被逻辑删除的路径，被物理删除的路径无法恢复 
int updatePathRestNum(string, int, int) 
                               修改以 start_city为起点与 car_number对应的路径上的余票 
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
        //初始化 cities
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
            throw string("文件 city.txt不存在。");
        }
        
        //初始化每座城市的时刻表
        //文件保存示例
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
            throw string("文件 path.txt不存在。");
        }
        
        //初始化 cities_map
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
    
    //将 citis，path数据全部写入文件 
    void dump(){
        if(this->cities != NULL){
            //写入 cities 
            ofstream city_file(this->city_file_name.c_str());
            for(int i=0; i<this->n; i++){
                city_file << this->cities[i].id << " "
                          << this->cities[i].name << "\n";
            } 
            city_file.close(); 
            
            //写入 path 
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
    
    
    //根据 car_number删除一条路径， truely_delete为 false时执行逻辑删除，为 true彻底删除 
    bool deletePath(string car_number, bool truely_delete=false){
        bool status = false;
        for(int i=0; i<this->n; i++){
            bool t = this->cities[i].time_table.deletePath(car_number, truely_delete);
            status = status || t;
        }
        //保存文件 
        if(status){
            this->dump();
        }
        return status;
    }
    
    //恢复一条被逻辑删除的路径，被物理删除的路径无法恢复 
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
    
    //修改以 start_city为起点与 car_number对应的路径上的余票 
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
    
    //接受一个 Path数组，向 map中添加一条路径
    void addPath(Path path[], int n){
        for(int i=0; i<n; i++){
            int city_id = path[i].start_city;
            this->cities[city_id].time_table.addPath(path[i]);
        }
        this->dump();
    }
    
    //打印可恢复的路径, int是起始城市 id 
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
            cout << "没有可恢复的路径" << endl; 
            return false;
        }
        cout << "+----------------------------------------------\n";
        cout << "| 车次  | 时间             | 状态  |  路线\n";
        for(iter = path_map.begin(); iter != path_map.end(); iter++){
            char c[200]; 
            Path p = this->cities[iter->second].time_table.getPathByCarNumber(iter->first);
            string status;
            if(p.avilable){
                status = "正常";
            }
            else{
                status = "删除";
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
    
    //打印所有城市 
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
    
    //对应的车次是否存在 
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
    
    //打印所有路径, int是起始城市 id 
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
            cout << "没有路径" << endl; 
            return;
        }
        cout << "+---------------------------------------------------\n";
        cout << "| 车次  | 时间             | 余票 | 状态 |  路线\n";
        for(iter = path_map.begin(); iter != path_map.end(); iter++){
            char c[200]; 
            Path p = this->cities[iter->second].time_table.getPathByCarNumber(iter->first);
            string status;
            if(p.avilable){
                status = "正常";
            }
            else{
                status = "删除";
            }
            cout << "+-------+------------------+------+------+------------\n";
            sprintf(c, "| %-5s | %16s | %-4d | %-4s | ", 
                        iter->first.c_str(), p.start_time.toString().c_str(), p.rest_num, status.c_str());
            cout << string(c);
            this->printPath(iter->second, iter->first);
        }
        cout << "+-----------------------------------------------\n";
    } 
    
    //打印状态正常的路径, int是起始城市 id 
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
            cout << "没有路径" << endl; 
            return;
        }
        cout << "+---------------------------------------------------\n";
        cout << "| 车次  | 时间             | 余票 | 状态 |  路线\n";
        for(iter = path_map.begin(); iter != path_map.end(); iter++){
            char c[200]; 
            Path p = this->cities[iter->second].time_table.getPathByCarNumber(iter->first);
            string status;
            if(p.avilable){
                status = "正常";
            }
            else{
                status = "删除";
            }
            cout << "+-------+------------------+------+------+------------\n";
            sprintf(c, "| %-5s | %16s | %-4d | %-4s | ", 
                        iter->first.c_str(), p.start_time.toString().c_str(), p.rest_num, status.c_str());
            cout << string(c);
            this->printPath(iter->second, iter->first);
        }
        cout << "+-----------------------------------------------\n";
    } 

    
    //打印 city_id对应的城市的所有路径 
    void displayPathByCity(int city_id){
        if(city_id >= 0 || city_id < this->n){
            if(this->cities[city_id].time_table.empty()){
                cout << this->cities[city_id].name << ": 该城市没有路径" << endl;
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
            cout << "所选择的城市不存在" << endl; 
        }
    }
    
    //根据 city_id和 car_number打印一条路径 
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
            cout << "所选择的城市或路径不存在" << endl;
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

//三种比较方式，按时间，花费，中转次数进行比较 
int cmp1(const struct HugeTicket nodea,const struct HugeTicket nodeb);
int cmp2(const struct HugeTicket nodea,const struct HugeTicket nodeb);
int cmp3(const struct HugeTicket nodea,const struct HugeTicket nodeb);
void hugeticketsinit(); 
/*
G为 Graph类的对象
cur为现在的站点编号
end为目的站点的编号
paths为路径数组
pathsNum为路径条数 
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
	int year;					//出发时间年
	int month;					//出发时间月
	int day;					//出发时间日
	int hour;					//出发时间小时 
	int minute; 				//出发时间分钟 
	char year_input[5];			//接收出发时间年
	char month_input[5];		//接收出发时间月
	char day_input[5];			//接收出发时间日	
	char hour_input[5];			//接收出发时间小时
	char minute_input[5];		//接收出发时间分钟 
	int path_type;				//交通方式
	char path_type_input[20];	//交通方式接收数据 
	char infor_is_right;		//输入信息是否正确
	
	cout << "\n1-查询路线\n\n";
	cout<<"\n请输入你的出发时间(用空格隔开每个输入 月 日 小时 分钟):"<<endl;
	cout<<" >";
	cin>>month_input>>day_input>>hour_input>>minute_input;
	month = atoi(month_input);
	day = atoi(day_input);
	hour = atoi(hour_input);
	minute = atoi(minute_input);
	while(month <= 0 || month > 12 || day <= 0 || day > 31 || hour < 0 || hour > 24 || minute < 0 || minute >= 60){
		cout<<"输入错误！请重新输入: ";
		cin>>month_input>>day_input>>hour_input>>minute_input;
		month = atoi(month_input);
		day = atoi(day_input);
		hour = atoi(hour_input);
		minute = atoi(minute_input);
	}
	Date start_time(2019, month, day, hour, minute);	//构造date类
	
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
	
	cout<<"\n城市列表如下：\n";
	mp.printAllCities();
	cout << "\n请输入起点城市序号：";
	cin>>start; 
	cout << "请输入终点城市序号：";
    cin>>end; 
    while(start < 0 || start >= mp.n || end < 0 || end >= mp.n){
        cout << "请输入起点城市序号：";
        cin>>start; 
        cout << "请输入终点城市序号：";
        cin>>end;
    }
    
	DFS(g,start,end,paths,&pathsNum);
	if(pathsNum==0){
		cout<<"\n输入的两座城市之间没有路线, 请重新输入\n"<<endl; 
		cout<<"输入 q退出, 其他继续：";
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
	        cout<<"\n请选择需要的方案：1.时间最短 2.旅费最少 3.中转次数最少(输入 q退出查询)\n"; 
        	c = getchar();
        	if(c >= '1' && c <= '3'){
        	    if(selectCheck(mp, paths, pathsNum, c-'0',start_time)){
            		selecttickets();
            	}
            	else{
            		cout<<"当前无余票充足且可达的路线"<<endl;
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
		//cout<<"selectCheck 失败了"<<endl;
		return false;
	}
	else{
		if(priority==1){//按时间短排序
			 sort(hugetickets,hugetickets+flag,cmp1);
		}
		else if(priority == 2) sort(hugetickets,hugetickets+flag,cmp2);//按旅费低排序
		else sort(hugetickets,hugetickets+flag,cmp3); //按中转次数低排序 
	}
	//cout<<"selectCheck成功了"<<endl;
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
			//余票为同一车次的最小值 
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
	//记得优化 
	cout<<"所花费用为:"<<temp.money<<"元"<<endl;
	cout<<"所花时间为:"<<temp.time<<"分钟"<<endl;
	cout<<"中转次数为:"<<temp.trans<<"次"<<endl;
	hugeticketsinit();
}

bool PathCheck(Map &mp,struct Gpath paths,int &flag,Date &startdate){
	s.clear();
	double starttime=0,endtime=0,costmoney=0;
	hugetickets[flag].money=0;
	hugetickets[flag].time=0;
	hugetickets[flag].trans=0;
	int costtime=0,k=0,station;
	City curCity;//为此时的城市 
	station=0;//指第flag条连续车票所经过的站点个数
	hugetickets[flag].stations=0; 								  
	for(int i=paths.size-1;i>0;i--){
		station=hugetickets[flag].stations;
		//从起点到临终点(即终点前一个) 
		curCity=mp.cities[paths.nodes[i]];//为此时的城市
		for(k=0;k<curCity.time_table.path_num;k++)
		{
		//找一条从该城市出发的，与下一个结点相匹配的路径 
			if(curCity.time_table.path[k].end_city==mp.cities[paths.nodes[i-1]].id){
				break;//找到后退出循环 
			}
		}
		
		//没有到该城市的路线或者没有余票 
		if(k==curCity.time_table.path_num||curCity.time_table.path[k].rest_num==0){
			//cout<<"没有余票或者路线不存在"<<endl;
			return false;
		}
		if(curCity.time_table.path[k].start_time.is_low(startdate)){
		      return false;
	    }
		//计算每站金额和累加总金额
		//cout<< curCity.time_table.path[k].price<<endl;
		hugetickets[flag].initticket[station]=curCity.time_table.path[k].init_num;
		hugetickets[flag].restticket[station]=curCity.time_table.path[k].rest_num;
		hugetickets[flag].stationmoney[station]=curCity.time_table.path[k].price;
		hugetickets[flag].money+=hugetickets[flag].stationmoney[station];
		//cout<<hugetickets[flag].money<<endl;
		//每站的到达城市及出发时间和出发城市
		hugetickets[flag].startcity[station]=curCity.name;
		hugetickets[flag].starttime[station]=curCity.time_table.path[k].start_time.toString();
		hugetickets[flag].totalpath[station]=mp.cities[curCity.time_table.path[k].end_city].name; 
		//该车票的总路线		 
		if(i==paths.size-1){
		//起点 
		starttime=curCity.time_table.path[k].start_time.day*24*60+curCity.time_table.path[k].start_time.hour*60+curCity.time_table.path[k].start_time.minute;
		}
		//endtime=curCity.time_table.path[k].start_time.day*24*60+curCity.time_table.path[k].start_time.hour*60+curCity.time_table.path[k].start_time.minute;
		if(i==1) //终点时间 
			endtime=curCity.time_table.path[k].end_time.day*24*60+curCity.time_table.path[k].end_time.hour*60+curCity.time_table.path[k].end_time.minute;	
		
		hugetickets[flag].endtime[station]=curCity.time_table.path[k].end_time.toString();
		if(station!=0){
			if(hugetickets[flag].endtime[station-1].compare(hugetickets[flag].starttime[station])>0){
				//cout<<"时间冲突"<<endl; 
				return false;//说明到达该处城市的时间要晚于从该路径 从当前城市出发的时间，
							//即该路线要排除 			
			}
		} 
		hugetickets[flag].carnumber[station]=curCity.time_table.path[k].car_number;
		//这是STL的set容器 
		s.insert(curCity.time_table.path[k].car_number);
		//经过的站点数+1 
		hugetickets[flag].stations++;
	}
	
	hugetickets[flag].trans=s.size()-1;//转车站数
    s.clear();//清空set容器里的内容 
    //该路线花费的时间 
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
**查找路线的主函数 
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
    int year;					//时间年
	int month;					//时间月
	int day;					//时间日
	int hour;					//时间小时 
	int minute; 				//时间分钟 
	char year_input[5];			//接收时间年
	char month_input[5];		//接收时间月
	char day_input[5];			//接收时间日	
	char hour_input[5];			//接收时间小时
	char minute_input[5];		//接收时间分钟
	
	cout << "\n2-添加路线\n\n";
	cout<<"请输入途径站点的个数(不超过"<<map_test.n<<"个, 包括起点)：";
    cin>>n_input;
    n = atoi(n_input);
    while(n <= 1 || n > map_test.n){
    	cout<<"输入错误！请重新输入:";
    	cin>>n_input;
    	n = atoi(n_input);
	}
    Path tmp_path[n - 1];  		//假设一趟车最多经过二十个站
    cout<<"请输入当日日期(月 日): ";
	cin>>month_input>>day_input;
	month = atoi(month_input);
	day = atoi(day_input);
	while(month <= 0 || month > 12 || day <= 0 || day > 31){
		cout<<"输入错误！请重新输入(月 日): ";
		cin>>month_input>>day_input;
		month = atoi(month_input);
		day = atoi(day_input);
	}
	
    cout<<"请输入交通类型(1-飞机，2-火车，3-汽车)：";
    cin>>type_input;
	type = atoi(type_input);
	while(type != 1 && type != 2 && type != 3){
		cout<<"输入错误！请重新输入(1-飞机，2-火车，3-汽车)：";
		cin>>type_input;
		type = atoi(type_input);
	}
	type = type - 1;
    
    map_test.printAllPath();
    
    cout<<"请输入车次号(不能输入已有车次):";
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
    cout<<"\n请输入起始城市id:";
    cin>>city_id_input;
    city_id = atoi(city_id_input);
    while(city_id < 0 || city_id >= 100){
    	cout<<"输入错误！请重新输入:";
    	cin>>city_id_input;
    	city_id = atoi(city_id_input);
	}
	tmp_path[0].start_city = city_id;
	tmp_path[0].from = -1;

    for(i = 0; i < n - 1; i++){
    	
    	cout<<"请输入从"<<tmp_path[i].start_city<<"发车的时间(小时 分钟):";
		cin>>hour_input>>minute_input;
		hour = atoi(hour_input);
		minute = atoi(minute_input);
		while(hour < 0 || hour > 24 || minute < 0 || minute >= 60){
			cout<<"输入错误！请重新输入(小时 分钟):";
			cin>>hour_input>>minute_input;
			hour = atoi(hour_input);
			minute = atoi(minute_input);
		}
		tmp_path[i].start_time.hour = hour;
		tmp_path[i].start_time.minute = minute;
		
        cout<<"请输入到达的第"<<i+1<<"个城市的id:";
        cin>>city_id_input;
    	city_id = atoi(city_id_input);
    	while(city_id < 0 || city_id >= 100 || city_id == tmp_path[i].start_city){
    		cout<<"输入错误！请重新输入:";
    		cin>>city_id_input;
    		city_id = atoi(city_id_input);
		}
		tmp_path[i].end_city = city_id;
		
		cout<<"请输入"<<tmp_path[i].end_city<<"到达的时间(小时 分钟):";
		cin>>hour_input>>minute_input;
		hour = atoi(hour_input);
		minute = atoi(minute_input);
		while(hour < 0 || hour > 24 || minute < 0 || minute >= 60){
			cout<<"输入错误！请重新输入(小时 分钟):";
			cin>>hour_input>>minute_input;
			hour = atoi(hour_input);
			minute = atoi(minute_input);
		}
		tmp_path[i].end_time.hour = hour;
		tmp_path[i].end_time.minute = minute;
		
		int distence;
		char distence_input[10];
        cout<<"请输入从"<<tmp_path[i].start_city<<"到"<<tmp_path[i].end_city<<"的距离：";
        cin>>distence_input;
    	distence = atoi(distence_input);
    	while(distence <= 0){
    		cout<<"输入错误！请重新输入"<<endl;
    		cin>>distence_input;
    		distence = atoi(distence_input);
		}
		tmp_path[i].distance = distence;
		
		int money1;
		char money1_input[10];
        cout<<"请输入从"<<tmp_path[i].start_city<<"到"<<tmp_path[i].end_city<<"的票价：";
        cin>>money1_input;
    	money1 = atoi(money1_input);
    	while(money1 <= 0){
    		cout<<"输入错误！请重新输入:"<<endl;
    		cin>>money1_input;
    		money1 = atoi(money1_input);
		}
		tmp_path[i].price = money1;

		int rest_num;
		char rest_num_input[5];
        cout<<"请输入从"<<tmp_path[i].start_city<<"到"<<tmp_path[i].end_city<<"的余票数量：";
        cin>>rest_num_input;
    	rest_num = atoi(rest_num_input);
    	while(rest_num <= 0){
    		cout<<"输入错误！请重新输入"<<endl;
    		cin>>rest_num_input;
    		rest_num = atoi(rest_num_input);
		}
		tmp_path[i].rest_num = rest_num;
		tmp_path[i].init_num=rest_num;
		//前一个path的start_city是这个path的from
		if(i != 0){
			tmp_path[i].from = tmp_path[i - 1].start_city; 
		}
		
        if(i != n - 1){
			//前一个path的到达城市是下一个path的出发城市
            tmp_path[i+1].start_city = tmp_path[i].end_city;
        }
    }
    map_test.addPath(tmp_path, n - 1);
    cout<<"\n添加路线完成!\n";
    map_test.printAllPath();
    cout<<"\n";
    system("pause");
    system("cls");
}
/*
**修改路径的主函数
*/
void fun_adjustPath(Map & map_test){
    //需要显示纪录信息  给出确认提示
    //输入车次号 出发地 修改这个path的额度票数 
    cout<<"\n3-修改余票数\n\n";

    string car_number;
    int start_city;
    int rest_num;
    int adjust_rest_num;
    char start_city_input[5];
    char rest_num_input[5];
    
    map_test.printExistPath();
    
    cout<<"\n请输入要修改的车次号:";
    cin>>car_number;
    while(!map_test.pathIsExist(car_number)){
    	cout<<"不存在这样的车次，请重新输入:";
    	cin>>car_number;
	}
    map_test.printAllCities();
    
    cout<<"\n请输入要修改的起始城市序号:";
    cin>>start_city_input;
    start_city = atoi(start_city_input);
	int before_rest_num = map_test.updatePathRestNum(car_number, start_city, 0);
    while(start_city < 0 || start_city >= map_test.n || before_rest_num < 0){
    	cout<<"输入错误！请重新输入:";
    	cin>>start_city_input;
    	start_city = atoi(start_city_input);
    	before_rest_num = map_test.updatePathRestNum(car_number, start_city, 0);
	}
	cout<<"当前此段余票数量为:"<<before_rest_num<<"票\n";
	cout<<"请输入修改的余票数量:(负数代表减少票数，0代表票数不变，正数代表增加票数):";
   	cin>>rest_num_input;
   	rest_num = atoi(rest_num_input);
   	adjust_rest_num = map_test.updatePathRestNum(car_number, start_city, rest_num);
   	cout<<"修改后，该路段还剩余"<<adjust_rest_num<<"张票"<<endl; 
   	cout<<"\n";
	system("pause");
	system("cls");
}
/*
**删除路径的主函数
*/
void fun_deletePath(Map & map_test){
   	int truely_delete;
    char truely_delete_input[5];
    string car_number;

    cout<<"\n4-删除路径\n\n";
	
	map_test.printAllPath();
	
    cout<<"请选择删除方法 1-逻辑删除 2-物理删除 3-返回主菜单:";
    cin>>truely_delete_input;
    truely_delete = atoi(truely_delete_input);

    while(truely_delete < 1||truely_delete>3){
        cout<<"输入错误！请重新输入:";
        cin>>truely_delete_input;
        truely_delete = atoi(truely_delete_input);
    }
    if(truely_delete == 3){
    	system("cls");
    	return;
	}
	truely_delete = truely_delete - 1;

    cout<<"请输入要删除的车次号(逻辑删除只能删除状态正常的车次):";
    cin>>car_number;
    
    //根据truely_delete调用deletePath的删除功能
    if(map_test.deletePath(car_number, truely_delete)) {
    	if(truely_delete){
    		cout<<"物理删除成功！"<<endl;
		}   
    	else {
    		cout<<"逻辑删除成功！"<<endl;
		}
	}   
    else cout<<"没有该车次号!"<<endl;
    cout<<"\n";
	system("pause");
	system("cls"); 
}
/*
**恢复逻辑删除的路径！
*/
void fun_recoverPath(Map & map_test){
	cout<<"\n5-恢复路线\n\n";   
    if(map_test.printReserveablePath()){ 	//显示被逻辑删除的路径
    	string car_number;
    	cout<<"\n请输入要恢复的车次号:";
    	cin>>car_number;
		if(map_test.reservePath(car_number)) {
    		cout<<"恢复完成！"<<endl;
		}   
		else cout<<"没有该车次号!"<<endl;
	}
	cout<<"\n";
	system("pause");
	system("cls");
}

void chooseMenu(Map &map_test){
	char choose[20];
	char c[100];
	cout << "\n欢迎使用车票管理咨询系统！\n";
	cout << "\n+--------------------------------------------+\n";
	sprintf(c, "|%-14s %-14s %-14s|\n|%-14s %-14s %-14s|\n",
             "1-查询车票信息", "2-添加路线", "3-修改余票数", "4-删除路线", "5-恢复路线", "6-退出系统");
    cout << string(c);
    cout << "+--------------------------------------------+\n\n";
    cout<<"请选择你需要的功能: ";
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
		cout<<"输入错误！请重新输入"<<endl;
		while(true){
			chooseMenu(map_test);
		}
	}
}

int main(){
	Map map_test;  		//初始化map对象
	while(true){
		chooseMenu(map_test);	//执行主菜单 
	}
	return 0;
}

