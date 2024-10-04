//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#pragma once

#include "volmesh/halfface.h"

#include <stdexcept>

namespace volmesh {

/*!
 * \class Cell
 * \brief A templated class representing a 3D cell composed of a fixed number of faces.
 *
 * The Cell class represents a geometric cell, storing references to its constituent half-faces.
 * The number of faces is determined by the template parameter \p NumFaces.
 *
 * \tparam NumFaces The number of half-faces associated with the cell.
 */
template <int NumFaces>
class Cell {
public:
  /*!
    * \typedef HalfFaceIndexArray
    * \brief An array of HalfFaceIndex elements with size determined by \p NumFaces.
    */
  typedef std::array<HalfFaceIndex, NumFaces> HalfFaceIndexArray;

  /*!
    * \brief Default constructor.
    * Initializes the cell with default half-face indices.
    */
  Cell();

  /*!
    * \brief Copy constructor.
    * Initializes the cell with the values from another cell.
    *
    * \param rhs The cell to copy from.
    */
  Cell(const Cell& rhs);

  /*!
    * \brief Constructor that initializes the cell with a specified array of half-face indices.
    *
    * \param in_hfaces An array of half-face indices used to initialize the cell.
    */
  explicit Cell(const HalfFaceIndexArray& in_hfaces);

  /*!
    * \brief Destructor.
    * Cleans up any resources held by the cell.
    */
  ~Cell();

  /*!
    * \brief Retrieves the half-face index at the specified position.
    *
    * \param i The index of the half-face to retrieve.
    * \return A constant reference to the half-face index at position \p i.
    */
  const HalfFaceIndex& halfFaceIndex(const int i) const;

  /*!
    * \brief Returns the number of faces (half-faces) associated with the cell.
    *
    * \return The number of faces, which is determined by the template parameter \p NumFaces.
    */
  static int numFaces();

  /*!
    * \brief Checks if this cell is equal to another cell.
    *
    * \param rhs The cell to compare against.
    * \return \c true if the two cells are equal, \c false otherwise.
    */
  bool equals(const Cell& rhs) const;

  /*!
    * \brief Assignment operator.
    * Assigns the values of one cell to this cell.
    *
    * \param rhs The cell to assign from.
    * \return A constant reference to this cell.
    */
  const Cell& operator=(const Cell& rhs);

private:
  /*!
    * \brief Array storing the half-face indices of the cell.
    *
    * The size of the array is determined by the template parameter \p NumFaces.
    */
  HalfFaceIndexArray half_face_indices_;
};

#include "volmesh/cell.tpp"

}