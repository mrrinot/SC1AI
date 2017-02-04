#include "RinotAI.h"
#include "Logger.hh"
#include <ostream>
#include "HeuristicManager.h"

void RinotAI::onStart()
{
  Logger::setGlobalOptions(LoggerOptions()
    .setProcessor(LoggerCallbacks::WriteToFile("SCIA.log", std::ofstream::out))
    .setAutoLineBreak(true));
  Logger::getLogger().logInfo() << "Start ia !";

  _tileManager = new TileManager(Broodwar->mapWidth(), Broodwar->mapHeight());

  _key = Key::K_RMENU;
  Broodwar->enableFlag(Flag::UserInput);


  isFirstFrame = false;
}

void RinotAI::onEnd(bool isWinner)
{
  delete _tileManager;
}

#pragma region Callback

void RinotAI::onUnitShow(Unit u)
{
  Logger::getLogger().logInfo() << "NEW UNIT SHOW : " << u->getType().toString() << " ID = " << u->getID() << " VISIBLE ? " << u->isVisible();
  if (!u->getType().isMineralField() && u->getType() != UnitTypes::Resource_Vespene_Geyser)
  {
    if (Broodwar->self() != u->getPlayer())
    {
      if (_enemyManager.showUnit(u) && IsBuilding(u) && !IsSpecialBuilding(u))
      {
        HeuristicManager::getInstance().OnEnemyUnitShow(u->getType());
        _tileManager->setBuilding(u->getType(), _enemyManager.getEnemyBuilding(u->getType(), u->getTilePosition()), u->getTilePosition(), true);
        LogInfo() << "NEW BUILDING SHOW, CALCULATE HEURISTICS NEARBY";
      }
    }
  }
}

void RinotAI::onUnitComplete(Unit u)
{
  Logger::getLogger().logInfo() << "NEW UNIT COMPLETE : " << u->getType().toString() << " ID = " << u->getID() << " VISIBLE ? " << u->isVisible();
  if (u->getType().isBuilding() && !u->getType().isMineralField() && !u->getType().isSpecialBuilding() && u->getType() != UnitTypes::Resource_Vespene_Geyser)
  {
    if (Broodwar->self() == u->getPlayer())
    {
      Logger::getLogger().logInfo() << "NEW BUILDING => " << u->getType() << " pos = " << u->getTilePosition();
      _buildingManager.setBuilder(nullptr, u->getTilePosition());
    }
  }
}

void RinotAI::onUnitHide(Unit u) // NO TYPE : Unknown
{
  if (!u->getType().isMineralField() && u->getType() != UnitTypes::Resource_Vespene_Geyser)
  {
    if (Broodwar->self() != u->getPlayer())
    {
      _enemyManager.hideUnit(u);
    }
  }
  Logger::getLogger().logInfo() << "NEW UNIT HIDE : " << u->getType().toString() << " ID = " << u->getID() << " VISIBLE ? " << u->isVisible();
}

void RinotAI::onUnitMorph(Unit u) // REFINERY, SIEGE TANKS
{
  Logger::getLogger().logInfo() << "NEW UNIT MORPH : " << u->getType().toString() << " ID = " << u->getID() << " VISIBLE ? " << u->isVisible();
}

