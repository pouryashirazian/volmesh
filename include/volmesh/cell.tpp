template <int NumFaces>
Cell<NumFaces>::Cell(const Cell& rhs):hfaces_(rhs.hfaces_) {

}

template <int NumFaces>
Cell<NumFaces>::Cell(const Cell::HalfFaceArray& in_hfaces):hfaces_(in_hfaces) {

}

template <int NumFaces>
Cell<NumFaces>::~Cell() {

}

template <int NumFaces>
const HalfFaceKey& Cell<NumFaces>::face(const int i) const {
  if(i < NumFaces) {
    return hfaces_[i];
  } else {
    throw std::out_of_range("invalid face index");
  }
}

template <int NumFaces>
int Cell<NumFaces>::numFaces() {
  return NumFaces;
}

template <int NumFaces>
CellKey Cell<NumFaces>::key() const {
  std::array<uint64_t, NumFaces> hfaces_u64;
  for(int i=0; i < NumFaces; i++) {
    hfaces_u64[i] = hfaces_[i].get();
  }

  return CellKey::create(MultiKeyHash<NumFaces, uint64_t>()(hfaces_u64));
}

template <int NumFaces>
bool Cell<NumFaces>::equals(const Cell& rhs) const {
  bool result = true;
  for(int i=0; i < NumFaces; i++) {
    if(hfaces_[i] != rhs.hfaces_[i]) {
      result = false;
      break;
    }
  }

  return result;
}

template <int NumFaces>
const Cell<NumFaces>& Cell<NumFaces>::operator=(const Cell& rhs) {
  this->hfaces_ = rhs.hfaces_;
  return *this;
}

