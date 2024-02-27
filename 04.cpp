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

typedef struct Work
{ 
	char zhi[3];
	char pian[30];
}*Hofcode;//ͬʱ������һ���ṹ�����͵�ָ�� 

int open(int quan[],FILE *fp1,FILE *fp2,char str[][3]); 
void compare(int *s1,int *s2,stack ps,int n);//Ѱ����Сֵ�ʹ�Сֵ���±꺯�� (ͨ��s1 s2����λ��)
stack create(int quan[],char str[][3],int n,stack ps,int *s1,int *s2);//��������������ʼ������� 
Hofcode coding(int n,stack ps,FILE *fp2,Hofcode Hof);//���� 
void compress(Hofcode Hof,FILE *fp3,FILE *fp2,int n);//ѹ�� 
void outpress(Hofcode Hof,FILE *fp4,FILE *fp5,int n);

int main(){
	int n,quan[N],s1,s2;
	char str[N][3];
	Hofcode Hof;	
	stack ps;
/*���ļ�����Ȩ�غ��ַ�*/ 
	FILE *fp1,*fp2,*fp3,*fp4,*fp5;
	fp1=fopen("Ȩֵ.txt","rb"); //��ֻ��ģʽ���ļ�
	fp2=fopen ("ѹ��.txt","wb"); 
	fp3=fopen ("����.txt","rb");
	fp4=fopen ("��ѹ.txt","wb");
	fp5=fopen ("ѹ��.txt","rb");
	n=open(quan,fp1,fp2,str);//��ȡ��������������鱾����ǵ�ַ�����÷���
    ps = create(quan,str,n,ps,&s1,&s2);
	Hof = coding(n,ps,fp2,Hof);
	compress(Hof,fp3,fp2,n);
	outpress(Hof,fp4,fp5,n);
	//printf("okk");
	free(ps);
	free(Hof);
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
		if(ch<0)//�������ֺͿո� 
		{
			str[i][0]=ch; 
			str[i][1]=fgetc(fp1);
			str[i][2]='\0';
		}
		else if(ch=='\n')
			{
			strcpy(str[i],"\\n");
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
			strcpy(ps[i].zhi, str[i]);
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
	return ps;
}
Hofcode coding(int n,stack ps,FILE *fp2,Hofcode Hof)
{
	int i,flag,c,pp,j;
	char *tmp = (char *)malloc(n * sizeof(char));//������ǰ����Ĺ����ռ�,�ڵ�ı��������ᳬ��������� 
	Hof = (Hofcode)malloc(n * sizeof(struct Work));
	char *ne = (char *)malloc(n * sizeof(char));
	for(i = 0; i < n; i++)		//��n��Ҷ�ӽ���Ӧ�Ĺ���������
    {	strcpy(Hof[i].zhi, ps[i].zhi);
        flag =  -1;		//��ʼ������λָ�롣ÿ���һ������λ���������tmp[flag]��
        //��Ҷ�ӵ�����������
		for(c = i, pp = ps[i].parent; pp != -1; c =pp, pp= ps[pp].parent)
        {
            if( ps[pp].lchild == c)
				tmp[++flag] = '0';  //���֧��0 ,���ҵ����˳�������������
            else
                tmp[++flag] = '1';  //�ҷ�֧��1
        }
        for (j=0;j<=flag;j++){
        	ne[j]=tmp[flag-j];
		}
        ne[++flag] = '\0';
        strcpy(Hof[i].pian, ne);
        //printf("%s:%s\n",Hof[i].zhi,Hof[i].pian);
    }
	free(tmp);
return Hof;
}	 

void compress(Hofcode Hof,FILE *fp3,FILE *fp2,int n){
	int i,j=0,h=0;
	printf("�ȴ�ѹ��.....\n");
	if (NULL==fp3) //�Է���ֵfp3�ж��Ƿ�򿪳ɹ������ΪNULL��ʾʧ��
	{
	    printf ("Failed to open the file1 !\n");
	    exit (0) ; //��ֹ����stdlib .hͷ�ļ���
	}
	if(NULL==fp2)
	{
	    printf ("Failed to open the file2 !\n");
	    exit (0);//��ֹ����stdlib .hͷ�ļ���
	}
	/*
	for (i=0;i<n;i++){
		printf("%s ",Hof[i].zhi);
	}*/
	char word[3];unsigned char ch;int a=0,b=0;
	fgets(word,3,fp3);
	do//�ļ����������ط�0ֵ���ļ�δ����������0ֵ
	{
		 if(word[1]=='\n') {
		 	h++;
		 	strcpy(word, "\\n");
		 } 
		 for(int i=0;i<n;i++)
        { 
        	//if(strcmp(Hof[i].zhi,"\\n")==0){printf("%s\n",Hof[i].pian);scanf("%d",&h);} 		
            if(strcmp(Hof[i].zhi,word)==0)
            {
            	a=0;
            	for(int c = 0; c < strlen(Hof[i].pian); c++){
            		a++;
            		if (Hof[i].pian == '\0')
            		break;
				}
				//printf("%d\n",a);
                for (int k = 0; k <=a-1; k++) 
                {
                    ch += (Hof[i].pian[k] - '0');
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
	}while(fgets(word,3,fp3));printf("ѹ�����\n");
}

//�����ѹʱ���ر𳤣���֪����ôŪ 
void outpress(Hofcode Hof,FILE *fp4,FILE *fp5,int n){//fp4��ѹ��fp5ѹ�� 
	char num[30];
	unsigned char ch;
	//char tmp;
	int i=0,j,a=0;
	//printf("%d",i);
	printf("�ȴ���ѹ.....\n");
	while(fread(&ch, sizeof(ch), 1, fp5) == 1)//�ļ����������ط�0ֵ���ļ�δ����������0ֵ
	{
		for(int bit =0;bit<=7;bit++){
			if(ch & 0x80) 
				{num[i]='1';i++;}
			else {num[i]='0';i++;}
			num[i]='\0';	
			ch = ch<<1;
		for (j=0;j<n;j++){
			//printf("%s\n",Hof[j].pian);
			
			if(strcmp(num,Hof[j].pian)==0){
				//printf("no");
				
				if (strcmp("\\n",Hof[j].zhi)==0){
					a++;
					if(a==9){
						a=0;printf("\n");
					}
				}
			    else printf("%s",Hof[j].zhi);
			    
				if (strcmp("\\n",Hof[j].zhi)==0){
					a++;
					if(a==9){
						a=0;fprintf(fp4,"\n");
					}
				}
			    else fprintf(fp4,"%s",Hof[j].zhi);
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
