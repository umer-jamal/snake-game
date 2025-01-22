#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include<time.h>
#include<string.h>

void display();
void update();
void generator();
void logic();
char inp;
const int mx=30, my=15;
int hx,hy, len, tx[100], ty[100], ex, checkinp, fx, fy, score;
int main(){
	srand(time(NULL));
	hx=mx/2;hy=my/2; len=0; ex=1; checkinp=8;score=0;
	inp='w';
	generator();
	display();
	do{
	if(_kbhit()){
		inp=tolower(_getch());
	}
	display();
	update();
	logic();
	Sleep(150-score);
	}while(ex);
	system("cls");
	printf("\n\t\t\t\t Game Over!\n\t\t\t\tScore is %d",score);
	getch();
	getch();
	return 0;
}
void display(){
	system("cls");
	printf("\n\t\t\t\t   SNAKE GAME\n\n");
	for(int i=0;i<=my;i++){
		printf("\t\t\t");
		for(int j=0;j<=mx;j++){
			if( j==0 || j==mx)	printf("|");
			else if(i==my || i==0 )	printf("#");
			else if(j==hx && i==hy)	printf("O");
			else if(j==fx && i==fy)	printf("$");
			else{
				for(int k=0;k<len;k++){
					if(tx[k]==j && ty[k]==i){
						printf("0");
						goto A;
					}	
				}
				printf(" ");
				A: printf("");
			} 
		}
		printf("\n");
	}
	printf("\n\n\t\t\t\t   Score : %d",score);
}
void update(){
	for(int i=len;i>0;i--){
		tx[i-1]=tx[i-2];
		ty[i-1]=ty[i-2];
	}
		tx[0]=hx;
		ty[0]=hy;
		switch(inp){
			case 'w':{
				if(checkinp!=2){
					checkinp=8;
					hy--;
				}else{
					inp='s';
					hy++;	
				} 
				break;
			} 
			case 'a':{
				if(checkinp!=6){
					hx--;
					checkinp=4;
				}else{
					inp='d';
					hx++;	
				} 
				break;
			}
			case 'd':{
				if(checkinp!=4){
					hx++;
					checkinp=6;
				}else{
					hx--;
					inp='a';
				}
				break;
			}
			case 's':{
				if(checkinp!=8){
					hy++;
					checkinp=2;
				}else{
					hy--;
					inp='w';
				} 
				break;
			}
			case 'q':{
				ex=0;
				break;
			}
		}
}
void generator(){
	B: do{
		fx=rand()%mx;
		fy=rand()%my;
	}while(!fx || !fy);
	for(int i=0;i<len;i++){
		if(tx[i]==fx && ty[i]==fy){
			goto B;
		}
	}
}
void logic(){
	if(!hx || !hy || hy>=my || hx>=mx )	ex=0;
	for(int i=0;i<len;i++){
		if(tx[i]==hx && ty[i]==hy){
			ex=0;
			break;
		}
	}
	if(hx==fx && hy==fy){
		score++;
		len++;
		generator();
	}	
}
