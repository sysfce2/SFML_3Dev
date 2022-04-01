#pragma once
#include "3Dev.h"

/*
 * This class simplifies the work with reactphysics3d library
 */
class PhysicsManager
{
public:
    /*
     * Basic constructor
     * @param settings settings of your physics world
     */
    PhysicsManager(rp3d::PhysicsWorld::WorldSettings settings);
    ~PhysicsManager();
    /*
     * Updates the physics
     * @param t time step
     */
    void Update(float t);
    
    /*
     * Create a box collision shape
     * NOTE: to create, for example, box shape with size (10, 10, 10)
     * you need to pass (5, 5, 5) in the function
     * @param size shape extent
     * @return pointer to the box shape object
     */
    rp3d::BoxShape* CreateBoxShape(rp3d::Vector3 size);

    /*
     * Create a sphere collision shape
     * @param radius sphere shape radius
     * @return pointer to the sphere shape object
     */
    rp3d::SphereShape* CreateSphereShape(float radius);

    /*
     * Create a capsule collision shape
     * @param radius radius of the top and down parts
     * @param height capsule shape height
     * @return pointer to the capsule shape object
     */
    rp3d::CapsuleShape* CreateCapsuleShape(float radius, float height);

    /*
     * Create a concave mesh collision shape
     * @param mesh a pointer to the TriangleMesh object
     */
    rp3d::ConcaveMeshShape* CreateConcaveMeshShape(rp3d::TriangleMesh* mesh);
    
    /*
     * Create a rigid body
     * @param transform initial transform of the body
     * @return pointer to the rigid body object
     */
    rp3d::RigidBody* CreateRigidBody(const rp3d::Transform& transform);
    
    // Functions for creating various types of joints
    /*
     * @param info information about ball and socket joint
     * @return pointer to the ball and socket joint object
     */
    rp3d::BallAndSocketJoint* CreateBallAndSocketJoint(rp3d::BallAndSocketJointInfo info);

    /*
     * @param info information about hinge joint
     * @return pointer to the hinge joint object
     */
    rp3d::HingeJoint* CreateHingeJoint(rp3d::HingeJointInfo info);

    /*
     * @param info information about slider joint
     * @return pointer to the slider joint object
     */
    rp3d::SliderJoint* CreateSliderJoint(rp3d::SliderJointInfo info);

    // @return a pointer to TriangleMesh object
    rp3d::TriangleMesh* CreateTriangleMesh();

    // @return settings of this world
    rp3d::PhysicsWorld::WorldSettings GetSettings();

    // @return pointer to the physics world
    rp3d::PhysicsWorld* GetWorld();

private:
    rp3d::PhysicsCommon common;
    rp3d::PhysicsWorld::WorldSettings settings;
    rp3d::PhysicsWorld* world;
};
