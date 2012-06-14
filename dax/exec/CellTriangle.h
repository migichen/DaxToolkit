//=============================================================================
//
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//
//  Copyright 2012 Sandia Corporation.
//  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
//  the U.S. Government retains certain rights in this software.
//
//=============================================================================
#ifndef __dax_exec_CellTriangle_h
#define __dax_exec_CellTriangle_h


#include <dax/Types.h>
#include <dax/exec/internal/TopologyUnstructured.h>

#include <dax/exec/Field.h>

namespace dax { namespace exec {

class CellTriangle
{
public:
  template<class ExecutionAdapter>
  struct GridStructures
  {
    typedef dax::exec::internal::TopologyUnstructured<
        CellTriangle,
        ExecutionAdapter> TopologyType;
  };

  /// static variable that returns the number of points per cell
  const static dax::Id NUM_POINTS = 3;
  typedef dax::Tuple<dax::Id,NUM_POINTS> PointConnectionsType;

private:
  dax::Id CellIndex;
  PointConnectionsType Connections;

  template<class ExecutionAdapter>
  DAX_EXEC_EXPORT static PointConnectionsType GetPointConnections(
      const dax::exec::internal::TopologyUnstructured<
          CellTriangle,ExecutionAdapter> &topology,
      dax::Id cellIndex)
  {
    PointConnectionsType connections;
    typename GridStructures<ExecutionAdapter>::TopologyType
        ::CellConnectionsIteratorType connectionIter
          = topology.CellConnections + cellIndex*NUM_POINTS;
    connections[0] = *(connectionIter);
    connections[1] = *(++connectionIter);
    connections[2] = *(++connectionIter);
    return connections;
  }

public:
  /// Create a cell for the given work.
  template<class ExecutionAdapter>
  DAX_EXEC_EXPORT CellTriangle(
      const dax::exec::internal::TopologyUnstructured<
          CellTriangle,ExecutionAdapter> &topology,
      dax::Id cellIndex)
    : CellIndex(cellIndex),
      Connections(GetPointConnections(topology, cellIndex))
    { }

  /// Get the number of points in the cell.
  DAX_EXEC_EXPORT dax::Id GetNumberOfPoints() const
  {
    return NUM_POINTS;
  }

  /// Given a vertex index for a point (0 to GetNumberOfPoints() - 1), returns
  /// the index for the point in point space.
  DAX_EXEC_EXPORT dax::Id GetPointIndex(const dax::Id vertexIndex) const
  {
    return this->GetPointIndices()[vertexIndex];
  }

  /// returns the indices for all the points in the cell.
  DAX_EXEC_EXPORT PointConnectionsType GetPointIndices() const
  {
    return this->Connections;
  }

  /// Get the cell index.  Probably only useful internally.
  DAX_EXEC_EXPORT dax::Id GetIndex() const { return this->CellIndex; }
};

}}
#endif // __dax_exec_CellTriangle_h