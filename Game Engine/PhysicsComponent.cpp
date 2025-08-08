#include "ECS/Components/PhysicsComponent.h"
#include "ECS/Entities/Entity.h"
#include "Game Engine/EngineTime.h"
#include "ECS/Systems/PhysicsSystem.h"

PhysicsComponent::PhysicsComponent(reactphysics3d::BodyType type) : Component(ComponentType::PHYSICS)
{ 
    this->bodyType = reactphysics3d::BodyType::DYNAMIC;
    this->rigidBody = nullptr;
	PhysicsSystem::AddPhysicsComponent(this);
   // Init();
}

void PhysicsComponent::Update(constant cc)
{
    if (!this->rigidBody) {
        std::cout << "rb null" << std::endl;
    }

    if (this->rigidBody->getType() == reactphysics3d::BodyType::DYNAMIC) {
        reactphysics3d::Transform transform = this->rigidBody->getTransform();
        reactphysics3d::Vector3 position = transform.getPosition();
        reactphysics3d::Quaternion quaternion = transform.getOrientation();
        this->owner->m_transform.m_translation = Vector3D(position.x, position.y, position.z);
        this->owner->m_transform.m_rotation = Vector3D(QuaternionToEuler(quaternion));
    }
}

void PhysicsComponent::Release()
{
	PhysicsSystem::RemovePhysicsComponent(this);
}

Component* PhysicsComponent::CreateSnapshot()
{
    //return new PhysicsComponent(this->bodyType);
    //auto* clone = new PhysicsComponent(this->bodyType);
    //clone->rigidBody = PhysicsSystem::CreateRigidBody(this->owner->m_transform); // or similar
    //return clone;
    Init();
    return this;
}

void PhysicsComponent::Init()
{ 
    reactphysics3d::PhysicsCommon* physicsCommon = PhysicsSystem::GetPhysicsCommon();
    reactphysics3d::PhysicsWorld* physicsWorld = PhysicsSystem::GetPhysicsWorld();

    //create rigid body
    Vector3D scale = this->owner->m_transform.m_scale;
    Vector3D posTemp = this->owner->m_transform.m_translation;
    reactphysics3d::Vector3 position = reactphysics3d::Vector3(posTemp.m_x, posTemp.m_y, posTemp.m_z);
    reactphysics3d::Quaternion quaternion = EulerToQuaternion(this->owner->m_transform.m_rotation);

    reactphysics3d::Transform transform(position, quaternion);
    reactphysics3d::BoxShape* boxShape = physicsCommon->createBoxShape(reactphysics3d::Vector3(scale.m_x / 2, scale.m_y / 2, scale.m_z / 2));

    this->rigidBody = physicsWorld->createRigidBody(transform);
    this->rigidBody->setType(this->bodyType);
    this->rigidBody->addCollider(boxShape, reactphysics3d::Transform::identity());

    if (this->bodyType == reactphysics3d::BodyType::DYNAMIC) {
        this->rigidBody->updateMassPropertiesFromColliders();
        this->rigidBody->setMass(this->mass);
    }

    std::cout << "rigid body created: " << this->rigidBody << std::endl;
}

reactphysics3d::RigidBody* PhysicsComponent::GetRigidBody()
{
	return this->rigidBody;
}

void PhysicsComponent::SetBodyType(reactphysics3d::BodyType type)
{
    this->bodyType = type;
}

reactphysics3d::Quaternion PhysicsComponent::EulerToQuaternion(Vector3D euler)
{
    double cy = cos(euler.m_x * 0.5);
    double sy = sin(euler.m_x * 0.5);
    double cp = cos(euler.m_y * 0.5);
    double sp = sin(euler.m_y * 0.5);
    double cr = cos(euler.m_z * 0.5);
    double sr = sin(euler.m_z * 0.5);

    reactphysics3d::Quaternion q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;

    return q;
}

Vector3D PhysicsComponent::QuaternionToEuler(reactphysics3d::Quaternion q)
{
    double w = q.w; double x = q.x;  double y = q.y;  double z = q.z;
    double roll = atan2(2 * (w * x + y * z), 1 - 2 * (x * x + y * y));
    double pitch = std::asin(std::clamp(2 * (w * y - z * x), -1.0, 1.0));
    double yaw = atan2(2 * (w * z + x * y), 1 - 2 * (y * y + z * z));

    return Vector3D(roll, pitch, yaw);
}