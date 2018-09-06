#include "physics_stdafx.h"

#include "Collision/Octree.h"
#include "RigidBodyComponent.h"
#include "Colliders/BoxColliderComponent.h"
#include "Colliders/SphereColliderComponent.h"
#include "Collision/CollisionSystem.h"

using namespace Physics::Collision;
using namespace Core::Maths;
using namespace Physics::Bounds;

std::queue<ACollider*> Octree::m_pendingInsertionObjects = std::queue<ACollider*>();
bool Octree::m_treeReady = false;
bool Octree::m_treeBuilt = false;

unsigned int upper_power_of_two(unsigned int v)
{
	--v;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	++v;
	return v;
}

Octree::Octree(const AABB& p_region, const std::vector<ACollider*>& p_objectList) : m_objects(p_objectList),
																					m_collisionSystem(ECS::ECS_Engine->GetSystemManager()->GetSystem<CollisionSystem>()),
																					m_region(p_region),
																					m_currentLife(-1)
{
}

Octree::Octree(const AABB& p_region) :	m_collisionSystem(ECS::ECS_Engine->GetSystemManager()->GetSystem<CollisionSystem>()),
										m_region(p_region),
										m_currentLife(-1)
{
}

Octree::Octree() :  m_collisionSystem(ECS::ECS_Engine->GetSystemManager()->GetSystem<CollisionSystem>()),
				    m_currentLife(-1)
{
}

Octree::~Octree()
{
	DeleteNodes();
}

void Octree::Update(const double dt_ms)
{
	UpdateTree();

	if (m_treeBuilt == true)
	{
		if (m_objects.size() == 0)
		{
			if (m_activeChilds == 0)
			{
				if (m_currentLife == -1)
					m_currentLife = m_maxLifetime;
				else if (m_currentLife > 0)
					m_currentLife--;
			}
		}
		else
		{
			if (m_currentLife != -1)
			{
				if (m_maxLifetime <= 64)
					m_maxLifetime *= 2;
				m_currentLife = -1;
			}
		}
		std::vector<ACollider*> movedObjects;
		movedObjects.reserve(m_objects.size());
		for (ACollider* gameObj : m_objects)
		{
			RigidBody::RigidBodyComponent* rb = nullptr;
			if (const BoxColliderComponent* box = dynamic_cast<const BoxColliderComponent*>(gameObj))
			{
				rb = ECS::ECS_Engine->GetComponentManager()->GetComponent<RigidBody::RigidBodyComponent>(box->GetOwnerID());
			}
			else if (const SphereColliderComponent* sphere = dynamic_cast<const SphereColliderComponent*>(gameObj))
			{
				rb = ECS::ECS_Engine->GetComponentManager()->GetComponent<RigidBody::RigidBodyComponent>(sphere->GetOwnerID());
			}

			if (rb && rb->HasMoved())
			{
				movedObjects.push_back(gameObj);
			}
		}

		//prune any dead objects from the tree.
		size_t objectsCount = m_objects.size();
		for (size_t i = 0; i < objectsCount; i++)
		{
			SphereColliderComponent* sphere = dynamic_cast<SphereColliderComponent*>(m_objects[i]);
			BoxColliderComponent* box		= dynamic_cast<BoxColliderComponent*>(m_objects[i]);
			if ((sphere && (!sphere->IsActive() || !ECS::ECS_Engine->GetEntityManager()->GetEntity(sphere->GetOwnerID())->IsActive())) || 
				(box && (!box->IsActive() || !ECS::ECS_Engine->GetEntityManager()->GetEntity(box->GetOwnerID())->IsActive())))
			{
				const auto res = std::find(movedObjects.begin(), movedObjects.end(), m_objects[i]);
				if (res != movedObjects.end())
					movedObjects.erase(res);
				m_objects.erase(m_objects.begin() + i--);
				objectsCount--;
			}
		}

		//recursively update any child nodes.
		for (int flags = m_activeChilds, index = 0; flags > 0; flags >>= 1, index++)
			if ((flags & 1) == 1)
				m_childs[index]->Update(dt_ms);


		//If an object moved, we can insert it into the parent and that will insert it into the correct tree node.
		//note that we have to do this last so that we don't accidentally update the same object more than once per frame.
		for (ACollider* movedObj : movedObjects)
		{
			if (movedObj)
			{
				Octree* current = this;
				//figure out how far up the tree we need to go to reinsert our moved object
				//we are either using a bounding rect or a bounding sphere
				//try to move the object into an enclosing parent node until we've got full containment

				while (m_collisionSystem->Contains(&current->m_region, movedObj) != ContainmentType::Contains)
					if (current->m_parent != nullptr)
						current = current->m_parent;
					else
						break;

				//now, remove the object from the current node and insert it into the current containing node.
				m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), movedObj), m_objects.end());
				current->Insert(movedObj); //this will try to insert the object as deep into the tree as we can go.
			}
		}

		//prune out any dead branches in the tree
		for (int flags = m_activeChilds, index = 0; flags > 0; flags >>= 1, index++)
		{
			if ((flags & 1) == 1 && m_childs[index]->m_currentLife == 0)
			{
				delete m_childs[index];
				m_childs[index] = nullptr;
				m_activeChilds ^= (unsigned char)(1 << index); //remove the node from the active nodes flag list
			}
		}
	}
}

