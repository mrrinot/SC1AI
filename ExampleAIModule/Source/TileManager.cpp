#include "TileManager.h"
#include "ConstGameValues.h"
#include "Logger.hh"
#include "HeuristicManager.h"

TileManager::TileManager(int width, int height)
: _mapWidth(width), _mapHeight(height), _tileMap(width * height)
{
  Logger::getLogger().logInfo() << "TILE MANAGER CONSTRUCTOR";
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      _tileMap[x + y * _mapWidth] = new TileInfos(TilePosition(x, y));
      _tileMap[x + y * _mapWidth]->setBuildable(Broodwar->isVisible(TilePosition(x, y)));
      if (_tileMap[x + y * _mapWidth]->isBuildable())
      {
        _tileMap[x + y * _mapWidth]->setBuildable(Broodwar->getUnitsOnTile(x, y).size() == 0 ? true : false);
      }
    }
  }
}

TileManager::~TileManager()
{
  for (auto &&tile : _tileMap)
    delete tile;
}

TileInfos *TileManager::getTileInfos(int x, int y) const
{
  x = std::min(std::max(x, 0), _mapWidth - 1);
  y = std::min(std::max(y, 0), _mapHeight - 1);
  return _tileMap.at(x + y * _mapWidth);
}

TileInfos *TileManager::getTileInfos(TilePosition pos) const
{
  pos.x = std::min(std::max(pos.x, 0), _mapWidth - 1);
  pos.y = std::min(std::max(pos.y, 0), _mapHeight - 1);
  return _tileMap.at(pos.x + pos.y * _mapWidth);
}

void  TileManager::setTileInfos(TileInfos *infos, int x, int y)
{
  x = std::min(std::max(x, 0), _mapWidth - 1);
  y = std::min(std::max(y, 0), _mapHeight - 1);
  _tileMap[x + y * _mapWidth] = new TileInfos(*infos);
}

void  TileManager::setTileInfos(TileInfos *infos, TilePosition pos)
{
  pos.x = std::min(std::max(pos.x, 0), _mapWidth - 1);
  pos.y = std::min(std::max(pos.y, 0), _mapHeight - 1);
  _tileMap[pos.x + pos.y * _mapWidth] = new TileInfos(*infos);
}

void  TileManager::updateAllTilesInfos()
{
  for (int y = 0; y < _mapHeight; ++y)
  {
    for (int x = 0; x < _mapWidth; ++x)
    {
      if (Broodwar->isVisible(x, y))
      {
        if (_tileMap[x + y * _mapWidth]->isBuildable())
          _tileMap[x + y * _mapWidth]->setBuildable(Broodwar->getUnitsOnTile(x, y).size() == 0 ? true : false);
        _tileMap[x + y * _mapWidth]->setBiggestBuildingPossible(calculateBiggestBuilding(x, y));
      }
    }
  }
}

TilePosition TileManager::calculateBiggestBuilding(int x, int y)
{
  int maxHeight = std::min(MAX_HEIGHT_BUILDING, _mapHeight - y);
  int maxWidth = std::min(MAX_WIDTH_BUILDING, _mapWidth - x);

  TilePosition biggest(maxWidth, maxHeight);

  for (int startY = y; startY <= std::min(y + biggest.y, _mapHeight) - 1; ++startY)
  {
    for (int startX = x; startX <= std::min(x + biggest.x, _mapWidth) - 1; ++startX)
    {
      if (getTileInfos(startX, startY)->isBuildable() == false || getTileInfos(startX, startY)->isMineralPath() == true || getTileInfos(startX, startY)->isPath() == true)
      {
        biggest.y = startY - y;
        if (biggest.x <= 1 || biggest.y <= 1)
          biggest = TilePosition(0, 0);
        return biggest;
      }
      else
      {
        if (biggest.x < startX - x)
          biggest.x = startX - x;
      }
    }
  }
  if (biggest.x <= 1 || biggest.y <= 1)
    biggest = TilePosition(0, 0);
  return biggest;
}

