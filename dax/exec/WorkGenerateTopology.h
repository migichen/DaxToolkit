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

#ifndef __dax_exec_WorkGenerateTopology_h
#define __dax_exec_WorkGenerateTopology_h

#include <dax/Types.h>
#include <dax/exec/Cell.h>
#include <dax/exec/Field.h>
#include <dax/exec/WorkMapCell.h>

#include <dax/exec/internal/FieldAccess.h>

namespace dax {
namespace exec {

///----------------------------------------------------------------------------
/// Worklet that determines how many new cells should be generated
/// from it with the same topology.
/// This worklet is based on the WorkMapCell type so you have access to
/// "CellArray" information i.e. information about what points form a cell.
/// There are different versions for different cell types, which might have
/// different constructors because they identify topology differently.

template<class ICT, class OCT, class ExecutionAdapter>
class WorkGenerateTopology
{
public:
  typedef ICT InputCellType;
  typedef OCT OutputCellType;

  typedef typename InputCellType::template GridStructures<ExecutionAdapter>
      ::TopologyType InputTopologyType;

  typedef typename InputCellType::PointConnectionsType
       InputPointConnectionsType;
  typedef typename OutputCellType::PointConnectionsType
       OutputPointConnectionsType;

private:
  const InputCellType InputCell;
  const dax::Id OutputIndex;
  const dax::exec::FieldOut<dax::Id, ExecutionAdapter> OutputConnectionField;
  const ExecutionAdapter Adapter;
public:

  DAX_EXEC_EXPORT WorkGenerateTopology(
      const InputTopologyType &gridStructure,
      dax::Id inputIndex,
      const dax::exec::FieldOut<dax::Id, ExecutionAdapter> &outConnectionField,
      dax::Id outputIndex,
      const ExecutionAdapter &executionAdapter)
    : InputCell(gridStructure, inputIndex),
      OutputIndex(outputIndex),
      OutputConnectionField(outConnectionField),
      Adapter(executionAdapter)
    { }

  /// Get the topology of the input cell
  DAX_EXEC_EXPORT InputPointConnectionsType GetInputConnections() const
  {
    return this->InputCell.GetPointIndices();
  }

  /// Set the topology of one of the output cells
  DAX_EXEC_EXPORT
  void SetOutputConnections(const OutputPointConnectionsType &connections) const
  {
    DAX_ASSERT_EXEC(OutputCellType::NUM_POINTS
                    == OutputPointConnectionsType::NUM_COMPONENTS,
                    *this);
    const dax::Id connectionIndexOffset =
        this->OutputIndex*OutputCellType::NUM_POINTS;
    for (dax::Id index = 0; index < OutputCellType::NUM_POINTS; index++)
      {
      dax::exec::internal::FieldAccess::SetField(this->OutputConnectionField,
                                                 index + connectionIndexOffset,
                                                 connections[index],
                                                 *this);
      }
  }

  DAX_EXEC_EXPORT dax::Id GetOutputCellIndex() const
  {
  return this->OutputIndex;
  }

  DAX_EXEC_EXPORT void RaiseError(const char* message) const
  {
    this->Adapter.RaiseError(message);
  }
};


}
}

#endif //__dax_exec_WorkGenerateTopology_h