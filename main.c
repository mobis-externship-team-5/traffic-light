#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h> //C�뼵�뼱
#include "math.h"
#include <string.h>
typedef struct axis_t{
	double x,y;
}axis;

typedef struct traffic_light_t{
	axis traffic_light_axis;
	int limit_speed;
	int light_cycle[3];
	char traffic_light_starttime[5];
}TRAFFIC_LIGHT;

typedef struct car_t{
	axis car_axis;
	int car_velocity;
} CAR;

void print_char(char a, int b);
void struc_copy(TRAFFIC_LIGHT *A, TRAFFIC_LIGHT *B);
int get_sum_arr(int arr[],int len){
	int sum = 0;
	for(int i = 0; i<len; i++){
		sum += arr[i];
	}
	return sum;
}

int get_distance(axis a, axis b){
	return a.x - b.x;
}


int get_distance_ax(axis a, axis b){
	return sqrt((pow((a.x-b.x),2)+pow((a.y-b.y),2)));
}

int check_color(int  arr[],int time,int sum_period,int len){
	int check_flag = (time%sum_period)+1;
	int *flag;

	flag = (int*)malloc(sizeof(int) * len);
	for(int k = 0; k < len; k++){
		flag[k] = get_sum_arr(arr,k+1);
		if(check_flag <= flag[k])
			return k;
	}
	return 0;
}

int set_int(char a){
	return a-48;
}



