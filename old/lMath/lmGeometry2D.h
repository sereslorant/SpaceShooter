#ifndef LM_GEOMETRY
#define LM_GEOMETRY

#include "lmVector2D.h"

bool lmLineLineIntersection(const lmVector2D &line0_point,const lmVector2D &line0_dir,const lmVector2D &line1_point,const lmVector2D &line1_normal,lmScalar *iDist = nullptr,lmVector2D *iPoint = nullptr);

bool lmCircleLineIntersection(const lmVector2D &circle_center,lmScalar circle_radius,const lmVector2D &line_point,const lmVector2D &line_normal,lmScalar *iDist = nullptr,lmVector2D *iPoint = nullptr);

bool lmCircleCircleIntersection(const lmVector2D &circle0_center,const lmScalar circle0_radius,const lmVector2D &circle1_center,const lmScalar circle1_radius);

bool lmPointAABBIntersection(const lmVector2D &point,const lmVector2D &aabb_min,const lmVector2D &aabb_max);

bool lmAABBAABBIntersection(const lmVector2D &aabb0_min,const lmVector2D &aabb0_max,const lmVector2D &aabb1_min,const lmVector2D &aabb1_max);

#endif
