#pragma once

#include "volmesh/halfface.h"

#include <stdexcept>

namespace volmesh {

  template <int NumFaces = 4>
  class Cell {
  public:
    typedef std::array<HalfFaceKey, NumFaces> HalfFaceArray;

    Cell(const Cell& rhs);
    explicit Cell(const HalfFaceArray& in_hfaces);
    ~Cell();

    const HalfFaceKey& face(const int i) const;

    CellKey key() const;

    bool equals(const Cell& rhs) const;

  private:
    HalfFaceArray hfaces_;
  };

  #include "volmesh/cell.tpp"

}