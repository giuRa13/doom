#ifndef _MATRIX_H
#define _MATRIX_H

#include "vector.h"

typedef union mat4 
{
    struct {
        float a1, a2, a3, a4;
        float b1, b2, b3, b4;
        float c1, c2, c3, c4;
        float d1, d2, d3, d4;
    };
    float m[4][4];
    float v[16];

} mat4_t ;

mat4_t mat4_identity();
mat4_t mat4_mult(mat4_t m1, mat4_t m2);
mat4_t mat4_ortho(float left, float right, float bottom, float top, float near, float far);

mat4_t mat4_translate(vec3_t translation);
mat4_t mat4_scale(vec3_t scale);
mat4_t mat4_rotate(vec3_t axis, float angle);

#endif // !_MATRIX_H
