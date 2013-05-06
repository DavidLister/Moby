/****************************************************************************
 * Copyright 2005 Evan Drumwright
 * This library is distributed under the terms of the GNU Lesser General Public 
 * License (found in COPYING).
 ****************************************************************************/

#ifndef _COLLISION_GEOMETRY_H
#define _COLLISION_GEOMETRY_H

#include <stack>
#include <list>
#include <vector>
#include <map>
#include <Ravelin/Pose3d.h>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <Moby/Constants.h>
#include <Moby/Base.h>
#include <Moby/Triangle.h>
#include <Moby/Primitive.h>

namespace Moby {

class SingleBody;  

/// Defines collision geometry that may be used (in principle) many ways: for rigid bodies, non-rigid bodies, ...
/**
 *  In principle the geometry may be very complex, and may support
 *  nonstationarity (changing over time) or deformities (e.g., due to 
 *  collision).  Note that while the underlying geometry may be shared, it is
 *  not intended for CollisionGeometry objects to be shared.
 */
class CollisionGeometry : public virtual Base
{
  public:
    CollisionGeometry();
    void set_pose(const Ravelin::Pose3d& pose);  
    void write_vrml(const std::string& filename) const;
    PrimitivePtr set_geometry(PrimitivePtr primitive);
    void set_rel_pose(const Ravelin::Pose3d& transform, bool update_global_transform = false);
    virtual void save_to_xml(XMLTreePtr node, std::list<boost::shared_ptr<const Base> >& shared_objects) const;
    virtual void load_from_xml(boost::shared_ptr<const XMLTree> node, std::map<std::string, BasePtr>& id_map);

    template <class OutputIterator>
    OutputIterator get_sub_geometries(OutputIterator begin) const;

    /// Gets the parent of this CollisionGeometry (or NULL if there is no parent)
    boost::shared_ptr<CollisionGeometry> get_parent() const { return (_parent.expired()) ? CollisionGeometryPtr() : CollisionGeometryPtr(_parent); }
    
    /// Sets the parent of this CollisionGeometry (or NULL to indicate no parent)
    void set_parent(boost::weak_ptr<CollisionGeometry> parent) { _parent = parent; }

    /// Gets the transform (in global coordinates) for this CollisionGeometry
    boost::shared_ptr<const Ravelin::Pose3d> get_pose() const { return _F; }

    /// Gets the relative transform for this CollisionGeometry
    boost::shared_ptr<const Ravelin::Pose3d> get_rel_pose() const { return _relF; }
    
    /// Gets the single body associated with this CollisionGeometry (if any)
    boost::shared_ptr<SingleBody> get_single_body() const { return (_single_body.expired()) ? SingleBodyPtr() : SingleBodyPtr(_single_body); }

    /// Sets the single body associated with this CollisionGeometry (if any)
    void set_single_body(boost::shared_ptr<SingleBody> s) { _single_body = s; }
    
    /// Gets the geometry for this primitive
    PrimitivePtr get_geometry() const { return _geometry; }

  protected:
    /// The adjusted (i.e., relative transform considered) transform of the CollisionGeometry
    boost::shared_ptr<Ravelin::Pose3d> _F;
  
    /// The relative transform to the CollisionGeometry frame
    boost::shared_ptr<Ravelin::Pose3d> _relF;

    /// The underlying geometry
    PrimitivePtr _geometry;

  private:
    boost::weak_ptr<SingleBody> _single_body;
    boost::weak_ptr<CollisionGeometry> _parent;
}; // end class

} // end namespace

#endif
