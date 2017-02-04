#pragma once
#include <BWAPI.h>

using namespace BWAPI;

class BuildingHandle;

class TileInfos
{
public:
  TileInfos(TilePosition pos);
  ~TileInfos();

  bool  isBuildable() const;
  bool  isPath() const;
  bool  isMineralPath() const;

  float getMilitaryPower() const;
  float getEconomicPower() const;
  float getEnemyEconomicPower() const;
  float getEnemyMilitaryPower() const;

  BuildingHandle *getBuilding() const;

  TilePosition  getPos() const;
  TilePosition  getBiggestBuildingPossible() const;

  TileInfos  *setPath(bool path);
  TileInfos  *setBuildable(bool built);
  TileInfos  *setMineralPath(bool path);

  TileInfos  *addMilitaryPower(float added);
  TileInfos  *addEconomicPower(float added);
  TileInfos  *addEnemyMilitaryPower(float added);
  TileInfos  *addEnemyEconomicPower(float added);

  TileInfos  *setBiggestBuildingPossible(TilePosition pos);
  TileInfos  *setBiggestBuildingPossible(int x, int y);

  TileInfos *setBuilding(BuildingHandle *handle);

private:
  TilePosition  _biggestBuildingPossible;
  TilePosition  _biggestBuildingWithoutPath;
  TilePosition  _pos;
  BuildingHandle *_building;

  bool  _isBuildable;
  bool  _isPath;
  bool  _isMineralPath;

  float _militaryPower;
  float _economicPower;
  float _militaryPowerEnemy;
  float _economicPowerEnemy;
};
