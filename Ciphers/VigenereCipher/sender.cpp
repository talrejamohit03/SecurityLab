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

char key[] = "point";

void encryptCaesarCipher(char in[],char out[]){
	
	int i=0,j=0,n=strlen(key);
	
	while(in[i]!='\0'){
		
		if(isalpha(in[i])){
			out[i] = ('a'+(in[i]-'a'+(key[j]-'a'+1))%26);
			j = (j+1)%n;
		}else
			out[i] = in[i];
			
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
	
	