void  TileManager::calculateBiggestBuildingForPosAndType(UnitType type, TilePosition pos)
{
  for (int startY = std::min(pos.y - MAX_HEIGHT_BUILDING - 1, 0); startY < pos.y + type.tileHeight() - 1; ++startY)
  {
    for (int startX = std::min(pos.x - MAX_WIDTH_BUILDING - 1, 0); startX < pos.x + type.tileWidth() - 1; ++startX)
    {
      TileInfos *infos = getTileInfos(startX, startY);
      if (infos->isBuildable() == true && infos->isMineralPath() == false && infos->isPath() == false)
        infos->setBiggestBuildingPossible(calculateBiggestBuilding(startX, startY));
      else
        infos->setBiggestBuildingPossible(TilePosition(0, 0));
    }
  }
}

void  TileManager::setBuilding(UnitType type, BuildingHandle *building, TilePosition pos, bool isEnemy)
{
  Logger::getLogger().logInfo() << "BUILDNG WIDTH / HEIGHT " << type.tileWidth() << "/" << type.tileHeight();
  for (int y = std::max(pos.y - 1, 0); y <= std::min(pos.y + type.tileHeight(), _mapHeight - 1); ++y)
  {
    for (int x = std::max(pos.x - 1, 0); x <= std::min(pos.x + type.tileWidth(), _mapWidth - 1); ++x)
    {
      if (y == pos.y - 1 || y == pos.y + type.tileHeight() || x == pos.x - 1 || x == pos.x + type.tileWidth())
        getTileInfos(x, y)->setPath(true);
      else
        getTileInfos(x, y)->setBuildable(false)->setBuilding(building);
    }
  }
  calculateBiggestBuildingForPosAndType(type, pos);
  modifHeuristicBuilding(type, pos, true, isEnemy);
}

void  TileManager::destroyBuilding(UnitType type, TilePosition pos, bool isEnemy)
{
  for (int y = std::max(pos.y - 1, 0); y <= std::min(pos.y + type.tileHeight(), _mapHeight - 1); ++y)
  {
    for (int x = std::max(pos.x - 1, 0); x <= std::min(pos.x + type.tileWidth(), _mapWidth - 1); ++x)
    {
      if (y == pos.y - 1 || y == pos.y + type.tileHeight() || x == pos.x - 1 || x == pos.x + type.tileWidth())
        getTileInfos(x, y)->setPath(false);
      else
        getTileInfos(x, y)->setBuildable(true)->setBuilding(nullptr);
    }
  }
  calculateBiggestBuildingForPosAndType(type, pos);
  modifHeuristicBuilding(type, pos, false, isEnemy);
}

