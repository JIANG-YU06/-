/*1.读取文件（权重）判断有多少个节点  2.构建哈夫曼树  3.根据构建好的哈夫曼树
写哈夫曼编码(左边为 '0' 右边为'1') 4.读原文文件根据哈夫曼编码进行压缩（8字节）
5.读压缩文件根据编码解压*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

typedef struct Node{
	char* name;//储存文字
	int weight;//储存权重
	
	int parent;
	int left;
	int right;
	
	char Huf[100];//储存哈夫曼编码	 
	
}*stack;//同时创建了一个结构体类型的指针 

int getn(); //获取节点数 
void compare(int *s1,int *s2,stack ps,int n);
stack Creat(int n,int *s1,int *s2,stack ps);//初始化
stack code(stack ps,int n);

int main(){
	int n;
	int s1,s2;
	
	stack ps;
	printf("加载中。。。");
	n =getn();//叶子节点数 
	ps = Creat(n,&s1,&s2,ps);//初始化 
	
	code(ps,n);
	
	free(ps);
	return 0;
}

int getn(){
	int n=0;
	char ch;
	FILE *fp;
	fp = fopen("权重.txt","r");
	if (fp == NULL) perror("打开失败！");
	while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            n++;
        }
    }
    rewind(fp);//指针回到文件头
    fclose(fp);
	return n;
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
	//printf("%s..",ps[*s2].name);
}

stack Creat(int n,int *s1,int *s2,stack ps){
	char word[5];
	char line[50];
	char* token;
	int i,a1=0;

	FILE *fp;	
	fp = fopen("权重.txt","rb");
	if (fp == NULL) perror("打开失败！");
	
	ps = (stack)malloc((2*n-1) * sizeof(struct Node));
	
	for (i=0;i<2*n-1;i++){
		ps[i].parent=ps[i].right=ps[i].left=-1;
		ps[i].weight=0;
	}
	
	while (fgets(line, sizeof(line), fp) != NULL) {
		token = strtok(line, ":");
		if (token != NULL) {
            ps[a1].name = strdup(token);
            token = strtok(NULL, "\n");
            if (token != NULL) {
                ps[a1].weight = atoi(token + 1);//读取次数 
            }
            a1++;
        }
        //printf("%d\n",ps[a1-1].weight);
        //printf("%s",ps[a1-1].name);
	}
	
	for(i=n;i<2*n-1;i++)
	{
		compare(s1,s2,ps,n);//printf("%s",ps[i-n].name);
		(ps+i)->weight=(ps+*s1)->weight+(ps+*s2)->weight;//该节点的权重=两个儿子的权重之和 
		ps[*s2].parent=ps[*s1].parent=i;//儿子的父结点是该节点（此节点连接左儿子和右儿子） 
		ps[i].left=*s1;//左儿子*s1 
		ps[i].right=*s2;//右儿子*s2 
	}		//printf("ok");

	fclose(fp);
	return ps;	//printf("%d",a1);
}

stack code(stack ps,int n){
	int i,c,pp,a2,j;
	int flag;
	FILE *fp;	
	
	fp=fopen("code.txt","wb");
	if (fp == NULL) perror("打开失败！");
	
	char *tmp = (char *)malloc(n * sizeof(char));
	char *ne = (char *)malloc(n * sizeof(char));
	for(i = 0; i < n; i++)		//求n个叶子结点对应的哈夫曼编码
    {
        flag =  -1;		//初始化编码位指针。每求出一个编码位，将存放在tmp[flag]中
        //从叶子到根结点求编码
        tmp[0]='\0';
		for(c = i, pp = ps[i].parent; pp != -1; c =pp, pp= ps[pp].parent)
        {
            if( ps[pp].left == c){
            	tmp[++flag] = '0';  //左分支标0 ,从右到左的顺序编码入数组内
            	
			}
            else
                tmp[++flag] = '1';  //右分支标1
        }
        for (j=0;j<=flag;j++){
        	ne[j]=tmp[flag-j];
		}
        ne[++flag] = '\0';
        strcpy(ps[i].Huf,ne);
        
        //printf("%s\n",tmp);
        fprintf(fp,"%s:%s\n",ps[i].name,ps[i].Huf);
        //printf("%s: %s\n",ps[i].name,ps[i].Huf);
        
        /*
        for(j = 0; j < 4; j++){
        	 tmp[++flag] = '0';
		}
		tmp[++flag] = '1';
        tmp[++flag] = '\0';
        strcpy(Hof[i].pian, tmp);*/
    }
 	//free(tmp);
	printf("结束");
	fclose(fp);
return ps;
}


