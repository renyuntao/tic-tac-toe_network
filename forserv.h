#ifndef FORSERV_H
#define FORSERV_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<netdb.h>
#include<signal.h>
#include<unistd.h>
#include<error.h>
#include<arpa/inet.h>
#include<sys/un.h>
#include<errno.h>
#include <pthread.h>
#define MODE S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH
#define LISTEN_NUM 8
#define BUF_SIZE 500
#define MY_PORT "8989"
#define MY_CHAT_PORT "6767"
//************About total score****************
#define WIN_SCORE 10
#define LOSE_SCORE -1
#define DRAW_SCORE 3
//*********************************************
#define TOTAL_ARRAY_LEN 25



void zero_buf(char ch[]);
void sign_up(int serv_clnt_sock);
void get_id(char *map_name,char *id);
void get_total(char *id,int *total);
int set_total(int flag,char *id);
void process_child(int serv_clnt_sock);
void init();
void get_score(char *id,int *score);
int set_score(int flag,char *curname,char *id);
void get_every_total();
void get_name(char *id,char *name);    
void InsertSort();
void sort_name();

#endif
