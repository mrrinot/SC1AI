#pragma once
#include "BWAPI.h"
#include "UnitInfos.h"
#include "AnyType.h"
#include <map>

using namespace BWAPI;
class HeuristicManager
{
public:
  static HeuristicManager &getInstance()
  {
    static HeuristicManager _instance;
    return _instance;
  }

  HeuristicManager(const HeuristicManager &) = delete;
  HeuristicManager &operator=(const HeuristicManager &) = delete;

  UnitInfos *getInfos(UnitType type);
  AnyType   getBestHeuristic();
  void      updateHeuristic();

  void  OnEnemyUnitShow(AnyType type);
  void  OnEnemyUnitDestroyed(AnyType unit);

  void  OnResearchTech(AnyType tech);
  void  OnAllyUnitBorn(AnyType unit);
  void  OnAllyUnitDeath(AnyType unit);

private:
  HeuristicManager();
  ~HeuristicManager();

  HeuristicManager *addAllyImpactValue(AnyType created, AnyType affected, float value); // Used to add or remove VALUE for each unit of CREATED type to AFFECTED type global heuristic
  HeuristicManager *addEnemyImpactValue(AnyType created, AnyType affected, float value); // Same but for enemy units when sighteds

  static HeuristicManager _instance;

  std::map<UnitType, UnitInfos *> _heuristicUnitMapInfos; // Heuristic values for mapping purpose

  std::map<AnyType, float>  _globalheuristicMap; // Heuristic values for decisions making in creating units/buildings and researching techs

  std::map<AnyType, std::map<AnyType, float>> _enemyMap; // Heuristics changes when a unit is discovered or dies
  std::map<AnyType, std::map<AnyType, float>> _allyMap; // Heuristics changes when a unit is created or dies
};