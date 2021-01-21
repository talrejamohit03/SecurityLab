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
int main()
{

    int d1,d2,d3;
    cout<<"Enter the degrees of Polynomial 1, Polynomial 2 and Irreducible polynomial\n";
    cin>>d1>>d2>>d3;
    vector<int> b1(d1+1),b2(d2+1), bIRR(d3+1);
    
    cout<<"Enter the coefficients of Polynomial 1 (least degree to highest degree) \n";
    for(int i=0;i<=d1;i++)
        cin>>b1[i];
    
    cout<<"Enter the coefficients of Polynomial 2 (least degree to highest degree) \n";
    for(int i=0;i<=d2;i++)
        cin>>b2[i];
       
    cout<<"Enter the coefficients of Irreducible polynomial (least degree to highest degree) \n";
    for(int i=0;i<=d3;i++)
        cin>>bIRR[i];

    cout<<"Result is:\n";
    vector<int> res = multiplyBitVectors(b1,b2);

    res = divideBitVectors(res,bIRR)[1];
    for(int i = res.size()-1;i>=0;i--){
    	if(res[i])
			(i>0) ? (cout<<"x^"<<i<<" +"):(cout<<"1");
    }
    cout<<"\n";
    return 0;
}
