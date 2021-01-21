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

using namespace std;

int key = 3;

void encryptCaesarCipher(char in[],char out[]){
	int i=0;
	while(in[i]!='\0'){

		out[i] = (isalpha(in[i]))? ('a'+(in[i]-'a'+key)%26) : in[i] ;
		
		i++;
	}
	out[i]='\0';
}

int main(){
	mkfifo("fifo",0666);
	
	int wfd = open("fifo",O_WRONLY);

	while(1){
		cout<<"Enter message = ";
		char msg[50];
		fgets(msg,100,stdin);
		msg[strlen(msg)-1]='\0';
		char toWrite[50];
		encryptCaesarCipher(msg,toWrite);
		cout<<"Encrypted Message = "<<toWrite<<"\n";
		write(wfd,toWrite,sizeof(toWrite));
	}
	return 0;
}		
	
	
