


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
void compress(stack ps,int n);
void outpress(stack ps,int n); 

int main(){
	int n;
	int s1,s2;
	
	stack ps;
	printf("加载中。。。");
	n =getn();//叶子节点数 
	ps = Creat(n,&s1,&s2,ps);//初始化 
	
	ps=code(ps,n);
	
	compress(ps,n);
    //outpress(ps,n); 
	free(ps);
	return 0;
}

int getn(){
	int n=0;
	char ch;
	FILE *fp;
	fp = fopen("权值.txt","r");
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
	fp = fopen("权值.txt","r");
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
       
        //printf("%s %d\n",ps[a1-1].name,ps[a1-1].weight);
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
		if(flag+1 != 8)
        ne[++flag] = '\0';
        strcpy(ps[i].Huf,ne);
        
        //printf("%s\n",tmp);
        fprintf(fp,"%s:s\n",ps[i].name,ps[i].Huf);
        //printf("%s: %s\n",ps[i].name,ps[i].Huf);
        
    }
 	//free(tmp);
	printf("结束");
	fclose(fp);
return ps;
}

void compress(stack ps,int n){
	int i,b=0,j,a;
	char word[3];
	unsigned char ch;
	printf("压缩....\n");
	FILE *fp1,*fp2;
	fp1 = fopen("三国.txt","r");
	fp2 = fopen("压缩.txt","w"); 
	if (fp1 == NULL) perror("打开失败！");
	if (fp2 == NULL) perror("打开失败！");
	
	while(fgets(word ,3, fp1) ){
		//printf("%s\t",word);
		if(word[0]=='\n'){
  		strcpy(word, "\\n"); 
			//printf("%s\t",word);
			}			
		if(word[2]=='\n'){
            word[2]='\0';
            strcat(word, "\\n"); 
        }
	//printf("%s\t",word);
        for(int i=0;i<n;i++)
        { //printf("%s",w);
            if(strcmp(ps[i].name,word)==0)
            {/*
            	int a=0;
            	for(int c = 0; ps[i].Huf[c] != '\0'; c++){
            		a++;
            		if (ps[i].Huf == '\0')
            		break; 
				}*/
				//printf("%d\n",a);
				//printf("%s",ps[i].name);
				//printf("%s\n",ps[i].Huf);
					a=0;
            	for(int c = 0; c < strlen(ps[i].Huf); c++){
            		a++;
            		if (ps[i].Huf == '\0')
            		break;
				}
				//printf("%d\n",a);
                for (int k = 0; k <=a-1; k++) 
                {
                	if(ps[i].Huf[k] == '\0') break;
                    ch += (ps[i].Huf[k] - '0');
                    //printf("0");
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
	}while(fgets(word,3,fp1));printf("压缩完成\n");
fclose(fp1);
fclose(fp2);
}

void outpress(stack ps,int n){//fp4解压；fp5压缩 
	FILE *fp4,*fp5;
	fp4 = fopen("解压.txt","wb");
	fp5 = fopen("压缩.txt","rb"); 
	
	char num[50];
	unsigned char ch;
	//char tmp;
	int i=0,j,a=0;
	//printf("%d",i);
	//printf("等待解压.....\n");
	while(fread(&ch, sizeof(ch), 1, fp5) == 1)//文件结束：返回非0值；文件未结束：返回0值
	{
		for(int bit =0;bit<=7;bit++){
			if(ch & 0x80) 
				{num[i]='1';i++;}
			else {num[i]='0';i++;}
			//printf("%c",&num[i]);
			num[i]='\0';	
			ch = ch<<1;
			//printf("ok");
			//printf("%s\n",num);
		for (j=0;j<n;j++){
			/*
			if(strcmp("并入",ps[j].name)==0){
				printf("%s\n",ps[j].Huf);
				printf("%s\n",num);
			}*/
			
			if(strcmp(num,ps[j].Huf)==0){
			    
				if (strcmp("\\n",ps[j].name)==0){
					fprintf(fp4,"\n");
					printf("\n");
				}
				else if(strstr(ps[j].name,"\\n")){
					for (int r=0;r<strlen(ps[j].name);r++){
						if (ps[j].name[r] == '\\' || ps[j].name[r]=='n')
						ps[j].name[r]='\0';
					}
					fprintf(fp4,"%s",ps[j].name);
					printf("%s",ps[j].name);
					fprintf(fp4,"\n");
					printf("\n");
				}
			    else {
			    	printf("%s",ps[j].name);
			    	fprintf(fp4,"%s",ps[j].name);
				}
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


