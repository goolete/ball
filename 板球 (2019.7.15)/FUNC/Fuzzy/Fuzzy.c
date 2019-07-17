#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"

#define NB 0
#define NM 1
#define NS 2
#define ZO 3
#define PS 4
#define PM 5
#define PB 6

extern float Kp,Ki,Kd;


float uf(float x,float a,float  b,float c)			//隶属度（三角形a→b→c）
{
if(x<=a)
return 0;
else if((a<x)&&(x<=b))
return  (x-a)/(b-a);
else if((b<x)&&(x<=c))
return (c-x)/(c-b);
else if(x>c)
return 0;
}

float cuf(float x,float a,float b,float c)			//规则库（）
{   float y,z;
	z=(b-a)*x+a;
	y=c-(c-b)*x;
    return (y+z)/2;
}


float ufl(float x,float a,float b)				//隶属度（下坡a→b）
{
	if(x<=a) 
	return 1;
	else if((a<x)&&(x<=b))
	return (b-x)/(b-a);
	else if(x>b)
    return 0;
}

float cufl(float x,float a,float b)
{
     
	return b-(b-a)*x;
}

float ufr(float x,float a,float b)				//隶属度（上坡a→b）
{
if(x<=a)
return 0;
if((a<x)&&(x<b))
return (x-a)/(b-a);
if(x>=b)
return 1;
}

float cufr(float x,float a,float b)				
{
return (b-a)*x+a;
}

float fand(float a,float b)					//将规则一与规则二求与，即将es和ecs的隶属度求最小		
{
	return (a<b)?a:b;
}

float forr(float a,float b)
{
	return (a<b)?b:a;
}


void fuzzy(float e,float ec)
{
int kp[7][7]={{PB,PB,PM,PM,PS,ZO,ZO},{PB,PB,PM,PS,PS,ZO,ZO},{PM,PM,PM,PS,ZO,NS,NS},{PM,PM,PS,ZO,NS,NM,NM},{PS,PS,ZO,NS,NS,NM,NM},{PS,ZO,NS,NM,NM,NM,NB},{ZO,ZO,NM,NM,NM,NB,NB}};
int kd[7][7]={{PS,NS,NB,NB,NB,NM,PS},{PS,NS,NB,NM,NM,NS,ZO},{ZO,NS,NM,NM,NS,NS,ZO},{ZO,NS,NS,NS,NS,NS,ZO},{ZO,ZO,ZO,ZO,ZO,ZO,ZO},{PB,NS,PS,PS,PS,PS,PB},{PB,PM,PM,PM,PS,PS,PB}};
int ki[7][7]={{NB,NB,NM,NM,NS,ZO,ZO},{NB,NB,NM,NS,NS,ZO,ZO},{NB,NM,NS,NS,ZO,PS,PS},{NM,NM,NS,ZO,PS,PM,PM},{NM,NS,ZO,PS,PS,PM,PB},{ZO,ZO,PS,PS,PM,PB,PB},{ZO,ZO,PS,PM,PM,PB,PB}};
float es[7],ecs[7];

es[NB]=ufl(e,-3,-1);  //e 				//模糊化，计算隶属度
es[NM]=uf(e,-3,-2,0);
es[NS]=uf(e,-3,-1,1);
es[ZO]=uf(e,-2,0,2);
es[PS]=uf(e,-1,1,3);
es[PM]=uf(e,0,2,3);
es[PB]=ufr(e,1,3);

ecs[NB]=ufl(ec,-0.3,-0.1);//ec
ecs[NM]=uf(ec,-0.3,-0.2,0);
ecs[NS]=uf(ec,-0.3,-0.1,0.1);
ecs[ZO]=uf(ec,-0.2,0,0.2);
ecs[PS]=uf(ec,-0.1,0.1,0.3);
ecs[PM]=uf(ec,0,0.2,0.3);
ecs[PB]=ufr(ec,0.1,0.3);

float form[7][7];
int i,j;
for(i=0;i<7;i++)
{float w,h,r;
  for(j=0;j<7;j++)
 {
	h=es[i];
	r=ecs[j];
   w=fand(h,r);
    form[i][j]=w;
 }
}

int a=0,b=0;
for(i=0;i<7;i++)
{
 for(j=0;j<7;j++)
 {
	 if(form[a][b]<form[i][j]) 
 {
		 a=i;
         b=j;
 }
 }
}
float lsd;
int p,d,in;
lsd=form[a][b];							//隶属度
p=kp[a][b];									//模糊推理，a、b代表e、ec的模糊化结果
d=kd[a][b];   
in=ki[a][b];

float detkp,detkd,detki;					//反模糊化
if(p==NB)
detkp=cufl(lsd,-0.3,-0.1);
else if(p==NM)
detkp=cuf(lsd,-0.3,0.2,0);
else if(p==NS)
detkp=cuf(lsd,-0.3,0.1,0.1);
else if(p==ZO)
detkp=cuf(lsd,-0.2,0,0.2);
else if(p==PS)
detkp=cuf(lsd,-0.1,0.1,0.3);
else if(p==PM)
detkp=cuf(lsd,0,0.2,0.3);
else if(p==PB)
detkp=cufr(lsd,0.1,0.3);


if(d==NB)
detkd=cufl(lsd,-3,-1);
else if(d==NM)
detkd=cuf(lsd,-3,2,0);
else if(d==NS)
detkd=cuf(lsd,-3,1,1);
else if(d==ZO)
detkd=cuf(lsd,-2,0,2);
else if(d==PS)
detkd=cuf(lsd,-1,1,3);
else if(d==PM)
detkd=cuf(lsd,0,2,3);
else if(d==PB)
detkd=cufr(lsd,1,3);

if(in==NB)
detki=cufl(lsd,-0.06,-0.02);
else if(in==NM)
detki=cuf(lsd,-0.06,-0.04,0);
else if(in==NS)
detki=cuf(lsd,-0.06,-0.02,0.02);
else if(in==ZO)
detki=cuf(lsd,-0.04,0,0.04);
else if(in==PS)
detki=cuf(lsd,-0.02,0.02,0.06);
else if(in==PM)
detki=cuf(lsd,0,0.04,0.06);
else if (in==PB)
detki=cufr(lsd,0.02,0.06);


Kp+=detkp;
Ki+=detkd;
Kd+=detki;

// det_u=kpint*(ec)+kpint*kiint*e+kpint*kdint*(e-2*e1+e2);
// u=det_u+u1;
//e2=e1; 
//u1=u;
// if(u>=10)
// {u=10;}
// if(u<=0)
// {u=0;}

 }