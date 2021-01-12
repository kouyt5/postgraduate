#include<stdio.h>
int binarySearch(int a[], int n, int x, int &min, int &max)
{
	int FLAG = -1;  
	int mid; 
	int right = n - 1;
	int left = 0;     
	 
	while (left <=right) {
		mid = (left + right) / 2;
		if (x == a[mid])   //若果相等:存在
		{                     
			min = max = mid;
			FLAG = mid;
			return FLAG;
		}
		else {
			if (x > a[mid])
			{
				
				left = mid + 1;
				max = left;
			}
			else {
				
				right = mid - 1;
				min = right;
			}
		}
	}
	return FLAG;
}

int main()
{
	int min, max;
	int x;
	int a[] = {1,2,3,4,5,6,8,10,15};
	x = 4;
	printf("测试用例1：");
		if (binarySearch(a, 9, x, min, max) >=0)
		{
			printf("%d存在数组a中，下标为：%d\n", x,binarySearch(a, 9, x, min, max) );
		}
		else {
			printf("%d不存在数组a中， 小于%d的最大元素的下标为：%d;大于%d的最小元素的下标为%d\n", x, x,min, x, max);
		}
		
		printf("测试用例2：");
		x = 9;
		if (binarySearch(a, 8, x, min, max) == 0)
		{
			printf("%d存在数组a中，下标为：%d\n", x, binarySearch(a, 9, x, min, max) );
		}
		else {
			printf("%d不存在数组a中， 小于%d的最大元素的下标为：%d;大于%d的最小元素的下标为%d\n", x, x, min, x, max);
		}


	return 0;
}