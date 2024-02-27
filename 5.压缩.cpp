/*1.读取文件（权重）判断有多少个节点  2.构建哈夫曼树  3.根据构建好的哈夫曼树
写哈夫曼编码(左边为 '0' 右边为'1') 4.读原文文件根据哈夫曼编码进行压缩（8字节）
5.读压缩文件根据编码解压*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

typedef struct Node{
	char* name;//储存文字
	char* Huf;//储存哈夫曼编码	 
	
}*stack;//同时创建了一个结构体类型的指针 

int getn(); //获取节点数 
stack Creat(int n,stack ps);//初始化
void compress(stack ps,int n);

int main(){
	int n;
	
	stack ps;
	n =getn();//叶子节点数 
	ps = Creat(n,ps);//初始化 

	compress(ps,n);
	
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
	char word[5];
	char line[50];
	char* token;
	int i,a1=0;
	
	ps = (stack)malloc((2*n-1) * sizeof(struct Node));

	FILE *fp;	
	fp = fopen("code.txt","rb");
	if (fp == NULL) perror("打开失败！");
	
	while (fgets(line, sizeof(line), fp) ) {	
	token = strtok(line, ":");	
		if  (token !=NULL){
			 ps[a1].name = strdup(token);
			token = strtok(NULL, "\n");
			ps[a1].Huf = strdup(token);
		}
            a1++;
        //printf("%s",ps[a1-1].name);
        //printf("%s\n",ps[a1-1].Huf);
	}
	fclose(fp);
	return ps;	//printf("%d",a1);
}
void compress(stack ps,int n){
	int i,b=0,j,a;
	char word[6];
	unsigned char ch;
	printf("压缩....\n");
	FILE *fp1,*fp2;
	fp1 = fopen("三国.txt","r");
	fp2 = fopen("压缩.txt","w"); 
	if (fp1 == NULL) perror("打开失败！");
	if (fp2 == NULL) perror("打开失败！");
	
	while(fgets(word ,5, fp1) ){
		//printf("%s\t",word);
		if(word[0]=='\n'){
  		strcpy(word, "\\n"); 
			//printf("%s\t",word);
			}			
		if(word[2]=='\n'){
            word[2]='\0';
            strcat(word, "\\n"); 
        }
        if(word[3]=='\n'){
            word[3]='\0';
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
				printf("%s",ps[i].name);
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
	}while(fgets(word,5,fp1));printf("压缩完成\n");
fclose(fp1);
fclose(fp2);
}
