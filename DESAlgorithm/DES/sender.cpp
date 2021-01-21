#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include<bits/stdc++.h>

using namespace std;

int IP[64]={58,50,42,34,26,18,10,2,
			60,52,44,36,28,20,12,4,
			62,54,46,38,30,22,14,6,
			64,56,48,40,32,24,16,8,
			57,49,41,33,25,19,9,1,
			59,51,43,35,27,19,11,3,
			61,53,45,37,29,21,13,5,
			63,55,47,39,31,23,15,7};

int IPinv[64];

map<char,vector<int> > charBinMap;

string key="aAbBcCdD";

void printVec(vector<int> &in){
	for(int i=0;i<in.size();i++)
		cout<<in[i];
	cout<<"\n";
}

void findInverse(){
	for(int i=0;i<64;i++)
		IPinv[IP[i]-1] = i+1;
}
int E[48]={32,1,2,3,4,5,
			4,5,6,7,8,9,
			8,9,10,11,12,13,
			12,13,14,15,16,17,
			16,17,18,19,20,21,
			20,21,22,23,24,25,
			24,25,26,27,28,29,
			28,29,30,31,32,1};

int SBoxTable[4][16]={{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
					  {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
					  {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
					  {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
					 };

int binToDecimal(vector<int> &bin){
	int out=0;
	for(int i=0;i<bin.size();i++)
		out=out*2+bin[i];
	return out;
}
vector<int> decimalToBin(int dec,int sizeOut){
	vector<int> ans(sizeOut,0);
	int i=sizeOut-1;
	while(dec>0){
		ans[i]=(dec%2);
		dec/=2;
		i--;
	}
	return ans;
}
void charToBinaryMap(){
	char c='a';
	for(int i=0;i<26;i++){
		charBinMap[c] = decimalToBin(i,8);
		c++;
	}
	c='A';
	for(int i=26;i<52;i++){
		charBinMap[c] = decimalToBin(i,8);
		c++;
	}
}
vector<int> convertStringToBin(string s){
	vector<int> out;
	for(int i=0;i<s.size();i++){
		vector<int> val=charBinMap[s[i]];
		for(int j=0;j<(val.size());j++)
			out.push_back(val[j]);
	}
	return out;
}
void bitwisePermute(vector<int> &plainText,int permuteTable[]){
	vector<int> ptCopy(plainText);
	for(int i=0;i<plainText.size();i++)
		plainText[i]=ptCopy[permuteTable[i]-1];
}

vector<int> expansionFunction(vector<int> &input){
	vector<int> output(48);
	for(int i=0;i<48;i++)
		output[i]=input[E[i]-1];
	return output;
	
}
void XOR(vector<int> &b1,vector<int> &b2){
	for(int i=0;i<b1.size();i++)
		b1[i]=b1[i]^b2[i];

}
vector<int> SBox(vector<int> &in){

	vector<int> row,col(in.begin()+1,in.end()-1);
	row.push_back(in[0]);
	row.push_back(in[in.size()-1]);
	int rowNum=binToDecimal(row),colNum=binToDecimal(col);

	int subValue=SBoxTable[rowNum][colNum];
	return decimalToBin(subValue,4);
}
	
vector<int> fFunction(vector<int> &in,vector<int> &roundKey){
	vector<int> Eout = expansionFunction(in);
	/*
	cout<<"Expansion fun output: \n";
	printVec(Eout);
	*/
	XOR(Eout,roundKey);
	/*
	cout<<"After XOR with subkey: \n";
	printVec(Eout);
	*/
	vector<int> outSbox;
	for(int i=0;i<Eout.size();i+=6){
		vector<int> block(Eout.begin()+i,Eout.begin()+i+6);
		/*
		cout<<"SBOX block starting at "<<i<<"\n";
		printVec(block);
		*/
		vector<int> outBlock = SBox(block);
		/*
		cout<<"SBOX block output starting at "<<i<<"\n";
		printVec(outBlock);
		*/
		for(int j=0;j<outBlock.size();j++)
			outSbox.push_back(outBlock[j]);
	}
	int P[32]={16,7,20,21,29,12,28,17,
			   1,15,23,26,5,18,31,10,
			   2,8,24,14,32,27,3,9,
			   19,13,30,6,22,11,4,25};
	
	bitwisePermute(outSbox,P);
	return outSbox;
}

void circularLeftShift(vector<int> &in){
	int lsb=in[0];
	in.erase(in.begin());
	in.push_back(lsb);
}
void keyTransform(vector<int> &roundKey,int roundNum){

	//splitting into left and right half
	int mid = roundKey.size()/2;
	vector<int> left(roundKey.begin(),roundKey.begin()+mid),right(roundKey.begin()+mid,roundKey.end());
	
	circularLeftShift(left);
	circularLeftShift(right);
	
	if(!(roundNum==1 || roundNum==2 || roundNum==9 || roundNum==16)){
		circularLeftShift(left);
		circularLeftShift(right);
	}
	//preparing input for key schedule next round
	for(int i=0;i<roundKey.size();i++)
		roundKey[i] = (i<mid)?left[i]:right[i-mid];
}
void encryptionRound(vector<int> &in,vector<int> &roundKey){
	
	//splitting into left and right parts
	int mid = in.size()/2;
	vector<int> left(in.begin(),in.begin()+mid),right(in.begin()+mid,in.end());
	
	//Applying f function to right half
	vector<int> intermediate = fFunction(right,roundKey);
	/*
	cout<<"after f fun right half is \n";
	printVec(intermediate);
	*/
	//XOR of result with left half
	XOR(intermediate,left);
	
	//output is right half followed by the result above
	
	int j=0;
	for(int i=0;i<right.size();i++)
		in[j++] = right[i];
	for(int i=0;i<intermediate.size();i++)
		in[j++] = intermediate[i];

}
void DES(vector<int> &plainText,vector<int> &key){
	
	//remove parity bits of key and perform permutation choice
	int PC1[56] = {57,49,41,33,25,17,9,1,
				   58,50,42,34,26,18,10,2,
				   59,51,43,35,27,19,11,3,
				   60,52,44,36,63,55,47,39,
				   31,23,15,7,62,54,46,38,
				   30,22,14,6,61,53,45,37,
				   29,21,13,5,28,20,12,4};
	
	int PC2[48]={14,17,11,24,1,5,3,28,
				 15,6,21,10,23,19,12,4,
				 26,8,16,7,27,20,13,2,
				 41,52,31,37,47,55,30,40,
				 51,45,33,48,44,49,39,56,
				 34,53,46,42,50,36,29,32};
	
	//PC-1 on key
	vector<int> roundKey(56);
	
	for(int i=0;i<56;i++)
		roundKey[i]=key[PC1[i]-1];
	
	
	//Initial Permutation
	bitwisePermute(plainText,IP);
	/*
	cout<<"After init permutation: ";
	printVec(plainText);
	*/
	for(int i=0;i<16;i++){
		
		keyTransform(roundKey,i+1);
		
		vector<int> permutedSubKey(48);
		for(int j=0;j<48;j++)
			permutedSubKey[j]=roundKey[PC2[j]-1];
		
		encryptionRound(plainText,permutedSubKey);
		/*
		cout<<"After round "<<(i+1)<<" plaintext is \n";
		printVec(plainText);
		*/
		
	}
	int mid=plainText.size()/2;
	for(int i=0;i<mid;i++)
		swap(plainText[i],plainText[i+mid]);
	bitwisePermute(plainText,IPinv);
}

int main(){

	findInverse();
	charToBinaryMap();
	vector<int> keyBin=convertStringToBin(key);
	mkfifo("fifo",0666);
	
	int wfd = open("fifo",O_WRONLY);

	while(1){
		cout<<"Enter message (exactly 8 characters) = ";
		string in;
		cin>>in;
		vector<int> msgBin = convertStringToBin(in);
		cout<<"Input message as binary:\n";
		printVec(msgBin);
		DES(msgBin,keyBin);
		

		char buff[100];
		int j=0;
		for(int i=0;i<msgBin.size();i++)
			buff[j++] = (msgBin[i]==0)?'0':'1';
		buff[j]='\0';
		cout<<"\nFinal Encrypted Message = "<<buff<<"\n";
		write(wfd,buff,sizeof(buff));
	}
	return 0;
}		
	
	
