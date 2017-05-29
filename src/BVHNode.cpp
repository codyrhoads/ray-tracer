//
//  BVHNode.cpp
//  raytrace
//
//  Created by Cody Rhoads on 5/28/17.
//
//

#include "BVHNode.hpp"
#include "SceneObject.hpp"
#include "Ray.hpp"
#include "IntersectionResults.h"

#define MAX_OBJECTS_PER_BB 1

using namespace std;
using namespace glm;

BVHNode::BVHNode()
{
    left = nullptr;
    right = nullptr;
    objs = vector<shared_ptr<SceneObject>>();
    BBmins = vec3(0);
    BBmaxes = vec3(0);
}

IntersectionResults BVHNode::findIntersection(const shared_ptr<Ray> &ray)
{
    IntersectionResults ir = IntersectionResults();
    IntersectionResults leftIR = IntersectionResults();
    IntersectionResults rightIR = IntersectionResults();
    
    if (testCubeIntersection(ray)) {
        // The ray intersects this node: recursively check the children for an
        // intersection.
        if (objs.empty()) {
            leftIR = left->findIntersection(ray);
            rightIR = right->findIntersection(ray);
            
            if (leftIR.foundIntersection && rightIR.foundIntersection) {
                if (std::min(leftIR.t, rightIR.t) == leftIR.t) {
                    ir = leftIR;
                }
                else {
                    ir = rightIR;
                }
            }
            else if (leftIR.foundIntersection) {
                ir = leftIR;
            }
            else if (rightIR.foundIntersection) {
                ir = rightIR;
            }
        }
        // Reached the leaf, so check the objects in the objs list for an
        // intersection.
        else {
            IntersectionResults tempIR = IntersectionResults();
            for (unsigned int i = 0; i < objs.size(); i++) {
                tempIR = objs.at(i)->findIntersection(ray);
                if (tempIR.foundIntersection) {
                    if (ir.intersectedObj == nullptr || tempIR.t < ir.t) {
                        ir = tempIR;
                    }
                }
            }
        }
    }
    
    return ir;
}

void BVHNode::recursiveBuildTree(vector<shared_ptr<SceneObject>> &objs,
                                 const int axis)
{
    if (objs.size() <= MAX_OBJECTS_PER_BB) {
        this->objs = objs;
        this->calculateBBox();
        return;
    }
    
    // sort objects on axis
    sortObjectsOnAxis(objs, axis);
    
    // Split the array in half and go through each half recursively.
    const size_t halfSize = objs.size()/2;
    vector<shared_ptr<SceneObject>> leftHalf(objs.begin(), objs.begin() + halfSize);
    vector<shared_ptr<SceneObject>> rightHalf(objs.begin() + halfSize, objs.end());
    
    this->left = new BVHNode();
    this->left->recursiveBuildTree(leftHalf, (axis + 1) % 3);
    if (!rightHalf.empty()) {
        this->right = new BVHNode();
        this->right->recursiveBuildTree(rightHalf, (axis + 1) % 3);
    }
    
    if (left == nullptr) {
        printf("Left is null\n");
    }
    if (right == nullptr) {
        printf("Right is null\n");
    }
    
    this->calculateBBox();
}

void BVHNode::calculateBBox()
{
    // Not a leaf. Find the mins/maxes that encompass both children nodes.
    if (objs.empty()) {
        BBmins = getMinsOfChildren();
        BBmaxes = getMaxesOfChildren();
    }
    // A leaf. Find the mins/maxes that encompass all objects
    else {
        BBmins = vec3(objs.at(0)->getMins());
        BBmaxes = vec3(objs.at(0)->getMaxes());
        for (int i = 1; i < objs.size(); i++) {
            vec3 tempMin = objs.at(i)->getMins();
            vec3 tempMax = objs.at(i)->getMaxes();
            
            BBmins = vec3(std::min(BBmins.x, tempMin.x),
                          std::min(BBmins.y, tempMin.y),
                          std::min(BBmins.z, tempMin.z));
            BBmaxes = vec3(std::max(BBmaxes.x, tempMax.x),
                           std::max(BBmaxes.y, tempMax.y),
                           std::max(BBmaxes.z, tempMax.z));
        }
    }
}

