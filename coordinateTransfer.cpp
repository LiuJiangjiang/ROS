#include <iostream>
#include <math.h>
#include <stdio.h>

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
		double tf_matrix[4][4];
		//void init();
		void setXYZ_coord(double x, double y, double z);
		void setXYZ_theta_coord(double x, double y, double z);
		void getTFmatrix();
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

void COORDINATE::getTFmatrix()
{
	tf_matrix[0][0] = cos(z_theta)*cos(y_theta);
	tf_matrix[0][1] = -sin(z_theta)*cos(x_theta) + cos(z_theta)*sin(y_theta)*sin(x_theta);
	tf_matrix[0][2] = sin(z_theta)*sin(x_theta) + cos(z_theta)*sin(y_theta)*cos(x_theta);
	tf_matrix[0][3] = x_coord;

	tf_matrix[1][0] = sin(z_theta)*cos(y_theta);
	tf_matrix[1][1] = cos(z_theta)*cos(x_theta) + sin(z_theta)*sin(y_theta)*sin(x_theta);
	tf_matrix[1][2] = -cos(z_theta)*sin(x_theta) + cos(x_theta)*sin(y_theta)*sin(z_theta);
	tf_matrix[1][3] = y_coord;	

	tf_matrix[2][0] = -sin(y_theta);
	tf_matrix[2][1] = sin(x_theta)*cos(y_theta);
	tf_matrix[2][2] = cos(x_theta)*cos(y_theta);
	tf_matrix[2][3] = z_coord;

	tf_matrix[3][0] = 0;
	tf_matrix[3][1] = 0;
	tf_matrix[3][2] = 0;
	tf_matrix[3][3] = 1;	
}

void getTFresults(double source[], double destination[], double tf_matrix[][4]) //source --->cameral  destination----->veh, cam--->veh
{
	double source_[4] = {1}, destination_[4] = {0};
	for(int i = 0; i < 3; i++)
		source_[i] = source[i];
	source_[3] = 1;

	for(int i = 0; i < 4; i++)
	{
		double sum = 0;
		for(int j = 0; j < 4; j++)
		{
			sum += tf_matrix[i][j]*source_[j];
		}

		destination_[i] = sum;
	}

	for(int i = 0; i < 3; i++)
		destination[i] = destination_[i];
}

int main(int argc, char** argv)
{
	COORDINATE cam2veh;
	cam2veh.setXYZ_coord(-0.47,-0.85,0.64);	
	cam2veh.setXYZ_theta_coord(0,0,-M_PI/180*93.3);
	
	cam2veh.setXYZ_coord(0,0.0,0);	
	cam2veh.setXYZ_theta_coord(M_PI/4,0,0);

	cam2veh.getTFmatrix();

	printf("**********************TF matrix**************************\n");
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
			printf("%f\t",cam2veh.tf_matrix[i][j]);
		printf("\n");
	}

	double source[] = {0, 1, 1}, destination[3]={0};
	getTFresults(source,destination,cam2veh.tf_matrix);

	printf("**********************TF results**************************\n");
	for(int i = 0; i < 3; i++)
		printf("%f\n",destination[i]);
}