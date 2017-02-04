#pragma once
#include <BWAPI.h>
#include "MineralManager.h"
#include "SCVManager.h"
#include "TileManager.h"
#include "BuildingManager.h"
#include "UnitManager.h"
#include "EnemyManager.h"

using namespace BWAPI;
using namespace Filter;

class RinotAI : public BWAPI::AIModule
{
public:

  virtual void onStart();
  virtual void onEnd(bool isWinner);
  virtual void onFrame();

  virtual void onUnitCreate(Unit unit);
  virtual void onUnitDestroy(Unit unit);
  virtual void onUnitComplete(Unit unit);
  virtual void onUnitHide(Unit unit);
  virtual void onUnitShow(Unit unit);
  virtual void onUnitMorph(Unit unit);

private: 
  void  onFirstFrame();
  void  manageMineralField();
  void  updateInfos();
  void  manageSupply();
  void  manageKeyStates();

private:
  bool isFirstFrame;
  Key _key;
  int _maxSupply;
  int _currentSupply;

  SCVManager _scvManager;
  MineralManager _mineralManager;
  TileManager *_tileManager;
  BuildingManager _buildingManager;
  UnitManager _unitManager;
  EnemyManager _enemyManager;
  
  std::vector<Unit> GasVector;
};
