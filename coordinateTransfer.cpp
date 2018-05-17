/**********************************************************
*   FileName: coordinateTransfer.cpp
*   Author  : LJJ
*   Date    : 2018.05.016
*   Describe: ...
**********************************************************/

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <cmath>
#include <ctime>
#include <cstring>
#include <stdlib.h>

#define N 4

using namespace std;

class COORDINATE
{
private:
	double x_coord;
	double y_coord;
	double z_coord;

	double x_theta;
	double y_theta;
	double z_theta;
	
public:
	double tf_matrix[N*N];
		//void init();
	void setXYZ_coord(double x, double y, double z);
	void setXYZ_theta_coord(double x, double y, double z);
	double* getTFmatrix();
};

void COORDINATE::setXYZ_coord(double x, double y, double z)
{
	x_coord = x;
	y_coord = y;
	z_coord = z;
}

void COORDINATE::setXYZ_theta_coord(double x, double y, double z)
{
	x_theta = x;
	y_theta = y;
	z_theta = z;
}

double * COORDINATE::getTFmatrix()
{
	tf_matrix[0] = cos(z_theta)*cos(y_theta);
	tf_matrix[1] = -sin(z_theta)*cos(x_theta) + cos(z_theta)*sin(y_theta)*sin(x_theta);
	tf_matrix[2] = sin(z_theta)*sin(x_theta) + cos(z_theta)*sin(y_theta)*cos(x_theta);
	tf_matrix[3] = x_coord;

	tf_matrix[4] = sin(z_theta)*cos(y_theta);
	tf_matrix[5] = cos(z_theta)*cos(x_theta) + sin(z_theta)*sin(y_theta)*sin(x_theta);
	tf_matrix[6] = -cos(z_theta)*sin(x_theta) + cos(x_theta)*sin(y_theta)*sin(z_theta);
	tf_matrix[7] = y_coord;	

	tf_matrix[8] = -sin(y_theta);
	tf_matrix[9] = sin(x_theta)*cos(y_theta);
	tf_matrix[10] = cos(x_theta)*cos(y_theta);
	tf_matrix[11] = z_coord;

	tf_matrix[12] = 0;
	tf_matrix[13] = 0;
	tf_matrix[14] = 0;
	tf_matrix[15] = 1;	

	return tf_matrix;
}

double* getTFresults(double source[], double tf_matrix[N*N]) //source --->cameral  destination----->veh, cam--->veh
{
	double *temp = new double[N-1]();
	double source_[4] = {1}, destination_[4] = {0};
	for(int i = 0; i < 3; i++)
		source_[i] = source[i];
	source_[3] = 1;

	for(int i = 0; i < 4; i++)
	{
		double sum = 0;
		for(int j = 0; j < 4; j++)
		{
			sum += tf_matrix[i*N + j]*source_[j];
		}

		destination_[i] = sum;
	}

	for(int i = 0; i < 3; i++)
		temp[i] = destination_[i];
	return temp;
}

////////////////////////get the inverse of matrix////////////////////

double * mul(double A[N*N],double B[N*N])
{
	double *C=new double[N*N];
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			for(int k=0;k<N;k++)
			{
				C[i*N+j] += A[i*N+k]*B[k*N+j];
			}
		}
	}

	for(int i=0;i<N*N;i++)
	{
		if(abs(C[i])<pow(10,-10))
		{
			C[i]=0;
		}
	}

	return C;
}

void LUP_Descomposition(double A[N*N],double L[N*N],double U[N*N],int P[N])
{
	int row=0;
	for(int i=0;i<N;i++)
	{
		P[i]=i;
	}
	for(int i=0;i<N-1;i++)
	{
		double p=0.0d;
		for(int j=i;j<N;j++)
		{
			if(abs(A[j*N+i])>p)
			{
				p = abs(A[j*N+i]);
				row = j;
			}
		}
		if(0==p)
		{
			cout<< "矩阵奇异，无法计算逆" <<endl;
			return ;
		}

		int tmp = P[i];
		P[i] 	= P[row];
		P[row]	= tmp;

		double tmp2 = 0.0d;
		for(int j=0;j<N;j++)
		{
			tmp2		= A[i*N+j];
			A[i*N+j]	= A[row*N+j];
			A[row*N+j]	= tmp2;
		}

		double u = A[i*N+i],l = 0.0d;
		for(int j = i+1; j < N;j++)
		{
			l = A[j*N+i]/u;
			A[j*N+i] = l;
			for(int k = i+1; k < N; k++)
			{
				A[j*N+k] = A[j*N+k]-A[i*N+k]*l;
			}
		}

	}

	for(int i=0;i<N;i++)
	{
		for(int j=0;j<=i;j++)
		{
			if(i!=j)
			{
				L[i*N+j]=A[i*N+j];
			}
			else
			{
				L[i*N+j]=1;
			}
		}
		for(int k=i;k<N;k++)
		{
			U[i*N+k]=A[i*N+k];
		}
	}

}

