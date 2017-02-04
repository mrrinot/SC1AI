#pragma once
#include "BWAPI.h"
#include "BuildingHandle.h"
#include "UnitHandle.h"
#include "AnyType.h"

class EnemyManager
{
public:
  EnemyManager();
  ~EnemyManager();

  void  hideUnit(Unit unit);
  bool  showUnit(Unit unit);
  void  destroyUnit(Unit unit);
  void  updatePreviousStates();
  const std::map<UnitType, std::vector<UnitHandle *>> &getEnemyInSightUnitList() const;
  const std::map<UnitType, std::vector<UnitHandle *>> &getEnemySeenUnitList() const;
  const std::map<UnitType, std::vector<BuildingHandle *>> &getEnemyInSightBuildingList() const;
  const std::map<UnitType, std::vector<BuildingHandle *>> &getEnemySeenBuildingList() const;
  UnitHandle *getEnemyUnit(UnitType type, TilePosition pos);
  BuildingHandle *getEnemyBuilding(UnitType type, TilePosition pos);

private:
  //std::map<AnyType, int> _techLevelMap;
  std::map<UnitType, std::vector<UnitHandle *>> _inSightUnitMap;
  std::map<UnitType, std::vector<BuildingHandle *>> _inSightBuildingMap;
  std::map<UnitType, std::vector<UnitHandle *>> _seenUnitMap;
  std::map<UnitType, std::vector<BuildingHandle *>> _seenBuildingMap;
};