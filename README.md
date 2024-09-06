# Introduction
**volmesh** is a C++ and Python library for working with surface and volumetric meshes. It provides mechanisms to generate volume meshes from simple triangle meshes. The volume meshes are high quality (no skinny wedges) and ready for physically based simulations.

## How volmesh works?
**volmesh** converts a 3D surface mesh to a corresponding signed distance field (SDF) representation. Later, it extracts a tetrahedral mesh from SDF. volmesh uses a half-edge data structure to provide top-down and bottom-up traversal. The top-down traversal provides access from faces to half-edges to vertices. Similarly, we use a half-face data structure to represent tetrahedral volume meshes. The top-down traversal in volume meshes starts from cells.

## How is the SDF computed?
The input to our SDF calculation is a triangle mesh. The system reads the triangle mesh file from the disk and computes a half-edge representation in memory. In the next step, we compute a normal per each triangle face, followed by pseudo normals per each half-edge and vertex of the model. A pseudo normal for a half-edge is the weighted average of the normals associated with the adjacent faces to that half-edge. Similarly, the pseudo normal for each vertex is the weighted average of the normals per incident face. The weight is the incident angle of the triangle face at the associated vertex.

At this point in the algorithm, we have a normal per each face and pseudo normals for half-edges and vertices. The next step is to compute an axis-aligned bounding box for the entire 3D model and overlay a voxel grid with the specified resolution that encompasses the bounding box. We compute the shortest distance between each triangle and the points in the intersecting voxels.

## Applications
**volmesh** is designed for high-fidelity physically-based simulation of soft tissues. Some of the main applications of that are as follows:
- soft-tissue animation
- volume rendering
