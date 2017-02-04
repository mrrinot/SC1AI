#pragma once

#include <BWAPI.h>
#include <map>
#include "BuildingHandle.h"

using namespace BWAPI;
using namespace Filter;
class BuildingManager
{
public:
  BuildingManager();
  ~BuildingManager();

  BuildingManager *addBuildingFundation(UnitType type, Unit builder, TilePosition pos);
  BuildingManager *addBuildingFundation(UnitType type, Unit builder, int x, int y);

  BuildingManager *buildAddon(UnitType type, Unit building, TilePosition pos);
  BuildingManager *buildAddon(UnitType type, Unit building, int x, int y);

  std::vector<BuildingHandle *> getAllBuildingOfTypeWithState(UnitType type, BuildingHandle::e_Building_State state);
  std::vector<BuildingHandle *> getAllBuildingWithState(BuildingHandle::e_Building_State state);
  std::vector<BuildingHandle *> &getAllBuildingOfType(UnitType type);
  BuildingHandle *getBuilding(TilePosition pos);

  BuildingManager *setBuilder(Unit builder, TilePosition pos);
  BuildingManager *setBuilding(Unit building, TilePosition pos);

  bool  removeBuilding(BuildingHandle *building);
  bool  eraseBuilding(BuildingHandle *building);

private:
  std::map<UnitType, std::vector<BuildingHandle *>> _buildingMap;
};