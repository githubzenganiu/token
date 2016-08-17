#include<limits.h> /* INT_MAX�� */
#include<stdio.h> /* EOF(=^Z��F6),NULL */
#define MAXSIZE 20 /* һ������ʾ����С˳�������󳤶� */
#define MAXKEY INT_MAX
#define RUNEND_SYMBOL INT_MAX
#define w 6 /* �ڴ湤���������ɵļ�¼���� */
#define M 10 /* �����M�����ݻ��� */
#define N 24 /* ����ļ���N������ */
typedef int InfoType; /* ������������������� */
typedef int KeyType; /* ����ؼ�������Ϊ���� */
typedef struct
{
  KeyType key; /* �ؼ����� */
  InfoType otherinfo; /* ������������������������ж��� */
}RedType; /* ��¼���� */
typedef struct
{
  RedType r[MAXSIZE+1]; /* r[0]���û������ڱ���Ԫ */
  int length; /* ˳������� */
}SqList; /* ˳������� */
typedef int LoserTree[w]; /* ����������ȫ�������Ҳ���Ҷ�ӣ��ɲ���˳��洢�ṹ */
typedef struct
{
  RedType rec; /* ��¼ */
  KeyType key; /* �Ӽ�¼�г�ȡ�Ĺؼ��� */
  int rnum; /* �����鲢�εĶκ� */
}RedNode,WorkArea[w]; /* �ڴ湤����������Ϊw */

