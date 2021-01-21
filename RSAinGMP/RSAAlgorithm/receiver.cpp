#include<iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<gmp.h>
#include<stdio.h>

using namespace std;


void multiplicativeInv(mpz_t res,mpz_t a,mpz_t b){
	mpz_t t,sa,sb,t0,t1,t2,q;
	mpz_inits(t,sa,sb,t0,t1,t2,q,NULL);
	mpz_set(sa,a);
	mpz_set(sb,b);
	mpz_set_ui(t0,1);
	mpz_set_ui(t1,0);
	while(mpz_cmp_ui(sb,0)!=0){
		mpz_set(t,sb);
		mpz_fdiv_qr(q,sb,sa,sb);
		mpz_set(sa,t);
		mpz_set(t,t1);
		mpz_mul(q,q,t1);
		mpz_sub(t1,t0,q);
		mpz_set(t0,t);
		
	}
	if(mpz_cmp_si(t0,0)<0)
		mpz_add(t0,t0,b);
	mpz_set(res,t0);
}

void gcd(mpz_t res,mpz_t a, mpz_t b){
	
	mpz_t t,sa,sb;
	mpz_inits(t,sa,sb,NULL);
	mpz_set(sa,a);
	mpz_set(sb,b);
	
	while(mpz_cmp_ui(sb,0)!=0){
		mpz_set(t,sb);
		mpz_mod(sb,sa,sb);
		mpz_set(sa,t);
	}
	mpz_set(res,sa);
}

void decrypt(mpz_t decryptedMsg,mpz_t msg, mpz_t exp, mpz_t mod){
	mpz_powm(decryptedMsg,msg,exp,mod);
}
int main(){

	mpz_t p,q,n,phi_n,phi_nt,e,d,gcdVal;
	mpz_inits(p,q,n,phi_n,phi_nt,e,d,gcdVal,NULL);
	gmp_printf("Enter value of large prime p: ");
	gmp_scanf("%Zd",p);
	gmp_printf("Enter value of large prime q: ");
	gmp_scanf("%Zd",q);
	
	//System modulus n = p*q
	mpz_mul(n,p,q);
	
	//Euler toitient funciton phi(n) = (p-1)*(q-1)
	mpz_sub_ui(p,p,1);
	mpz_sub_ui(q,q,1);
	mpz_mul(phi_n,p,q);
	
	//Public Key e (encrypt)
	//generating a (random number in the range of 2 to phi_n-1) == 2 + (random number in range 0 to phi_n-3)
	mpz_sub_ui(phi_nt,phi_n,2);
	
	unsigned long int seed = 9863487;
	gmp_randstate_t state;
	gmp_randinit_mt(state);
	gmp_randseed_ui(state,seed);
	
	
	mpz_urandomm(e,state,phi_nt);
	mpz_add_ui(e,e,2);
	
	gcd(gcdVal,e,phi_n);
	
	//Keep sampling e until gcd(e,phi_n)!=1
	while(mpz_cmp_ui(gcdVal,1)!=0){
		seed++;
		gmp_randseed_ui(state,seed);
		mpz_urandomm(e,state,phi_nt);
		mpz_add_ui(e,e,2);
		gcd(gcdVal,e,phi_n);
	}
	gmp_printf("Public Key for encryption = %Zd\n",e);
	
	multiplicativeInv(d,e,phi_n);
	
	gmp_printf("Private Key for decryption = %Zd\n",d);
	
	//Writing public key into file
	FILE *outfile = fopen("key.dat","w");
	mpz_out_raw(outfile,e);
	mpz_out_raw(outfile,n);
	fclose(outfile);
	printf("Public Key is now known to all other users\n");
	
	printf("Waiting for message from sender\n");
	sleep(8);
	
	FILE *in = fopen("message.dat","r");
	cout<<"File opened\n";
	mpz_t msg,decryptedMsg;
	mpz_inits(msg,decryptedMsg,NULL);
	mpz_inp_raw(msg,in);
	
	gmp_printf("Encrypted message = %Zd\n",msg);
	fclose(in);
	
	decrypt(decryptedMsg,msg,d,n);
	
	gmp_printf("Decrypted message = %Zd\n",decryptedMsg);
	
	return 0;
}
	
	
	
