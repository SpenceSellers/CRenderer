#include "vectors.h"

Point3D Point3D_add(Point3D p1, Point3D p2){
    Point3D result;
    result.x = p1.x + p2.x;
    result.y = p1.y + p2.y;
    result.z = p1.z + p2.z;
    return result;
}

Point3D Point3D_subtract(Point3D p1, Point3D p2){
    Point3D result;
    result.x = p1.x - p2.x;
    result.y = p1.y - p2.y;
    result.z = p1.z - p2.z;
    return result;
}

Point3D Point3D_rotZ(Point3D p, double angle){
    if (angle == 0.0) return p;
    int x = p.x * cos(angle) - p.y * sin(angle);
    int y = p.x * sin(angle) + p.y * cos(angle);
    int z = p.z;

    Point3D result = {x,y,z};
    return result;
}

Point3D Point3D_rotX(Point3D p, double angle){
    if (angle == 0.0) return p;
    int y = p.y * cos(angle) - p.z * sin(angle);
    int z = p.y * sin(angle) + p.z * cos(angle);
    int x = p.x;

    Point3D result = {x,y,z};
    return result;
}

Point3D Point3D_rotY(Point3D p, double angle){
    if (angle == 0.0) return p;
    int z = p.z * cos(angle) - p.x * sin(angle);
    int x = p.z * sin(angle) + p.x * cos(angle);
    int y = p.y;

    Point3D result = {x,y,z};
    return result;
}

Point3D Point3D_rotate(Point3D p, RotationVector rotation){
    p =  Point3D_rotY(p, rotation.rotY); // Y rotation has to be first... Don't ask me why.
    p =  Point3D_rotX(p, rotation.rotX);
    p =  Point3D_rotZ(p, rotation.rotZ);
    return p;
}

Point3D Point3D_rotate_around_point(Point3D p, RotationVector rotation, Point3D center){
    return Point3D_add(
	Point3D_rotate(Point3D_subtract(p, center),rotation),
	center);
}

void Line3D_scale(Line3D *line, double factor){
    line->p1.x = (int) line->p1.x * factor;
    line->p1.y = (int) line->p1.y * factor;
    line->p1.z = (int) line->p1.z * factor;

    line->p2.x = (int) line->p2.x * factor;
    line->p2.y = (int) line->p2.y * factor;
    line->p2.z = (int) line->p2.z * factor;
}

Line3D Line3D_rotate_around_point(Line3D line, RotationVector rotation, Point3D center){
    line.p1 = Point3D_rotate_around_point(line.p1, rotation, center);
    line.p2 = Point3D_rotate_around_point(line.p2, rotation, center);
    return line;
}

Point3D Line3D_vectorize(Line3D line){
    Point3D p;
    p.x = line.p2.x - line.p1.x;
    p.y = line.p2.y - line.p1.y;
    p.z = line.p2.z - line.p1.z;
    return p;
}

int Point3D_dot_product(Point3D a, Point3D b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

Point3D Point3D_cross_product(Point3D a, Point3D b){
    Point3D result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}
