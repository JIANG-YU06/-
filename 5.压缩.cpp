/*1.��ȡ�ļ���Ȩ�أ��ж��ж��ٸ��ڵ�  2.������������  3.���ݹ����õĹ�������
д����������(���Ϊ '0' �ұ�Ϊ'1') 4.��ԭ���ļ����ݹ������������ѹ����8�ֽڣ�
5.��ѹ���ļ����ݱ����ѹ*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

typedef struct Node{
	char* name;//��������
	char* Huf;//�������������	 
	
}*stack;//ͬʱ������һ���ṹ�����͵�ָ�� 

int getn(); //��ȡ�ڵ��� 
stack Creat(int n,stack ps);//��ʼ��
void compress(stack ps,int n);

int main(){
	int n;
	
	stack ps;
	n =getn();//Ҷ�ӽڵ��� 
	ps = Creat(n,ps);//��ʼ�� 

	compress(ps,n);
	
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
	char word[5];
	char line[50];
	char* token;
	int i,a1=0;
	
	ps = (stack)malloc((2*n-1) * sizeof(struct Node));

	FILE *fp;	
	fp = fopen("code.txt","rb");
	if (fp == NULL) perror("��ʧ�ܣ�");
	
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
	printf("ѹ��....\n");
	FILE *fp1,*fp2;
	fp1 = fopen("����.txt","r");
	fp2 = fopen("ѹ��.txt","w"); 
	if (fp1 == NULL) perror("��ʧ�ܣ�");
	if (fp2 == NULL) perror("��ʧ�ܣ�");
	
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
                    { //��ch0�ﵽһ�ֽ�д���ļ�
                        fwrite(&ch, sizeof(char), 1, fp2);
                        b = 0;
                    }
                    ch = ch << 1;  //Ϊ��һλд��׼��
                }
	        }
	        
        }
	}while(fgets(word,5,fp1));printf("ѹ�����\n");
fclose(fp1);
fclose(fp2);
}