void  TileManager::modifHeuristicBuilding(UnitType type, TilePosition pos, bool isContructed, bool isEnemy)
{
  UnitInfos *infos = HeuristicManager::getInstance().getInfos(type);
  //LogInfo() << "TYPE = " << type << " POS BUILDING " << pos.x << "/" << pos.y << " sizes = " << type.tileWidth() << "/" << type.tileHeight();
  //LogInfo() << "RANGES : E/M " << infos->getEcoRange() << "/" << infos->getMilRange();
  //LogInfo() << "POWS : E/M " << infos->getEcoPower() << "/" << infos->getMilPower();
  //LogInfo() << "DEAD ? " << isContructed << " enemy ? " << isEnemy;
  int minX = std::max(pos.x - infos->getEcoRange(), 0);
  int minY = std::max(pos.y - infos->getEcoRange(), 0);
  int maxX = std::min(pos.x + type.tileWidth() + infos->getEcoRange(), _mapHeight - 1);
  int maxY = std::min(pos.y + type.tileHeight() + infos->getEcoRange(), _mapHeight - 1);
  if (infos->getEcoRange() != 0)
  {
    for (int startY = minY; startY < maxY; ++startY)
    {
      int yRank = -99999;
      if (startY < pos.y)
        yRank = pos.y - startY;
      else if (startY > pos.y + type.tileHeight() - 1)
        yRank = startY - (pos.y + type.tileHeight() - 1);
      for (int startX = minX; startX < maxX; ++startX)
      {
        int xRank = -99999;
        if (startX < pos.x)
          xRank = pos.x - startX;
        else if (startX > pos.x + type.tileWidth() - 1)
          xRank = startX - (pos.x + type.tileWidth() - 1);
        //       LogInfo() << "pos " << startX << "/" << startY << " ranks = " << xRank << "-" << yRank;
        if (std::max(yRank, xRank) != -99999) // Not in the building
        {
          if (!isEnemy)
            getTileInfos(startX, startY)->addEconomicPower(infos->getEcoPower() / std::max(yRank, xRank) * (isContructed ? 1 : -1));
          else
            getTileInfos(startX, startY)->addEnemyEconomicPower(infos->getEcoPower() / std::max(yRank, xRank) * (isContructed ? 1 : -1));
        }
      }
    }
  }
  minX = std::max(pos.x - infos->getMilRange(), 0);
  minY = std::max(pos.y - infos->getMilRange(), 0);
  maxX = std::min(pos.x + type.tileWidth() + infos->getMilRange(), _mapHeight - 1);
  maxY = std::min(pos.y + type.tileHeight() + infos->getMilRange(), _mapHeight - 1);
  if (infos->getMilRange() == 0)
    return;
  for (int startY = minY; startY < maxY; ++startY)
  {
    int yRank = -99999;
    if (startY < pos.y)
      yRank = pos.y - startY;
    else if (startY > pos.y + type.tileHeight() - 1)
      yRank = startY - (pos.y + type.tileHeight() - 1);
    for (int startX = minX; startX < maxX; ++startX)
    {
      int xRank = -99999;
      if (startX < pos.x)
        xRank = pos.x - startX;
      else if (startX > pos.x + type.tileWidth() - 1)
        xRank = startX - (pos.x + type.tileWidth() - 1);
      //   LogInfo() << "pos " << startX << "/" << startY << " ranks = " << xRank << "-" << yRank;
      if (std::max(yRank, xRank) != -99999) // Not in the building
      {
        if (!isEnemy)
          getTileInfos(startX, startY)->addMilitaryPower(infos->getMilPower() / std::max(yRank, xRank) * (isContructed ? 1 : -1));
        else
          getTileInfos(startX, startY)->addEnemyMilitaryPower(infos->getMilPower() / std::max(yRank, xRank) * (isContructed ? 1 : -1));
      }
    }
  }
}

void  TileManager::updateTile(UnitInfos *infos, TilePosition pos, std::vector<TilePosition> &list, bool isDead, bool isEnemy, int step, TilePosition origin)
{
  //LogInfo() << "Step = " << step << " pos " << pos.x << "/" << pos.y;
  if (pos.x < 0 || pos.x >= _mapWidth || pos.y < 0 || pos.y >= _mapHeight || std::find(list.begin(), list.end(), pos) != list.end())
    return;
  if (step > infos->getEcoRange() && step > infos->getMilRange())
    return;
  //LogInfo() << "distance to origin : " << ceil(pos.getDistance(origin)) << " origin = " << origin;
  int pow = static_cast<int>(ceil(pos.getDistance(origin)) + 1);
  //LogInfo() << "step / pow " << step << " " << pow;
  if (!isEnemy)
    getTileInfos(pos)->addEconomicPower((pow <= infos->getEcoRange() ? infos->getEcoPower() / pow : 0) * (isDead ? -1 : 1))->addMilitaryPower((pow <= infos->getMilRange() ? infos->getMilPower() / pow : 0) *(isDead ? -1 : 1));
  else
    getTileInfos(pos)->addEnemyEconomicPower((pow <= infos->getMilRange() ? infos->getEcoPower() / pow : 0) * (isDead ? -1 : 1))->addEnemyMilitaryPower((pow <= infos->getMilRange() ? infos->getMilPower() / pow : 0) * (isDead ? -1 : 1));
  list.push_back(pos);
  updateTile(infos, TilePosition(pos.x + 1, pos.y), list, isDead, isEnemy, step + 1, origin);
  updateTile(infos, TilePosition(pos.x - 1, pos.y), list, isDead, isEnemy, step + 1, origin);
  updateTile(infos, TilePosition(pos.x, pos.y + 1), list, isDead, isEnemy, step + 1, origin);
  updateTile(infos, TilePosition(pos.x, pos.y - 1), list, isDead, isEnemy, step + 1, origin);
  return;
}

