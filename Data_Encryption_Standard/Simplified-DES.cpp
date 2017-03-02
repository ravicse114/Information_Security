/*=======================================================|
|    AUTHOR :- RAVI SHANKAR KUMAR 			 |
|    FROM :- SITAMARHI(BIHAR),INDIA      		 |
|    Email :- ravisk.cs.14@gmail.com                     |
|    NIT JALANDHAR, CSE PRE-FINAL YEAR                   |
|=======================================================*/
#include<iostream>
#include<cstring>
using namespace std;
//KEY PERMUTATION BLOCKS
const int p10[10]={3,5,2,7,4,10,1,9,8,6}; 
const int p8[8]={6,3,7,4,8,5,10,9};
//PLAIN TEXT PERMUTATION BLOCKS
const int IP[8]={2,6,3,1,4,8,5,7}; //Initial Permutation block
const int p4[4]={2,4,3,1};      
const int IIP[8]={4,1,3,5,7,2,8,6}; //Inverse of Initial permutation block
const int EP[8]={4,1,2,3,2,3,4,1}; //Expansion block
//S-Box
const int sbx[2][4][4]={//First S-Box
			1,0,3,2,
		        3,2,1,0,
		        0,2,1,3,
		        3,1,3,2,
		        //Second S-Box
		        0,1,2,3,
		        2,0,1,3,
		        3,0,1,0,
		        2,1,0,3
		       };
