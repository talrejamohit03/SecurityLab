#include<bits/stdc++.h>
using namespace std;

vector<vector<int> > divideBitVectors(vector<int> &numx, vector<int> &denx){

    int dnum = numx.size()-1, dden = denx.size() -1, dr=dnum;

    vector<int> divisor(dnum + 1,0),quotient(dnum-dden+1,0),remainder(dr+1,0);

	while(dnum >= dden) {

		quotient[dnum-dden] = numx[dnum]/denx[dden];

		for(int  i = 0 ; i <=dden ; i++)
			divisor[i+dnum-dden] = denx[i] * quotient[dnum-dden];

		for(int  i = 0 ; i <=dnum; i++ )
			numx[i] = (numx[i] ^ divisor[i]);

		dnum--;

	}
	 
	for( int i = 0 ; i <= dden ; i++ )
		remainder[i] = numx[i];

	vector<vector<int> > res;
    res.push_back(quotient);
    res.push_back(remainder);
    return res;
}
vector<int> mod(vector<int> &A, vector<int> &B){

    vector<int> rem = divideBitVectors(A,B)[1];

    while(rem.size()!=0 && rem[rem.size()-1]==0){
        rem.pop_back();
    }
    
    return rem;

}
vector<int> euclidGCD(vector<int> &A, vector<int> &B){
	
	if(B.size()==0)
		return A;
	
	vector<int> remainder = mod(A,B);
	return euclidGCD(B,remainder);    
    
}
int main()
{
    int d1,d2;
    cout<<"Enter the degrees of Polynomial 1 followed by Polynomial 2\n";
    cin>>d1>>d2;
    
    vector<int> b1(d1+1),b2(d2+1);

    cout<<"Enter the coefficients of Polynomial 1 (least degree to highest degree) \n";
    for(int i=0;i<=d1;i++)
        cin>>b1[i];
    
    cout<<"Enter the coefficients of Polynomial 2 (least degree to highest degree) \n";
    for(int i=0;i<=d2;i++)
        cin>>b2[i];
       
    vector<int> res = euclidGCD(b1,b2);

    cout<<"GCD is\n";
    for(int i = res.size()-1;i>=0;i--){
    	if(res[i])
			(i>0) ? (cout<<"x^"<<i<<" +"):(cout<<"1");
    }
    cout<<"\n";
}
