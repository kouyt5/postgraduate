#include<stdio.h>
#include<stdlib.h>
#include<time.h>
//第一个元素为枢纽 
int Partion(int a[],int head,int end){
	a[0]=a[head];
	int i=head;
	int j=end;
	while(true){
		while(i<j&&a[j]>=a[0]){
			j--;
		};
		a[i]=a[j];
		while(i<j&&a[i]<=a[0]){
			i++;
		};
		a[j]=a[i];
		if(i>=j){
			a[j]=a[0];
			return j;
		}
	}
	
} 
void QuickSort(int a[],int left,int right){
	if(left>=right){
		return;
	}
	int pivot=Partion(a,left,right);
	QuickSort(a,left,pivot-1);
	QuickSort(a,pivot+1,right);
}

//三者去中法快排
void swap(int *a, int *b)
{
   int temp = *a;
   *a = *b;
   *b = temp;
}

//获取数组中的首尾元素和枢纽元素
 void  getPivot(int arr[], int left, int right)
{
	int mid = (left + right) / 2;
	if (arr[left] > arr[mid])
	{
		swap(&arr[left], &arr[mid]);
	}
		
	if (arr[left] > arr[right])
	{
		swap(&arr[left], &arr[right]);
	}
	if (arr[mid] > arr[right])
	{
		swap(&arr[mid], &arr[right]);
	}
	swap(&arr[right - 1], &arr[mid]);
	
}

//三者取中
void QuickSort_M(int arr[], int left, int right)
{
	if (left < right)
	{
		//获取枢纽值
		getPivot(arr, left, right);
		int pivot = right - 1;
		int i = left;
		int j = right - 1;
		//按照枢纽划分 
		while(true)
		{
			while (arr[++i] < arr[pivot]);
			while (i<j && arr[--j] > arr[pivot]);
			if (i < j)
			{
				swap(&arr[i], &arr[j]);
			}
			else {
				break;
			}
	    }

		if (i <= j)
		{
			swap(&arr[i], &arr[pivot]);
		}
		QuickSort_M(arr, left, i - 1);
		QuickSort_M(arr, i + 1, right);
	}
}
 

//测试主函数 
int main(int argc,char **argv){
	printf("队首元为枢纽\n");
	int a[]={4, 2, 28, 1, 26, 14, 27, 16, 13, 22, 21, 15, 18, 25, 0, 20, 10, 11, 8, 19, 24, 7, 3, 29, 5, 12, 23, 9, 6, 17};
	double start_time = clock();
	QuickSort_M(a,1,29);
	double end_time = clock();
	printf("用时为 %f\t",(double)(end_time-start_time));
	printf("\n--------------\n");
	printf("三者取中\n");
	int b[]={4, 2, 28, 1, 26, 14, 27, 16, 13, 22, 21, 15, 18, 25, 0, 20, 10, 11, 8, 19, 24, 7, 3, 29, 5, 12, 23, 9, 6, 17};
	start_time = clock();
	QuickSort_M(b,1,29);
	end_time = clock();
	printf("用时为 %f\t",(double)(end_time-start_time));
}