#include<iostream>
#include<cstring>
#define maxn 100
using namespace std;

class Graph{
	private:
		int **Matrix;
		int n;
	public:
		int visited[maxn];
		int ** getMatrix(){
			return Matrix;
		}
		int getn(){
			return n;
		}
		void init(){
			memset(visited,0,sizeof(visited));
		}
		void setEdge(int i,int j){
			Matrix[i][j]=1;
			Matrix[j][i]=1;
		}
		bool isEdge(int i,int j){
			return Matrix[i][j];
		}
		Graph(){
			int p[maxn];
			for(int i=0;i<maxn;i++){
				Matrix[i]= &p[maxn];
			}
			memset(Matrix,0,sizeof(Matrix));
			this->n=maxn;
		}
		Graph(int n){
			Matrix = (int **)new int *[maxn]; 
			for(int i=0;i<n;i++){
				Matrix[i]=new int [maxn];
			}
			memset(Matrix,0,sizeof(Matrix));
			this->n=n; 
		}
		Graph(int **A,int n){
			Matrix =(int **) new int *[maxn];
			for(int i=0;i<maxn;i++){
				Matrix[i]= new int [maxn];
			}
			for(int i=0;i<n;i++){
				for(int j=0;j<n;j++){
					*((int *)Matrix+i*n+j)=*((int *)A+n*i+j);
				}	
			}
			this->n=n;
		}
};
