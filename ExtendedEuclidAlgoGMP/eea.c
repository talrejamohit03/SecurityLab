#include<gmp.h>
#include<stdio.h>

void main(){
	mpz_t a,b,t,sa,sb,t0,t1,t2,q;
	mpz_inits(a,b,t,sa,sb,t0,t1,t2,q,NULL);
	gmp_printf("Enter value of a: ");
	gmp_scanf("%Zd",a);
	gmp_printf("Enter value of b: ");
	gmp_scanf("%Zd",b);
	mpz_set(sa,a);
	mpz_set(sb,b);
	mpz_set_ui(t0,1);
	mpz_set_ui(t1,0);
	while(mpz_cmp_ui(b,0)!=0){
		mpz_set(t,b);
		mpz_fdiv_qr(q,b,a,b);
		mpz_set(a,t);
		mpz_set(t,t1);
		mpz_mul(q,q,t1);
		mpz_sub(t1,t0,q);
		mpz_set(t0,t);
		
	}
	if(mpz_cmp_si(t0,0)<0)
		mpz_add(t0,t0,sb);
	if(mpz_cmp_ui(a,1)!=0)
		gmp_printf("Multiplicative Inverse of %Zd mod %Zd does not exist\n",sa,sb);
	else
		gmp_printf("Multiplicative Inverse of %Zd mod %Zd = %Zd\n",sa,sb,t0);
}
