.. VolMesh documentation master file, created by
   sphinx-quickstart on Thu Jul  4 18:20:14 2024.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to VolMesh's documentation!
===================================

.. toctree::
   :maxdepth: 2
   :caption: Contents:

C++ Classes
-----------

.. doxygenclass:: volmesh::AABB
    :members: AABB, ~AABB, lower, upper, extent, expand, intersects, contains, operator=

.. doxygenclass:: volmesh::Cell
    :members:

.. doxygenclass:: volmesh::HalfEdge
    :members:

.. doxygenclass:: volmesh::HalfFace
    :members:

.. doxygenclass:: volmesh::BaseIndex
    :members:

.. doxygenclass:: volmesh::VertexIndex
    :members:

.. doxygenclass:: volmesh::HalfEdgeIndex
    :members:

.. doxygenclass:: volmesh::HalfFaceIndex
    :members:

.. doxygenclass:: volmesh::CellIndex
    :members:

.. doxygenclass:: volmesh::MergeList
    :members:

.. doxygenclass:: volmesh::SignedDistanceField
    :members:

.. doxygenclass:: volmesh::SurfaceMesh
    :members:

.. doxygenclass:: volmesh::TetMesh
    :members:

.. doxygenclass:: volmesh::Tetrahedra
    :members:

.. doxygenclass:: volmesh::TriangleMesh
    :members:

.. doxygenclass:: volmesh::VolMesh
    :members:

.. doxygenclass:: volmesh::Voxel
    :members:


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