void key_generation(char *input,char **output)
{
   char *temp=new char[11];
   //Apply P10 on input
   for(int i=0;i<10;i++) temp[i]=input[p10[i]-1];
   temp[10]='\0';
   //Shift left temp array by 1-bit in left and right part
   //of 5-bit each
   char msb1=temp[0],msb2=temp[5];
   for(int i=0;i<4;i++) temp[i]=temp[i+1],temp[i+5]=temp[i+6];
   temp[9]=msb2,temp[4]=msb1;
   //Apply P8 to get first key
   for(int i=0;i<8;i++) output[0][i]=temp[p8[i]-1];
   //Shift left temp array by 2-bit in left and right part
   //of 5-bit each
   msb2=temp[1];msb1=temp[0];
   for(int i=0;i<4;i++) temp[i]=temp[i+2];
   temp[3]=msb1; temp[4]=msb2;
   msb2=temp[6];msb1=temp[5];
   for(int i=0;i<4;i++) temp[i+5]=temp[i+7];
   temp[8]=msb1; temp[9]=msb2;
   //Apply P8 to get second key
   for(int i=0;i<8;i++) output[1][i]=temp[p8[i]-1];
   return;
}
void xors(char *inp1,char *inp2)
{
  int len=strlen(inp1);
  for(int i=0;i<len;i++)
  {
     if(inp1[i]==inp2[i]) inp1[i]='0';
     else inp1[i]='1';
  }
  return;
}
char *sboxes(char *inp)
{
  char *out=new char[5];
  int row,col,value;
  for(int i=0;i<2;i++)
  {
  	row=(inp[0+i*4]-'0')*2+(inp[3+i*4]-'0');
  	col=(inp[1+i*4]-'0')*2+(inp[2+i*4]-'0');
        value=sbx[i][row][col];
  	if(value==0) out[0+i*2]=out[1+i*2]='0';
  	else if(value==1) out[0+i*2]=0,out[1+i*2]='1';
  	else if(value==2) out[0+i*2]='1',out[1+i*2]='0';
  	else out[0+i*2]=out[1+i*2]='1';
  }
  return out;
}
void Encryption(char *pt,char **keys)
{
   //Apply Initial Permutation IP
   char *temp=new char[9];
   for(int i=0;i<8;i++) temp[i]=pt[IP[i]-1];
   //==========================ROUND-FIRST=====================
   //Divide into two parts
   char *left=new char[5];
   char *right=new char[5];
   for(int i=0;i<4;i++)
   	left[i]=temp[i],right[i]=temp[i+4];
   //Apply expansion on right part
   char *expands=new char[9];
   for(int i=0;i<8;i++) expands[i]=right[EP[i]-1];
   //XOR with first key;
   xors(expands,keys[0]);
   //USE S-Boxes
   right=sboxes(expands);
   //Apply P4
   for(int i=0;i<4;i++) expands[i]=right[p4[i]-1];
   //Xor it with left part
   xors(left,expands);
   //Swap between right part and output of above
   for(int i=0;i<4;i++)
     temp[i]=temp[i+4],temp[i+4]=left[i];
   //========================ROUND-SECOND======================
   for(int i=0;i<4;i++)
   	left[i]=temp[i],right[i]=temp[i+4];
   //Apply expansion on right part
   for(int i=0;i<8;i++) expands[i]=right[EP[i]-1];
   //XOR with second key;
   xors(expands,keys[1]);
   //USE S-Boxes
   right=sboxes(expands);
   //Apply P4
   for(int i=0;i<4;i++) expands[i]=right[p4[i]-1];
   //Xor it with left part
   xors(left,expands);
   for(int i=0;i<4;i++)
       temp[i]=left[i];
   //Apply IP Inverse on output
   for(int i=0;i<8;i++) pt[i]=temp[IIP[i]-1];
   return;
}
void Decryption(char *ct,char **keys)
{
  //Apply Initial Permutation IP
   char *temp=new char[9];
   for(int i=0;i<8;i++) temp[i]=ct[IP[i]-1];
   //==========================ROUND-FIRST=====================
   //Divide into two parts
   char *left=new char[5];
   char *right=new char[5];
   for(int i=0;i<4;i++)
   	left[i]=temp[i],right[i]=temp[i+4];
   //Apply expansion on right part
   char *expands=new char[9];
   for(int i=0;i<8;i++) expands[i]=right[EP[i]-1];
   //XOR with second key;
   xors(expands,keys[1]);
   //USE S-Boxes
   right=sboxes(expands);
   //Apply P4
   for(int i=0;i<4;i++) expands[i]=right[p4[i]-1];
   //Xor it with left part
   xors(left,expands);
   //Swap between right part and output of above
   for(int i=0;i<4;i++)
     temp[i]=temp[i+4],temp[i+4]=left[i];
   //========================ROUND-SECOND======================
   for(int i=0;i<4;i++)
   	left[i]=temp[i],right[i]=temp[i+4];
   //Apply expansion on right part
   for(int i=0;i<8;i++) expands[i]=right[EP[i]-1];
   //XOR with first key;
   xors(expands,keys[0]);
   //USE S-Boxes
   right=sboxes(expands);
   //Apply P4
   for(int i=0;i<4;i++) expands[i]=right[p4[i]-1];
   //Xor it with left part
   xors(left,expands);
   for(int i=0;i<4;i++)
      temp[i]=left[i];
   //Apply IP inverse on output
   for(int i=0;i<8;i++) ct[i]=temp[IIP[i]-1];
   return;
}
int main()
{
   //Take Plain text input
   cout<<"Enter Plain Text (8-bit binary) : ";
   char *pt=new char[9];
   cin>>pt;
   //Take Key input
   cout<<"Enter Key (10-bit binary) : ";
   char *key=new char[11];
   cin>>key;
   //Declare storage for key to be generated
   char **keys=new char*[2];
   keys[0]=new char[9];
   keys[1]=new char[9];
   //Call key generation method
   key_generation(key,keys);
   cout<<"First round keys : "<<keys[0]<<endl;
   cout<<"Second round keys : "<<keys[1]<<endl<<endl;
   //Call Encryption method
   Encryption(pt,keys);
   cout<<"Encrypted text is : "<<pt<<endl;
   //Call Decryption method
   Decryption(pt,keys);
   cout<<"Decrypted text is : "<<pt<<endl;
   return 0;
}
