#include<stdio.h>
#include<gmp.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

struct publickey{
    mpz_t q;    // prime number
    mpz_t alpha;    // primitive root
    mpz_t y;    // public key
};

int main(){
    struct publickey public_key;
    mpz_inits(public_key.q,public_key.alpha,public_key.y,NULL);

    // read public key of receiver
    FILE *file = fopen("publicKey.txt","r");
    char *prime, *primitive, *pub;
    prime = (char*)malloc(100*sizeof(char));
    primitive = (char*)malloc(100*sizeof(char));
    pub = (char*)malloc(100*sizeof(char));
    fscanf(file,"%s\n%s\n%s",prime,primitive,pub);
    fclose(file);
    mpz_set_str(public_key.q,prime,10);
    mpz_set_str(public_key.alpha,primitive,10);
    mpz_set_str(public_key.y,pub,10);
    gmp_printf("Reading public key of receiver ...\n");
    gmp_printf("Prime Number q = %Zd\n",public_key.q);
    gmp_printf("Primitive root alpha = %Zd\n",public_key.alpha);

    mkfifo("elgamal",0666);
    // encryption
    //while(1){
    srand(time(NULL));
    mpz_t M, k, K, c1, c2;
    mpz_inits(M,k,K,c1,c2,NULL);
    printf("\nPlain text (number less than q-1) = ");
    gmp_scanf("%Zd",&M);
    // random number k < P
    do{
        mpz_set_ui(k,rand());
        mpz_mod(k,k,public_key.q);
    }while(mpz_cmp_ui(k,1)<=0);
    mpz_powm(K,public_key.y,k,public_key.q);
    // compute c1
    mpz_powm(c1,public_key.alpha,k,public_key.q);
    // compute c2
    mpz_mul(c2,K,M);
    mpz_mod(c2,c2,public_key.q);
    // write c1 and c2 to a file
    FILE *f = fopen("channel.txt","w");
    char *C1, *C2;
    C1 = (char*)malloc(100*sizeof(char));
    C2 = (char*)malloc(100*sizeof(char));
    mpz_get_str(C1,10,c1);
    mpz_get_str(C2,10,c2);
    fprintf(f,"%s\n%s",C1,C2);
    fclose(f);
    gmp_printf("C1 = %Zd\n",c1);
    gmp_printf("C2 = %Zd\n",c2);
    gmp_printf("K  = %Zd\n",K);
    printf("Sending Encrypted message\n");
    char status[] = "sent";
    int fd = open("elgamal",O_WRONLY);
    write(fd,status,strlen(status));
    close(fd);
    if(!mpz_cmp_ui(M,0))
        return 1;
    return 0;
}
