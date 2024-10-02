# Introduction
**volmesh** is a C++ library for working with 2D and 3D meshes. It provides an efficient in-memory data structure to access geometric entities in both surface and volumetric meshes.

## How volmesh works?
**volmesh** converts a 3D surface mesh to a corresponding signed distance field (SDF) representation. The user can export the SDF to a VTI or MHD image file and then extract the iso-surface as a triangle mesh with [Paraview](https://www.paraview.org).

volmesh uses a half-edge data structure to provide top-down and bottom-up traversal. The top-down traversal in surface meshes proceeds from faces to half-edges to vertices. We currently support triangle surface meshes only.

![Surface mesh top-down traversal](https://github.com/pouryashirazian/volmesh/blob/main/docs/images/surface_topdown_traversal.svg?raw=true)

Similarly, we use a half-face data structure to represent volume meshes. The top-down traversal in volume meshes proceeds from cells to half-faces, half-edges, and finally to vertices.

![Volume mesh top-down traversal](https://github.com/pouryashirazian/volmesh/blob/main/docs/images/volume_topdown_traversal.svg?raw=true)

## How is the SDF computed?
The input to our SDF calculation is a triangle mesh. The system reads the triangle mesh file from the disk and computes a half-edge representation in memory. In the next step, we compute a normal per each triangle face, followed by pseudo normals per each half-edge and vertex of the model. A pseudo normal for a half-edge is the weighted average of the normals associated with the adjacent faces to that half-edge. Similarly, the pseudo normal for each vertex is the weighted average of the normals per incident face. The weight is the incident angle of the triangle face at the associated vertex.

At this point in the algorithm, we have a normal per each face and pseudo normals for half-edges and vertices. The next step is to compute an axis-aligned bounding box for the entire 3D model and overlay a voxel grid with the specified resolution that encompasses the bounding box. We compute the shortest distance between each triangle and the points in the intersecting voxels.

## Applications
**volmesh** is designed for high-fidelity physically-based simulation of soft tissues. Some of the main applications of that are as follows:

- soft-tissue animation
- volume rendering
- surface reconstruction
