#include "define.h"
#include "etc.h"
#include<stdio.h>
#include<ncurses.h>
#include<stdlib.h>

FILE *fp;

void init_auto(int number){
	if(number==0){
		if((fp=fopen("best1.txt","r"))==NULL){
			endwin();
			exit(-2);
		}
	}
	else if(number==1){
		if((fp=fopen("best2.txt","r"))==NULL){
			endwin();
			exit(-2);
		}
	}
	else if(number==2){
		if((fp=fopen("best3.txt","r"))==NULL){
			endwin();
			exit(-2);
		}
	}
}

int autoPlay(){
	int command=0;
	if(fscanf(fp,"%d",&command)==EOF)
		command=-99;
	return command;
}

void end_auto(){
	if(fclose(fp)==EOF){
		endwin();
		exit(-2);
	}
}