void Select_MiniMax(LoserTree ls,WorkArea wa,int q)
{ /* ��wa[q]�𵽰������ĸ��Ƚ�ѡ��MINIMAX��¼������qָʾ�����ڵĹ鲢�� */
  int p,s,t;
  for(t=(w+q)/2,p=ls[t];t>0;t=t/2,p=ls[t])
    if(wa[p].rnum<wa[q].rnum||wa[p].rnum==wa[q].rnum&&wa[p].key<wa[q].key)
    {
      s=q;
      q=ls[t]; /* qָʾ�µ�ʤ���� */
      ls[t]=s;
    }
  ls[0]=q;
}
void Construct_Loser(LoserTree ls,WorkArea wa,FILE *fi)
{ /* ����w����¼���ڴ湤����wa,���ð�����ls,ѡ���ؼ�����С�ļ�¼����sָʾ */
  /* ����wa�е�λ�á�*/
  int i;
  for(i=0;i<w;++i)
    wa[i].rnum=wa[i].key=ls[i]=0; /* ��������ʼ�� */
  for(i=w-1;i>=0;--i)
  {
    fread(&wa[i].rec,sizeof(RedType),1,fi); /* ����һ����¼ */
    wa[i].key=wa[i].rec.key; /* ��ȡ�ؼ��� */
    wa[i].rnum=1; /* ��κ�Ϊ��1�� */
    Select_MiniMax(ls,wa,i); /* �������� */
  }
}
void get_run(LoserTree ls,WorkArea wa,int rc,int *rmax,FILE *fi,FILE *fo)
{ /* ���һ����ʼ�鲢�Σ�fiΪ�����ļ�ָ�룬foΪ����ļ�ָ�롣*/
  int q;
  KeyType minimax;
  while(wa[ls[0]].rnum==rc) /* ѡ�õ�MINIMAX��¼����ǰ��ʱ */
  {
    q=ls[0]; /* qָʾMINIMAX��¼��wa�е�λ�� */
    minimax=wa[q].key;
    fwrite(&wa[q].rec,sizeof(RedType),1,fo); /* ����ѡ�õ�MINIMAX��¼д������ļ� */
    fread(&wa[q].rec,sizeof(RedType),1,fi); /* �������ļ�������һ��¼(��) */
    if(feof(fi))
    { /* �����ļ������������¼������rmax+1���Σ� */
      wa[q].rnum=*rmax+1;
      wa[q].key=MAXKEY;
    }
    else
    { /* �����ļ��ǿ�ʱ */
      wa[q].key=wa[q].rec.key; /* ��ȡ�ؼ��� */
      if(wa[q].key<minimax)
      { /* �¶���ļ�¼����һ�� */
        *rmax=rc+1;
 wa[q].rnum=*rmax;
      }
      else /* �¶���ļ�¼����ǰ�� */
 wa[q].rnum=rc;
    }
    Select_MiniMax(ls,wa,q); /* ѡ���µ�MINIMAX��¼ */
  }
}
void Replace_Selection(LoserTree ls,WorkArea wa,FILE *fi,FILE *fo)
{ /* �ڰ�����ls���ڴ湤����wa�����û���ѡ���������ʼ�鲢�Σ�fiΪ�����ļ� */
  /* (ֻ���ļ�)ָ��,foΪ����ļ�(ֻд�ļ�)ָ��,�����ļ����Ѵ򿪡�*/
  int rc,rmax;
  RedType j;
  j.key=RUNEND_SYMBOL;
  Construct_Loser(ls,wa,fi); /* ���������� */
  rc=rmax=1; /* rcָʾ��ǰ���ɵĳ�ʼ�鲢�εĶκţ�rmaxָʾwa�йؼ���������ʼ�鲢�ε����κ� */
  while(rc<=rmax) /* ��rc=rmax+1����־�����ļ����û���ѡ����������� */
  {
    get_run(ls,wa,rc,&rmax,fi,fo); /* ���һ����ʼ�鲢�� */
    j.otherinfo=rc;
    fwrite(&j,sizeof(RedType),1,fo); /* ���ν�����־д������ļ� */
    rc=wa[ls[0]].rnum; /* ������һ�εĶκ� */
  }
}
void print(RedType t)
{
  printf("(%d,%d)",t.key,t.otherinfo);
}
void main()
{
  RedType b,a[N]={{51,1},{49,2},{39,3},{46,4},{38,5},{29,6},{14,7},{61,8},{15,9},{30,10},{1,11},{48,12},{52,13},{3,14},{63,15},{27,16},{4,17},{13,18},{89,19},{24,20},{46,21},{58,22},{33,23},{76,24}};
  FILE *fi,*fo;
  LoserTree ls;
  WorkArea wa;
  int i,k,j=RUNEND_SYMBOL;
  char s[3],fname[4];
  fo=fopen("ori","wb"); /* ��д�ķ�ʽ�򿪴��ļ�ori */
  fwrite(a,sizeof(RedType),N,fo); /* ������aд����ļ�ori */
  fclose(fo);
  fi=fopen("ori","rb"); /* �Զ��ķ�ʽ���´򿪴��ļ�ori */
  printf("���ļ��ļ�¼Ϊ:\n");
  for(i=1;i<=N;i++)
  {
    fread(&b,sizeof(RedType),1,fi); /* ���ν����ļ�ori�����ݶ���b */
    print(b); /* ���b������ */
    if(i%M==0)
      printf("\n");
  }
  printf("\n");
  rewind(fi); /* ʹfi��ָ�����·��ش��ļ�ori����ʼλ�ã��Ա����¶����ڴ棬������������ļ� */
  fo=fopen("out","wb"); /* ��д�ķ�ʽ�򿪳�ʼ�鲢���ļ�out */
  Replace_Selection(ls,wa,fi,fo); /* ���û���ѡ���������ʼ�鲢�� */
  fclose(fo);
  fclose(fi);
  fi=fopen("out","rb"); /* �Զ��ķ�ʽ���´򿪳�ʼ�鲢���ļ�out */
  printf("��ʼ�鲢���ļ��ļ�¼Ϊ:\n");
  i=1;
  do
  {
    k=fread(&b,sizeof(RedType),1,fi); /* ���ν����ļ�out�����ݶ���b */
    if(k==1)
    {
      print(b); /* ���b������ */
      if(i++%M==0)
        printf("\n");
    }
  }while(k==1);
  printf("\n");
  rewind(fi); /* ʹfi��ָ�����·��ش��ļ�ori����ʼλ�ã��Ա����¶����ڴ棬������������ļ� */
  k=0;
  while(!feof(fi)) /* ���������ʼ�鲢���ļ�out */
  {
    itoa(k,s,10); /* ���������ļ���f0,f1,�� */
    strcpy(fname,"f");
    strcat(fname,s);
    fo=fopen(fname,"wb"); /* ������д�ķ�ʽ���ļ�f0,f1,�� */
    do
    {
      i=fread(&b,sizeof(RedType),1,fi);
      if(i==1) /* fread()���óɹ� */
      {
 fwrite(&b,sizeof(RedType),1,fo); /* ��bд���ļ�f0,f1,�� */
        if(b.key==j) /* 1���鲢�ν��� */
        {
          k++;
          fclose(fo);
          break;
        }
      }
    }while(i==1);
  };
  fclose(fi);
  printf("������%d����ʼ�鲢���ļ�\n",k);
}