void RinotAI::onUnitCreate(Unit u)
{
  Logger::getLogger().logInfo() << "NEW UNIT CREATE : " << u->getType().toString() << " ID = " << u->getID() << " VISIBLE ? " << u->isVisible();
  // Resources;
  if (u->getType() == UnitTypes::Resource_Vespene_Geyser)
  {
    if (u->isVisible())
      GasVector.push_back(u);
    return;
  }
  if (IsMineralField(u))
  {
    LogInfo() << "MINERAL FIELD BITCHES";
    if (IsVisible(u))
    {
      _mineralManager.addMineralField(new MineralField(u));
      _buildingManager.addBuildingFundation(u->getType(), nullptr, u->getTilePosition())->setBuilding(u, u->getTilePosition());
      _tileManager->setBuilding(u->getType(), _buildingManager.getBuilding(u->getTilePosition()), u->getTilePosition(), false);
    }
    else
    {
      _enemyManager.showUnit(u);
      _tileManager->setBuilding(u->getType(), _enemyManager.getEnemyBuilding(u->getType(), u->getTilePosition()), u->getTilePosition(), true);
    }
    HeuristicManager::getInstance().OnAllyUnitBorn(u->getType());
    return;
  }
  // ALLY / ENEMY
  if (Broodwar->self() == u->getPlayer())
  {
    HeuristicManager::getInstance().OnAllyUnitBorn(u->getType());
    if (u->getType() == UnitTypes::Terran_Command_Center || u->getType() == UnitTypes::Special_Power_Generator)
      _buildingManager.addBuildingFundation(u->getType(), nullptr, u->getTilePosition());
    if (u->getType().isBuilding() && !u->getType().isMineralField())
    {
      Logger::getLogger().logInfo() << "NEW BUILDING => " << u->getType() << " pos = " << u->getTilePosition();
      _buildingManager.setBuilding(u, u->getTilePosition());
      _tileManager->setBuilding(u->getType(), _buildingManager.getBuilding(u->getTilePosition()), u->getTilePosition(), false);
    }
    if (u->getType() == UnitTypes::Terran_SCV)
    {
      _scvManager.addSCVToState(SCVManager::UNBORN, u);
    }
    if (!u->getType().isBuilding())
      _unitManager.addUnit(u->getType(), u);
  }
  else
  {
    _enemyManager.showUnit(u);
    if (IsBuilding(u))
      _tileManager->setBuilding(u->getType(), _enemyManager.getEnemyBuilding(u->getType(), u->getTilePosition()), u->getTilePosition(), true);
  }
}

void RinotAI::onUnitDestroy(Unit u)
{
  Logger::getLogger().logInfo() << "NEW UNIT DESTROY : " << u->getType().toString() << " ID = " << u->getID() << " VISIBLE ? " << u->isVisible();
  if (u->getType() == UnitTypes::Resource_Vespene_Geyser && u->isVisible())
  {
    for (auto it = GasVector.begin(); it != GasVector.end(); ++it)
    {
      if ((*it) == u)
      {
        GasVector.erase(it);
        return;
      }
    }
  }
  if (IsMineralField(u) && !u->isVisible())
  {
    _mineralManager.removeMineralField(u);
    return;
  }
  if (Broodwar->self() == u->getPlayer())
  {
    if (u->getType() == UnitTypes::Terran_SCV)
    {
      _mineralManager.unassignGatherer(u);
      _mineralManager.unassignWaiter(u);
      _scvManager.removeSCV(u);
      _unitManager.removeUnit(UnitTypes::Terran_SCV, u);
      HeuristicManager::getInstance().OnAllyUnitDeath(u->getType());
    }
    else if (u->getType().isBuilding() && !u->getType().isMineralField())
    {
      _tileManager->destroyBuilding(u->getType(), u->getTilePosition(), false);
      _buildingManager.eraseBuilding(_buildingManager.getBuilding(u->getTilePosition()));
      HeuristicManager::getInstance().OnAllyUnitDeath(u->getType());
      if (u->getType() == UnitTypes::Terran_Supply_Depot)
        _maxSupply -= 8;
    }
    else
    {
      _unitManager.removeUnit(u->getType(), u);
      HeuristicManager::getInstance().OnAllyUnitDeath(u->getType());
    }
  }
  else
  {
    _enemyManager.destroyUnit(u);
    HeuristicManager::getInstance().OnEnemyUnitDestroyed(u->getType());
  }
}

#pragma endregion

#pragma region Personnal functions

