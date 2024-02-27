


#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

typedef struct Node{
	char* name;//��������
	int weight;//����Ȩ��
	
	int parent;
	int left;
	int right;
	
	char Huf[100];//�������������	 
	
}*stack;//ͬʱ������һ���ṹ�����͵�ָ�� 

int getn(); //��ȡ�ڵ��� 
void compare(int *s1,int *s2,stack ps,int n);
stack Creat(int n,int *s1,int *s2,stack ps);//��ʼ��
stack code(stack ps,int n);
void compress(stack ps,int n);
void outpress(stack ps,int n); 

int main(){
	int n;
	int s1,s2;
	
	stack ps;
	printf("�����С�����");
	n =getn();//Ҷ�ӽڵ��� 
	ps = Creat(n,&s1,&s2,ps);//��ʼ�� 
	
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
	fp = fopen("Ȩֵ.txt","r");
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

void compare(int *s1,int *s2,stack ps,int n){
	int min1=9999999,min2=9999999,i;//���Ȱ���������ֵһ���Ƚϴ����ֵ����������ıȽ�,����һ��Ҫ������Ϊ��Ļ�����ô�� 
	
	//��ȡ��Сֵ���±� 
	for(i=0;i<2*n-1;i++)
	{
		if((ps+i)->weight!=0 && (ps+i)->parent==-1)//�����ǰȨ�ز�ΪNULL(Ȩ�ز�Ϊ��)�����Ҹ����ΪNULL�����ǹ½��
		{
		 	if(min1>(ps+i)->weight)
		 	{
		 		*s1=i;//��ȡ�±� 
		 		min1=(ps+i)->weight;//ÿ�αȽϺ��ȡ��Сֵ��������һ�αȽ�  
			}
		} 
	}
	//��ȡ��Сֵ���±� 
	for(i=0;i<2*n-1;i++)
	{
		if((ps+i)->weight!=0 && (ps+i)->parent==-1)//�����ǰȨ�ز�ΪNULL�����Ҹ����ΪNULL�����ǹ½��
		{
		 	if(min2>(ps+i)->weight && i!=*s1)
		 	{
		 		*s2=i;//��ȡ�±� 
		 		min2=(ps+i)->weight;//ÿ�αȽϺ��ȡ��Сֵ��������һ�αȽ�  
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
	fp = fopen("Ȩֵ.txt","r");
	if (fp == NULL) perror("��ʧ�ܣ�");
	
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
                ps[a1].weight = atoi(token + 1);//��ȡ���� 
            }
            a1++;
        }
       
        //printf("%s %d\n",ps[a1-1].name,ps[a1-1].weight);
	}
	
	for(i=n;i<2*n-1;i++)
	{
		compare(s1,s2,ps,n);//printf("%s",ps[i-n].name);
		(ps+i)->weight=(ps+*s1)->weight+(ps+*s2)->weight;//�ýڵ��Ȩ��=�������ӵ�Ȩ��֮�� 
		ps[*s2].parent=ps[*s1].parent=i;//���ӵĸ�����Ǹýڵ㣨�˽ڵ���������Ӻ��Ҷ��ӣ� 
		ps[i].left=*s1;//�����*s1 
		ps[i].right=*s2;//�Ҷ���*s2 
	}		//printf("ok");

	fclose(fp);
	return ps;	//printf("%d",a1);
}

stack code(stack ps,int n){
	int i,c,pp,a2,j;
	int flag;
	FILE *fp;	
	
	fp=fopen("code.txt","wb");
	if (fp == NULL) perror("��ʧ�ܣ�");
	
	char *tmp = (char *)malloc(n * sizeof(char));
	char *ne = (char *)malloc(n * sizeof(char));
	for(i = 0; i < n; i++)		//��n��Ҷ�ӽ���Ӧ�Ĺ���������
    {
        flag =  -1;		//��ʼ������λָ�롣ÿ���һ������λ���������tmp[flag]��
        //��Ҷ�ӵ�����������
        tmp[0]='\0';
		for(c = i, pp = ps[i].parent; pp != -1; c =pp, pp= ps[pp].parent)
        {
            if( ps[pp].left == c){
            	tmp[++flag] = '0';  //���֧��0 ,���ҵ����˳�������������
            	
			}
            else
                tmp[++flag] = '1';  //�ҷ�֧��1
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
	printf("����");
	fclose(fp);
return ps;
}

void compress(stack ps,int n){
	int i,b=0,j,a;
	char word[3];
	unsigned char ch;
	printf("ѹ��....\n");
	FILE *fp1,*fp2;
	fp1 = fopen("����.txt","r");
	fp2 = fopen("ѹ��.txt","w"); 
	if (fp1 == NULL) perror("��ʧ�ܣ�");
	if (fp2 == NULL) perror("��ʧ�ܣ�");
	
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
                    { //��ch0�ﵽһ�ֽ�д���ļ�
                        fwrite(&ch, sizeof(char), 1, fp2);
                        b = 0;
                    }
                    ch = ch << 1;  //Ϊ��һλд��׼��
                }
	        }
	        
        }
	}while(fgets(word,3,fp1));printf("ѹ�����\n");
fclose(fp1);
fclose(fp2);
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


