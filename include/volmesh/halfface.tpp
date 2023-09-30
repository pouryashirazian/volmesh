template <int NumEdges>
HalfFace<NumEdges>::HalfFace(const HalfFace& rhs):hedges_(rhs.hedges_) {
}

template <int NumEdges>
HalfFace<NumEdges>::HalfFace(const std::array<HalfEdgeKey, NumEdges>& in_hedges):hedges_(in_hedges) {

}

template <int NumEdges>
HalfFace<NumEdges>::~HalfFace() {
}

template <int NumEdges>
void HalfFace<NumEdges>::copyFrom(const HalfFace& rhs) {
  hedges_ = rhs.hedges_;
}

template <int NumEdges>
const HalfEdgeKey& HalfFace<NumEdges>::edge(const int i) const {
  return hedges_[i];
}

template <int NumEdges>
int HalfFace<NumEdges>::numEdges() {
  return NumEdges;
}

template <int NumEdges>
HalfFaceKey HalfFace<NumEdges>::key() const {
  std::array<uint64_t, NumEdges> hedges_u64;
  for(int i=0; i < NumEdges; i++) {
    hedges_u64[i] = hedges_[i].get();
  }
  return HalfFaceKey::create(MultiKeyHash<NumEdges, uint64_t>()(hedges_u64));
}

template <int NumEdges>
bool HalfFace<NumEdges>::equals(const HalfFace& rhs) const {
  bool result = true;
  for(int i=0; i < NumEdges; i++) {
    if(hedges_[i] != rhs.hedges_[i]) {
      result = false;
      break;
    }
  }

  return result;
}

template <int NumEdges>
const HalfFace<NumEdges>& HalfFace<NumEdges>::operator=(const HalfFace& rhs) {
  copyFrom(rhs);
  return *this;
}