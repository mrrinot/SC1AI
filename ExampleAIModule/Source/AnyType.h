#pragma once
#include "BWAPI.h"

using namespace BWAPI;
class AnyType
{
public:
  AnyType(UnitType unit);
  AnyType(UpgradeType unit);
  AnyType(TechType unit);
  ~AnyType();

  bool  isTech() const;
  bool  isUnit() const;
  bool  isUpgrade() const;
  int   getId() const;

  UnitType  getUnitType() const;
  UpgradeType getUpgradeType() const;
  TechType  getTechType() const;
  const std::string &toString() const;

private:
  int     _id;
  UnitType _unit;
  UpgradeType _upgrade;
  TechType  _tech;
};

namespace std
{
  template<> struct less<AnyType>
  {
    bool operator() (const AnyType& lhs, const AnyType& rhs) const
    {
      return lhs.getId() < rhs.getId();
    }
  };
}