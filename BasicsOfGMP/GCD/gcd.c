#include<gmp.h>
#include<stdio.h>

void main(){
	mpz_t a,b,t,sa,sb;
	mpz_inits(a,b,t,sa,sb,NULL);
	gmp_printf("Enter value of a: ");
	gmp_scanf("%Zd",a);
	gmp_printf("Enter value of b: ");
	gmp_scanf("%Zd",b);
	mpz_set(sa,a);
	mpz_set(sb,b);
	while(mpz_cmp_ui(b,0)!=0){
		mpz_set(t,b);
		mpz_mod(b,a,b);
		mpz_set(a,t);
	}
	gmp_printf("GCD of %Zd and %Zd = %Zd\n",sa,sb,a);
}
