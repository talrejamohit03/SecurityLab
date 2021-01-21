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

char key[] = "tutorials";

char keyTable[5][5];

void fillKeyTable(){
	bool taken[26];
	
	for(int i=0;i<26;i++)
		taken[i]=0;

	int i=0,j=0;
	for(int c=0;c<strlen(key);c++){
		int index = key[c]-'a';
		if(!taken[index] && index!=('j'-'a')){
			taken[index] = 1;
			keyTable[i][j] = key[c];
			j = (j+1)%5;
			if(j==0)
				i = (i+1)%5;
		}
	}
	for(int c=0;c<26;c++){
		if(!taken[c] && c!=('j'-'a')){
			keyTable[i][j] = 'a'+c;
			j = (j+1)%5;
			if(j==0)
				i = (i+1)%5;
		}
	}
	cout<<"Key Table:\n";
	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++)
			cout<<keyTable[i][j]<<" ";
		cout<<"\n";
	}
}

void getPositionInKeyTable(char c,int &row,int &col){
	if(c=='j')
		c='i';
	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			if(keyTable[i][j]==c){
				row=i;
				col=j;
				return;
			}
		}
	}
}

string decryptPlayfairCipher(char buff[]){
	string out;
	
	int i=0,n=strlen(buff);

	while(i<n){
		char elem1=buff[i],elem2=buff[i+1];
		int r1,c1,r2,c2;
		getPositionInKeyTable(elem1,r1,c1);
		getPositionInKeyTable(elem2,r2,c2);
		char out1,out2;
		if(c1==c2){
			out1=keyTable[(r1-1+5)%5][c1];
			out2=keyTable[(r2-1+5)%5][c2];
		}else if(r1==r2){
			out1=keyTable[r1][(c1-1+5)%5];
			out2=keyTable[r2][(c2-1+5)%5];
		}else{
			out1=keyTable[r1][c2];
			out2=keyTable[r2][c1];
		}
		out.push_back(out1);
		out.push_back(out2);
		
		i+=2;
	}
	return out;
}
int main(){

	fillKeyTable();
	
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
			cout<<"Decrypted message = "<<decryptPlayfairCipher(buff)<<"\n";
		}
	}
	return 0;
}		
	
	
