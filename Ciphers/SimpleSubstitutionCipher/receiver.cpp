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

char key[] = "kdgfnslvbwahexjmqcpzrtyiuo";

char getLetter(char c){
	for(int i=0;i<26;i++){
		if(c==key[i]){
			return 'a'+i;
		}
	}
}
string decryptSimpleSubCipher(char buff[]){
	string out;
	int i=0;
	while(buff[i]!='\0'){
		out.push_back((isalpha(buff[i]))?getLetter(buff[i]):buff[i]);
		i++;
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
			cout<<"Decrypted message = "<<decryptSimpleSubCipher(buff)<<"\n";
		}
	}
	return 0;
}		
	
	
