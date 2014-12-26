#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <algorithm>

#define maxx 90000
#define runtimes 1000
#define initnum 10

using namespace std;

struct friending{
	int id;
	double rand;
};

struct social{
	int id;
	bool active;	//if 1 this is be infected, if 0, otherwise
	int round_time;
	vector<friending> friends;
	bool exist;
};

social user[maxx];
int initnode[initnum];
int nummax;		//nummax to record the max id

double take_random(){
	double result=(double) rand() / (RAND_MAX + 1.0 );
	return result;
}

int main(int argc,char* argv[]){
	int a,b;
	double c;
	queue<int>infl;		//to do bfs
	int influence_times[maxx];
	int result_num[100]={0},sum=0;
	FILE *read_edge;

	nummax=0;	//nummax to record the max id
	read_edge=fopen(argv[1],"r");
	c = atof(argv[2]);
	memset(user,0,sizeof(user));
	friending inputfriend;

	while(fscanf(read_edge,"%d %d",&a,&b) != EOF){	//read the adge
		user[a].id=a;
		inputfriend.id=b;
		inputfriend.rand=c;
		user[a].friends.push_back(inputfriend);
		nummax=max(nummax,a);
		user[a].exist=1;
	}
	srand(time(NULL));
	nummax++;	//it is convience for for_loop

	int tmp_num=0;
	int curnum=0, max_influence, best_seed;
	while(curnum < initnum){	//backtracking
		max_influence=0;
		for(int x=0;x<nummax;x++){
			if(!user[x].exist)	continue;
			int check=0;
			for(int i=0;i<curnum;i++)	//prevent choose the same seed
				if(x == initnode[i]){
					//printf("%d == %d %d\n", x, initnode[i], i);
					check=1;
					break;
				}
			if(check)	continue;

			initnode[curnum]=x;
			memset(result_num,0,sizeof(result_num));
			//The follow is run the experiment
			for(int t=0;t<runtimes;t++){	//run the result
				//while(!infl.empty())	infl.pop();
				for(int i=0;i<nummax;i++){	//initial each node
					user[i].active=0;
					user[i].round_time=-1;
				}
				for(int i=0;i<curnum+1;i++){	//put all initial node to queue
					user[initnode[i]].active=1;
					user[initnode[i]].round_time=0;
					infl.push(initnode[i]);
				}
				//int x=0;
				int tmp,tmp_round=0;	//tmp_round record round_time of queue.front
				while(!infl.empty()){	//use bfs to propagate the influence
					tmp=infl.front();
					infl.pop();
					tmp_round=user[tmp].round_time;
					result_num[tmp_round]++;
					//printf("tmp_round=%d %d %d\n",tmp_round,putt,put_activenode[putt]);
					int friendnum=user[tmp].friends.size();
					for(int i=0,j;i<friendnum;i++){
						j=user[tmp].friends[i].id;
						if(!user[j].active && take_random()<user[tmp].friends[i].rand){
							infl.push(j);
							user[j].round_time=user[tmp].round_time+1;
							user[j].active=1;
						}
					}
				}//while(!infl.empty())
			}//for(runtimes)
			
			tmp_num=0;
			for(int i=0;result_num[i]!=0;i++)
				tmp_num+=result_num[i];
			/*for(int i=0;i<curnum;i++)	printf("%d\t",initnode[i]);
			printf("%d\t",x);
			printf("%.3lf\n", tmp_num/(double)runtimes);
			*/
			if(tmp_num > max_influence){
				max_influence = tmp_num;
				best_seed = x;
			}
		}//for(int x=0;x<nummax;x++)
		initnode[curnum] = best_seed;
		curnum++;
	}//while(curnum < initnum)
	printf("-----------------\n");
	for(int i=0;i<curnum;i++)	printf("%d\t",initnode[i]);
	printf("%.3lf\n", max_influence/(double)runtimes);
	fclose(read_edge);
	return 0;
}
