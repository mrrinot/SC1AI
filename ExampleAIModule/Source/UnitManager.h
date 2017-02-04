#pragma once
#include "UnitHandle.h"
#include <map>

class UnitManager
{
public:
  UnitManager();
  ~UnitManager();

  std::vector<UnitHandle *> &getAllAllyUnitsOfType(UnitType type);
  const std::map<UnitType, std::vector<UnitHandle *>> &getAllAllyUnits() const;

  void  addUnit(UnitType type, Unit unit);
  void  removeUnit(UnitType type, Unit unit);
  void  updatePreviousStates();

private:
  std::map<UnitType, std::vector<UnitHandle *>> _allyUnitMap;
};