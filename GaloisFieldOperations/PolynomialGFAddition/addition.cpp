#include<bits/stdc++.h>
using namespace std;

vector<int> addBitVectors(vector<int> &b1, vector<int> &b2){

    if(b1.size()>b2.size())
    	swap(b1,b2);
   
    vector<int> res(b2);
    for(int i=0;i<b1.size();i++){
        res[i]=(b1[i]^b2[i]);
    }
    return res;
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
    
    cout<<"Result of addition of polynomial 1 and 2 is:\n";
    vector<int> addn = addBitVectors(b1,b2);
    for(int i = addn.size()-1;i>=0;i--){
    	if(addn[i])
			(i>0) ? (cout<<"x^"<<i<<" +"):(cout<<"1");
    }
    cout<<"\n";
    return 0;
}
