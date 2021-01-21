#include<bits/stdc++.h>
using namespace std;

vector<int> multiplyBitVectors(vector<int> &b1, vector<int> &b2){

    
    vector<int> res(b1.size()+b2.size()-1,0);
    for(int i=0;i<b1.size();i++){
        for(int j=0;j<b2.size();j++){
            res[i+j]=(res[i+j]+(b1[i]*b2[j]))%2;
        }
    }
    return res;
}

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

vector<int> extendedEuclidGCD(vector<int> &A, vector<int> &B,vector<int> &t0,vector<int> &t1){
	
	if(B.size()==0)
		return t0;
	vector<int> tmp1(B),tmp2(t1);
	vector<vector<int> > res = divideBitVectors(A,B);
	
	while(res[1].size()!=0 && res[1][res[1].size()-1]==0){
        res[1].pop_back();
    }
    res[0] = multiplyBitVectors(res[0],t1);
    
    while(t0.size()<res[0].size())
    	t0.push_back(0);
    
    vector<int> newT1(t0);
    for(int  i = 0 ; i <t0.size(); i++ )
		newT1[i] = (t0[i] ^ res[0][i]);

	return extendedEuclidGCD(tmp1,res[1],tmp2,newT1);
    
}
int main()
{
    int d1,d2;
    cout<<"Enter the degrees of Polynomial followed by Irreducible Polynomial\n";
    cin>>d1>>d2;
    
    vector<int> b1(d1+1),b2(d2+1);

    cout<<"Enter the coefficients of Polynomial (least degree to highest degree) \n";
    for(int i=0;i<=d1;i++)
        cin>>b1[i];
    
    cout<<"Enter the coefficients of Irreducible Polynomial(least degree to highest degree) \n";
    for(int i=0;i<=d2;i++)
        cin>>b2[i];
       
    vector<int> t0,t1;
    t0.push_back(0);
    t1.push_back(1);
    vector<int> res = extendedEuclidGCD(b2,b1,t0,t1);

    cout<<"Multiplicative Inverse is\n";
    for(int i = res.size()-1;i>=0;i--){
    	if(res[i])
			(i>0) ? (cout<<"x^"<<i<<" +"):(cout<<"1");
    }
    cout<<"\n";
}
