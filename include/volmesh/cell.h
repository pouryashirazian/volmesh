#pragma once

#include "volmesh/halfface.h"

#include <stdexcept>

namespace volmesh {

  template <int NumFaces = 4>
  class Cell {
  public:
    Cell();
    explicit Cell(const Cell& rhs);
    explicit Cell(const HalfFaceKey faces_[NumFaces]);
    ~Cell();

    const HalfFaceKey& face(const int i) const;


  private:
    HalfFaceKey faces_[NumFaces];
  };

  #include "volmesh/cell.tpp"

}