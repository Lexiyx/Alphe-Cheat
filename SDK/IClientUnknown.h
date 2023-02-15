#pragma once
#include "IClientNetworkable.h"
#include "IClientRenderable.h"
#include "IClientThinkable.h"
#include "CHandle.h"

class IClientAlphaProperty;
class C_BaseEntity;
class IClientEntity;
struct ray_t_t {
	Vector m_start; // starting point, centered within the extents
	Vector m_delta; // direction + length of the ray
	Vector m_start_offset; // Add this to m_Start to get the actual ray start
	Vector m_extents; // Describes an axis aligned box extruded along a ray
	const matrix3x4_t* m_world_axis_transform;
	//const matrix_t *m_pWorldAxisTransform;
	bool m_is_ray; // are the extents zero?
	bool m_is_swept; // is delta != 0?

	void initialize(const Vector& start, const Vector& end) {
		m_delta = end - start;

		m_is_swept = (m_delta.LengthSqr() != 0);

		m_extents.x = m_extents.y = m_extents.z = 0.0f;
		m_is_ray = true;

		m_start_offset.x = m_start_offset.y = m_start_offset.z = 0.0f;

		m_start = start;
	}

	void initialize(Vector& vecStart, Vector& vecEnd, Vector min, Vector max) {
		m_delta = vecEnd - vecStart;

		m_is_swept = (m_delta.LengthSqr() != 0);

		m_extents.x = (max.x - min.x);
		m_extents.y = (max.y - min.y);
		m_extents.z = (max.z - min.z);
		m_is_ray = false;

		m_start_offset.x = m_start_offset.y = m_start_offset.z = 0.0f;

		m_start = vecStart + ((max + min) * 0.5f);
	}

private:
};
class ICollideable
{
public:
	virtual IHandleEntity* GetEntityHandle() = 0;
	virtual Vector& OBBMins() const = 0;
	virtual Vector& OBBMaxs() const = 0;
	virtual void					WorldSpaceTriggerBounds(Vector* pVecWorldMins, Vector* pVecWorldMaxs) const = 0;
	virtual bool					TestCollision(const ray_t_t& ray, unsigned int fContentsMask, ray_t_t& tr) = 0;
	virtual bool					TestHitboxes(const ray_t_t& ray, unsigned int fContentsMask, ray_t_t& tr) = 0;
	virtual int						GetCollisionModelIndex() = 0;
	virtual const model_t* GetCollisionModel() = 0;
	virtual Vector& GetCollisionOrigin() const = 0;
	virtual Vector& GetCollisionAngles() const = 0;
	virtual const matrix3x4_t& CollisionToWorldTransform() const = 0;
	virtual int				GetSolid() const = 0;
	virtual int						GetSolidFlags() const = 0;
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual int						GetCollisionGroup() const = 0;

	/*
	 * @note: should be equivalent to C_BaseAnimating::ComputeHitboxSurroundingBox
	 * DOESNT NEEDED TRANSORMATION!
	 * and bugged when trying to get non-player entity box
	 */
	virtual void					WorldSpaceSurroundingBounds(Vector* pVecMins, Vector* pVecMaxs) = 0;
	virtual unsigned int			GetRequiredTriggerFlags() const = 0;
	virtual const matrix3x4_t* GetRootParentToWorldTransform() const = 0;
	virtual void* GetVPhysicsObject() const = 0;
};

class IClientUnknown : public IHandleEntity
{
public:
	virtual ICollideable* GetCollideable() = 0;
	virtual IClientNetworkable* GetClientNetworkable() = 0;
	virtual IClientRenderable* GetClientRenderable() = 0;
	virtual IClientEntity* GetIClientEntity() = 0;
	virtual C_BaseEntity* GetBaseEntity() = 0;
	virtual IClientThinkable* GetClientThinkable() = 0;
	//virtual IClientModelRenderable*  GetClientModelRenderable() = 0;
	virtual IClientAlphaProperty* GetClientAlphaProperty() = 0;
};