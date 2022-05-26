#ifndef SHAPE
#define SHAPE

#include "main.h"
#include <algorithm>
class shape
{
protected:

    vec3 color = { 225,0,0 };

public:
    shape() = default;
    shape( const vec3& color_)
    {
        color = color_;
    }

    //ôöèè
    virtual bool ray_intersect(const vec3& orig, const vec3& dir, double& t0, vec3& hit, vec3& N) = 0;
    vec3 get_col()
    {
        return color;
    }
   
};

class sphere : public shape
{
private:
    vec3 center = { 0.0, 0.0, 0.0 };
    double radius = 2.0;
public:
    sphere() = default;
    sphere( const vec3& center, const double& radius, const vec3& color) : shape(color)
    {
        this->center = center;
        this->radius = radius;
    }

   
    bool ray_intersect(const vec3& orig, const vec3& dir, double& t0, vec3& hit, vec3& N)  override
    {
        bool s = true;
        vec3 L = center - orig;
        float tca = L * dir;
        float d2 = L * L - tca * tca;
        if (d2 > radius * radius) s = false;
        float thc = sqrtf(radius * radius - d2);
        t0 = tca - thc;
        float t1 = tca + thc;

        if (t0 < 0) t0 = t1;
        if (t0 < 0) s = false;

        hit = orig + dir * t0;
        N = (hit - get_center()).normalize();

        return s;
    }
    vec3 get_center()
    {
        return center;
    }
    double get_radius()
    {
        return radius;
    }

};
class box : public shape
{
private:
    vec3 v_min = { 0.0, 0.0, 0.0 };
    vec3 v_max = { 2.0, 2.0, 2.0 };



    int func(const vec3& A, const vec3& B, const vec3& C, const vec3& D, const vec3& orig, const vec3& dir, double& t1, vec3& N)
    {
        double d = triangle_intersection(orig, dir, A, B, C);
        if (d == 0)
            d = triangle_intersection(orig, dir, A, C, D);
        if (d <= 0)
            return -1;
        if (d > t1 && t1 > 0)
            return -1;

        N = cross(B - A, D - A).normalize();
        t1 = d;

        return 0;
    }
    double triangle_intersection(const vec3& orig, const vec3 dir, const vec3& v0, const vec3& v1, const vec3& v2)
    {
        vec3 e1 = v1 - v0;
        vec3 e2 = v2 - v0;
        vec3 pvec = cross(dir, e2);
        double det = e1 * pvec;

        
        if (det < 1e-8 && det > -1e-8)
        {
            return 0;
        }

        double inv_det = 1 / det;
        vec3 tvec = orig - v0;
        double u = (tvec * pvec) * inv_det;
        if (u < 0 || u > 1) {
            return 0;
        }

        vec3 qvec = cross(tvec, e1);
        double v = (dir * qvec) * inv_det;
        if (v < 0 || u + v > 1) {
            return 0;
        }
        return (e2 * qvec) * inv_det;
    }

public:
    box() = default;
    box( const vec3& v_min, const vec3& v_max, const vec3& color) : shape(color)
    {
        this->v_min = v_min;
        this->v_max = v_max;
    }

