#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX_COEF 5
#define POLY_MAX_TERM 10

char var_cand[50]={"xy\0"};

int rn(int min, int max) { //[min,max]
return min+rand()%(max-min+1);}

int abs(int x) {
int y=x;
if (x<0) {y=-x;}
return y;
}

struct term {
int coef;
char var[50];
};

struct term rand_term () {
struct term x={rn(-MAX_COEF,MAX_COEF), {'\0'}};

int i;
for (i=0;var_cand[i]!='\0';i++) {;} //max index of var_cand = i-1

int max_power=rn(1,5);
for (int j=0;j<max_power;j++) {x.var[j]=var_cand[rn(0, i-1)];}

return x;
}

struct term multiply_term(struct term t1, struct term t2) {
struct term t3={0,{'\0'}};
t3.coef=t1.coef*t2.coef;

int i;
for (i=0;t1.var[i]!='\0';i++) {t3.var[i]=t1.var[i];}
for (int j=0;t2.var[j]!='\0';j++) {t3.var[i-1+1+j]=t2.var[j];}

return t3;
}

int print_term (struct term t) {
if (t.coef==0) {;}
else {
if (t.coef>0) {printf("+");}
if (t.coef!=-1 && t.coef!=1) {printf("%d", t.coef);}
if ((t.coef==-1 || t.coef==1) && t.var[0]=='\0') {printf("%d", t.coef);}

//max len 50

char vset[50]={'\0'};
int expo[50]={0};

for (int i=0;t.var[i]!='\0';i++) {
for (int j=0;j<50;j++) {
if (vset[j]==t.var[i]) {expo[j]++;break;}
else if (vset[j]=='\0') {vset[j]=t.var[i];expo[j]=1;break;}
}
}

for (int k=0;vset[k]!='\0';k++) {
printf("%c", vset[k]);
if (expo[k]>1) {printf("^%d", expo[k]);}
}

/*printf("%s", var);*/

}
}

struct poly {
struct term terms[POLY_MAX_TERM];
int nom;
};

int print_poly (struct poly p) {
for (int i=0;i<p.nom;i++) {print_term(p.terms[i]);}
}

struct poly multiply_poly (struct poly p1, struct poly p2) {
struct poly p3;

int k=0;
for (int i=0;i<p1.nom;i++) {
for (int j=0;j<p2.nom;j++) {p3.terms[k]=multiply_term(p1.terms[i],p2.terms[j]);k++;} 
}

p3.nom=p1.nom*p2.nom;

return p3;
}

struct poly shuffle_poly (struct poly p) {

struct poly sh;
sh.nom=p.nom;
int sh01[100]={0};

for (int i=0;i<p.nom;i++) {
int j=rn(0,p.nom-1);
while (true) {
if (sh01[j]==0) {sh.terms[j]=p.terms[i];sh01[j]=1;break;}
else j=rn(0,p.nom-1);}
}

return sh;

}

int com_fac_coef(struct poly p) {
int finder=1;
int largest=finder;

while (true) {

bool is_com_fac=true;
bool too_large=true;
for (int i=0;i<p.nom;i++) {
if (abs(p.terms[i].coef)%finder!=0) {is_com_fac=false;}
if (abs(p.terms[i].coef)>=finder) {too_large=false;}
}

if (too_large==true) {return largest;}
if (is_com_fac==true) {largest=finder;}

finder++;
}

}

struct term com_fac(struct poly p) {
struct term com;
com.coef=com_fac_coef(p);

char vset[50]={'\0'};
int pow[50]={0};

for (int i=0;p.terms[0].var[i]!='\0';i++) {

char var_i=p.terms[0].var[i];

int pow_i=1000;
for (int j=0;j<p.nom;j++) {
int pow_ij=0;
for (int k=0;p.terms[j].var[k]!='\0';k++) {
if (var_i==p.terms[j].var[k]) {pow_ij++;}
}
if (pow_ij<pow_i && p.terms[j].coef!=0) {pow_i=pow_ij;}
}

for (int h=0;h<50;h++) {
if (vset[h]==var_i) {break;}
if (vset[h]=='\0') {vset[h]=var_i;pow[h]=pow_i;break;}
}

}

int w=0;
for (int u=0;u<50;u++) {
for (int v=0;v<pow[u];v++) {com.var[w]=vset[u];w++;}
}
com.var[w]='\0';


return com;
}

struct poly fac_poly(struct poly p) {
struct term fac=com_fac(p);

for (int i=0;i<p.nom;i++) {
struct term factored;
factored.coef=p.terms[i].coef/fac.coef;

for (int j=0;fac.var[j]!='\0';j++) {
for (int k=0;p.terms[i].var[k]!='\0';k++) {
if (fac.var[j]==p.terms[i].var[k]) {p.terms[i].var[k]='@';break;}
}
}

int h=0;
for (int u=0;p.terms[i].var[u]!='\0';u++) {
if (p.terms[i].var[u]!='@') {factored.var[h]=p.terms[i].var[u];h++;}
}
factored.var[h]='\0';

p.terms[i]=factored;

}

return p;
}


void main () {

srand(time(NULL));

struct poly p1;
p1.terms[0]=rand_term();
p1.terms[1]=rand_term();
p1.nom=2;

struct poly p2;
p2.terms[0]=rand_term();
p2.terms[1]=rand_term();
p2.nom=2;


struct poly p3=multiply_poly(p1, p2);

p3=shuffle_poly(p3);
print_poly(p3);
printf("\n");

int c=getchar();
(void)c;

print_term(multiply_term(com_fac(p1),com_fac(p2)));
printf("*");
printf("(");
print_poly(fac_poly(p1));
printf(")*(");
print_poly(fac_poly(p2));
printf(")\n");


/*struct term t1=rand_term();
print_term(t1.coef,t1.var);
printf("\n");
struct term t2=rand_term();
print_term(t2.coef,t2.var);
printf("\n");

struct term t3=multiply_term(t1,t2);
print_term(t3.coef,t3.var); db*/



}