void  TileManager::modifHeuristicUnit(UnitType unit, TilePosition pos, bool isMoved, bool isEnemy)
{
  UnitInfos *infos = HeuristicManager::getInstance().getInfos(unit);
  std::vector<TilePosition>   done;
  //LogInfo() << "TYPE = " << unit;
  //LogInfo() << "RANGES : E/M " << infos->getEcoRange() << "/" << infos->getMilRange();
  //LogInfo() << "POWS : E/M " << infos->getEcoPower() << "/" << infos->getMilPower();
  //LogInfo() << "DEAD ? " << isMoved << " enemy ? " << isEnemy;
  updateTile(infos, pos, done, isMoved, isEnemy, 1, pos);
}

std::list<s_tilePathfinding *>::iterator find(std::list<s_tilePathfinding *>::iterator begin, std::list<s_tilePathfinding *>::iterator end, s_tilePathfinding *toF)
{
  for (auto it = begin; it != end; ++it)
  {
    // Logger::getLogger().logInfo() << "FIND LOOP " << (*it)->pos << "/" << toF->pos << " " << (*it)->distanceToObj << "/" << toF->distanceToObj;
    if ((*it)->pos == toF->pos && (*it)->distanceToObj == toF->distanceToObj)
      return it;
  }
  return end;
}