void RinotAI::onFirstFrame()
{
  _maxSupply = Broodwar->self()->supplyTotal() / 2;
  _currentSupply = Broodwar->self()->supplyUsed() / 2;
  Logger::getLogger().logInfo() << "START OF FIRST FRAME";
  Logger::getLogger().logInfo() << "SET DISTANCE TO CC";
  _mineralManager.setDistanceToCC(_buildingManager.getAllBuildingOfType(UnitTypes::Terran_Command_Center)[0]->getUnit());
  Logger::getLogger().logInfo() << "A* FOR MINERAL PATH";
  for (auto &&mineral : _mineralManager.getMineralFieldList())
  {
    _tileManager->detectMineralPath(_buildingManager.getAllBuildingOfType(UnitTypes::Terran_Command_Center)[0], mineral->getUnit()->getTilePosition());
  }
  for (auto &&vespene : GasVector)
  {
    _tileManager->detectMineralPath(_buildingManager.getAllBuildingOfType(UnitTypes::Terran_Command_Center)[0], vespene->getTilePosition());
  }
  Logger::getLogger().logInfo() << "CALCULATE BIGGEST POSSIBLE ON EACH TILE";
  _tileManager->updateAllTilesInfos();
  Logger::getLogger().logInfo() << "END OF FIRST FRAME";
}

void RinotAI::manageMineralField()
{
  //Logger::getLogger().logInfo() << "START MANAGING";
  std::vector<Unit> &mineralGathererList = _scvManager.getSCVListByState(SCVManager::MINERAL_GATHER);
  std::vector<Unit> &idlerList = _scvManager.getSCVListByState(SCVManager::IDLE);

  auto SCVGathererIt = mineralGathererList.begin();
  //Logger::getLogger().logInfo() << "GATHERER LIST SIZE " << mineralGathererList.size();
  while (SCVGathererIt != mineralGathererList.end())
  {
    if ((*SCVGathererIt)->isCarryingMinerals() == true)
    {
      //Logger::getLogger().logInfo() << "Carrying Minerals => return to CC " << (*SCVGathererIt)->getID();
      (*SCVGathererIt)->returnCargo();
      MineralField *field = _mineralManager.getMineralFieldBySCV((*SCVGathererIt));
      //Logger::getLogger().logInfo() << "REMIANING FRAMES = " << field->getGatheringFrameLeft() << " IS BEING GATHERED " << field->getUnit()->isBeingGathered();
      field->resetMineralField();
      field->removeGatherer(*SCVGathererIt);
      Unit waiter = field->getWaiter();
      if (waiter != nullptr)
      {
        //Logger::getLogger().logInfo() << "HAD WAITER " << waiter->getID() << ": start gathering => " << field->getUnit()->getID();
        field->removeWaiter(waiter);
        field->setGatherer(waiter);
        waiter->gather(field->getUnit());
      }
      _scvManager.moveSCVToState(SCVManager::MINERAL_CARRY, *SCVGathererIt);
      if (waiter != nullptr)
        _scvManager.moveSCVToState(SCVManager::MINERAL_GATHER, waiter);
      SCVGathererIt = mineralGathererList.begin();
    }
    else
      ++SCVGathererIt;
  }
  auto SCVIdlerIt = idlerList.begin();
  //Logger::getLogger().logInfo() << "IDLERS\n";

  while (SCVIdlerIt != idlerList.end())
  {
    MineralField *field = _mineralManager.getBestMineralField(*SCVIdlerIt);
    if (field != nullptr)
    {
      //Logger::getLogger().logInfo() << "MINERAL FOUND ! " << field->getUnit()->getID();
      if (field->getGatherer() == nullptr)
      {
        //Logger::getLogger().logInfo() << "NO GATHERER !";
        field->setGatherer(*SCVIdlerIt);
        //Logger::getLogger().logInfo() << "GATHERER SET ! ID MINERAL / GATHERER = " << field->getUnit()->getID() << " IDG= " << (*SCVIdlerIt)->getID();
        (*SCVIdlerIt)->gather(field->getUnit());
        //Logger::getLogger().logInfo() << "NEW GATHERER : " << (*SCVIdlerIt)->getID() << "\n";
        _scvManager.moveSCVToState(SCVManager::MINERAL_GATHER, *SCVIdlerIt);
        SCVIdlerIt = idlerList.begin();
      }
      else if (field->getWaiter() == nullptr)
      {
        //Logger::getLogger().logInfo() << "GATHERER BUT NO WAITER";
        field->setWaiter(*SCVIdlerIt);
        (*SCVIdlerIt)->move(field->getUnit()->getPosition(), true);
        //Logger::getLogger().logInfo() << "MOVE NEAR MINERAL FIELD " << 
        //Logger::getLogger().logInfo() << "NEW WAITER " << (*SCVIdlerIt)->getID();
        _scvManager.moveSCVToState(SCVManager::WAIT, *SCVIdlerIt);
        SCVIdlerIt = idlerList.begin();
      }
      else
        ++SCVIdlerIt;
    }
    else // No mineral field available : quit
    {
      //Logger::getLogger().logInfo() << "NO MINERAL FIELD AVAILABLE";
      break;
    }
  }
}

