/*1.��ȡ�ļ���Ȩ�أ��ж��ж��ٸ��ڵ�  2.������������  3.���ݹ����õĹ�������
д����������(���Ϊ '0' �ұ�Ϊ'1') 4.��ԭ���ļ����ݹ������������ѹ����8�ֽڣ�
5.��ѹ���ļ����ݱ����ѹ*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

typedef struct Node{
	char* name;//��������
	char Huf[50];//�������������	 
	
}*stack;//ͬʱ������һ���ṹ�����͵�ָ�� 

int getn(); //��ȡ�ڵ��� 
stack Creat(int n,stack ps);//��ʼ��
void outpress(stack ps,int n);

int main(){
	int n;
	
	stack ps;
	n =getn();//Ҷ�ӽڵ��� 
	ps = Creat(n,ps);//��ʼ�� 

	outpress(ps,n);
	
	free(ps);
	return 0;
}

int getn(){
	int n=0;
	char ch;
	FILE *fp;
	fp = fopen("Ȩ��.txt","r");
	if (fp == NULL) perror("��ʧ�ܣ�");
	while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            n++;
        }
    }
    rewind(fp);//ָ��ص��ļ�ͷ
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
	if (fp == NULL) perror("��ʧ�ܣ�");
	
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

void outpress(stack ps,int n){//fp4��ѹ��fp5ѹ�� 
	FILE *fp4,*fp5;
	fp4 = fopen("��ѹ.txt","wb");
	fp5 = fopen("ѹ��.txt","rb"); 
	
	char num[50];
	unsigned char ch;
	//char tmp;
	int i=0,j,a=0;
	//printf("%d",i);
	//printf("�ȴ���ѹ.....\n");
	while(fread(&ch, sizeof(ch), 1, fp5) == 1)//�ļ����������ط�0ֵ���ļ�δ����������0ֵ
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
			if(strcmp("����",ps[j].name)==0){
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

printf("��ѹ���\n");
//printf("ok");
}

