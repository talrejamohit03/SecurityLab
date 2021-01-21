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

void extended_gcd(mpz_t a, mpz_t b, mpz_t *s, mpz_t *t,mpz_t *gcd){
    // condition to stop the recursion (base case)
    if(mpz_cmp_ui(b,0)==0){
        mpz_set_ui(*s,1);
        mpz_set_ui(*t,0);
        mpz_set(*gcd,a);
        return;
    }
    mpz_t s1,t1,tmp;
    mpz_inits(s1,t1,tmp,NULL);
    mpz_mod(tmp,a,b);
    extended_gcd(b,tmp,&s1,&t1,gcd);
    // update s
    mpz_set(*s,t1);
    // update t
    mpz_t temp;
    mpz_inits(temp,NULL);
    mpz_fdiv_q(temp,a,b);
    mpz_mul(temp,temp,t1);
    mpz_sub(temp,s1,temp);
    mpz_set(*t,temp);
    return;
}

int main(){
    srand(time(NULL));
    struct publickey public_key;

    // key generation
    mpz_t x;
    mpz_inits(public_key.q,public_key.alpha,public_key.y,x,NULL);
    mpz_set_ui(public_key.q,23);
    mpz_set_ui(public_key.alpha,4);
    gmp_printf("Prime Number q : %Zd\n",public_key.q);
    gmp_printf("Primitive Root alpha : %Zd\n",public_key.alpha);
    
    // private key - choose a random number 1 < x < p - 1
    do{
        mpz_set_ui(x,rand());
        mpz_t temp;
        mpz_init(temp);
        mpz_sub_ui(temp,public_key.q,1);
        mpz_mod(x,x,temp);
    }while(mpz_cmp_ui(x,1)<=0);
    // public key y
    mpz_powm(public_key.y,public_key.alpha,x,public_key.q);
    // write public key to a file
    gmp_printf("Sending Public Key\n");
    FILE* file = fopen("publicKey.txt","w");
    char *prime, *primitive, *pub;
    prime = (char*)malloc(100*sizeof(char));
    primitive = (char*)malloc(100*sizeof(char));
    pub = (char*)malloc(100*sizeof(char));
    mpz_get_str(prime,10,public_key.q);
    mpz_get_str(primitive,10,public_key.alpha);
    mpz_get_str(pub,10,public_key.y);
    fprintf(file,"%s\n%s\n%s",prime,primitive,pub);
    fclose(file);

    mkfifo("elgamal",0666);
    int fd = open("elgamal",O_RDONLY);
    char *status = (char*)malloc(100*sizeof(char));
    read(fd,status,100*sizeof(char));
    close(fd);
    
    FILE *f = fopen("channel.txt","r");
    char *C1, *C2;
    C1 = (char*)malloc(100*sizeof(char));
    C2 = (char*)malloc(100*sizeof(char));
    fscanf(f,"%s\n%s",C1,C2);
    fclose(f);
    mpz_t M,c1,c2,k;
    mpz_inits(M,c1,c2,k,NULL);
    mpz_set_str(c1,C1,10);
    mpz_set_str(c2,C2,10);
    // compute k
    mpz_powm(k,c1,x,public_key.q);
    gmp_printf("C1 = %Zd\n",c1);
    gmp_printf("C2 = %Zd\n",c2);
    gmp_printf("K  = %Zd\n",k);
    // compute M
    mpz_t gcd,s,t;
    mpz_inits(gcd,s,t,NULL);
    extended_gcd(k,public_key.q,&s,&t,&gcd);
    mpz_mul(M,c2,s);
    mpz_mod(M,M,public_key.q);
    gmp_printf("Received plain text = %Zd\n\n",M);
    if(!mpz_cmp_ui(M,0))
        return 1;
     return 0;
}
