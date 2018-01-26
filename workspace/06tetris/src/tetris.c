#include<stdio.h>
#include<sys/time.h>
#include"../inc/keyboard.h"
 
 #define ROW 25 /*界面数组行数*/
 #define COL 40 /*界面数组列数*/
 #define UP 4  /*上边界*/
 #define LEFT 5  /*左边界*/
 #define FRONT 35 /*前景色*/
 #define BACK  45 /*背景色*/
 
 int UIarray[ROW][COL]; /*界面数组*/
 
 typedef struct Point
 {
     int x;
     int y;
 }point_t;
 
 point_t diamond[7][4]; /*定义七种类型，四个一组的方块*/
 
 void setPoint(int row,int col,int x,int y)
 {
     diamond[row][col].x=x;
     diamond[row][col].y=y;
 }
 
 /*初始化全部方块*/
 void initDiamond()
 {
         /*正 L 型初始化*/
         setPoint( 0,0,-1,0 );
         setPoint( 0,1,0,0 );
         setPoint( 0,2,0,1 );
         setPoint( 0,3,0,2 );
         /*T型初始化*/
 
         setPoint( 1,0,0,0 );
         setPoint( 1,1,0,1 );
         setPoint( 1,2,-1,0 );
         setPoint( 1,3,0,-1 );
         /*反 L 型初始化*/
         setPoint( 2,0,0,0 );
         setPoint( 2,1,0,-1 );
         setPoint( 2,2,0,-2 );
         setPoint( 2,3,-1,0 );
         /*正闪电型初始化*/
         setPoint( 3,0,0,0 );
         setPoint( 3,1,0,-1 );
         setPoint( 3,2,1,-1 );
         setPoint( 3,3,-1,0 );
         /*田型方块初始化*/
         setPoint( 4,0,0,0 );
         setPoint( 4,1,-1,0 );
         setPoint( 4,2,-1,1 );
         setPoint( 4,3,0,1 );
 
         /*条形方块*/
         setPoint( 5,0,-1,0 );
         setPoint( 5,1,0,0 );
         setPoint( 5,2,1,0 );
         setPoint( 5,3,2,0 );
 
         /*反闪电型方块*/
         setPoint( 6,0,-1,0 );
         setPoint( 6,1,0,0 );
         setPoint( 6,2,0,1 );
         setPoint( 6,3,1,1 );
 
 }
 
 /*输出框架*/
 void show_frame()
 {
     int i,j;
     for(i=0;i<=COL+26;i++)
     {
         printf("\033[%d;%dH",UP-1,i+LEFT);
         printf("\033[%dm\033[%dm[]\033[0m",FRONT,BACK);
     }
 
     for(i=0;i<=COL+26;i++)
     {
         printf("\033[%d;%dH",ROW+UP,i+LEFT);
         printf("\033[%dm\033[%dm[]\033[0m",FRONT,BACK);
     }
 
     for(i=0;i<=ROW+1;i++)
     {
         printf("\033[%d;%dH",i+UP-1,LEFT);
         printf("\033[%dm\033[%dm[\033[0m",FRONT,BACK);
     }
 
 
     for(i=0;i<=ROW;i++)
     {
         printf("\033[%d;%dH",i+UP,COL+LEFT);
         printf("\033[%dm\033[%dm[\033[0m",FRONT,BACK);
     }
 
 }
  /*设置基准坐标*/
 void setbasic(int *bx,int *by)
 {
     *bx = 3;
     *by = 10;
 }
 
 /*当前方块位置数组*/
 point_t nowdiamond[4];
 
 point_t prediamond[4];/*上一个方块*/

/*当前数组编号*/ 
 int rands;
/*当前成绩*/
 int score;
/*当前游戏状态*/
 int cases=1;

/*判断是否为满行*/
int Isfull(int row)
{
	int i,count=0;
	for(i=0;i<COL;i++)
	{
		if(UIarray[row][i] == 1)
		{
			count++;
		}
	}
	if(count == 19)
	{
		return 1;
	}
	return 0;
}