bool BVHNode::testCubeIntersection(const shared_ptr<Ray> &ray) const
{
    float tgmin = -numeric_limits<float>::max();
    float tgmax = numeric_limits<float>::max();
    float t1, t2;
    
    if (ray->getDirection().x == 0) {
        if (ray->getOrigin().x <= BBmins.x || ray->getOrigin().x >= BBmaxes.x) {
            return false;
        }
    }
    else {
        // X component
        t1 = (BBmins.x - ray->getOrigin().x) / ray->getDirection().x;
        t2 = (BBmaxes.x - ray->getOrigin().x) / ray->getDirection().x;
        
        if (t1 > t2) {
            swap(t1, t2);
        }
        
        if (t1 > tgmin) {
            tgmin = t1;
        }
        if (t2 < tgmax) {
            tgmax = t2;
        }
    }
    
    if (ray->getDirection().y == 0) {
        if (ray->getOrigin().y <= BBmins.y || ray->getOrigin().y >= BBmaxes.y) {
            return false;
        }
    }
    else {
        // Y component
        t1 = (BBmins.y - ray->getOrigin().y) / ray->getDirection().y;
        t2 = (BBmaxes.y - ray->getOrigin().y) / ray->getDirection().y;
        
        if (t1 > t2) {
            swap(t1, t2);
        }
        
        if (t1 > tgmin) {
            tgmin = t1;
        }
        if (t2 < tgmax) {
            tgmax = t2;
        }
    }
    
    if (ray->getDirection().z == 0) {
        if (ray->getOrigin().z <= BBmins.z || ray->getOrigin().z >= BBmaxes.z) {
            return false;
        }
    }
    else {
        // Z component
        t1 = (BBmins.z - ray->getOrigin().z) / ray->getDirection().z;
        t2 = (BBmaxes.z - ray->getOrigin().z) / ray->getDirection().z;
        
        if (t1 > t2) {
            swap(t1, t2);
        }
        
        if (t1 > tgmin) {
            tgmin = t1;
        }
        if (t2 < tgmax) {
            tgmax = t2;
        }
    }
    
    if (tgmin > tgmax) {
        return false;
    }
    if (tgmax < 0) {
        return false;
    }
    
    return true;
}

vec3 BVHNode::getMinsOfChildren() const
{
    vec3 leftMins = left->getMins();
    vec3 rightMins = right->getMins();
    
    return vec3(std::min(leftMins.x, rightMins.x),
                std::min(leftMins.y, rightMins.y),
                std::min(leftMins.z, rightMins.z));
}

vec3 BVHNode::getMaxesOfChildren() const
{
    vec3 leftMaxes = left->getMaxes();
    vec3 rightMaxes = right->getMaxes();
    
    return vec3(std::max(leftMaxes.x, rightMaxes.x),
                std::max(leftMaxes.y, rightMaxes.y),
                std::max(leftMaxes.z, rightMaxes.z));
}

void BVHNode::sortObjectsOnAxis(vector<shared_ptr<SceneObject>> &objs,
                                const int axis)
{
    if (axis == 0) {
        sort(objs.begin(), objs.end(), &BVHNode::compareByXAxis);
    }
    else if (axis == 1) {
        sort(objs.begin(), objs.end(), &BVHNode::compareByYAxis);
    }
    else {
        sort(objs.begin(), objs.end(), &BVHNode::compareByZAxis);
    }
}

bool BVHNode::compareByXAxis(const shared_ptr<SceneObject> &a,
                             const shared_ptr<SceneObject> &b)
{
    return a->getMins().x < b->getMins().x;
}

bool BVHNode::compareByYAxis(const shared_ptr<SceneObject> &a,
                             const shared_ptr<SceneObject> &b)
{
    return a->getMins().y < b->getMins().y;
}

bool BVHNode::compareByZAxis(const shared_ptr<SceneObject> &a,
                             const shared_ptr<SceneObject> &b)
{
    return a->getMins().z < b->getMins().z;
}
