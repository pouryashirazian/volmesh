template <int NumFaces>
Cell<NumFaces>::Cell() {

}

template <int NumFaces>
Cell<NumFaces>::Cell(const Cell& rhs) {

}

template <int NumFaces>
Cell<NumFaces>::Cell(const HalfFaceKey faces_[NumFaces]) {

}

template <int NumFaces>
Cell<NumFaces>::~Cell() {

}

template <int NumFaces>
const HalfFaceKey& Cell<NumFaces>::face(const int i) const {
  if(i < NumFaces) {
    return faces_[i];
  } else {
    throw std::out_of_range("invalid face index");
  }
}
