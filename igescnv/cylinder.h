#ifndef _CYLINDER_H
#define _CYLINDER_H

#include <math.h>
#include <vector>
#include <stdio.h>

#define SQR(a) ((a)*(a))
#define VEC3D_CROSS(d,v,w)	(d)[0]=(v)[1]*(w)[2] - (v)[2]*(w)[1]; \
							(d)[1]=(v)[2]*(w)[0] - (v)[0]*(w)[2]; \
							(d)[2]=(v)[0]*(w)[1] - (v)[1]*(w)[0]
#define VEC3D_NORMALIZE(v,len)	{ len=sqrt((v)[0]*(v)[0]+(v)[1]*(v)[1]+(v)[2]*(v)[2]); \
							if(fabs(len)>1e-8) { double __m=1.0/len; (v)[0]*=__m; (v)[1]*=__m; (v)[2]*=__m; } }
class Cylinder
{
	double xx[3];
	double yy[3];
	double zz[3];
	double length;
	int color[3];
public:
	double radius;
	double p1[3];
	double p2[3];
	int id[2];
	double UV[2][2];
	Cylinder( double q[3], double p[3], double r)
	{
		color[0] = 0;
		color[1] = 20;
		color[2] = 0;

		p1[0] = q[0];
		p1[1] = q[1];
		p1[2] = q[2];
		p2[0] = p[0];
		p2[1] = p[1];
		p2[2] = p[2];
		radius = r;

		zz[0] = p[0] - q[0];
		zz[1] = p[1] - q[1];
		zz[2] = p[2] - q[2];

		double ln;
		VEC3D_NORMALIZE(zz,ln);
		length = ln;

		xx[0] = -zz[0];
		xx[1] = 0.0;
		xx[2] = 0.0;

		if ( fabs( xx[0] ) < 1.0e-8 )
		{
			xx[0] = -zz[1];
			xx[1] = 1.0;
			xx[2] = 1.0;
		}
		if ( fabs( xx[0] ) < 1.0e-8 )
		{
			xx[0] = -zz[2];
			xx[1] = 1.0;
			xx[2] = 1.0;
		}
		if ( fabs( xx[0] ) < 1.0e-8 )
		{
			xx[0] = 1.0;
			xx[1] = -zz[0];
			xx[2] = 1.0;
		}
		if ( fabs( xx[1] ) < 1.0e-8 )
		{
			xx[0] = 1.0;
			xx[1] = -zz[1];
			xx[2] = 1.0;
		}
		if ( fabs( xx[1] ) < 1.0e-8 )
		{
			xx[0] = 0.0;
			xx[1] = -zz[2];
			xx[2] = 0.0;
		}

		VEC3D_NORMALIZE(xx,ln);
		VEC3D_CROSS(yy, zz, xx);

		VEC3D_NORMALIZE(yy,ln);

		VEC3D_CROSS(xx, yy, zz);
		VEC3D_NORMALIZE(xx,ln);

		//printf("%f \n", xx[0]*yy[0] + xx[1]*yy[1] + xx[2]*yy[2] );
		//printf("%f \n", zz[0]*xx[0] + zz[1]*xx[1] + zz[2]*xx[2] );
		//printf("%f \n", zz[0]*yy[0] + zz[1]*yy[1] + zz[2]*yy[2] );
	}
	void setVertexID(int idx[2])
	{
		id[0] = idx[0];
		id[1] = idx[1];
	}
	void setUV(double uv[2][2])
	{
		UV[0][0] = uv[0][0];
		UV[0][1] = uv[0][1];
		UV[1][0] = uv[1][0];
		UV[1][1] = uv[1][1];
	}

	void setColor(int rgb[3] )
	{
		color[0] = rgb[0];
		color[1] = rgb[1];
		color[2] = rgb[2];
	}

	void putMesh(FILE* fp, int& vcount, double r, int* color_rgb = NULL)
	{
		if ( r == 0.0 ) r = radius;
		int n = 4;
		fprintf(fp, "#stat-end:%f %f %f %f %f %f\n", p1[0], p1[1], p1[2], p2[0], p2[1], p2[2]);

		if (color_rgb == NULL) color_rgb = color;

		double dth = 2.0*3.1415926535897932/(double)n;
		for ( int i = 0; i < n; i++ )
		{
			double th = (double)i*dth;
			double x1 = p1[0] + r*cos(th)*xx[0] + r*sin(th)*yy[0];
			double y1 = p1[1] + r*cos(th)*xx[1] + r*sin(th)*yy[1];
			double z1 = p1[2] + r*cos(th)*xx[2] + r*sin(th)*yy[2];

			int j = i + 1;
			double th2 = (double)j*dth;
			if ( j == n ) th2 = 0.0;
			double x2 = p1[0] + r*cos(th2)*xx[0] + r*sin(th2)*yy[0];
			double y2 = p1[1] + r*cos(th2)*xx[1] + r*sin(th2)*yy[1];
			double z2 = p1[2] + r*cos(th2)*xx[2] + r*sin(th2)*yy[2];

			double x3 = p2[0] + r*cos(th)*xx[0] + r*sin(th)*yy[0];
			double y3 = p2[1] + r*cos(th)*xx[1] + r*sin(th)*yy[1];
			double z3 = p2[2] + r*cos(th)*xx[2] + r*sin(th)*yy[2];

			double x4 = p2[0] + r*cos(th2)*xx[0] + r*sin(th2)*yy[0];
			double y4 = p2[1] + r*cos(th2)*xx[1] + r*sin(th2)*yy[1];
			double z4 = p2[2] + r*cos(th2)*xx[2] + r*sin(th2)*yy[2];

			fprintf(fp, "v %.4f %.4f %.4f %d %d %d\n", x1, y1, z1, color_rgb[0], color_rgb[1], color_rgb[2]);
			fprintf(fp, "v %.4f %.4f %.4f %d %d %d\n", x2, y2, z2, color_rgb[0], color_rgb[1], color_rgb[2]);
			fprintf(fp, "v %.4f %.4f %.4f %d %d %d\n", x3, y3, z3, color_rgb[0], color_rgb[1], color_rgb[2]);
			fprintf(fp, "v %.4f %.4f %.4f %d %d %d\n", x4, y4, z4, color_rgb[0], color_rgb[1], color_rgb[2]);
			fprintf(fp, "f %d %d %d\n", vcount, vcount+1, vcount+3);
			fprintf(fp, "f %d %d %d\n", vcount+3, vcount+2, vcount);
			vcount += 4;
		}
	}
};


class N_Cylinder
{
public:
	std::vector<Cylinder> cylinders;
	N_Cylinder()
	{}

	void Add(Cylinder& cyl)
	{
		cylinders.push_back( cyl );
	}

	void put( char* filename, double radius, int* color_rgb = NULL)
	{
		FILE* fp = fopen( filename, "w");
		
		int vcount =1;

		for ( int i = 0; i < cylinders.size(); i++ )
		{
			cylinders[i].putMesh(fp, vcount, radius, color_rgb);
		}

		fclose( fp );
	}
	void put( FILE* fp, double radius, int& vcount)
	{
		for ( int i = 0; i < cylinders.size(); i++ )
		{
			cylinders[i].putMesh(fp, vcount, radius);
		}
	}

	void close_loop()
	{
		Cylinder cyl(cylinders[cylinders.size()-1].p2,  cylinders[0].p1, cylinders[0].radius);
		cylinders.push_back( cyl );
	}
};

#endif