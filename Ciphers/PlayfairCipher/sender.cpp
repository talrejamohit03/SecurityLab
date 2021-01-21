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
void encryptPlayfairCipher(char in[],char out[]){
	
	int n=strlen(in);
	if(n&1){
		in[n]='z';
		in[n+1]='\0';
		n++;
	}
	int i=0;
	while(i<n){
		char elem1=in[i],elem2=in[i+1];
		int r1,c1,r2,c2;
		getPositionInKeyTable(elem1,r1,c1);
		getPositionInKeyTable(elem2,r2,c2);
		if(c1==c2){
			out[i]=keyTable[(r1+1)%5][c1];
			out[i+1]=keyTable[(r2+1)%5][c2];
		}else if(r1==r2){
			out[i]=keyTable[r1][(c1+1)%5];
			out[i+1]=keyTable[r2][(c2+1)%5];
		}else{
			out[i]=keyTable[r1][c2];
			out[i+1]=keyTable[r2][c1];
		}
		
		i+=2;
	}
	out[i]='\0';
}

int main(){
	fillKeyTable();
	mkfifo("fifo",0666);
	
	int wfd = open("fifo",O_WRONLY);
	

	while(1){
		cout<<"Enter message = ";
		char msg[50];
		fgets(msg,100,stdin);
		msg[strlen(msg)-1]='\0';
		char toWrite[50];
		encryptPlayfairCipher(msg,toWrite);
		cout<<"Encrypted Message = "<<toWrite<<"\n";
		write(wfd,toWrite,sizeof(toWrite));
	}
	return 0;
}		
	
	