void  TileManager::detectMineralPath(BuildingHandle *cc, TilePosition mine)
{
  std::list<s_tilePathfinding *> open;
  std::list<s_tilePathfinding *> closed;
  TilePosition ccPos = cc->getUnit()->getTilePosition();
  s_tilePathfinding *best = new s_tilePathfinding(mine, ccPos.getDistance(mine), 0, nullptr);
  bool found = false;

  closed.push_back(best);
  while (found == false)
  {
    //Logger::getLogger().logInfo() << "CLOSED= " << closed.size() << " OPEN= " << open.size() << " CURRENT STEP = " << best->step << " distance = " << best->distanceToObj;
    for (auto it = closed.begin(); it != closed.end(); ++it)
    {
      //Logger::getLogger().logInfo() << "CHECKING POS " << (*it)->pos;
      if (getTileInfos((*it)->pos)->getBuilding() != nullptr && getTileInfos((*it)->pos)->getBuilding() == cc)  // FOUND
      {
        //Logger::getLogger().logInfo() << "THE END !!";
        s_tilePathfinding *node = *it;
        while (node->step > 0 && node->parent != nullptr)
        {
          //Logger::getLogger().logInfo() << "BACK UP TO " << node->pos << " step = " << node->step << " previous " << (*node->parent).pos;
          getTileInfos(node->pos)->setMineralPath(true)->setBuildable(false);
          node = node->parent;
        }
        //Logger::getLogger().logInfo() << "DELETING !!";
        for (auto it = closed.begin(); it != closed.end(); ++it)
        {
          //Logger::getLogger().logInfo() << "CLOSED DELETED " << (*it)->pos;
          delete *it;
        }
        //Logger::getLogger().logInfo() << "DELETED CLOSED";
        for (auto it = open.begin(); it != open.end(); ++it)
        {
          //Logger::getLogger().logInfo() << "OPEN DELETED " << (*it)->pos;
          delete *it;
        }
        //Logger::getLogger().logInfo() << "DELETED OPEN";
        return;
      }
    }
    s_tilePathfinding *up = new s_tilePathfinding(best->pos + TilePosition(0, -1), ccPos.getDistance(best->pos + TilePosition(0, -1)), best->step + 1, best);
    s_tilePathfinding *down = new s_tilePathfinding(best->pos + TilePosition(0, 1), ccPos.getDistance(best->pos + TilePosition(0, 1)), best->step + 1, best);
    s_tilePathfinding *left = new s_tilePathfinding(best->pos + TilePosition(-1, 0), ccPos.getDistance(best->pos + TilePosition(-1, 0)), best->step + 1, best);
    s_tilePathfinding *right = new s_tilePathfinding(best->pos + TilePosition(1, 0), ccPos.getDistance(best->pos + TilePosition(1, 0)), best->step + 1, best);
    //Logger::getLogger().logInfo() << "PREVIOUS POS : U/D/L/R " << up->parent->pos << "/" << down->parent->pos << "/" << left->parent->pos << "/" << right->parent->pos;
    //Logger::getLogger().logInfo() << "POS : U/D/L/R " << up->pos << "/" << down->pos << "/" << left->pos << "/" << right->pos;
    //Logger::getLogger().logInfo() << "DISTANCES : U/D/L/R " << up->distanceToObj << "/" << down->distanceToObj << "/" << left->distanceToObj << "/" << right->distanceToObj;
    //Logger::getLogger().logInfo() << "BUILDABLE : U/D/L/R " << getTileInfos(up->pos)->isBuildable() << "/" << getTileInfos(down->pos)->isBuildable() << "/" << getTileInfos(left->pos)->isBuildable() << "/" << getTileInfos(right->pos)->isBuildable();
    if (find(open.begin(), open.end(), up) == open.end() && find(closed.begin(), closed.end(), up) == closed.end())
    {
      //Logger::getLogger().logInfo() << "ADDED UP";
      open.push_front(up);
    }
    else
      delete up;
    if (find(open.begin(), open.end(), down) == open.end() && find(closed.begin(), closed.end(), down) == closed.end())
    {
      //Logger::getLogger().logInfo() << "ADDED DOWN";
      open.push_front(down);
    }
    else
      delete down;
    if (find(open.begin(), open.end(), left) == open.end() && find(closed.begin(), closed.end(), left) == closed.end())
    {
      //Logger::getLogger().logInfo() << "ADDED LEFT";
      open.push_front(left);
    }
    else
      delete left;
    if (find(open.begin(), open.end(), right) == open.end() && find(closed.begin(), closed.end(), right) == closed.end())
    {
      //Logger::getLogger().logInfo() << "ADDED RIGHT";
      open.push_front(right);
    }
    else
      delete right;
    double bestDistance = 99999;
    int bestStep = 0;
    s_tilePathfinding *bestNew;
    for (auto &&node : open)
    {
      if (node->distanceToObj < bestDistance && node->step >= bestStep)
      {
        bestStep = node->step;
        bestDistance = node->distanceToObj;
        bestNew = node;
        //Logger::getLogger().logInfo() << "NEW BEST " << bestStep << " dist = " << bestDistance << " pos = " << bestNew->pos;
      }
    }
    //Logger::getLogger().logInfo() << "CLOSEST = " << bestNew->pos << " " << bestNew->step;
    closed.push_back(bestNew);
    open.remove(bestNew);
    best = bestNew;
  }
}

TilePosition TileManager::checkTile(TilePosition size, TilePosition pos, std::vector<TilePosition> &list, int step)
{
  if (pos.x < 0 || pos.x >= _mapWidth || pos.y < 0 || pos.y >= _mapHeight || step > MAX_SEARCH_STEP)
    return TilePosition(-1, -1);
  //LogInfo() << "check tile " << pos << " looking for " << size.x << "/" << size.y << " => " << getTileInfos(pos)->getBiggestBuildingPossible().x << "/" << getTileInfos(pos)->getBiggestBuildingPossible().y;
  if (getTileInfos(pos)->getBiggestBuildingPossible().x >= size.x && getTileInfos(pos)->getBiggestBuildingPossible().y >= size.y)
    return pos;
  if (std::find(list.begin(), list.end(), pos) != list.end())
    return TilePosition(-1, -1);
  list.push_back(pos);
  TilePosition res;
  res = checkTile(size, TilePosition(pos.x + 1, pos.y), list, step + 1);
  if (res.x != -1 && res.y != -1)
    return res;
  res = checkTile(size, TilePosition(pos.x - 1, pos.y), list, step + 1);
  if (res.x != -1 && res.y != -1)
    return res;
  res = checkTile(size, TilePosition(pos.x, pos.y + 1), list, step + 1);
  if (res.x != -1 && res.y != -1)
    return res;
  res = checkTile(size, TilePosition(pos.x, pos.y - 1), list, step + 1);
  if (res.x != -1 && res.y != -1)
    return res;
  return TilePosition(-1, -1);
}

