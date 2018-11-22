#include <ncurses.h>
#include <unistd.h>
#include "playerLooks.h"
#define ACCEL 9.8
#define V0 -4.5
#define VA -4.5
#define TIME_FRAME 0.03
#define DEATH_LINE 220
#define PL_Y 20		//キャラクタのＹサイズ
#define PL_X 20		//キャラクタのＸサイズ
#define ST_Y 260	//ステージのＹサイズ
#define ST_X 1580	//ステージのＸサイズ
#define DS_Y 160	//表示域のＹサイズ
#define DS_X 316	//表示域のＸサイズ
int dis_y=0,dis_x=0;//ディスプレイのy,xが0,0となるステージの絶対座標
int game_over_flg=false;
int stage[ST_Y][ST_X];
int game_over[5][63]=
{
{0,1,1,1,0,0,0,1,1,1,0,0,1,0,0,0,1,0,1,1,1,1,1,0,0,1,1,1,0,0,1,0,0,0,1,0,1,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,0,0,0,0,0,1,0,0,0,1,0,1,1,0,1,1,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,0,0,1,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,0,0,1,0,0,0,1,0,0,1,0,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,1,1,1,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,1,1,1,1,0,0,1,1,1,0,0,0,0,1,0,0,0,1,1,1,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
typedef struct{
	int look[PL_Y][PL_X];
	int y,x;
	float v0,vA,vX;
	char jump_cnt;
	char fall_flg;
	float fall_time;
	char wall_crush_flg;
	int up_wall,up_y,down_wall,down_y,down_x,left_wall,left_x,right_wall,right_x;
} CD;

void init_stage(){
	int i,j;
	for(i=0;i<ST_Y;i++){
		for(j=0;j<ST_X;j++){
			if(i==0)
				stage[i][j]=5;
			else if(i==ST_Y-1||i>=DS_Y-20&&!(j%60>=0&&j%60<=40)||j==0||j==ST_X-1)
				stage[i][j]=6;
			else if(i>=34&&i<=36&&j<=40||j%130>=0&&j%130<=5&&i>=40&&j!=0)
				stage[i][j]=7;
			else
				stage[i][j]=0;
		}
	}
}

void draw_stage(){
	int i,j,k;
	for(i=dis_y;i<DS_Y+dis_y;i++){
		for(j=dis_x;j<DS_X+dis_x;j++){
			if(stage[i][j]!=0){
				attron(COLOR_PAIR(stage[i][j]+1));
				for(k=0;k<2;k++)
					mvaddch(i-dis_y,(j-dis_x)*2+k,(char)(stage[i][j]+'A'));
			}
		}
	}
	refresh();
}

void delt_player(CD *cd){
	int y,x;
	for(y=0;y<PL_Y;y++){
		for(x=0;x<PL_X;x++){
			if(cd->look[y][x]!=0){
				stage[(int)cd->y+y][(int)cd->x+x]=0;
			}
		}
	}
}

void draw_player(CD *cd){
	int y,x;
	for(y=0;y<PL_Y;y++){
		for(x=0;x<PL_X;x++){
			if(cd->look[y][x]!=0)
				stage[(int)cd->y+y][(int)cd->x+x]=cd->look[y][x];
		}
	}
}

void out_player(CD *cd){
	int y,x,z;
	for(y=0;y<PL_Y;y++){
		for(x=0;x<PL_X;x++){
			if(cd->look[y][x]!=0){
				attron(COLOR_PAIR(cd->look[y][x]+1));
				for(z=0;z<2;z++)
					mvaddch(cd->y+y-dis_y,(cd->x+x-dis_x)*2+z,(char)(cd->look[y][x]+'A'));
			}
		}
	}
	refresh();
}

void arrive_check(CD *cd){
	int y,x;
	int distant,abs_distant;
	int min_distant,min_abs_distant,max_abs_distant;

	//下方向にチェック
	min_distant=ST_Y-1;
	max_abs_distant=0;
	for(x=0;x<PL_X;x++){
		for(y=0;y<PL_Y;y++){
			if(cd->look[y][x]!=0){
				for(distant=(int)cd->y+y;stage[distant][(int)cd->x+x]==0&&distant<ST_Y;distant++);
				if(distant<ST_Y){
					abs_distant=distant;
					distant-=((int)cd->y+y);
					if(distant<=min_distant){
						if(distant==min_distant){
							if(abs_distant<max_abs_distant)	continue;
						}
						min_distant=distant;
						max_abs_distant=abs_distant;
						cd->down_y=y;
						cd->down_x=x;
					}
				}
			}
		}
	}
	cd->down_wall=max_abs_distant;

	//上方向にチェック
	min_distant=ST_Y-1;
	min_abs_distant=ST_Y-1;
	for(x=0;x<PL_X;x++){
		for(y=PL_Y-1;y>=0;y--){
			if(cd->look[y][x]!=0){
				for(distant=(int)cd->y+y;stage[distant][(int)cd->x+x]==0&&distant>=0;distant--);
				if(distant>=0){
					abs_distant=distant;
					distant=(int)cd->y+y-distant;
					if(distant<=min_distant){
						if(distant==min_distant){
							if(abs_distant>min_abs_distant)	continue;
						}
						min_distant=distant;
						min_abs_distant=abs_distant;
						cd->up_y=y;
					}
				}
			}
		}
	}
	cd->up_wall=min_abs_distant;

	//右方向にチェック
	min_distant=ST_X-1;
	max_abs_distant=0;
	for(y=0;y<PL_Y;y++){
		for(x=0;x<PL_X;x++){
			if(cd->look[y][x]!=0){
				for(distant=(int)cd->x+x;stage[(int)cd->y+y][distant]==0&&distant<ST_X;distant++);
				if(distant<ST_X){
					abs_distant=distant;
					distant-=((int)cd->x+x);
					if(distant<=min_distant){
						if(distant==min_distant){
							if(abs_distant<max_abs_distant)	continue;
						}
						min_distant=distant;
						max_abs_distant=abs_distant;
						cd->right_x=x;
					}
				}
			}
		}
	}
	cd->right_wall=max_abs_distant;

	//左方向にチェック
	min_distant=ST_X-1;
	min_abs_distant=ST_X-1;
	for(y=0;y<PL_Y;y++){
		for(x=PL_X-1;x>=0;x--){
			if(cd->look[y][x]!=0){
				for(distant=(int)cd->x+x;stage[(int)cd->y+y][distant]==0&&distant>=0;distant--);
				if(distant>=0){
					abs_distant=distant;
					distant=(int)cd->x+x-distant;
					if(distant<=min_distant){
						if(distant==min_distant){
							if(abs_distant>min_abs_distant)	continue;
						}
						min_distant=distant;
						min_abs_distant=abs_distant;
						cd->left_x=x;
					}
				}
			}
		}
	}
	cd->left_wall=min_abs_distant;
}

char crush_check(CD *cd){
	int y,x;
	for(y=0;y<PL_Y;y++){
		for(x=0;x<PL_X;x++){
			if(cd->look[y][x]!=0){
				if(stage[(int)cd->y+y][(int)cd->x+x]!=0)
					return true;
			}
		}
	}
	return false;
}

void crush_revision(CD *cd,int top_time){
	int y,x,flg;
	int wall_jump=false,arrive_flg=false;
	flg=false;
	y=cd->down_y;
	while(cd->y+y>=cd->down_wall||cd->y+y>=ST_Y){
		cd->y--;
		flg=true;
	}
	if(flg==true){
		arrive_flg=true;
		cd->vA=0;
		cd->fall_time=0;
		cd->jump_cnt=0;
		cd->wall_crush_flg=false;
	}
	if(cd->y+cd->down_y+1==cd->down_wall){
		cd->fall_flg=false;
		cd->v0=0;
	}
	else{
		cd->fall_flg=true;
		if(cd->v0==0){
			cd->v0=-V0;
			cd->fall_time=0;
		}
	}
	flg=false;
	y=cd->up_y;
	while(cd->y+y<=cd->up_wall||cd->y+y<0){
		cd->y++;
		flg=true;
	}
	if(flg==true){
		cd->fall_time=1.5273;
	}
	flg=false;
	x=cd->right_x;
	while(cd->x+x>=cd->right_wall||cd->x+x>=ST_X){
		cd->x--;
		flg=true;
	}
	if(flg==true)
		wall_jump=true;
	flg=false;
	x=cd->left_x;
	while(cd->x+x<=cd->left_wall||cd->x+x<0){
		cd->x++;
		flg=true;
	}
	if(flg==true)
		wall_jump=true;

	//斜め跳び時の壁すり抜け対策
	while(crush_check(cd)==true){
		if(cd->vX>0)
			cd->x--;
		else if(cd->vX<0)
			cd->x++;
	}
	//足以外の着地対策	
	if(cd->look[cd->down_y][cd->down_x]!=2&&cd->y+cd->down_y+1==cd->down_wall){
		if(cd->down_x-PL_X/2<0)
			cd->x++;
		else
			cd->x--;
	}

	if(wall_jump==true){
		cd->wall_crush_flg=true;
		cd->jump_cnt=0;
	}
	else
		cd->wall_crush_flg=false;
	
	if(arrive_flg==true)
		cd->vX=0;
}

void death_check(CD *cd){
	int i,j;
	if(cd->y>=DEATH_LINE){
		game_over_flg=true;
		for(i=0;i<5;i++){
			for(j=0;j<63;j++){
				if(game_over[i][j]!=0){
					stage[i+dis_y+10][j+dis_x+10]=game_over[i][j];
				}
			}
		}
	}
}

void player_bect(CD *cd,char ope){
	int y,x;
	for(y=0;y<PL_Y;y++){
		for(x=0;x<PL_X;x++){
			if(ope>0)
				cd->look[y][x]=playerLooks[0][y][x];
			else
				cd->look[y][x]=playerLooks[0][y][PL_X-1-x];
		}
	}
}

void jump(CD *cd){
	if(cd->jump_cnt<5){
		cd->jump_cnt++;
		cd->fall_time=0;
		cd->fall_flg=true;
		cd->v0=V0;
		cd->vA=VA;
		if(cd->wall_crush_flg==true){
			cd->wall_crush_flg=false;
			cd->vX*=-1;
			if(cd->vX>0){
				player_bect(cd,1);
				arrive_check(cd);
				if(crush_check(cd)==true){
					player_bect(cd,-1);
					arrive_check(cd);
				}
			}
			else if(cd->vX<0){
				player_bect(cd,-1);
				arrive_check(cd);
				if(crush_check(cd)==true){
					player_bect(cd,1);
					arrive_check(cd);
				}
			}
		}
	}
}

void control(CD *cd){
	int top_time=-(int)(cd->v0/(ACCEL/2));
	arrive_check(cd);
	if(game_over_flg==false){
	switch(getch()){
		case KEY_LEFT:
			player_bect(cd,-1);
			arrive_check(cd);
			if(crush_check(cd)==true){
				player_bect(cd,1);
				arrive_check(cd);
			}
			if(cd->fall_flg==false){
				cd->x-=4.0;
				cd->vX=-1;
			}
			else{
				if(cd->vX>0)
					cd->vX=0;
			}
			break;
		case KEY_RIGHT:
			player_bect(cd,1);
			arrive_check(cd);
			if(crush_check(cd)==true){
				player_bect(cd,-1);
				arrive_check(cd);
			}
			if(cd->fall_flg==false){
				cd->x+=4.0;
				cd->vX=1;
			}
			else{
				if(cd->vX<0)
					cd->vX=0;
			}
			break;
		case KEY_DOWN:
			if(cd->fall_flg==true&&cd->fall_time<1.52){
				cd->fall_time=1.52;
			}
			break;
		case ' ':
			jump(cd);
			break;
		default:
			break;
	}
	}
	
	cd->fall_time+=TIME_FRAME;
	cd->vA+=0.2;
	if(cd->vA>0)
		cd->vA=0;
	
	//x,y座標の計算
	if(cd->fall_flg==true)
		cd->x+=cd->vX;
	cd->y+=cd->v0*cd->fall_time+(ACCEL/2)*cd->fall_time*cd->fall_time+cd->vA;
	
	//画面スクロールの計算
	if(cd->vX<0){
		if(cd->x<=dis_x+40){
			dis_x-=4;
			if(dis_x<0)
				dis_x=0;
		}
	}
	else if(cd->vX>0){
		if(cd->x+PL_X>=dis_x+DS_X-40){
			dis_x+=4;
			if(dis_x+DS_X>=ST_X)
				dis_x=ST_X-DS_X;
		}
	}
	crush_revision(cd,top_time);
	death_check(cd);
}

void main(){
	initscr();
	noecho();
	timeout(0);
	cbreak();
	curs_set(0);
	keypad(stdscr,true);
	start_color();
	init_pair(1,COLOR_RED,COLOR_YELLOW);
	init_pair(2,COLOR_RED,COLOR_RED);
	init_pair(3,COLOR_RED,COLOR_RED);
	init_pair(6,COLOR_BLACK,COLOR_BLACK);
	init_pair(7,COLOR_WHITE,COLOR_WHITE);
	init_pair(8,COLOR_YELLOW,COLOR_YELLOW);
	CD player;
	player.y=10;player.x=40;
	player.v0=0;player.vA=0;player.vX=0;
	player.wall_crush_flg=false;
	player.fall_time=0;player.fall_flg=false;player.jump_cnt=0;
	init_stage();
	player_bect(&player,1);
	draw_player(&player);
	draw_stage();
	//out_player(&player);
	while(1){
		delt_player(&player);
		control(&player);
		erase();
		draw_player(&player);
		draw_stage();
		//out_player(&player);
		refresh();
		usleep(10000);
	}
	endwin();
}