double * LUP_Solve(double L[N*N],double U[N*N],int P[N],double b[N])
{
	double *x=new double[N]();
	double *y=new double[N]();

	for(int i = 0;i < N;i++)
	{
		y[i] = b[P[i]];
		for(int j = 0;j < i;j++)
		{
			y[i] = y[i] - L[i*N+j]*y[j];
		}
	}

	for(int i = N-1;i >= 0; i--)
	{
		x[i]=y[i];
		for(int j = N-1;j > i;j--)
		{
			x[i] = x[i] - U[i*N+j]*x[j];
		}
		x[i] /= U[i*N+i];
	}
	return x;
}


int getNext(int i, int m, int n)
{
	return (i%n)*m + i/n;
}

int getPre(int i, int m, int n)
{
	return (i%m)*n + i/m;
}

void movedata(double *mtx, int i, int m, int n)
{
	double temp = mtx[i]; 
	int cur = i;    
  	int pre = getPre(cur, m, n);
  	while(pre != i)
  	{
		mtx[cur] = mtx[pre];
		cur = pre;
		pre = getPre(cur, m, n);
  	}
  	mtx[cur] = temp;
}

void transpose(double *mtx, int m, int n)
{
  	for(int i=0; i<m*n; ++i)
  	{
		int next = getNext(i, m, n);
		while(next > i) 
	  	next = getNext(next, m, n);
		if(next == i)  
	  	movedata(mtx, i, m, n);
  	}
}

double * LUP_solve_inverse(double A[N*N])
{
	double *A_mirror = new double[N*N]();
	double *inv_A=new double[N*N]();
	double *inv_A_each=new double[N]();

	double *b    =new double[N]();

	for(int i=0;i<N;i++)
	{
		double *L=new double[N*N]();
		double *U=new double[N*N]();
		int *P=new int[N]();

		for(int j=0;j<N;j++)
		{
			b[j]=0;
		}
		b[i]=1;

		for(int m=0;m<N*N;m++)
		{
			A_mirror[m]=A[m];
		}

		LUP_Descomposition(A_mirror,L,U,P);

		inv_A_each=LUP_Solve (L,U,P,b);
		memcpy(inv_A+i*N,inv_A_each,N*sizeof(double));
	}
	transpose(inv_A,N,N);

	return inv_A;
}

int main(int argc, char** argv)
{
	COORDINATE cam2veh;
	cam2veh.setXYZ_coord(-0.47,-0.85,0.64);	
	cam2veh.setXYZ_theta_coord(0,0,-M_PI/180*93.3);

	double *cam2veh_tf_matrix = new double[N*N]();
	double inv_test[N*N]={0};
	double  *inv_cam2veh_tf_matrix = new double[N*N]();
	double  *mul_result = new double[N*N]();

	cam2veh_tf_matrix= cam2veh.getTFmatrix();

	inv_cam2veh_tf_matrix = LUP_solve_inverse(cam2veh_tf_matrix);
	mul_result = mul(cam2veh_tf_matrix, inv_cam2veh_tf_matrix);

	printf("**********************TF matrix**************************\n");
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			printf("%f\t",cam2veh_tf_matrix[i*N+j]);
		}
		printf("\n");
	}

	printf("********************Inv TF matrix*************************\n");
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			printf("%f\t",inv_cam2veh_tf_matrix[i*N+j]);
		}
		printf("\n");
	}
	printf("********************Mul resutls*************************\n");
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			cout<< mul_result[i*N+j]<< " " ;
		}
		cout<<endl;
	}

	double point_in_cam[] = {0, 1, 1};
	double *cam_point_in_veh = new double[N-1]();
	cam_point_in_veh = getTFresults(point_in_cam,cam2veh_tf_matrix);

	printf("**********************TF results**************************\n");
	for(int i = 0; i < 3; i++)
		printf("%f\n",cam_point_in_veh[i]);

	return 0;
}
