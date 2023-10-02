template <int NumEdges>
HalfFace<NumEdges>::HalfFace() {
  for(int i=0; i < NumEdges; i++) {
    half_edge_indices_[i] = HalfEdgeIndex::create(kSentinelIndex);
  }
}

template <int NumEdges>
HalfFace<NumEdges>::HalfFace(const HalfFace& rhs):half_edge_indices_(rhs.half_edge_indices_) {
}

template <int NumEdges>
HalfFace<NumEdges>::HalfFace(const std::array<HalfEdgeIndex, NumEdges>& in_hedges):half_edge_indices_(in_hedges) {

}

template <int NumEdges>
HalfFace<NumEdges>::~HalfFace() {
}

template <int NumEdges>
void HalfFace<NumEdges>::copyFrom(const HalfFace& rhs) {
  half_edge_indices_ = rhs.half_edge_indices_;
}

template <int NumEdges>
const HalfEdgeIndex& HalfFace<NumEdges>::halfEdgeIndex(const int i) const {
  return half_edge_indices_[i];
}

template <int NumEdges>
int HalfFace<NumEdges>::numEdges() {
  return NumEdges;
}

template <int NumEdges>
bool HalfFace<NumEdges>::equals(const HalfFace& rhs) const {
  bool result = true;
  for(int i=0; i < NumEdges; i++) {
    if(half_edge_indices_[i] != rhs.half_edge_indices_[i]) {
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