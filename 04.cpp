#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>//用里面的isdigit函数 
#define N 5000

typedef struct Node
{ 
	int weight;//结点权重 
	char zhi[3];//值 
	int parent;//父结点号 
	int lchild;//左孩子号 
	int rchild;//右孩子号 
}*stack;//同时创建了一个结构体类型的指针 

typedef struct Work
{ 
	char zhi[3];
	char pian[30];
}*Hofcode;//同时创建了一个结构体类型的指针 

int open(int quan[],FILE *fp1,FILE *fp2,char str[][3]); 
void compare(int *s1,int *s2,stack ps,int n);//寻找最小值和次小值的下标函数 (通过s1 s2排列位置)
stack create(int quan[],char str[][3],int n,stack ps,int *s1,int *s2);//创建哈夫曼树初始化并填好 
Hofcode coding(int n,stack ps,FILE *fp2,Hofcode Hof);//编码 
void compress(Hofcode Hof,FILE *fp3,FILE *fp2,int n);//压缩 
void outpress(Hofcode Hof,FILE *fp4,FILE *fp5,int n);

int main(){
	int n,quan[N],s1,s2;
	char str[N][3];
	Hofcode Hof;	
	stack ps;
/*打开文件存入权重和字符*/ 
	FILE *fp1,*fp2,*fp3,*fp4,*fp5;
	fp1=fopen("权值.txt","rb"); //以只读模式打开文件
	fp2=fopen ("压缩.txt","wb"); 
	fp3=fopen ("三国.txt","rb");
	fp4=fopen ("解压.txt","wb");
	fp5=fopen ("压缩.txt","rb");
	n=open(quan,fp1,fp2,str);//获取了数组个数，数组本身就是地址，不用返回
    ps = create(quan,str,n,ps,&s1,&s2);
	Hof = coding(n,ps,fp2,Hof);
	compress(Hof,fp3,fp2,n);
	outpress(Hof,fp4,fp5,n);
	//printf("okk");
	free(ps);
	free(Hof);
	return 0;
}

int open(int quan[],FILE *fp1,FILE*fp2,char str[][3])
{
	char ch; 
	int number=0,i=0;
	if (NULL==fp1) //以返回值fp1判断是否打开成功，如果为NULL表示失败
	{
	    printf ("Failed to open the file1 !\n");
	    exit (0) ; //终止程序，stdlib .h头文件中
	}
	if(NULL==fp2)
	{
	    printf ("Failed to open the file2 !\n");
	    exit (0);//终止程序，stdlib .h头文件中
	}
	while(!feof(fp1))//文件结束：返回非0值；文件未结束：返回0值
	{
		ch=fgetc(fp1);//获取一个字符
		if(ch<0)//包括汉字和空格 
		{
			str[i][0]=ch; 
			str[i][1]=fgetc(fp1);
			str[i][2]='\0';
		}
		else if(ch=='\n')
			{
			strcpy(str[i],"\\n");
		}
		else 
		{
			if(isdigit(ch))
			{
				while(isdigit(ch))
				{
					number=number*10+ch-'0';//得到一个字符的权重 
					ch=fgetc(fp1);
				}
			}
		}
		if(number!=0)
		{
			quan[i]=number;
			i++;
			number=0;//单次读取完毕后归0 
		}
	}
	return i;//得到权重数组和数组元素个数 
}

void compare(int *s1,int *s2,stack ps,int n){
	int min1=9999999,min2=9999999,i;//首先把这两个赋值一个比较大的数值，方面下面的比较,这里一定要够大，因为真的还有那么大 
	
	//获取最小值的下标 
	for(i=0;i<2*n-1;i++)
	{
		if((ps+i)->weight!=0 && (ps+i)->parent==-1)//如果当前权重不为NULL(权重不为空)，并且父结点为NULL，就是孤结点
		{
		 	if(min1>(ps+i)->weight)
		 	{
		 		*s1=i;//获取下标 
		 		min1=(ps+i)->weight;//每次比较后获取较小值，进行下一次比较  
			}
		} 
	}
	//获取次小值的下标 
	for(i=0;i<2*n-1;i++)
	{
		if((ps+i)->weight!=0 && (ps+i)->parent==-1)//如果当前权重不为NULL，并且父结点为NULL，就是孤结点
		{
		 	if(min2>(ps+i)->weight && i!=*s1)
		 	{
		 		*s2=i;//获取下标 
		 		min2=(ps+i)->weight;//每次比较后获取较小值，进行下一次比较  
			}
		} 
	}
}

