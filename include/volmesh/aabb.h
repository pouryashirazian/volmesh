#pragma once

#include "volmesh/basetypes.h"

namespace volmesh {

/**
 * @class AABB
 * @brief Axis-Aligned Bounding Box (AABB) class.
 *
 * Represents a 3D axis-aligned bounding box defined by two corners, lower and upper.
 */
class AABB {
public:
  /**
   * @brief Default constructor for the AABB class.
   * Initializes the bounding box with zero-sized boundaries.
   */
  AABB();

  /**
   * @brief Parameterized constructor.
   * Initializes the bounding box with the specified lower and upper corners.
   *
   * @param lower A 3D vector representing the lower corner of the bounding box.
   * @param upper A 3D vector representing the upper corner of the bounding box.
   */
  AABB(const vec3& lower, const vec3& upper);

  /**
   * @brief Copy constructor.
   * Initializes the bounding box with values from another AABB.
   *
   * @param rhs The AABB object to copy from.
   */
  AABB(const AABB& rhs);

  /**
   * @brief Destructor.
   * Cleans up the AABB object.
   */
  ~AABB();

  /**
   * @brief Get the lower corner of the bounding box.
   *
   * @return A 3D vector representing the lower corner.
   */
  vec3 lower() const;

  /**
   * @brief Get the upper corner of the bounding box.
   *
   * @return A 3D vector representing the upper corner.
   */
  vec3 upper() const;

  /**
   * @brief Get the extent of the bounding box.
   *
   * @return A 3D vector representing the size (extent) of the bounding box in each dimension.
   */
  vec3 extent() const;

  /**
   * @brief Expands the bounding box along the major axes by the specified amounts.
   *
   * @param dx Amount to expand along the X axis.
   * @param dy Amount to expand along the Y axis.
   * @param dz Amount to expand along the Z axis.
   */
  void expand(const real_t dx, const real_t dy, const real_t dz);

  /**
   * @brief Expands the bounding box by a 3D vector.
   * Expands the bounding box by the components of the vector in the X, Y, and Z directions.
   *
   * @param expansion A 3D vector specifying the expansion amounts along each axis.
   */
  void expand(const vec3& expansion);

  /**
   * @brief Checks if this bounding box intersects with another AABB.
   *
   * @param rhs The AABB to check for intersection with.
   *
   * @return True if the bounding boxes intersect, false otherwise.
   */
  bool intersects(const AABB& rhs) const;

  /**
   * @brief Checks if the bounding box contains a given point.
   *
   * @param p The point to check, represented as a 3D vector.
   *
   * @return True if the point is inside the bounding box, false otherwise.
   */
  bool contains(const vec3& p) const;

  /**
   * @brief Assignment operator.
   * Copies the values from another AABB to this one.
   *
   * @param rhs The AABB to assign from.
   *
   * @return A reference to this AABB.
   */
  AABB& operator=(const AABB& rhs);

private:
  vec3 lower_ = {0.0, 0.0, 0.0};
  vec3 upper_ = {0.0, 0.0, 0.0};
};

}