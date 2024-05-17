#include <iostream>  
using namespace std;

const int n = 3;
int i = 0, j = 0, m = 0, k = 0;
double aa = 0, bb = 0;
double a[n][n + 1]
{
    {10,-1, 2, 16},
    {2, -7,  4, 14},
    {1, 1, -5, -14}
};
void main()
{
    double x[n];

    for (k = 0; k < n; k++)//test
    {
        aa = abs(a[k][k]);
        i = k;
        for (m = k + 1; m < n; m++)
            if (abs(a[m][k]) > aa)
            {
                i = m;
                aa = abs(a[m][k]);
            }
        if (i != k)
        {
            for (j = k; j < n + 1; j++)
            {
                aa = a[k][j];
                a[k][j] = a[i][j];
                a[i][j] = bb;
            }
        }
        aa = a[k][k];
        a[k][k] = 1;

        for (j = k + 1; j < n + 1; j++)
            a[k][j] = a[k][j] / aa;

        for (i = k + 1; i < n; i++)
        {
            bb = a[i][k];
            a[i][k] = 0;
            if (bb != 0)
                for (j = k + 1; j < n + 1; j++)
                    a[i][j] = a[i][j] - bb * a[k][j];
        }
    }

    for (i = n - 1; i >= 0; i--)
    {
        x[i] = 0;
        aa = a[i][n];
        for (j = n; j > i; j--)
            aa = aa - a[i][j] * x[j];
        x[i] = aa;
    }

    cout << "Metod main elements" << endl;
    for (i = 0; i < n; i++)
    {
        cout << "x" << i << "= " << x[i];   cout << endl;
    }
}