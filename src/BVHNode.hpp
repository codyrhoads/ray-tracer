//
//  BVHNode.hpp
//  raytrace
//
//  Created by Cody Rhoads on 5/28/17.
//
//

#ifndef BVHNode_hpp
#define BVHNode_hpp

#include "SceneObject.hpp"

class BVHNode : public SceneObject
{
public:
    BVHNode();
    
    IntersectionResults findIntersection(const std::shared_ptr<Ray> &ray);
    void recursiveBuildTree(std::vector<std::shared_ptr<SceneObject>> &objs,
                            const int axis);
    void calculateBBox();
    
    std::string getObjectType() const {return "BVHNode";}
private:
    bool testCubeIntersection(const std::shared_ptr<Ray> &ray) const;
    glm::vec3 getMinsOfChildren() const;
    glm::vec3 getMaxesOfChildren() const;
    void sortObjectsOnAxis(std::vector<std::shared_ptr<SceneObject>> &objs,
                           const int axis);
    
    static bool compareByXAxis(const std::shared_ptr<SceneObject> &a,
                               const std::shared_ptr<SceneObject> &b);
    static bool compareByYAxis(const std::shared_ptr<SceneObject> &a,
                               const std::shared_ptr<SceneObject> &b);
    static bool compareByZAxis(const std::shared_ptr<SceneObject> &a,
                               const std::shared_ptr<SceneObject> &b);
    
    BVHNode *left;
    BVHNode *right;
    std::vector<std::shared_ptr<SceneObject>> objs;
};

#endif /* BVH_hpp */
