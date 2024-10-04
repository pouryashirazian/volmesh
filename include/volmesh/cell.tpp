//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

template <int NumFaces>
Cell<NumFaces>::Cell() {
  for(int i=0; i < NumFaces; i++) {
    half_face_indices_[i] = HalfFaceIndex::create(kSentinelIndex);
  }
}

template <int NumFaces>
Cell<NumFaces>::Cell(const Cell& rhs):half_face_indices_(rhs.half_face_indices_) {

}

template <int NumFaces>
Cell<NumFaces>::Cell(const Cell::HalfFaceIndexArray& in_hfaces):half_face_indices_(in_hfaces) {

}

template <int NumFaces>
Cell<NumFaces>::~Cell() {

}

template <int NumFaces>
const HalfFaceIndex& Cell<NumFaces>::halfFaceIndex(const int i) const {
  if(i < NumFaces) {
    return half_face_indices_[i];
  } else {
    throw std::out_of_range("invalid half face index");
  }
}

template <int NumFaces>
int Cell<NumFaces>::numFaces() {
  return NumFaces;
}

template <int NumFaces>
bool Cell<NumFaces>::equals(const Cell& rhs) const {
  bool result = true;
  for(int i=0; i < NumFaces; i++) {
    if(half_face_indices_[i] != rhs.half_face_indices_[i]) {
      result = false;
      break;
    }
  }

  return result;
}

template <int NumFaces>
const Cell<NumFaces>& Cell<NumFaces>::operator=(const Cell& rhs) {
  this->half_face_indices_ = rhs.half_face_indices_;
  return *this;
}