void RinotAI::manageSupply()
{
  LogInfo() << "SUPPLY = " << _currentSupply << " ON " << _maxSupply << " can build ? " << Broodwar->canMake(UnitTypes::Terran_Supply_Depot);
  LogInfo() << "MINERALS = " << Broodwar->self()->minerals() << " <=> " << UnitTypes::Terran_Supply_Depot.mineralPrice();
  LogInfo() << "ERROR " << Broodwar->getLastError();
  if (_currentSupply + 8 >= _maxSupply && Broodwar->canMake(UnitTypes::Terran_Supply_Depot))
  {
    std::vector<Unit> &scvList = _scvManager.getSCVListByState(SCVManager::IDLE);
    auto SCVIdler = scvList.begin();

    while (SCVIdler != scvList.end())
    {
      LogInfo() << "SEARCH FOR " << (*SCVIdler)->getTilePosition();
      TilePosition loc = _tileManager->getBestBuildingLocation(UnitTypes::Terran_Supply_Depot, (*SCVIdler)->getTilePosition());
      LogInfo() << "LOL loc = " << loc;
      if (loc != TilePosition(-1, -1))
      {
        LogInfo() << "SIZE AT " << loc << _tileManager->getTileInfos(loc)->getBiggestBuildingPossible();
        Position pos = (*SCVIdler)->getPosition();
        Broodwar->registerEvent([loc, pos](Game *){Broodwar->drawBoxMap(Position(loc), Position(TilePosition(loc.x + UnitTypes::Terran_Supply_Depot.tileWidth(), loc.y + UnitTypes::Terran_Supply_Depot.tileHeight())), Colors::Blue); Broodwar->drawCircleMap(pos, 15, Colors::Black); }, nullptr, 170);
        _maxSupply += 8;
        LogInfo() << "BUILDER ID = " << (*SCVIdler)->getID();
        bool ret = (*SCVIdler)->build(UnitTypes::Terran_Supply_Depot, loc);
        LogInfo() << "BUILDING SUCCEED ? " << ret;
        _buildingManager.addBuildingFundation(UnitTypes::Terran_Supply_Depot, *SCVIdler, loc);
        _tileManager->setBuilding(UnitTypes::Terran_Supply_Depot, _buildingManager.getBuilding(loc), loc, false);
        _scvManager.moveSCVToState(SCVManager::BUILD, *SCVIdler);
        return;
      }
      ++SCVIdler;
    }
  }
}

