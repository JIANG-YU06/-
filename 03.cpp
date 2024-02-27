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

int open(int quan[],FILE *fp1,FILE *fp2,char str[][3]); 
void compare(int *s1,int *s2,stack ps,int n);//寻找最小值和次小值的下标函数 (通过s1 s2排列位置)
stack create(int quan[],char str[][3],int n,stack ps,int *s1,int *s2);//创建哈夫曼树初始化并填好 
char ** coding(int n,stack ps,FILE *fp2);//编码 
//void display(char **HufCode,stack ps,FILE *fp2,int n);
//void display(stack ps,int n);//显示 

int main(){
	int i,n,quan[N],s1,s2;
	char str[N][3];
	char **HufCode;	
	stack ps;
/*打开文件存入权重和字符*/ 
	FILE *fp1,*fp2;
	fp1=fopen("权值.txt","r"); //以只读模式打开文件
	fp2=fopen ("哈夫曼编码.txt","w"); //以写入的模式打开文件f2 .txt
	n=open(quan,fp1,fp2,str);//获取了数组个数，数组本身就是地址，不用返回
    ps = create(quan,str,n,ps,&s1,&s2);
	coding(n,ps,fp2);
	//display(HufCode,ps,fp2,n);
	free(ps);
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
		if(ch<0 ||  ch==39)//包括汉字和空格 
		{
			str[i][0]=ch; 
			str[i][1]=fgetc(fp1);
			str[i][2]='\0';
		}
		else if(ch==92 || ch==58)
		{
			continue; 
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
			ps[i].zhi[0] = str[i][0];
			ps[i].zhi[1] = str[i][1];
			ps[i].zhi[2] = str[i][2];
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
/* 
	printf("下标   权值    节点     父结点   左孩子   右孩子\n");
	//printf("0                                  \n");
	for ( i = 0; i <2*n-1 ; i++)
	{
		printf("%-4d   %-6d   %-6c   %-6d   %-6d   %-6d\n", i, ps[i].weight, ps[i].zhi, ps[i].parent, ps[i].lchild, ps[i].rchild);
	}
	printf("\n");
	*/ 
	return ps;
}
char ** coding(int n,stack ps,FILE *fp2)
{
	int i,flag,c,pp,j;
	char *tmp = (char *)malloc(n * sizeof(char));//分配求当前编码的工作空间,节点的编码数不会超过结点总数 
	//HufCode=(char **)malloc( n * sizeof(char *));
	for(i = 0; i < n; i++)		//求n个叶子结点对应的哈夫曼编码
    {
        flag =  0;		//初始化编码位指针。每求出一个编码位，将存放在tmp[flag]中
        //从叶子到根结点求编码
		for(c = i, pp = ps[i].parent; pp != -1; c =pp, pp= ps[pp].parent)
        {
            if( ps[pp].lchild == c)
				tmp[++flag] = '0';  //左分支标0 ,从右到左的顺序编码入数组内
            else
                tmp[++flag] = '1';  //右分支标1
        }
        //tmp[++flag] = '\0';
        tmp[0]='\0';
        //HufCode[i] = (char *)malloc((n - flag) * sizeof(char));
        //strcpy(HufCode[i], &tmp[flag]);
        //printf("%s\n",tmp);
        fprintf(fp2,"%s:%s\n",(ps+i)->zhi,tmp);
        
        
    printf("当前值为：%c,保存的编码为：",ps[i].zhi);  
	for(j = n-1; j >0; j--)	{
		printf("%c",tmp[j]);  
	} 
	printf("\n");                
    }
	free(tmp);
}	 
/*
void display(char **HufCode,stack ps,FILE *fp2,int n)  //将编码文件写进去 
{ 
	int i;
	for(i=0;i<n;i++)
	{ 
		fprintf(fp2,"%s\t%s\n",(ps+i)->zhi,HufCode[i]);//写入数量
	} 
} */
