//
//  Triangle.cpp
//  raytrace
//
//  Created by Cody Rhoads on 5/9/17.
//
//

#include "Triangle.hpp"
#include "LightSource.hpp"
#include "Ray.hpp"
#include "IntersectionResults.h"

using namespace std;
using namespace glm;

Triangle::Triangle() :
SceneObject(),
v0(vec3(0)),
v1(vec3(0)),
v2(vec3(0))
{
    
}

Triangle::Triangle(const int ID, const vec3 &v0, const vec3 &v1, const vec3 &v2,
                   const vec3 &color, const float ambient, const float diffuse,
                   const float specular, const float reflection, const float filter,
                   const float roughness, const float metallic, const float ior,
                   const mat4 &inverseModelMatrix) :
SceneObject(ID, color, ambient, diffuse, specular, reflection, filter, roughness,
            metallic, ior, inverseModelMatrix),
v0(v0),
v1(v1),
v2(v2)
{
    float minX = std::min(v0.x, v1.x);
    minX = std::min(minX, v2.x);
    float minY = std::min(v0.y, v1.y);
    minY = std::min(minY, v2.y);
    float minZ = std::min(v0.z, v1.z);
    minZ = std::min(minZ, v2.z);
    float maxX = std::max(v0.x, v1.x);
    maxX = std::max(maxX, v2.x);
    float maxY = std::max(v0.y, v1.y);
    maxY = std::max(maxY, v2.y);
    float maxZ = std::max(v0.z, v1.z);
    maxZ = std::max(maxZ, v2.z);

    BBmins = vec3(minX, minY, minZ);
    BBmaxes = vec3(maxX, maxY, maxZ);
}

IntersectionResults Triangle::findIntersection(const shared_ptr<Ray> &ray)
{
    IntersectionResults ir = IntersectionResults();
    
    // v0 = a, v1 = b, v2 = c
    const float Xa_minus_Px = v0.x - ray->getOrigin().x;
    const float Ya_minus_Py = v0.y - ray->getOrigin().y;
    const float Za_minus_Pz = v0.z - ray->getOrigin().z;
    const float Xa_minus_Xb = v0.x - v1.x;
    const float Ya_minus_Yb = v0.y - v1.y;
    const float Za_minus_Zb = v0.z - v1.z;
    const float Xa_minus_Xc = v0.x - v2.x;
    const float Ya_minus_Yc = v0.y - v2.y;
    const float Za_minus_Zc = v0.z - v2.z;
    const float Xd = ray->getDirection().x;
    const float Yd = ray->getDirection().y;
    const float Zd = ray->getDirection().z;
    
    float topDeterminant;
    const float bottomDeterminant = (Xa_minus_Xb * Ya_minus_Yc * Zd) +
                                    (Xa_minus_Xc * Yd * Za_minus_Zb) +
                                    (Xd * Ya_minus_Yb * Za_minus_Zc) -
                                    (Xa_minus_Xb * Yd * Za_minus_Zc) -
                                    (Xa_minus_Xc * Ya_minus_Yb * Zd) -
                                    (Xd * Ya_minus_Yc * Za_minus_Zb);
    
    // Calculate t
    topDeterminant = (Xa_minus_Xb * Ya_minus_Yc * Za_minus_Pz) +
                     (Xa_minus_Xc * Ya_minus_Py * Za_minus_Zb) +
                     (Xa_minus_Px * Ya_minus_Yb * Za_minus_Zc) -
                     (Xa_minus_Xb * Ya_minus_Py * Za_minus_Zc) -
                     (Xa_minus_Xc * Ya_minus_Yb * Za_minus_Pz) -
                     (Xa_minus_Px * Ya_minus_Yc * Za_minus_Zb);
    const float calcT = topDeterminant/bottomDeterminant;
    
    if (calcT < 0) {
        return ir;
    }
    
    // Calculate gamma
    topDeterminant = (Xa_minus_Xb * Ya_minus_Py * Zd) +
                     (Xa_minus_Px * Yd * Za_minus_Zb) +
                     (Xd * Ya_minus_Yb * Za_minus_Pz) -
                     (Xa_minus_Xb * Yd * Za_minus_Pz) -
                     (Xa_minus_Px * Ya_minus_Yb * Zd) -
                     (Xd * Ya_minus_Py * Za_minus_Zb);
    const float gamma = topDeterminant/bottomDeterminant;
    
    if (gamma < 0 || gamma > 1) {
        return ir;
    }
    
    // Calculate beta
    topDeterminant = (Xa_minus_Px * Ya_minus_Yc * Zd) +
                     (Xa_minus_Xc * Yd * Za_minus_Pz) +
                     (Xd * Ya_minus_Py * Za_minus_Zc) -
                     (Xa_minus_Px * Yd * Za_minus_Zc) -
                     (Xa_minus_Xc * Ya_minus_Py * Zd) -
                     (Xd * Ya_minus_Yc * Za_minus_Pz);
    const float beta = topDeterminant/bottomDeterminant;
    
    if (beta < 0 || (beta > 1 - gamma)) {
        return ir;
    }
    
    ir.foundIntersection = true;
    ir.t = calcT;
    ir.intersectedObj = shared_from_this();
    return ir;
}

void Triangle::printObjectInfo() const
{
    printf("- Type: Triangle\n");
    printf("- Vertex 0: {%.4g %.4g %.4g}\n", v0.x, v0.y, v0.z);
    printf("- Vertex 1: {%.4g %.4g %.4g}\n", v1.x, v1.y, v1.z);
    printf("- Vertex 2: {%.4g %.4g %.4g}\n", v2.x, v2.y, v2.z);
    SceneObject::printObjectInfo();
}
