# volmesh
**volmesh** is a C++ and Python library for producing volumetric meshes from simple 3D surface meshes.

## How volmesh works?
**volmesh** converts a 3D surface mesh to a corresponding signed distance field (SDF) representation. Later, it extracts a tetrahedral or hexahedral volumetric mesh from that SDF data. Once in volume form, the surface and volume facets are accessible via a half-edge data structure.

## Applications
**volmesh** is designed for high-fidelity physically-based simulation of soft tissues on the GPUs. Some of the main applications of that are as follows:
soft tissue registration of pre-operative CT/MR scans
soft tissue tracking
games

![build status](https://github.com/pouryashirazian/volmesh/actions/workflows/cmake-multi-platform.yml/badge.svg)