std::vector<CollisionInfo> Octree::GetCollisions(std::vector<ACollider*> parentObjs)
{
	std::vector<CollisionInfo> collisions;
	for (ACollider* pObj : parentObjs)
	{
		for (ACollider* lObj : m_objects)
		{
			CollisionInfo collision = m_collisionSystem->Collides(pObj, lObj);
			if (collision.containmentType == ContainmentType::Intersects)
			{
				collisions.emplace_back(collision);
			}
		}
	}

	//Check all our local objects against all other local objects in the node
	if (m_objects.size() > 1)
	{
		std::vector<ACollider*> tmp = m_objects;

		while (tmp.size() > 0)
		{
			for (ACollider* lObj2 : tmp)
			{
				//TODO: Move dynamic object check to Physic system 
				RigidBody::RigidBodyComponent* lobj2Rb = nullptr;
				RigidBody::RigidBodyComponent* tempObjRb = nullptr;
				if (BoxColliderComponent* boxColl = dynamic_cast<BoxColliderComponent*>(lObj2))
				{
					lobj2Rb = ECS::ECS_Engine->GetComponentManager()->GetComponent<RigidBody::RigidBodyComponent>(boxColl->GetOwnerID());
				}
				else if (SphereColliderComponent* sphereColl = dynamic_cast<SphereColliderComponent*>(lObj2))
				{
					lobj2Rb = ECS::ECS_Engine->GetComponentManager()->GetComponent<RigidBody::RigidBodyComponent>(sphereColl->GetOwnerID());
				}

				if (BoxColliderComponent* boxColl = dynamic_cast<BoxColliderComponent*>(tmp[tmp.size() - 1]))
				{
					tempObjRb = ECS::ECS_Engine->GetComponentManager()->GetComponent<RigidBody::RigidBodyComponent>(boxColl->GetOwnerID());
				}
				else if (SphereColliderComponent* sphereColl = dynamic_cast<SphereColliderComponent*>(tmp[tmp.size() - 1]))
				{
					tempObjRb = ECS::ECS_Engine->GetComponentManager()->GetComponent<RigidBody::RigidBodyComponent>(sphereColl->GetOwnerID());
				}

				if (tmp[tmp.size() - 1] == lObj2 || (tempObjRb == nullptr && lobj2Rb == nullptr))
					continue;

				const CollisionInfo collision = m_collisionSystem->Collides(tmp[tmp.size() - 1], lObj2);
				if (collision.containmentType == ContainmentType::Intersects)
				{
					collisions.push_back(collision);
				}
			}
			//remove this object from the temp list so that we can run in O(N(N+1)/2) time instead of O(N*N)
			tmp.erase(tmp.begin() + (tmp.size() - 1));
		}
	}

	parentObjs.insert(parentObjs.end(),m_objects.begin(), m_objects.end());

	for (int flags = m_activeChilds, index = 0; flags > 0; flags >>= 1, index++)
	{
		if ((flags & 1) == 1)
		{
			std::vector<CollisionInfo> childCollisions = m_childs[index]->GetCollisions(parentObjs);
			collisions.insert(collisions.end(), childCollisions.begin(), childCollisions.end());
		}
	}
	return collisions;
}

void Octree::AddEntity(ACollider* p_object)
{
	m_pendingInsertionObjects.push(p_object);
}

