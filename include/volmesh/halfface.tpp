template <int NumEdges>
HalfFace<NumEdges>::HalfFace() {

}

template <int NumEdges>
HalfFace<NumEdges>::HalfFace(const HalfFace& rhs) {
  copyFrom(rhs);
}

template <int NumEdges>
void HalfFace<NumEdges>::copyFrom(const HalfFace& rhs) {
  for(int i=0; i < NumEdges; i++) {
    edges_[i] = rhs.edges_[i];
  }
}

template <int NumEdges>
int HalfFace<NumEdges>::numEdges() const {
  return NumEdges;
}

template <int NumEdges>
const HalfEdgeKey& HalfFace<NumEdges>::edge(const int i) const {
  return edges_[i];
}

template <int NumEdges>
const HalfFace<NumEdges>& HalfFace<NumEdges>::operator=(const HalfFace& rhs) {
  copyFrom(rhs);
  return *this;
}