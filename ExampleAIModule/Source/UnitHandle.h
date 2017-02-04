#pragma once
#include "BWAPI.h"
#include "UnitInfos.h"

using namespace BWAPI;
class UnitHandle
{
public:
  UnitHandle(Unit unit);
  ~UnitHandle();

  Unit  getUnit() const;
  UnitInfos *getUnitInfos() const;
  void  updateState();
  int   getPreviousHP() const;
  TilePosition  getPreviousPos() const;

private:
  Unit      _unit;
  UnitInfos *_infos;
  int           _previousHP;
  TilePosition  _previousPos;
  // Mission *mission, COMING SOON
};