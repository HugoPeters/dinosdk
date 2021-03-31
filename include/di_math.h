#ifndef _DIMATH_H_
#define _DIMATH_H_

#include "di_base.h"

extern float    DLL_IMPORT  floorf(float v);
extern float    DLL_IMPORT  vec3f_dot(const vec3f* a, const vec3f* b);
extern float    DLL_IMPORT  vec3f_length(const vec3f* a);
extern float    DLL_IMPORT  vec3f_lengthSquared(const vec3f* a);
extern void     DLL_IMPORT  vec3f_cross(const vec3f* a, const vec3f* b, vec3f* dst);
extern float    DLL_IMPORT  vec3f_distance(const vec3f* a, const vec3f* b);
extern float    DLL_IMPORT  vec3f_distanceSquared(const vec3f* a, const vec3f* b);

// only uses X and Z for distance calculation
extern float    DLL_IMPORT  vec3f_distance2D(const vec3f* a, const vec3f* b);
extern float    DLL_IMPORT  vec3f_distance2DSquared(const vec3f* a, const vec3f* b);

// normalizes vec, and returns the original length
extern float    DLL_IMPORT  vec3f_normalize(vec3f* vec);

// dst = a - b
extern void     DLL_IMPORT  vec3f_subtract(const vec3f* a, const vec3f* b, vec3f* dst);

// dst = a + b * c;
extern void     DLL_IMPORT  vec3f_scale_add(const vec3f* a, const vec3f* b, float c, vec3f* dst);

extern void     DLL_IMPORT  mtx44_scale(mtx44* mtx, float s);
extern void     DLL_IMPORT  mtx44_translate(mtx44* mtx, float x, float y, float z);

inline void vec3f_add(const vec3f* a, const vec3f* b, vec3f* dst)
{
    dst->x = a->x + b->x;
    dst->y = a->y + b->y;
    dst->z = a->z + b->z;
}

#endif // _DIMATH_H_