# volmesh
volmesh is a C++ and Python library for producing volumetric meshes from simple 3D surface meshes

## build instructions
To run the example with Nix, issue the following command:
```
nix-shell --command 'bazel run --config=nix :hello'
```

To run the example without Nix, make sure you have Bazel installed, and issue the following command:
```
bazel run :hello
```
This non-Nix example will look for the C++ toolchain on the PATH.