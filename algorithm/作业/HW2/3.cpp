#include <stdio.h>   
#define LENGTH 10

void Greedy(int n, int s[], int f[], bool a[]){
    a[1]=true; 
    int j=1;    
    for(int i=2;i<=n;i++){  
        if (s[i]>=f[j]){   
            a[i]=true;  
            j=i;  
        }  
        else  a[i]=false;  
    }  
}
int main(){
    int s[] = {0,2,2,3,4,6,7,9,10,13};         
    int f[] = {0,3,4,5,7,8,11,12,15,17};      
    bool a[LENGTH];
    int n = (sizeof(s) / sizeof(s[0])) - 1;
    Greedy(n, s, f, a);
    printf("Greedy result: ");
    for(int i = 1; i <= n; i++){  
    	if(a[i]) printf("%d  ",i);
    }  
    return 0;  
}
