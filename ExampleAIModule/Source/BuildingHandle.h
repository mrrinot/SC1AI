#pragma once
#include <BWAPI.h>
#include "UnitInfos.h"

using namespace BWAPI;
class BuildingHandle
{
public:
  enum e_Building_State
  {
    IDLE = 0,
    BUILDING,
    UNBORN,
    TRAINING,
    RESEARCH,
    LIFT,
    ERROR
  };

public:
  BuildingHandle(TilePosition pos, Unit builder);
  ~BuildingHandle();

  BuildingHandle  *setUnit(Unit unit);
  BuildingHandle  *setAssignedBuilder(Unit builder);
  BuildingHandle  *setState(e_Building_State state);

  e_Building_State getState() const;
  Unit  getAssignedBuilder() const;
  Unit  getUnit() const;
  TilePosition  getPosition() const;
  UnitInfos     *getInfos() const;

private:
  e_Building_State  _state;
  TilePosition _position;
  UnitInfos     *_infos;
  Unit  _unit;
  Unit  _assignedBuilder;
};