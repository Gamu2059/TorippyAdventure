#include"define.h"
#include<stdio.h>
#include<ncurses.h>
#include<stdlib.h>
#include<string.h>

FILE *fp,*fp2;
char file1[10]="\0",file2[10]="\0",file3[10]="\0";

int gene_log(){
	int mana;
	if((fp=fopen("mana.txt","r"))==NULL){
		endwin();
		exit(-2);
	}
	fscanf(fp,"%d",&mana);
	fclose(fp);
	if(mana==1)
		return FALSE;
	if((fp=fopen("mana.txt","w"))==NULL){
		endwin();
		exit(-2);
	}
	fprintf(fp,"1\n");
	fclose(fp);
	strcat(file1,"log1.txt");
	strcat(file2,"log2.txt");
	strcat(file3,"log3.txt");
	if((fp=fopen(file1,"w"))==NULL){
		endwin();
		exit(-2);
	}
	fclose(fp);
	if((fp=fopen(file2,"w"))==NULL){
		endwin();
		exit(-2);
	}
	fclose(fp);
	if((fp=fopen(file3,"w"))==NULL){
		endwin();
		exit(-2);
	}
	fclose(fp);
	return TRUE;
}

void break_log(){
	if((fp=fopen("mana.txt","w"))==NULL){
		endwin();
		exit(-2);
	}
	fprintf(fp,"0\n");
	fclose(fp);
	remove(file1);
	remove(file2);
	remove(file3);
}

void init_log(int number){
	if(number==0){
		if((fp=fopen(file1,"w"))==NULL){
			endwin();
			exit(-2);
		}
	}
	else if(number==1){
		if((fp=fopen(file2,"w"))==NULL){
			endwin();
			exit(-2);
		}
	}
	else if(number==2){
		if((fp=fopen(file3,"w"))==NULL){
			endwin();
			exit(-2);
		}
	}
}

void record_log(int command){
	fprintf(fp,"%d\n",command);
}

void end_log(){
	if(fclose(fp)==EOF){
		endwin();
		exit(-2);
	}
}

void change_best(){
	int brank;
	if((fp=fopen(file1,"r"))==NULL){
		endwin();
		exit(-2);
	}
	if((fp2=fopen("best1.txt","w"))==NULL){
		endwin();
		exit(-2);
	}
	while(fscanf(fp,"%d",&brank)!=EOF)
		fprintf(fp2,"%d\n",brank);
	fclose(fp);
	fclose(fp2);
	if((fp=fopen(file2,"r"))==NULL){
		endwin();
		exit(-2);
	}
	if((fp2=fopen("best2.txt","w"))==NULL){
		endwin();
		exit(-2);
	}
	while(fscanf(fp,"%d",&brank)!=EOF)
		fprintf(fp2,"%d\n",brank);
	fclose(fp);
	fclose(fp2);
	if((fp=fopen(file3,"r"))==NULL){
		endwin();
		exit(-2);
	}
	if((fp2=fopen("best3.txt","w"))==NULL){
		endwin();
		exit(-2);
	}
	while(fscanf(fp,"%d",&brank)!=EOF)
		fprintf(fp2,"%d\n",brank);
	fclose(fp);
	fclose(fp2);
}

int getScore(void){
	int score;
	if((fp2=fopen("score.txt","r"))==NULL){
		endwin();
		exit(-2);
	}
	fscanf(fp2,"%d",&score);
	fclose(fp2);
	return score;
}

void setScore(int score){
	if((fp2=fopen("score.txt","w"))==NULL){
		endwin();
		exit(-2);
	}
	fprintf(fp2,"%d",score);
	fclose(fp2);
}
