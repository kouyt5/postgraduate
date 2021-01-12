#include<stdio.h>
#include <stdlib.h>

int MergeSort(int *array, int low, int high)
{
	int i, j, k;
	int mid;
	int *temp;

	if (array == NULL) {
		return -1;
	}

	if (low >= high) {
		return -1;
	}
	//辅助数组存
	temp = (int *)malloc(sizeof(int)*(high - low + 1));
	if (temp == NULL) {
		return -1;
	}

	mid = (low + high) / 2;

	//递归调用归并排序
	MergeSort(array, low, mid);
	MergeSort(array, mid + 1, high);

	i = low;
	j = mid + 1;
	k = 0;
	//i指向第一个数组当前的比较值，j指向第二个数组的比较值
	while (i <= mid && j <= high) {
		//比较i指向的值和j指向的值，将比较小的赋值给temp
		if (array[i] < array[j]) {
			temp[k++] = array[i++];
		}
		else {
			temp[k++] = array[j++];
		}
	}


	while (i <= mid) {
		temp[k++] = array[i++];
	}


	//这些数肯定都是大于第一个序列的所有的数的。将这些数全部依次赋值给temp
	while (j <= high) {
		temp[k++] = array[j++];
	}

	//将temp中所保存的整个有序的序列赋值到原先序列的对应位置中
	for (i = 0; i <= high - low; i++) {
		array[low + i] = temp[i];
	}

	return 0;
}

int main()
{
	int a[7] = { 49, 38, 65, 97, 76, 13, 27 };
	MergeSort(a, 0, 6);

	for (int i = 0;i < 7;i++)
	{
		printf("%d\t", a[i]);
	}
}