stack create(int quan[],char str[][3],int n,stack ps,int *s1,int *s2){
	int i;
	
	ps = (stack)malloc((2*n-1) * sizeof(struct Node));
	
	for (i=0;i<(2*n-1);i++){
		ps[i].parent=ps[i].lchild=ps[i].rchild=-1;//初始化为-1 
		ps[i].zhi[0]=NULL;
		ps[i].zhi[1]=NULL;
		ps[i].zhi[2]=NULL;
		if (i<n){
			ps[i].weight = quan[i];
			//printf("%d\n",quan[i]);
			strcpy(ps[i].zhi, str[i]);
		}
		else ps[i].weight = 0;
	}
	
	for(i=n;i<2*n-1;i++)
	{
		compare(s1,s2,ps,n);
		(ps+i)->weight=(ps+*s1)->weight+(ps+*s2)->weight;//该节点的权重=两个儿子的权重之和 
		ps[*s2].parent=ps[*s1].parent=i;//儿子的父结点是该节点（此节点连接左儿子和右儿子） 
		ps[i].lchild=*s1;//左儿子*s1 
		ps[i].rchild=*s2;//右儿子*s2 
	}
	return ps;
}
Hofcode coding(int n,stack ps,FILE *fp2,Hofcode Hof)
{
	int i,flag,c,pp,j;
	char *tmp = (char *)malloc(n * sizeof(char));//分配求当前编码的工作空间,节点的编码数不会超过结点总数 
	Hof = (Hofcode)malloc(n * sizeof(struct Work));
	char *ne = (char *)malloc(n * sizeof(char));
	for(i = 0; i < n; i++)		//求n个叶子结点对应的哈夫曼编码
    {	strcpy(Hof[i].zhi, ps[i].zhi);
        flag =  -1;		//初始化编码位指针。每求出一个编码位，将存放在tmp[flag]中
        //从叶子到根结点求编码
		for(c = i, pp = ps[i].parent; pp != -1; c =pp, pp= ps[pp].parent)
        {
            if( ps[pp].lchild == c)
				tmp[++flag] = '0';  //左分支标0 ,从右到左的顺序编码入数组内
            else
                tmp[++flag] = '1';  //右分支标1
        }
        for (j=0;j<=flag;j++){
        	ne[j]=tmp[flag-j];
		}
        ne[++flag] = '\0';
        strcpy(Hof[i].pian, ne);
        //printf("%s:%s\n",Hof[i].zhi,Hof[i].pian);
    }
	free(tmp);
return Hof;
}	 

void compress(Hofcode Hof,FILE *fp3,FILE *fp2,int n){
	int i,j=0,h=0;
	printf("等待压缩.....\n");
	if (NULL==fp3) //以返回值fp3判断是否打开成功，如果为NULL表示失败
	{
	    printf ("Failed to open the file1 !\n");
	    exit (0) ; //终止程序，stdlib .h头文件中
	}
	if(NULL==fp2)
	{
	    printf ("Failed to open the file2 !\n");
	    exit (0);//终止程序，stdlib .h头文件中
	}
	/*
	for (i=0;i<n;i++){
		printf("%s ",Hof[i].zhi);
	}*/
	char word[3];unsigned char ch;int a=0,b=0;
	fgets(word,3,fp3);
	do//文件结束：返回非0值；文件未结束：返回0值
	{
		 if(word[1]=='\n') {
		 	h++;
		 	strcpy(word, "\\n");
		 } 
		 for(int i=0;i<n;i++)
        { 
        	//if(strcmp(Hof[i].zhi,"\\n")==0){printf("%s\n",Hof[i].pian);scanf("%d",&h);} 		
            if(strcmp(Hof[i].zhi,word)==0)
            {
            	a=0;
            	for(int c = 0; c < strlen(Hof[i].pian); c++){
            		a++;
            		if (Hof[i].pian == '\0')
            		break;
				}
				//printf("%d\n",a);
                for (int k = 0; k <=a-1; k++) 
                {
                    ch += (Hof[i].pian[k] - '0');
                    b++;
                    if (b == 8)
                    { //当ch0达到一字节写入文件
                        fwrite(&ch, sizeof(char), 1, fp2);
                        b = 0;
                    }
                    ch = ch << 1;  //为下一位写入准备
                }
	        }
        }
	}while(fgets(word,3,fp3));printf("压缩完成\n");
}

//这个解压时间特别长，不知道怎么弄 
void outpress(Hofcode Hof,FILE *fp4,FILE *fp5,int n){//fp4解压；fp5压缩 
	char num[30];
	unsigned char ch;
	//char tmp;
	int i=0,j,a=0;
	//printf("%d",i);
	printf("等待解压.....\n");
	while(fread(&ch, sizeof(ch), 1, fp5) == 1)//文件结束：返回非0值；文件未结束：返回0值
	{
		for(int bit =0;bit<=7;bit++){
			if(ch & 0x80) 
				{num[i]='1';i++;}
			else {num[i]='0';i++;}
			num[i]='\0';	
			ch = ch<<1;
		for (j=0;j<n;j++){
			//printf("%s\n",Hof[j].pian);
			
			if(strcmp(num,Hof[j].pian)==0){
				//printf("no");
				
				if (strcmp("\\n",Hof[j].zhi)==0){
					a++;
					if(a==9){
						a=0;printf("\n");
					}
				}
			    else printf("%s",Hof[j].zhi);
			    
				if (strcmp("\\n",Hof[j].zhi)==0){
					a++;
					if(a==9){
						a=0;fprintf(fp4,"\n");
					}
				}
			    else fprintf(fp4,"%s",Hof[j].zhi);
				memset(num, '\0', sizeof(num));
				i=0;
				break;				
			}		
		}	
	}
}
printf("解压完成\n");
//printf("ok");
}
