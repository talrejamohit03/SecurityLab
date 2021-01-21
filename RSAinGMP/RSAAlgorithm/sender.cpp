#include<iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include<gmp.h>

using namespace std;


void encrypt(mpz_t encryptedMsg,mpz_t msg, mpz_t exp, mpz_t mod){
	mpz_powm(encryptedMsg,msg,exp,mod);
}
int main(){
	
	FILE *infile = fopen("key.dat","r");
	mpz_t e,n;
	mpz_inits(e,n,NULL);
	mpz_inp_raw(e,infile);
	mpz_inp_raw(n,infile);
	
	gmp_printf("Public Key for encryption = %Zd, %Zd\n",e,n);
	
	fclose(infile);
	
	mpz_t msg,encryptedMsg;
	mpz_inits(msg,encryptedMsg,NULL);
	cout<<"Enter message M ";
	gmp_printf("(an integer less than %Zd) = ",n);
	gmp_scanf("%Zd",msg);
	
	encrypt(encryptedMsg,msg,e,n);
	gmp_printf("Encrypted message = %Zd\n",encryptedMsg);
	
	FILE *fout=fopen("message.dat","w");
	mpz_out_raw(fout,encryptedMsg);
	fclose(fout);
	cout<<"written to file\n";
	
	return 0;
}		
	
	
