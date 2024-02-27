#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>//�������isdigit���� 
#define N 5000

typedef struct Node
{ 
	int weight;//���Ȩ�� 
	char zhi[3];//ֵ 
	int parent;//������ 
	int lchild;//���Ӻ� 
	int rchild;//�Һ��Ӻ� 
}*stack;//ͬʱ������һ���ṹ�����͵�ָ�� 

int open(int quan[],FILE *fp1,FILE *fp2,char str[][3]); 
void compare(int *s1,int *s2,stack ps,int n);//Ѱ����Сֵ�ʹ�Сֵ���±꺯�� (ͨ��s1 s2����λ��)
stack create(int quan[],char str[][3],int n,stack ps,int *s1,int *s2);//��������������ʼ������� 
char ** coding(int n,stack ps,FILE *fp2);//���� 
//void display(char **HufCode,stack ps,FILE *fp2,int n);
//void display(stack ps,int n);//��ʾ 

int main(){
	int i,n,quan[N],s1,s2;
	char str[N][3];
	char **HufCode;	
	stack ps;
/*���ļ�����Ȩ�غ��ַ�*/ 
	FILE *fp1,*fp2;
	fp1=fopen("Ȩֵ.txt","r"); //��ֻ��ģʽ���ļ�
	fp2=fopen ("����������.txt","w"); //��д���ģʽ���ļ�f2 .txt
	n=open(quan,fp1,fp2,str);//��ȡ��������������鱾����ǵ�ַ�����÷���
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
	if (NULL==fp1) //�Է���ֵfp1�ж��Ƿ�򿪳ɹ������ΪNULL��ʾʧ��
	{
	    printf ("Failed to open the file1 !\n");
	    exit (0) ; //��ֹ����stdlib .hͷ�ļ���
	}
	if(NULL==fp2)
	{
	    printf ("Failed to open the file2 !\n");
	    exit (0);//��ֹ����stdlib .hͷ�ļ���
	}
	while(!feof(fp1))//�ļ����������ط�0ֵ���ļ�δ����������0ֵ
	{
		ch=fgetc(fp1);//��ȡһ���ַ�
		if(ch<0 ||  ch==39)//�������ֺͿո� 
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
					number=number*10+ch-'0';//�õ�һ���ַ���Ȩ�� 
					ch=fgetc(fp1);
				}
			}
		}
		if(number!=0)
		{
			quan[i]=number;
			i++;
			number=0;//���ζ�ȡ��Ϻ��0 
		}
	}
	return i;//�õ�Ȩ�����������Ԫ�ظ��� 
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
}

stack create(int quan[],char str[][3],int n,stack ps,int *s1,int *s2){
	int i;
	
	ps = (stack)malloc((2*n-1) * sizeof(struct Node));
	
	for (i=0;i<(2*n-1);i++){
		ps[i].parent=ps[i].lchild=ps[i].rchild=-1;//��ʼ��Ϊ-1 
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
		(ps+i)->weight=(ps+*s1)->weight+(ps+*s2)->weight;//�ýڵ��Ȩ��=�������ӵ�Ȩ��֮�� 
		ps[*s2].parent=ps[*s1].parent=i;//���ӵĸ�����Ǹýڵ㣨�˽ڵ���������Ӻ��Ҷ��ӣ� 
		ps[i].lchild=*s1;//�����*s1 
		ps[i].rchild=*s2;//�Ҷ���*s2 
	}
/* 
	printf("�±�   Ȩֵ    �ڵ�     �����   ����   �Һ���\n");
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
	char *tmp = (char *)malloc(n * sizeof(char));//������ǰ����Ĺ����ռ�,�ڵ�ı��������ᳬ��������� 
	//HufCode=(char **)malloc( n * sizeof(char *));
	for(i = 0; i < n; i++)		//��n��Ҷ�ӽ���Ӧ�Ĺ���������
    {
        flag =  0;		//��ʼ������λָ�롣ÿ���һ������λ���������tmp[flag]��
        //��Ҷ�ӵ�����������
		for(c = i, pp = ps[i].parent; pp != -1; c =pp, pp= ps[pp].parent)
        {
            if( ps[pp].lchild == c)
				tmp[++flag] = '0';  //���֧��0 ,���ҵ����˳�������������
            else
                tmp[++flag] = '1';  //�ҷ�֧��1
        }
        //tmp[++flag] = '\0';
        tmp[0]='\0';
        //HufCode[i] = (char *)malloc((n - flag) * sizeof(char));
        //strcpy(HufCode[i], &tmp[flag]);
        //printf("%s\n",tmp);
        fprintf(fp2,"%s:%s\n",(ps+i)->zhi,tmp);
        
        
    printf("��ǰֵΪ��%c,����ı���Ϊ��",ps[i].zhi);  
	for(j = n-1; j >0; j--)	{
		printf("%c",tmp[j]);  
	} 
	printf("\n");                
    }
	free(tmp);
}	 
/*
void display(char **HufCode,stack ps,FILE *fp2,int n)  //�������ļ�д��ȥ 
{ 
	int i;
	for(i=0;i<n;i++)
	{ 
		fprintf(fp2,"%s\t%s\n",(ps+i)->zhi,HufCode[i]);//д������
	} 
} */
