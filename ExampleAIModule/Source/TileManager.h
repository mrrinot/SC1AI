#pragma once
#include "TileInfos.h"
#include "DynArray.h"
#include "BuildingHandle.h"

class TileManager
{
public:
  TileManager(int width, int height);
  ~TileManager();

  TilePosition getBestBuildingLocation(UnitType type, TilePosition start);
  TilePosition getBestBuildingLocation(UnitType type, int xStart, int yStart);

  void  setTileInfos(TileInfos *infos, int x, int y);
  void  setTileInfos(TileInfos *infos, TilePosition pos);

  TileInfos *getTileInfos(int x, int y) const;
  TileInfos *getTileInfos(TilePosition pos) const;

  void  setBuilding(UnitType type, BuildingHandle *building, TilePosition pos, bool isEnemy);
  void  destroyBuilding(UnitType type, TilePosition pos, bool isEnemy);
  void  modifHeuristicUnit(UnitType type, TilePosition pos, bool isDead, bool isEnemy);
  void  modifHeuristicBuilding(UnitType type, TilePosition pos, bool isDead, bool isEnemy);

  void  detectMineralPath(BuildingHandle *cc, TilePosition mineralField);
  void  updateDisplay(Key key);
  void  updateAllTilesInfos();

public:
  const static int BUILDING_FINDING_MAX_RANGE = 5;

private:
  TilePosition  calculateBiggestBuilding(int x, int y);
  void          calculateBiggestBuildingForPosAndType(UnitType type, TilePosition pos);
  void          updateTile(UnitInfos *infos, TilePosition pos, std::vector<TilePosition> &list, bool isDead, bool isEnemy, int step, TilePosition origin);
  TilePosition  checkTile(TilePosition size, TilePosition pos, std::vector<TilePosition> &list, int step);

private:
  int _mapHeight;
  int _mapWidth;
  DynArray<TileInfos *> _tileMap;
};

struct s_tilePathfinding
{
  s_tilePathfinding(TilePosition pos, double dist, int step, s_tilePathfinding *ptr)
  : pos(pos), distanceToObj(dist), step(step), parent(ptr)
  {
  }
  TilePosition pos;
  double distanceToObj;
  int step;
  s_tilePathfinding *parent;

};
