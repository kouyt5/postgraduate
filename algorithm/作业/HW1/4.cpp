#include <stdio.h>
#include <stdlib.h>
#define N 16
int a[100][100];
int t = 1;
void Tromino(int(*a)[N], int dr, int dc, int tr, int tc, int size)
{
	int s;
	if (size == 1) return;
	if (size>1)
	{
		s = size / 2;
		if (dr <= (tr + s - 1) && dc <= (tc + s - 1))       /*特殊方块在左上部分*/
		{
			a[tr + s - 1][tc + s] = t;
			a[tr + s][tc + s] = t;
			a[tr + s][tc + s - 1] = t;
			t++;
			Tromino(a, dr, dc, tr, tc, s);//左上角子棋盘递归处理
			Tromino(a, tr + s - 1, tc + s, tr, tc + s, s);//右上角子棋盘递归处理
			Tromino(a, tr + s, tc + s, tr + s, tc + s, s);//右下角子棋盘递归处理
			Tromino(a, tr + s, tc + s - 1, tr + s, tc, s);//左下角子棋盘递归处理
		}

		if (dr <= (tr + s - 1) && dc>(tc + s - 1))	/*特殊方块在右上部分*/
		{
			a[tr + s - 1][tc + s - 1] = t;
			a[tr + s][tc + s - 1] = t;
			a[tr + s][tc + s] = t;
			t++;
			Tromino(a, dr, dc, tr, tc + s, s); //右上角子棋盘递归处理
			Tromino(a, tr + s - 1, tc + s - 1, tr, tc, s); //左上角子棋盘递归处理
			Tromino(a, tr + s, tc + s - 1, tr + s, tc, s); //左下角子棋盘递归处理
			Tromino(a, tr + s, tc + s, tr + s, tc + s, s); //右下角子棋盘递归处理
		}

		if (dr>(tr + s - 1) && dc <= (tc + s - 1))	/*特殊方块在左下部分*/
		{
			a[tr + s - 1][tc + s - 1] = t;
			a[tr + s - 1][tc + s] = t;
			a[tr + s][tc + s] = t;
			t++;
			Tromino(a, dr, dc, tr + s, tc, s);
			Tromino(a, tr + s - 1, tc + s - 1, tr, tc, s);
			Tromino(a, tr + s - 1, tc + s, tr, tc + s, s);
			Tromino(a, tr + s, tc + s, tr + s, tc + s, s);
		}

		if (dr>(tr + s - 1) && dc>(tc + s - 1))	/*特殊方块在右下部分*/
		{
			a[tr + s][tc + s - 1] = t;
			a[tr + s - 1][tc + s - 1] = t;
			a[tr + s - 1][tc + s] = t;
			t++;
			Tromino(a, dr, dc, tr + s, tc + s, s);
			Tromino(a, tr + s, tc + s - 1, tr + s, tc, s);
			Tromino(a, tr + s - 1, tc + s - 1, tr, tc, s);
			Tromino(a, tr + s - 1, tc + s, tr, tc + s, s);
		}
	}
}



int main()
{
	int i, j, dr, dc, a[N][N];
	printf("特殊方格所在的行号dr(0<dr<%d):", N);
	scanf("%d", &dr);
	printf("特殊方格所在的列号dc(0<dc<%d):", N);
	scanf("%d", &dc);
	a[dr][dc] = 0;
	Tromino(a, dr, dc, 0, 0, N);
	for (i = 0; i<N; i++)
	{
		for (j = 0; j<N; j++)
			printf("%-4d", a[i][j]);
		printf("\n");
	}
	return 0;
}