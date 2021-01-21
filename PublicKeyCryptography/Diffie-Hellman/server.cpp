#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
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

//msg structure
struct msg {
    char s[256]; //message content
};

//all the vars needed
int sfd, portno = 51720, clientfd;
struct sockaddr_in serv_addr, cli_addr;
struct msg buff;

void initServer() {
    //initialize server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);

    //create socket
    if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    //setsockopt to reuse the same address
    int opt = 1;
    if((setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))) < 0) {
        perror("setsockopt");
        exit(1);
    }

    //bind address to socket fd
    if(bind(sfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    if(listen(sfd, 5) < 0) {
        perror("listen");
        exit(1);
    }

    int len = sizeof(cli_addr);
    clientfd = accept(sfd, (struct sockaddr*)&cli_addr, (socklen_t*)&len);
    if(clientfd < 0) {
        perror("client");
        exit(1);
    }
}

int main() {

    initServer();
    
    cout << "p value is : " << p << endl;
    cout << "g value is : " << g << endl;

    mpz_class privateKey = generatePrivateKey(50);
    cout << "Private key or secret key generated(randomly) is : " << privateKey << endl;

    mpz_class publicKey = generatePublicKey(privateKey);
    cout << "Public key(g^(secret) mod p) is : " << publicKey << endl;

    read(clientfd, &buff, sizeof(buff));
    mpz_class A(buff.s, 10);
    cout << "Recieved public key is : " << A << endl;

    memset(&buff, 0, sizeof(buff));
    strcpy(buff.s, publicKey.get_str().c_str());
    write(clientfd, &buff, sizeof(buff));

    cout << "\nShared secret : " << getSharedSecret(A, privateKey) << endl;

    close(sfd);
}
