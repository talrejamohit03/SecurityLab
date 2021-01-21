#include<gmp.h>
#include<stdio.h>

void main(){
	mpz_t a,b,c;
	mpz_inits(a,b,c,NULL);
	gmp_printf("Enter value of a: ");
	gmp_scanf("%Zd",a);
	gmp_printf("Enter value of b: ");
	gmp_scanf("%Zd",b);
	if(mpz_cmp(a,b)>0)
		printf("a is greater\n");
	else
		printf("b is greater\n");
}
