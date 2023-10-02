#pragma once

#include "volmesh/halfface.h"

#include <stdexcept>

namespace volmesh {

  template <int NumFaces>
  class Cell {
  public:
    typedef std::array<HalfFaceIndex, NumFaces> HalfFaceIndexArray;

    Cell();
    Cell(const Cell& rhs);
    explicit Cell(const HalfFaceIndexArray& in_hfaces);
    ~Cell();

    const HalfFaceIndex& halfFaceIndex(const int i) const;

    static int numFaces();

    bool equals(const Cell& rhs) const;

    const Cell& operator=(const Cell& rhs);

  private:
    HalfFaceIndexArray half_face_indices_;
  };

  #include "volmesh/cell.tpp"

}