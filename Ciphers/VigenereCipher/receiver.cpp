#include<iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace std;

char key[] = "point";


string decryptVigenereCipher(char buff[]){
	string out;
	
	int i=0,j=0,n=strlen(key);
	while(buff[i]!='\0'){
		
		char curr;
		if(isalpha(buff[i])){
			curr = ('a'+(buff[i]-'a'-(key[j]-'a'+1)+26)%26);
			j = (j+1)%n;		
		}else
			curr=buff[i];
		i++;
		out.push_back(curr);

	}
	return out;
}
int main(){
	mkfifo("fifo",0666);
	
	int rfd = open("fifo",O_RDONLY);
	
	while(1){
		struct pollfd pfd[1];
		pfd[0].fd = rfd;
		pfd[0].events = POLLIN;
		poll(pfd,1,-1);
		
		if(pfd[0].revents & POLLIN){
			char buff[100];
			read(rfd,buff,sizeof(buff));
			cout<<"Encrypted message = "<<buff<<"\n";
			cout<<"Decrypted message = "<<decryptVigenereCipher(buff)<<"\n";
		}
	}
	return 0;
}		
	
	