    bool ray_intersect(const vec3& orig, const vec3& dir, double& t0, vec3& hit, vec3& N)  override
    {
        double t1 = -1;
        vec3 A = v_min;
        vec3 A1 = vec3(v_min[0], v_max[1], v_min[2]);
        vec3 B = vec3(v_max[0], v_min[1], v_min[2]);
        vec3 B1 = vec3(v_max[0], v_max[1], v_min[2]);
        vec3 C = vec3(v_max[0], v_min[1], v_max[2]);
        vec3 C1 = v_max;
        vec3 D = vec3(v_min[0], v_min[1], v_max[2]);
        vec3 D1 = vec3(v_min[0], v_max[1], v_max[2]);

        
        int f1 = func(A, B, B1, A1, orig, dir, t1, N); 

        int f2 = func(B, C, C1, B1, orig, dir, t1, N);
        int f3 = func(C, D, D1, C1, orig, dir, t1, N);
        int f4 = func(D, A, A1, D1, orig, dir, t1, N); 
        int f5 = func(A1, B1, C1, D1, orig, dir, t1, N); 

        
        int f6 = func(D, C, B, A, orig, dir, t1, N);

        t0 = t1;
        hit = orig + dir * t0;
        if (f1 * f2 * f3 * f4 * f5 * f6 == 0)
            return true;
        return false;
    }
    vec3 get_v_min()
    {
        return v_min;
    }
    vec3 get_v_max()
    {
        return v_max;
    }
};
//
class tetra : public shape
{
private:
    vec3 v1 = { 0.0, 0.0, 0.0 };
    vec3 v2 = { 2.0, 2.0, 0.0 };
    vec3 v3 = { 0.0, 2.0, 2.0 };
    vec3 v4 = { 2.0, 0.0, 2.0 };
    int func(const vec3& A, const vec3& B, const vec3& C, const vec3& D, const vec3& orig, const vec3& dir, double& t1, vec3& N)
    {
        double d = triangle_intersection(orig, dir, A, B, C);
        if (d <= 0)
            return -1;
        if (d > t1 && t1 > 0)
            return -1;

        auto n = cross(B - A, C - B).normalize();
        if (n * (D - C) >= 0)
            n = n * (-1);

        if (n * dir > 0)
            return -1;


        N = n;
        t1 = d;
        return 0;
    }
    double triangle_intersection(const vec3& orig, const vec3 dir, const vec3& v0, const vec3& v1, const vec3& v2)
    {
        vec3 e1 = v1 - v0;
        vec3 e2 = v2 - v0;
        vec3 pvec = cross(dir, e2);
        double det = e1 * pvec;

       
        if (det < 1e-8 && det > -1e-8)
        {
            return 0;
        }

        double inv_det = 1 / det;
        vec3 tvec = orig - v0;
        double u = (tvec * pvec) * inv_det;
        if (u < 0 || u > 1) {
            return 0;
        }

        vec3 qvec = cross(tvec, e1);
        double v = (dir * qvec) * inv_det;
        if (v < 0 || u + v > 1) {
            return 0;
        }
        return (e2 * qvec) * inv_det;
    }

public:
    tetra() = default;
    tetra(const vec3& v1, const vec3& v2, const vec3& v3, const vec3& v4, const vec3& color) : shape(color)
    {
        this->v1 = v1; this->v2 = v2; this->v3 = v3; this->v4 = v4;
    }


    bool ray_intersect(const vec3& orig, const vec3& dir, double& t0, vec3& hit, vec3& N)  override
    {
        double t1 = -1;

        
        int f1 = func(v1, v2, v3, v4, orig, dir, t1, N); 
        int f2 = func(v2, v3, v4, v1, orig, dir, t1, N);
        int f3 = func(v1, v4, v3, v2, orig, dir, t1, N);
        int f4 = func(v1, v2, v4, v3, orig, dir, t1, N); 

        t0 = t1;
        hit = orig + dir * t0;
        if (f1 * f2 * f3 * f4 == 0)
            return true;
        return false;
    }

    vec3 get_v1()
    {
        return v1;
    }
    vec3 get_v2()
    {
        return v2;
    }
    vec3 get_v3()
    {
        return v3;
    }
    vec3 get_v4()
    {
        return v4;
    }
};

class triangle : public shape
{
private:
    vec3 v1 = { 0.0, 0.0, 0.0 };
    vec3 v2 = { 2.0, 2.0, 0.0 };
    vec3 v3 = { 0.0, 2.0, 2.0 };
    int func(const vec3& A, const vec3& B, const vec3& C, const vec3& orig, const vec3& dir, double& t1, vec3& N)
    {
        double d = triangle_intersection(orig, dir, A, B, C);
        if (d <= 0)
            return -1;
        if (d > t1 && t1 > 0)
            return -1;

        auto n = cross(B - A, C - B).normalize();

        if (n * dir > 0)
            return -1;


        N = n;
        t1 = d;
        return 0;
    }
    double triangle_intersection(const vec3& orig, const vec3 dir, const vec3& v0, const vec3& v1, const vec3& v2)
    {
        vec3 e1 = v1 - v0;
        vec3 e2 = v2 - v0;
        vec3 pvec = cross(dir, e2);
        double det = e1 * pvec;


        if (det < 1e-8 && det > -1e-8)
        {
            return 0;
        }

        double inv_det = 1 / det;
        vec3 tvec = orig - v0;
        double u = (tvec * pvec) * inv_det;
        if (u < 0 || u > 1) {
            return 0;
        }

        vec3 qvec = cross(tvec, e1);
        double v = (dir * qvec) * inv_det;
        if (v < 0 || u + v > 1) {
            return 0;
        }
        return (e2 * qvec) * inv_det;
    }

public:
    triangle() = default;
    triangle(const vec3& v1, const vec3& v2, const vec3& v3, const vec3& color) : shape(color)
    {
        this->v1 = v1; this->v2 = v2; this->v3 = v3; 
    }


    bool ray_intersect(const vec3& orig, const vec3& dir, double& t0, vec3& hit, vec3& N)  override
    {
        double t1 = -1;


        int f1 = func(v1, v2, v3, orig, dir, t1, N);

        t0 = t1;
        hit = orig + dir * t0;

           
        return f1 == 0;
    }

    vec3 get_v1()
    {
        return v1;
    }
    vec3 get_v2()
    {
        return v2;
    }
    vec3 get_v3()
    {
        return v3;
    }
    
};

#endif

