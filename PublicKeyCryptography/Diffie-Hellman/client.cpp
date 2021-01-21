#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <iostream>
#include <gmpxx.h>
#include <string>
#include <bits/stdc++.h>

using namespace std;

mpz_class p = 71;
mpz_class g = 61;

// Set the value of p
void setp(mpz_class newp){
    p = newp;
}

// Set the value of g
void setg(mpz_class newg){
    g = newg;
}

// Genereate a random integer which acts as the private key
mpz_class generatePrivateKey(int bits){
    mpz_class rand;

    gmp_randstate_t rstate;
    gmp_randinit_mt(rstate);
    unsigned long seed;
    gmp_randseed_ui(rstate, seed);
    mpz_urandomb(rand.get_mpz_t(), rstate, bits);

    return rand;
}

// Generate sharable public key
mpz_class generatePublicKey(mpz_class privateKey){
    mpz_class publicKey;

    mpz_powm(publicKey.get_mpz_t(), g.get_mpz_t(), privateKey.get_mpz_t(), p.get_mpz_t());

    return publicKey;
}

// Get the shared key from recieved public key and generated private key
mpz_class getSharedSecret(mpz_class publicKey, mpz_class privateKey){
     mpz_class sharedSecret;

    mpz_powm(sharedSecret.get_mpz_t(), publicKey.get_mpz_t(), privateKey.get_mpz_t(), p.get_mpz_t());

    return sharedSecret;
}


#define h_addr h_addr_list[0]

struct msg {
    char s[256];
};

//portno should be known prior
int sfd, portno = 51720;
string name = "localhost"; //name can be ipv4 address(leave it as localhost if local chatting)
struct sockaddr_in serv_addr, cli_addr;//server_details address
struct hostent *server_details;//server_details details

void myConnect() {
    if(connect(sfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting");
        exit(1);
    }
    
}

void initClient() {
    //gets server details by name of the server
    server_details = gethostbyname(name.c_str());
    if(server_details == NULL) {
        perror("Invalid host name");
        exit(1);
    }

    if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    //initialize serv_addr
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);//htons adjusts byte ordering

    //copy server address from server_details
    bcopy((char *) server_details->h_addr, 
         (char *) &serv_addr.sin_addr.s_addr, 
         server_details->h_length);

    myConnect();
}


int main() {

    initClient();

    cout << "p value is : " << p << endl;
    cout << "g value is : " << g << endl;

    mpz_class privateKey = generatePrivateKey(50);
    cout << "Private key or secret key generated(randomly) is : " << privateKey << endl;

    mpz_class publicKey = generatePublicKey(privateKey);
    cout << "Public key(g^(secret) mod p) is : " << publicKey << endl;

    struct msg buff;
    strcpy(buff.s, publicKey.get_str().c_str());
    write(sfd, &buff, sizeof(buff));

    memset(&buff, 0, sizeof(buff));
    read(sfd, &buff, sizeof(buff));
    mpz_class A(buff.s, 10);
    cout << "Recieved public key is : " << A << endl;

    cout << "\nShared secret : " << getSharedSecret(A, privateKey) << endl;

    close(sfd);
}