/*满行之上下落*/
void godown(int row)
{
	int i,j,k;
	for(i=row,j=row-1;j>=0;i--,j--)
	{
		for(k = 0;k<COL;k++)
		{
			UIarray[i][k] = UIarray[j][k];
		}	
	}	
}
/*消除满行*/
void killfull()
{
	int i,j;
	for(i=0;i<ROW;i++)
	{
		if(1 == Isfull(i))
		{
			score += 10;
			for(j=i;j<COL;j++)
			{
				UIarray[i][j] = 0;
			}
			godown(i);
		}
	}
}

 /*输出界面数组*/
 void showUIArray()
 {
     int i,j,flag=0;
     for(i=0;i<ROW;i++)
     {
		flag = 0;
         for(j= 0;j<COL;j+=1)
        {
             if(UIarray[i][j] == 1)
             {
                 printf("\033[%d;%dH",i+UP,j*2+LEFT);
				if(rands==0)
                {
					 printf("\033[%dm\033[40m[]\033[0m",30+1+rands);
				}
				else
				{
					printf("\033[%dm\033[40m[]\033[0m",30+rands);
				}
             }
             else
             {
                 printf("\033[%d;%dH",i+UP,j*2+LEFT);
				 if(j*2 == 0)
                 	printf("\033[%dm\033[%dm[]\033[0m",FRONT,BACK);
				 if(j*2 == COL)
                 	printf("\033[%dm\033[%dm[]\033[0m",FRONT,BACK); 
				 if(j*2 >= COL+25&&flag==0)
					{
						flag=1;
						printf("\033[%dm\033[%dm[]\033[0m",FRONT,BACK);
					}
                 printf("\033[30m\033[40m[]\033[0m");
             }
		}
		
          printf("\033[%d;%dH",UP+2,COL+LEFT+7);
          printf("\033[37m\033[40m俄罗斯方块游戏\033[0m");
		  
          printf("\033[%d;%dH",15+UP,COL+LEFT+10);
          printf("\033[37m\033[40m得分:  %d\033[0m",score);
		
			
		  printf("\033[%d;%dH",10+UP,COL+LEFT+10);
		  if(1==cases)
    	  {
			printf("\033[37m\033[40m游戏中\033[0m");
	      }	
		  else
		  {
			printf("\033[37m\033[40m已暂停\033[0m");
		  }
     }
 }
 
 /*拷贝将随机产生的数组拷贝进当前数组*/
 void CopyArray(point_t *nowdiamond)
 {
     int i;
     for(i= 0;i<4;i++)
     {
         nowdiamond[i].x = diamond[rands][i].x;
         nowdiamond[i].y = diamond[rands][i].y;
     }
 }
 
 /*初始化一个随机方块*/
 void initRandDiamond()
 {
     rands = rand()%7;
     CopyArray(nowdiamond);
 }
 
 /*将当前方块数组装入界面数组*/
 void setUIarray(int bx,int by)
 {
    int i;
     for(i=0;i<4;i++)
     {
         UIarray[nowdiamond[i].x+bx][nowdiamond[i].y+by] = 1;
     }
 }
 /*将上一方块数组在界面数组中清除*/
 void clearPreUIarray(int bx,int by)
 {
     int i;
     for(i=0;i<4;i++)
     {
         UIarray[prediamond[i].x+bx][prediamond[i].y+by] = 0;
     }
 }
 /*将当前方块数组数据拷进上一个方块数组*/
 void copyNow_pre()
 {
     int i;
     for(i=0;i<4;i++)
     {
         prediamond[i]= nowdiamond[i];
     }
 }
/*判断是否能够旋转*/
int judgeremolve(int bx,int by)
{
	 int i;
	 point_t tempPoint[4];
	 for(i=0;i<4;i++)
	 {
		tempPoint[i] = nowdiamond[i];
	 }
     for(i=0;i<4;i++)
     {
         int temp;
         temp = tempPoint[i].x;
         tempPoint[i].x = -tempPoint[i].y;
         tempPoint[i].y = temp;
     }
     for(i=0;i<4;i++)
     {
		if(tempPoint[i].x+bx>=ROW||( tempPoint[i].y+by)*2>=COL||(tempPoint[i].y+by)*2<=0)
		{
			return 0;
		}
		if(UIarray[tempPoint[i].x+bx][tempPoint[i].y+by]==1)
		{
			return 0;
		}
	 }
	return 1;
} 
 /*旋转方块*/
 void revolve(int bx,int by)
 {
     int i;
	 if(0==judgeremolve(bx,by))
	 {
		return;
	 }
     copyNow_pre();
     clearPreUIarray(bx,by);/*清除上一个方块*/
     for(i=0;i<4;i++)
     {
         int temp;
         temp = nowdiamond[i].x;
         nowdiamond[i].x = -nowdiamond[i].y;
         nowdiamond[i].y = temp;
     }
     copyNow_pre();
     setUIarray(bx,by);  /*将当前方块数组装入界面数组*/
 
 }

/*判断是否存在*/
int isexist(int x,int y)
{
	int i;
	for(i=0;i<4;i++)
	{
		if(nowdiamond[i].x==x&&nowdiamond[i].y==y)
		{
			return 1;
		}
	}
	return 0;
}

/*完善判断数组*/
void FinishJudge(point_t *judge,int *length)
{
	int i;
	for(i=0;i<4;i++)
	{
		int temp = nowdiamond[i].x+1;
		if(!isexist(temp,nowdiamond[i].y))
		{
			judge[*length]=nowdiamond[i];
			(*length)++;
		}
	}
}

