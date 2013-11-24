#ifndef VECTORS_H
#define VECTORS_H

#include <math.h>

typedef struct Point3D {
    int x;
    int y;
    int z;
} Point3D;

typedef struct Point2D {
    int x;
    int y;
} Point2D;

typedef struct Line3D {
    Point3D p1;
    Point3D p2;
} Line3D;

typedef struct RotationVector {
    double rotX;
    double rotY;
    double rotZ;
} RotationVector;

Point3D Point3D_add(Point3D p1, Point3D p2);
Point3D Point3D_subtract(Point3D p1, Point3D p2);

double Point3D_distance(Point3D p1, Point3D p2);

Point3D Point3D_rotZ(Point3D p, double angle);
Point3D Point3D_rotX(Point3D p, double angle);
Point3D Point3D_rotY(Point3D p, double angle);

// Rotates a point around the origin.
Point3D Point3D_rotate(Point3D p, RotationVector rotation);

Point3D Point3D_rotate_around_point(Point3D p,
				    RotationVector rotation,
				    Point3D center);

/* Scales a line by a factor.
   Note that lines are scaled around the origin!
*/
void Line3D_scale(Line3D *line, double factor);
    
// Rotates a line around a specified point. 
Line3D Line3D_rotate_around_point(Line3D line,
				  RotationVector rotation,
				  Point3D center);

Point3D Line3D_vectorize(Line3D line);
double Line3D_length(Line3D line);
Point3D Line3D_midpoint(Line3D line);

int Point3D_dot_product(Point3D a, Point3D b);

Point3D Point3D_cross_product(Point3D a, Point3D b);
#endif
