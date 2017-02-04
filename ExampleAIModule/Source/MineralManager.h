#pragma once
#include <BWAPI.h>
#include "MineralField.h"

class MineralManager
{
public:
  MineralManager();
  ~MineralManager();

  void  unassignGatherer(Unit scv);
  void  unassignWaiter(Unit scv);
  void  updateFields();

  void  addMineralField(MineralField *field);
  void  removeMineralField(MineralField *field);
  void  removeMineralField(Unit field);

  MineralField *getMineralFieldBySCV(Unit scv);
  MineralField *getBestMineralField(Unit scv);
  const std::vector<MineralField *> &getMineralFieldList() const;
  void  setDistanceToCC(Unit cc);

private:
  std::vector<MineralField *> _mineralFieldList;
};