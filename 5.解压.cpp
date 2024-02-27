/*1.读取文件（权重）判断有多少个节点  2.构建哈夫曼树  3.根据构建好的哈夫曼树
写哈夫曼编码(左边为 '0' 右边为'1') 4.读原文文件根据哈夫曼编码进行压缩（8字节）
5.读压缩文件根据编码解压*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

typedef struct Node{
	char* name;//储存文字
	char Huf[50];//储存哈夫曼编码	 
	
}*stack;//同时创建了一个结构体类型的指针 

int getn(); //获取节点数 
stack Creat(int n,stack ps);//初始化
void outpress(stack ps,int n);

int main(){
	int n;
	
	stack ps;
	n =getn();//叶子节点数 
	ps = Creat(n,ps);//初始化 

	outpress(ps,n);
	
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

stack Creat(int n,stack ps){
	
	char line[100];
	char* token;
	int i,a1=0;
	
	ps = (stack)malloc((2*n-1) * sizeof(struct Node));

	FILE *fp;	
	fp = fopen("code.txt","r");
	if (fp == NULL) perror("打开失败！");
	
	while (fgets(line, sizeof(line), fp) != NULL) {
		memset(ps[a1].Huf, '\0', sizeof(ps[a1].Huf));
		token = strtok(line, ":");	
		if  (token !=NULL){
			 ps[a1].name = strdup(token);
			token = strtok(NULL, "\n");
			if  (token !=NULL)
			strcpy(ps[a1].Huf,token);
			//ps[a1].Huf = strdup(token);
		}
            a1++;
        // printf("%s",b[0]);  
        //printf("%s\n",ps[a1-1].Huf);
	}
	//fclose(fp);
return ps;	//printf("%d",a1);
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