void RinotAI::updateInfos()
{
  HeuristicManager::getInstance().updateHeuristic();
  _mineralManager.updateFields();
  _scvManager.checkForIdlers();
  LogInfo() << "UPDATE HEURISTICS";
  std::map<UnitType, std::vector<UnitHandle *>> units = _enemyManager.getEnemyInSightUnitList();
  for (auto &&type : units)
  {
    for (auto &&unit : type.second)
    {
      if (unit->getPreviousPos() != unit->getUnit()->getTilePosition())
      {
        LogInfo() << "UNIT MOVED : " << unit->getUnit()->getID();
        _tileManager->modifHeuristicUnit(unit->getUnit()->getType(), unit->getPreviousPos(), true, true);
        _tileManager->modifHeuristicUnit(unit->getUnit()->getType(), unit->getUnit()->getTilePosition(), false, true);
      }
    }
  }
  units = _unitManager.getAllAllyUnits();
  LogInfo() << "UPDATE HEURISTICS : ALLYUNITS";
  for (auto &&type : units)
  {
    for (auto &&unit : type.second)
    {
      if (unit->getPreviousPos() != unit->getUnit()->getTilePosition())
      {
        LogInfo() << "UNIT MOVED : " << unit->getUnit()->getID();
        _tileManager->modifHeuristicUnit(unit->getUnit()->getType(), unit->getPreviousPos(), true, false);
        _tileManager->modifHeuristicUnit(unit->getUnit()->getType(), unit->getUnit()->getTilePosition(), false, false);
      }
    }
  }
  units = _enemyManager.getEnemySeenUnitList();
  LogInfo() << "UPDATE HEURISTICS : ENEMY SEENS";
  for (auto &&type : units)
  {
    for (auto &&unit : type.second)
    {
      TilePosition pos = unit->getUnit()->getTilePosition();
      if (Broodwar->isVisible(unit->getUnit()->getTilePosition()) == true)
      {
        Unitset onTile = Broodwar->getUnitsOnTile(unit->getUnit()->getTilePosition());
        if (onTile.find(unit->getUnit()) != onTile.end())
        {
          LogInfo() << "UNIT MOVED : " << unit->getUnit()->getID();
          _tileManager->modifHeuristicUnit(unit->getUnit()->getType(), unit->getPreviousPos(), true, true);
        }
      }
    }
  }
  _tileManager->updateDisplay(_key);
}

void  RinotAI::manageKeyStates()
{
  if (Broodwar->getKeyState(Key::K_BACK))
    _key = Key::K_BACK;
  if (Broodwar->getKeyState(Key::K_B)) // Buildable
    _key = Key::K_B;
  if (Broodwar->getKeyState(Key::K_S)) // Biggest building
    _key = Key::K_S;
  if (Broodwar->getKeyState(Key::K_M)) // Mineral Path
    _key = Key::K_M;
  if (Broodwar->getKeyState(Key::K_P)) // Path
    _key = Key::K_P;
  if (Broodwar->getKeyState(Key::K_D)) // Building id
    _key = Key::K_D;
  if (Broodwar->getKeyState(Key::K_E)) // Economic ally
    _key = Key::K_E;
  if (Broodwar->getKeyState(Key::K_L)) // Military ally
    _key = Key::K_L;
  if (Broodwar->getKeyState(Key::K_R)) // Economic enemy
    _key = Key::K_R;
  if (Broodwar->getKeyState(Key::K_K)) // Military enemy
    _key = Key::K_K;
}

#pragma endregion

void RinotAI::onFrame()
{
  Logger::getLogger().logInfo() << "\n\nStart frame : " << Broodwar->getFrameCount();
  if (isFirstFrame == false)
  {
    isFirstFrame = true;
    onFirstFrame();
  }
  if (Broodwar->getFrameCount() % 72 == 0)
  {
    _tileManager->updateAllTilesInfos();
  }
  if (Broodwar->getFrameCount() > 1)
  {
    // START OF FRAME
    manageKeyStates();
    LogInfo() << "UPDATE KEYS";
    updateInfos();
    LogInfo() << "UPDATES INFOS";
    manageSupply();
    manageMineralField();
    // END OF FRAME;
    LogInfo() << "UPDATES STATES";
    _unitManager.updatePreviousStates();
    _enemyManager.updatePreviousStates();
    Logger::getLogger().logInfo() << "END OF FRAME";
  }
}