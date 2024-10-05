# Introduction
**volmesh** is a C++ library for working with 2D and 3D meshes. It provides an efficient in-memory data structure to access geometric entities in both surface and volumetric meshes.

## How volmesh works?
**volmesh** converts a 3D surface mesh to a corresponding signed distance field (SDF) representation. The user can export the SDF to a VTK Image data [VTI format](https://docs.vtk.org/en/latest/design_documents/VTKFileFormats.html#imagedata) and then extract the iso-surface as a triangle mesh with [Paraview](https://www.paraview.org).

**volmesh** uses a half-edge data structure to provide top-down and bottom-up traversal. The top-down traversal in surface meshes proceeds from faces to half-edges to vertices (figure 1). We currently support triangle surface meshes only.

| ![Surface mesh top-down traversal](https://github.com/pouryashirazian/volmesh/blob/main/docs/images/surface_topdown_traversal.svg?raw=true&sanitize=true) |
| :--: |
| *Figure 1. Surface mesh top-down traversal* |

Similarly, we use a half-face data structure to represent volume meshes. The top-down traversal in volume meshes proceeds from cells to half-faces, half-edges, and finally to vertices (figure 2).

| ![Volume mesh top-down traversal](https://github.com/pouryashirazian/volmesh/blob/main/docs/images/volume_topdown_traversal.svg?raw=true&sanitize=true) |
| :--: |
| *Figure 2. Volume mesh top-down traversal* |

## How is the SDF computed?
The input to our SDF calculation is a triangle mesh. The system reads the triangle mesh file from the disk and computes a half-edge representation in memory.
We compute the axis-aligned bounding box (AABB) per the vertex list and embed a voxel grid with the specified voxel size to encompass the entire bounding box.
In the next step, we compute a normal per each triangle face, followed by pseudo normals per each half-edge and vertex of the model. A pseudo normal for a half-edge is the weighted average of the normals associated with the adjacent faces to that half-edge. Similarly, the pseudo normal for each vertex is the weighted average of the normals per incident face. The weight is the incident angle of the triangle face at the associated vertex (figure 3).

| ![normal](https://github.com/pouryashirazian/volmesh/blob/main/docs/images/vertex_pseudo_normals.svg?raw=true&sanitize=true) |
| :--: |
| *Figure 3. The pseudo normal per each vertex is the sum of all angle-weighted normals per adjacent faces* |

Next, the system computes the shortest distance from each point in the voxel grid to the triangle mesh. Per each voxel-grid point p, the system computes its intersection point q with the closest triangle feature. The intersection point q can belong to seven different areas of a triangle (figure 4).

| ![point-triangle regions](https://github.com/pouryashirazian/volmesh/blob/main/docs/images/point_triangle_regions.svg?raw=true&sanitize=true) |
| :--: |
| *Figure 4. The point-triangle intersection point can belong to seven different areas of the triangle* |

If the intersection point is inside the triangle (A1), we use the face-normal vector for signing. Otherwise, if it is inside areas 2, 4, or 6, we use the pseudo-normal associated with the half-edge adjacent to the corresponding area. Finally, if it is inside areas 3, 5, or 7, we use the pseudo normal associated with the vertex adjacent to the corresponding area.

To compute the sign, we take the dot product of vector qp (directional vector from the intersection point q to the voxel-grid point p) with the selected pseudo-normal. A positive dot product indicates a positive distance field value and a negative otherwise.

## Applications
**volmesh** is designed for high-fidelity physically-based simulation of soft tissues. Some of the main applications of that are as follows:

- soft-tissue animation
- volume rendering
- surface reconstruction

## How to build?
It is very straightforward to build **volmesh**. Use the supplied `build.sh` script on Unix-based systems to build the library and the apps. This will build a release version of the library on your platform.
```bash
./build.sh
```

If you intend to extend **volmesh** with more features, you can build a debug version of it by supplying the "-b" switch:
```bash
./build.sh -b debug
```
**volmesh** uses a small number of third-party libraries and it manages these with [vcpkg](https://vcpkg.io/en/).

## How to generate SDF?
Use the provided **makesdf** application to generate signed distance fields from triangle meshes.
```bash
cd ~/volmesh/build-darwin-release/apps/makesdf
./makesdf -i ~/Desktop/volmesh_samples/stanford_happy_buddha.stl -o ~/volmesh_samples/stanford_happy_buddha_0.001.vti -v 0.001
```
![Stanford Bunny SDF](https://github.com/pouryashirazian/volmesh/blob/main/docs/images/stanford_bunny_sdf_1920×1291.png?raw=true&sanitize=true)
![Stanford Bunny SDF](https://github.com/pouryashirazian/volmesh/blob/main/docs/images/stanford_happy_buddha_sdf_1920x1080.png?raw=true&sanitize=true)

