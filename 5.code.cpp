/*1.��ȡ�ļ���Ȩ�أ��ж��ж��ٸ��ڵ�  2.������������  3.���ݹ����õĹ�������
д����������(���Ϊ '0' �ұ�Ϊ'1') 4.��ԭ���ļ����ݹ������������ѹ����8�ֽڣ�
5.��ѹ���ļ����ݱ����ѹ*/
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

int main(){
	int n;
	int s1,s2;
	
	stack ps;
	printf("�����С�����");
	n =getn();//Ҷ�ӽڵ��� 
	ps = Creat(n,&s1,&s2,ps);//��ʼ�� 
	
	code(ps,n);
	
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
	fp = fopen("Ȩ��.txt","rb");
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
        //printf("%d\n",ps[a1-1].weight);
        //printf("%s",ps[a1-1].name);
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
	printf("����");
	fclose(fp);
return ps;
}


