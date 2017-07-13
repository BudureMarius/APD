#include <iostream>
#include <cstdio>
#include <time.h>
#include <iostream>
#include <fstream>

using namespace std;

#define MAX 501
#define INF 1000000

int d[MAX][MAX];
int n;

int main()
{
	int i, j, k;

	ifstream input("royfloyd.in");
	ofstream output("royfloyd.out");

	input >> n;

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++) 
		{
			input >> d[i][j];
			if (d[i][j] == 0) 
				d[i][j] = INF;
		}
	
	clock_t t;
	t = clock();

	for (k = 0; k < n; k++)
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
				if (d[i][j]>d[i][k] + d[k][j])
					d[i][j] = d[i][k] + d[k][j];
	
	t = clock() - t;
	cout << "Time : " << ((double)t) / CLOCKS_PER_SEC << endl;

	for (i = 0; i < n; i++) 
	{
		for (j = 0; j < n; j++)
			if (i == j || d[i][j] == INF)
				output << "0 ";
			else
				output << d[i][j] << " ";
		output << endl;
	}

	return 0;
}