/*判断是否可以移动*/
int JudgeCanMove(int dirction,int bx,int by)
{
	int i,length=0;
	point_t judge[4];
	FinishJudge(judge,&length);	
     switch(dirction)
     {
         case 1:
				by--;/*模拟向左移动*/
             break;
         case 2:
				by++;/*模拟向右移动*/
             break;
         case 3:
            	bx++;/*向下移动*/
             break;
	}
	for(i=0;i<length;i++)
	{
		if(1 == UIarray[judge[i].x+bx][judge[i].y+by])
		{
			return 0;
		}
	}
	for(i=0;i<4;i++)
	{
		
		if(nowdiamond[i].x+bx>=ROW||(nowdiamond[i].y+by)*2>=COL||(nowdiamond[i].y+by)*2<=0)
		{
			return 0;
		}
	}
	return 1;
	
} 
 int move(int *bx,int *by,int d)
 {
     int dirction=d;
	 int result=-1; 
     copyNow_pre();
     clearPreUIarray(*bx,*by);/*清除上一个方块*/
 
     switch(dirction)
     {
         case 1:
			 result=JudgeCanMove(1,*bx,*by);/*判断是否可以移动*/
			 if(1==result)
             {
				(*by)--;/*向左移动*/
				result=1;
			 }
			 else
				result=0;
             break;
         case 2:
			 result=JudgeCanMove(2,*bx,*by); /*判断是否可以移动*/
            if(1==result) 
		  	 {
				(*by)++;/*向右移动*/
				result=1;
			 }
			 else
				result=0;
             break;
         case 3:
			 result=JudgeCanMove(3,*bx,*by); /*判断是否可以移动*/
            if(1==result) 
			 {
            	(*bx)++;/*向下移动*/
				result=1;
			 }
			 else
				result=0;
             break;
         case 4:
            revolve(*bx,*by); 
             break;
     }
     setUIarray(*bx,*by);
     copyNow_pre();
     setUIarray(*bx,*by);    /*将当前方块数组装入界面数组*/
	 return result;
 }

/*清空界面*/
 void clearUI()
 {
	int i,j;
	for(i=0;i<ROW;i++)
	{
		for(j=0;j<COL;j++)
		{
			UIarray[i][j]=0;
		}
	}
 }
/*游戏结束输出*/
void gameover()
{
	printf("\033[%d;%dH",10+UP,LEFT+17);
	printf("\033[37m\033[40m游戏结束！\n\033[0m");
	printf("\033[%d;%dH",12+UP,LEFT+15);
	printf("\033[37m\033[40m你的得分是 : %d \n\n\n\n\n\n\n\n\n\n\n\n\n\n\033[0m",score);
	recover_keyboard();/*关闭键盘环境*/
	printf("\033[?25h");/*回显光标*/
}
/*判断游戏结束*/
int JudgeFail(int bx,int by)
{
	if( !JudgeCanMove(3,bx,by))
	{
		return 1;	
	}
	else
	{
		return 0;
	}
}

void getKey() 
{
	int key, ret, flag=0;
	
     int bx,by;
     printf("\033[2J");/*清屏*/
     initDiamond(); /*初始化所有方块*/
     setbasic(&bx,&by); /*设置初始基准*/
     initRandDiamond(); /*初始化随机方块*/
     setUIarray(bx,by);  /*将当前方块数组装入界面数组*/
     show_frame(); /*输出框架*/

	ret = init_keyboard();
	printf("\033[?25l");/*隐藏光标*/
	if(ret < 0)
		return;
	
    showUIArray(bx,by); /*输出界面数组*/
	struct timeval tv1;
	gettimeofday(&tv1,NULL);/*第一次获取毫秒数*/

	while(1)
	{
		flag=0;
		struct timeval tv2;
		gettimeofday(&tv2,NULL);/*第二次获取毫秒数*/

		if((tv2.tv_sec*1000+tv2.tv_usec/1000)-(tv1.tv_sec*1000+tv1.tv_usec/1000)>400)
		{
			if(0==move(&bx,&by,3))
			{
				flag=1;
			}
			tv1 = tv2;
    		showUIArray(bx,by); /*输出界面数组*/
		}
	
		key = get_key();
	if(key>=0)		
	{
		if(is_down(key))
		{
			if(0==move(&bx,&by,3))
			{
				flag=1;
			}
		}
		if(is_left(key))
		{
			move(&bx,&by,1);
		}	
		if(is_right(key))
		{
			move(&bx,&by,2);
		}
		if(is_up(key))
		{
			move(&bx,&by,0);
		}
		if(is_down(key))
		{	
			if(0==move(&bx,&by,3))
			{
				flag=1;
			}
		}
		if(is_enter(key))
		{
			move(&bx,&by,4);
		}
		if(is_space(key))
		{
			int fg=0;
			while(1)
			{
				
				if(fg==0)
				{
					cases=0;
					showUIArray(bx,by);
					fg++;
				}
				key = get_key();
				if(is_space(key))
				{
					cases=1;
					showUIArray(bx,by);
					break;	
				}
			}
		}
		if(is_esc(key))
		{
			gameover();	
			break;
		}
	}	
		if(1==flag)
		{
			
 	 		killfull(); /*消灭满行*/
     		setbasic(&bx,&by); /*设置初始基准*/
     		initRandDiamond(); /*初始化随机方块*/
			if(JudgeFail(bx,by))
			{
				gameover();
				break;
			}
     		setUIarray(bx,by);  /*将当前方块数组装入界面数组*/
    		showUIArray(bx,by); /*输出界面数组*/
		}
		else
     	{
			setUIarray(bx,by);  /*将当前方块数组装入界面数组*/
		}
	}
}	

int main()
{
	getKey();
	return 0;
}