int main(void)
{
	system("clear");

	int dist = 0;
	int color_flag = 0;
	int remain_time_to_change = 0;
	int next_signal_check = 0;

	int load_len = 100;
	int count = 0;
	char color[3][7] = {"G","Y","R"};

	int traffic_start_time =0;
	int 	new_time = 0;
	int 	sum_period = 0;
	int     sum_period_1 = 0;
	int     sum_period_2 = 0; 
	int     color_flag_1 = 0;
	int     color_flag_2 = 0;
	TRAFFIC_LIGHT light = {{50,10},3,{6,2,5},"00:00"};

	TRAFFIC_LIGHT light2 = {{100,10},6,{9,6,9},"12:00"};

	CAR car={{0,10},2};

	time_t timer;
	struct tm* t;
	timer = time(NULL); // 1970년 1월 1일 0시 0분 0초부터 시작하여 현재까지의 초
	t = localtime(&timer); // 포맷팅을 위해 구조체에 넣기
	int recent_time =( (t->tm_hour) *3600 +(t->tm_min)*60 +(t->tm_sec) );//실



	int traffic_start_time_1 = set_int(light.traffic_light_starttime[0])*36000+set_int(light.traffic_light_starttime[1])*3600+set_int(light.traffic_light_starttime[3])*600+set_int(light.traffic_light_starttime[4])*60;

	int traffic_start_time_2 = set_int(light2.traffic_light_starttime[0])*36000+set_int(light2.traffic_light_starttime[1])*3600+set_int(light2.traffic_light_starttime[3])*600+set_int(light2.traffic_light_starttime[4])*60;

	int new_time_1 =0;
	int new_time_2 =0;
	int switch_loc = 0;
	// get_sum_arr(light.light_cycle,3);

	while(1){
		TRAFFIC_LIGHT temp;

		if(car.car_axis.x < light.traffic_light_axis.x){
			printf("SELECT FIRST TRAFFIC LIGHT!!\n\n");
			struc_copy(&temp,&light);
			switch_loc = 0;

		}else if(car.car_axis.x <light2.traffic_light_axis.x){
			printf("SELECT SECOND TRAFFIC LIGHT!!\n\n");
			switch_loc = 1;
			(struc_copy(&temp,&light2));
		}else{
			printf("NO SIGNAL\n\n");
			break;
		}

		traffic_start_time = set_int(temp.traffic_light_starttime[0])*36000+set_int(temp.traffic_light_starttime[1])*3600+set_int(temp.traffic_light_starttime[3])*600+set_int(temp.traffic_light_starttime[4])*60;

		if(recent_time<traffic_start_time_1)
			new_time =24*3600 - traffic_start_time_1 +recent_time;
		else
			new_time = recent_time - traffic_start_time_1;

		if(recent_time<traffic_start_time_2)
			new_time_1 =24*3600 - traffic_start_time_2 +recent_time;
		else
			new_time_1 = recent_time - traffic_start_time_2;

		if(recent_time<traffic_start_time)
			new_time_2 =24*3600 - traffic_start_time +recent_time;
		else
			new_time_2 = recent_time - traffic_start_time;


		sum_period = get_sum_arr(temp.light_cycle,3);
		sum_period_1 = get_sum_arr(light.light_cycle,3);
		sum_period_2 = get_sum_arr(light2.light_cycle,3);

		dist = get_distance_ax(temp.traffic_light_axis,car.car_axis);	
		color_flag = check_color(temp.light_cycle,recent_time-traffic_start_time,sum_period,3);
		color_flag_1 = check_color(light.light_cycle,new_time_1,sum_period_1,3);
		color_flag_2 = check_color(light2.light_cycle,new_time_2,sum_period_2,3);

		if(switch_loc == 1)
		{		remain_time_to_change = get_sum_arr(temp.light_cycle,color_flag_2 + 1) - new_time_2 % sum_period_2;

			next_signal_check = 1;
			if(strcmp(color[color_flag_2],"R")){ // now not red 
				while(strcmp(color[(color_flag_2 + next_signal_check)%3],"R")){

					remain_time_to_change += temp.light_cycle[(color_flag_2+next_signal_check)%3];
					next_signal_check++;
				}

				printf("\nREMAIN TIME CHANGE TO RED SIGNAL					:  %d s",remain_time_to_change);  
			}else{
				printf("\nREMAIN TIME CHANGE FROM RED SIGNAL                                	:  %d s",remain_time_to_change);  
			}

			printf("\nREMAIN TIME TO REACH TRAFFIC SIGNAL WHEN CAR SPEED IS LIMIT SPEED	:  %d s",dist/temp.limit_speed);
			if(!strcmp(color[color_flag_2],"R")) // now red

				if(dist <= 20)
					{car.car_axis.x -= car.car_velocity;
					count -= car.car_velocity;
					printf("\n\n\n 		IS RED SIGNAL!!");
				}else{
					printf("\n\n\n		FORWARD  %d M TRAFFIC LIGHT is RED!!!",dist);
				}
			else{	
				if(dist<= 20 && remain_time_to_change <= 20/temp.limit_speed )
					printf("\n\n\nFROM20 	YOU CANT PASS FRONT TRAFFIC SIGNAL ALTHOUGH YOUR SPEED IS LIMIT SPEED!!  ");
				else{
					printf("\n\n\n		GOOOOOOOOOOOOOOOOOO!!");
				}
			}
		}

		else
		{		remain_time_to_change = get_sum_arr(temp.light_cycle,color_flag_1 + 1) - new_time_1 % sum_period_1;

			next_signal_check = 1;
			if(strcmp(color[color_flag_1],"R")){ // now not red
				while(strcmp(color[(color_flag_1 + next_signal_check)%3],"R")){

					remain_time_to_change += temp.light_cycle[(color_flag+next_signal_check)%3];
					next_signal_check++;
				}
				printf("\nREMAIN TIME CHANGE TO RED SIGNAL   					:  %d s",remain_time_to_change);
			}else{
				printf("\nREMAIN TIME CHANGE FROM RED SIGNAL					:  %d s",remain_time_to_change);

			}

			printf("\nREMAIN TIME TO REACH TRAFFIC SIGNAL WHEN CAR SPEED IS LIMIT SPEED	:  %d s",dist/temp.limit_speed);
			if(!strcmp(color[color_flag_1],"R")) // now red
				if(dist <= 20)
				{car.car_axis.x -= car.car_velocity;
					count -= car.car_velocity;
					printf("\n\n\n 		IS RED SIGNAL!!");
				}else{
					printf("\n\n\n 		FORWARD  %d M TRAFFIC LIGHT is RED",dist);
				}

				else{
					if(dist<=20 && remain_time_to_change <= 20/temp.limit_speed )
						printf("\n\n\nFROM20     YOU CANT PASS FRONT TRAFFIC SIGNAL ALTHOUGH YOUR SPEED IS LIMIT SPEED!!  ");
					else{
						printf("\n\n\n          GOOOOOOOOOOOOOOOOOO!!");
					}

				}
		}
		printf("\n\n\n TRAFFIC IGNITE STRAT TIME :   %s\n",temp.traffic_light_starttime);
		printf(" CURRENT TIME                    :   %d:%d:%d\n",t->tm_hour,t->tm_min,recent_time%60);
		printf(" SIGNAL PERIOD                   G : %d s | Y : %d s | R : %d s \n",temp.light_cycle[0],temp.light_cycle[1],temp.light_cycle[2]);
		printf(" DISTANCE T0 TRAFFIC LIGHT       :   %d m\n",get_distance(temp.traffic_light_axis,car.car_axis));
		printf(" TRAFFIC LIGHT COLOR             :   %s\n", color[color_flag] );
		printf(" VEHICLE        		 :   %d m / sec \n\n",car.car_velocity);

		print_char('+',110);
                printf("\n");
                printf("\n");
                print_char('-',110);
                printf("\n");
                print_char(' ',48);
                printf("LIGHT");
                print_char(' ',45);
                printf("LIGHT");
                printf("\n");
                print_char(' ',49);
                printf("%s",color[color_flag_1]);
                print_char(' ',52);
                printf("%s",color[color_flag_2]);
                printf("\n");
                print_char('=',count);
                printf("|CAR|");
                print_char('=',105-count);
                printf("\n");
                print_char(' ',49);
                printf("%s",color[color_flag_1]);
                print_char(' ',52);
                printf("%s",color[color_flag_2]);
                printf("\n");
                printf("\n");
                print_char('-',110);

                printf("\n");
                printf("\n");
                print_char('+',110);
                printf("\n");
                printf("\n"); 

		recent_time++;
		car.car_axis.x += car.car_velocity;
		count += car.car_velocity;		

		sleep(1);
		system("clear");
		if(count ==110)
			break;

	}
	return 0;
}

void print_char(char a,int num){
	for(int i = 0; i<num; i++)
		printf("%c",a);
}

void struc_copy(TRAFFIC_LIGHT *A, TRAFFIC_LIGHT *B){
	A->traffic_light_axis.x = B->traffic_light_axis.x;
	A->traffic_light_axis.y = B->traffic_light_axis.y;
	A->limit_speed = B->limit_speed ;
	memcpy(A->light_cycle,B->light_cycle,sizeof(int)*3) ;
	strcpy(A->traffic_light_starttime, B->traffic_light_starttime);
}

