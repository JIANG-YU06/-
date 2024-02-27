#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>//�������isdigit���� 
#define N 100

typedef struct Node
{
	int weight;//���Ȩ�� 
	char zhi;//ֵ 
	int parent;//������ 
	int lchild;//���Ӻ� 
	int rchild;//�Һ��Ӻ� 
}*stack;//ͬʱ������һ���ṹ�����͵�ָ�� 

void compare(int *s1,int *s2,stack ps,int n);//Ѱ����Сֵ�ʹ�Сֵ���±꺯�� (ͨ��s1 s2����λ��)
stack create(int quan[],char str[],int n,stack ps,int *s1,int *s2);//��������������ʼ������� 
void coding(int n,stack ps);//���� 
//void display(stack ps,int n);//��ʾ 

int main(){
	int i,n=-1,quan[N],s1,s2;
	char str[N];
	char **HufCode;	
	stack ps;
	while(1){
		n++;
		scanf("%c,%d,",&str[n],&quan[n]);
		//printf("%d\n",n);
		if (str[n] > 90 || str[n]<65)
		break;
		//printf("%c,%d",str[n],quan[n]);
	}
	ps = create(quan,str,n,ps,&s1,&s2);
	coding(n,ps);
	free(ps);
}

void compare(int *s1,int *s2,stack ps,int n){
	int min1=9999999,min2=9999999,i;//���Ȱ���������ֵһ���Ƚϴ����ֵ����������ıȽ�,����һ��Ҫ������Ϊ��Ļ�����ô�� 
	
	//��ȡ��Сֵ���±� 
	for(i=0;i<2*n-1;i++)
	{
		if((ps+i)->weight !=0 && (ps+i)->parent==-1)//�����ǰȨ�ز�ΪNULL(Ȩ�ز�Ϊ��)�����Ҹ����ΪNULL�����ǹ½��
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

stack create(int quan[],char str[],int n,stack ps,int *s1,int *s2){
	int i;
	
	ps = (stack)malloc((2*n-1) * sizeof(struct Node));
	
	for (i=0;i<(2*n-1);i++){
		ps[i].parent=ps[i].lchild=ps[i].rchild=-1;//��ʼ��Ϊ-1 
		ps[i].zhi=NULL;
		if (i<n){
			ps[i].weight = quan[i];
			//printf("%d\n",quan[i]);
			ps[i].zhi = str[i];
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
	printf("�±�   Ȩֵ    �ڵ�     �����   ����   �Һ���\n");
	//printf("0                                  \n");
	for ( i = 0; i <2*n-1 ; i++)
	{
		printf("%-4d   %-6d   %-6c   %-6d   %-6d   %-6d\n", i, ps[i].weight, ps[i].zhi, ps[i].parent, ps[i].lchild, ps[i].rchild);
	}
	printf("\n");
	return ps;
}
void coding(int n,stack ps)
{
	int i,flag,c,pp,j;
	char tmp[n];//������ǰ����Ĺ����ռ�,�ڵ�ı��������ᳬ��������� 
	for(i = 0; i < n; i++)		//��n��Ҷ�ӽ���Ӧ�Ĺ���������
    {
        flag =  0;		//��ʼ������λָ�롣ÿ���һ������λ���������tmp[flag]��
        //��Ҷ�ӵ�����������
		for(c = i, pp = ps[i].parent ; pp != -1;  c =pp, pp= ps[pp].parent)
        {
            if( ps[pp].lchild == c)
				tmp[++flag] = '0';  //���֧��0 ,���ҵ����˳�������������
            else
                tmp[++flag] = '1';  //�ҷ�֧��1
        }
        tmp[++flag] = '\0';
        //strcpy(HufCode[i], &tmp[flag]);//������ôʹ�õĲ�̫���� 
    printf("��ǰֵΪ��%c,����ı���Ϊ��",ps[i].zhi);  
	for(j = n-1; j >0; j--)	{
		printf("%c",tmp[j]);  
	} 
	printf("\n");               
    }
	free(tmp);
}	 
//A,3,B,5,C,11,D,23,E,29,F,14,G,7,H,8