AABB Octree::FindEnclosingCube(AABB p_region)
{
	//we can't guarantee that all bounding regions will be relative to the origin, so to keep the math
	//simple, we're going to translate the existing region to be oriented off the origin and remember the translation.
	//find the min offset from (0,0,0) and translate by it for a short while
	const vec3f offset = -p_region.m_min;
	p_region.m_min += offset;
	p_region.m_max += offset;

	//A 3D rectangle has a length, height, and width. Of those three dimensions, we want to find the largest dimension.
	//the largest dimension will be the minimum dimensions of the cube we're creating.
	const float highX = std::ceil(std::max(std::max(p_region.m_max.x, p_region.m_max.y), p_region.m_max.z));

	//see if our cube dimension is already at a power of 2. If it is, we don't have to do any work.
	for (int bit = 0; bit < 32; bit++)
	{
		if (highX == 1 << bit)
		{
			p_region.m_max = vec3f(highX, highX, highX);

			p_region.m_min -= offset;
			p_region.m_max -= offset;
			return AABB(p_region.m_min, p_region.m_max);
		}
	}

	//We have a cube with non-power of two dimensions. We want to find the next highest power of two.
	//example: 63 -> 64; 65 -> 128;
	const float x = static_cast<float>(upper_power_of_two(static_cast<unsigned int>(highX)));

	p_region.m_max = vec3f(x, x, x);

	p_region.m_min -= offset;
	p_region.m_max -= offset;

	return AABB(p_region.m_min, p_region.m_max);
}

std::vector<CollisionInfo> Octree::GetCollisions()
{
	return GetCollisions(std::vector<ACollider*>());
}

void Octree::UpdateTree()
{
	if (!m_treeBuilt)
	{
		while (m_pendingInsertionObjects.size() > 0)
		{
			m_objects.push_back(m_pendingInsertionObjects.front());
			m_pendingInsertionObjects.pop();
		}
		BuildTree();
	}
	else
	{
		while (m_pendingInsertionObjects.size() > 0)
		{
			Insert(m_pendingInsertionObjects.front());
			m_pendingInsertionObjects.pop();
		}
	}
	m_treeReady = true;
}

void Octree::BuildTree()
{
	if (m_objects.size() <= 1)
		return;

	vec3f dimensions = m_region.m_max - m_region.m_min;
	if (dimensions.IsNull())
	{
		FindEnclosingCube(AABB(vec3f(-15.0f, -15.0f, -15.0f), vec3f(5.0f, 5.0f, 5.0f)));
		dimensions = m_region.m_max - m_region.m_min;
	}

	if (dimensions.x <= MIN_REGION_SIZE && dimensions.y <= MIN_REGION_SIZE && dimensions.z <= MIN_REGION_SIZE)
		return;

	const vec3f half = dimensions / 2.0f;
	const vec3f center = m_region.m_min + half;

	AABB octant[8] = {
		{m_region.m_min, center},
		{{center.x, m_region.m_min.y, m_region.m_min.z}, {m_region.m_max.x, center.y, center.z}},
		{{center.x, m_region.m_min.y, center.z}, {m_region.m_max.x, center.y, m_region.m_max.z}},
		{{m_region.m_min.x, m_region.m_min.y, center.z}, {center.x, center.y, m_region.m_max.z}},
		{{m_region.m_min.x, center.y, m_region.m_min.z}, {center.x, m_region.m_max.y, center.z}},
		{{center.x, center.y, m_region.m_min.z}, {m_region.m_max.x, m_region.m_max.y, center.z}},
		{center, m_region.m_max},
		{{m_region.m_min.x, center.y, center.z}, {center.x, m_region.m_max.y, m_region.m_max.z}},
	};

	std::vector<ACollider*> octList[8];
	std::vector<ACollider*> delist;

	for (ACollider* obj : m_objects)
	{
		for (int i = 0; i < 8; i++)
		{
			if (m_collisionSystem->Contains(&octant[i], obj) == ContainmentType::Contains)
			{
				octList[i].push_back(obj);
				delist.push_back(obj);
				break;
			}
		}
	}

	for (ACollider* obj : delist)
		m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), obj), m_objects.end());

	for (int i = 0; i < 8; i++)
	{
		if (octList[i].size() > 0)
		{
			m_childs[i] = CreateNode(octant[i], octList[i]);
			m_activeChilds |= (unsigned char)(1 << i);
			m_childs[i]->BuildTree();
		}
	}
	m_treeBuilt = true;
	m_treeReady = true;
}