TilePosition TileManager::getBestBuildingLocation(UnitType type, TilePosition start)
{
  int step = 1;
  std::vector<TilePosition> list;
  TilePosition size(type.tileWidth(), type.tileHeight());
  return checkTile(size, start, list, step);
}

void  TileManager::updateDisplay(Key k)
{
  if (k == Key::K_BACK)
    return;
  for (int cpt = 0; cpt < _mapHeight * _mapWidth; ++cpt)
  {
    Broodwar->drawBoxMap(Position(_tileMap[cpt]->getPos().x * 32, _tileMap[cpt]->getPos().y * 32), Position(_tileMap[cpt]->getPos().x * 32 + 32, _tileMap[cpt]->getPos().y * 32 + 32), Colors::White);
    if (k == Key::K_B)
      Broodwar->drawTextMap(_tileMap[cpt]->getPos().x * 32 + 16, _tileMap[cpt]->getPos().y * 32 + 10, "%d", (_tileMap[cpt]->isBuildable() == true ? 1 : 0));
    if (k == Key::K_M)
      Broodwar->drawTextMap(_tileMap[cpt]->getPos().x * 32 + 16, _tileMap[cpt]->getPos().y * 32 + 10, "%d", (_tileMap[cpt]->isMineralPath() == true ? 1 : 0));
    else if (k == Key::K_S)
      Broodwar->drawTextMap(_tileMap[cpt]->getPos().x * 32 + 16, _tileMap[cpt]->getPos().y * 32 + 10, "%d/%d", _tileMap[cpt]->getBiggestBuildingPossible().x, _tileMap[cpt]->getBiggestBuildingPossible().y);
    else if (k == Key::K_P)
      Broodwar->drawTextMap(_tileMap[cpt]->getPos().x * 32 + 16, _tileMap[cpt]->getPos().y * 32 + 10, "%d", (_tileMap[cpt]->isPath() == true ? 1 : 0));
    //else if (k == Key::K_D)
    //  Broodwar->drawTextMap(_tileMap[cpt]->getPos().x * 32 + 16, _tileMap[cpt]->getPos().y * 32 + 10, "%d", (_tileMap[cpt]->getBuilding() == nullptr ? 0 : _tileMap[cpt]->getBuilding()->getUnit()->getType()));
    else if (k == Key::K_L)
      Broodwar->drawTextMap(_tileMap[cpt]->getPos().x * 32 + 16, _tileMap[cpt]->getPos().y * 32 + 10, "%.1f", _tileMap[cpt]->getMilitaryPower());
    else if (k == Key::K_E)
      Broodwar->drawTextMap(_tileMap[cpt]->getPos().x * 32 + 16, _tileMap[cpt]->getPos().y * 32 + 10, "%.1f", _tileMap[cpt]->getEconomicPower());
    else if (k == Key::K_K)
      Broodwar->drawTextMap(_tileMap[cpt]->getPos().x * 32 + 16, _tileMap[cpt]->getPos().y * 32 + 10, "%.1f", _tileMap[cpt]->getEnemyMilitaryPower());
    else if (k == Key::K_R)
      Broodwar->drawTextMap(_tileMap[cpt]->getPos().x * 32 + 16, _tileMap[cpt]->getPos().y * 32 + 10, "%.1f", _tileMap[cpt]->getEnemyEconomicPower());
  }
}
