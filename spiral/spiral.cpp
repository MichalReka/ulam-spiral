// spiral.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//
#include <stdio.h>
#include <math.h>
#include <thread>
#include <vector>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
//rdzen kodu z trojkata sierpinskiego
int N = 1024;
using namespace std;
FILE* fp;
const int iYmax = 900;
/* screen ( integer) coordinate */
const int iXmax = 900;
unsigned char ImageMatrix[iXmax][iYmax][3];
char filename[] = "spiral.ppm";
char comment[] = "# ";/* comment should start with # */
const int MaxColorComponentValue = 255;
const int maxDepth = 200;

void zeros() //tworzenie bialego obrazu
{
	for (int i = 0;i < iYmax;i++)
	{
		for (int j = 0;j < iXmax;j++)
		{
			for (int c = 0;c < 3;c++)
			{
				ImageMatrix[j][i][c] = 255;
			}
		}
	}
}
class SpiralLayer
{
	int number;
	int depth;
	int X;
	int Y;
	bool primenumber(int number)
	{
		bool output = false;
		for (int i = 2; i < number; i++)
		{
			if (number % i != 0)
				output = true;
			else
				return false;
		}
		return output;
	}
	void drawWhitePixel(int X, int Y, int number)
	{
		if (primenumber(number))
		{
			for (int i = 0;i < 3;i++)
			{
				ImageMatrix[X][Y][i] = 0;
			}
		}

	}
	void paintRight()
	{
		for (int i = Y + depth-1;i >= Y - depth;i--)
		{
			drawWhitePixel(X + depth, i, number);
			number++;

		}
	}
	void paintTop()
	{
		for (int i = X + depth - 1;i >= X - depth;i--)
		{

			drawWhitePixel(i, Y - depth, number);
			number++;

		}
	}
	void paintLeft()
	{
		for (int i = Y - depth + 1;i <= Y + depth;i++)
		{

			drawWhitePixel(X - depth, i, number);
			number++;
		}
	}
	void paintBottom()
	{
		for (int i = X - depth + 1;i <= X + depth;i++)
		{

			drawWhitePixel(i, Y + depth, number);
			number++;
		}
	}
	void drawLayer()
	{
		paintRight();
		paintTop();
		paintLeft();
		paintBottom();	
	}
	void nextLayer()
	{
		SpiralLayer newLayer(8 * depth + number, X, Y, depth + 1);
	}
public:
	SpiralLayer(int nNumber, int nX, int nY, int nDepth)
	{
		number = nNumber;
		X = nX;
		Y = nY;
		depth = nDepth;
		if (depth == maxDepth)
		{
			return;
		}
		thread t(&SpiralLayer::nextLayer,this);
		t.join();
		drawLayer();
	}
};

int main()
{
	/*create new file,give it a name and open it in binary mode  */
	fp = fopen(filename, "wb"); /* b -  binary mode */
	/*write ASCII header to the file*/
	fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, iXmax, iYmax, MaxColorComponentValue);
	/* compute and write image data bytes to the file*/
	zeros();
	SpiralLayer Spiral(2, 450, 450, 1);
	//firstCh.join();

	for (int i = 0;i < iYmax;i++)
	{
		for (int j = 0;j < iXmax;j++)
		{
			fwrite(ImageMatrix[j][i], 1, 3, fp);
		}
	}
	fclose(fp);
	return 0;
}