void Octree::Insert(ACollider* p_item)
{
	if (m_objects.size() <= 1 && m_activeChilds == 0)
	{
		m_objects.push_back(p_item);
		return;
	}

	const vec3f dimensions = m_region.m_max - m_region.m_min;
	if (dimensions.x <= MIN_REGION_SIZE && dimensions.y <= MIN_REGION_SIZE && dimensions.z <= MIN_REGION_SIZE)
	{
		m_objects.push_back(p_item);
		return;
	}

	const vec3f half = dimensions / 2.0f;
	const vec3f center = m_region.m_min + half;

	AABB childOctant[8];
	childOctant[0] = (m_childs[0] != nullptr) ? m_childs[0]->m_region : AABB(m_region.m_min, center);
	childOctant[1] = (m_childs[1] != nullptr) ?
						m_childs[1]->m_region :
						AABB({center.x, m_region.m_min.y, m_region.m_min.z}, {m_region.m_max.x, center.y, center.z});
	childOctant[2] = (m_childs[2] != nullptr) ?
						m_childs[2]->m_region :
						AABB({center.x, m_region.m_min.y, center.z}, {m_region.m_max.x, center.y, m_region.m_max.z});
	childOctant[3] = (m_childs[3] != nullptr) ?
						m_childs[3]->m_region :
						AABB({m_region.m_min.x, m_region.m_min.y, center.z}, {center.x, center.y, m_region.m_max.z});
	childOctant[4] = (m_childs[4] != nullptr) ?
						m_childs[4]->m_region :
						AABB({m_region.m_min.x, center.y, m_region.m_min.z}, {center.x, m_region.m_max.y, center.z});
	childOctant[5] = (m_childs[5] != nullptr) ?
						m_childs[5]->m_region :
						AABB({center.x, center.y, m_region.m_min.z}, {m_region.m_max.x, m_region.m_max.y, center.z});
	childOctant[6] = (m_childs[6] != nullptr) ? m_childs[6]->m_region : AABB(center, m_region.m_max);
	childOctant[7] = (m_childs[7] != nullptr) ?
						m_childs[7]->m_region :
						AABB({m_region.m_min.x, center.y, center.z}, {center.x, m_region.m_max.y, m_region.m_max.z});

	//First, is the item completely contained within the root bounding box?
	//note2: I shouldn't actually have to compensate for this. If an object is out of our predefined bounds, then we have a problem/error.
	// Wrong. Our initial bounding box for the terrain is constricting its height to the highest peak. Flying units will be above that.
	// Fix: I resized the enclosing box to 256x256x256. This should be sufficient.
	if (m_collisionSystem->Contains(&m_region, p_item) == ContainmentType::Contains)
	{
		bool found = false;
		//we will try to place the object into a child node. If we can't fit it in a child node, then we insert it into the current node object list.
		for (int i = 0; i < 8; i++)
		{
			//is the object fully contained within a quadrant?
			if (m_collisionSystem->Contains(&childOctant[i],p_item) == ContainmentType::Contains)
			{
				if (m_childs[i] != nullptr)
					m_childs[i]->Insert(p_item); //Add the item into that tree and let the child tree figure out what to do with it
				else
				{
					m_childs[i] = CreateNode(childOctant[i], p_item); //create a new tree node with the item
					m_activeChilds |= (unsigned char)(1 << i);
				}
				found = true;
			}
		}
		if (!found)
			m_objects.push_back(p_item);
	}
	else
	{
		//either the item lies outside of the enclosed bounding box or it is intersecting it. Either way, we need to rebuild
		//the entire tree by enlarging the containing bounding box
		//BoundingBox enclosingArea = FindBox();
		BuildTree();
	}
}

Octree* Octree::CreateNode(const AABB& p_region, const std::vector<ACollider*>& p_objectList)
{
	if (p_objectList.size() == 0)
		return nullptr;
	Octree* ret = new Octree(p_region, p_objectList);
	ret->m_parent = this;
	return ret;
}

Octree* Octree::CreateNode(const AABB& p_region, ACollider* p_item)
{
	std::vector<ACollider*> objectList;
	objectList.push_back(p_item);
	Octree* ret = new Octree(p_region, objectList);
	ret->m_parent = this;
	return ret;
}

void Octree::DeleteNodes()
{
	if (m_activeChilds == 0)
		return;
	for (int flags = m_activeChilds, index = 0; flags > 0; flags >>= 1, index++)
	{
		if ((flags & 1) == 1)
		{
			m_childs[index]->DeleteNodes();
			delete m_childs[index];
			m_activeChilds &= (unsigned char)(0 << index);
		}
	}
}