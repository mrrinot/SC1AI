#include "AnyType.h"

AnyType::AnyType(UnitType unit)
: _unit(unit), _upgrade(UpgradeTypes::Unknown), _tech(TechTypes::Unknown)
{
  _id = (int)(unit);
}

AnyType::AnyType(UpgradeType up)
: _unit(UnitTypes::Unknown), _upgrade(up), _tech(TechTypes::Unknown)
{
  _id = (int)(up);
}

AnyType::AnyType(TechType tech)
: _unit(UnitTypes::Unknown), _upgrade(UpgradeTypes::Unknown), _tech(tech)
{
  _id = (int)(tech);
}

AnyType::~AnyType()
{
}

const std::string &AnyType::toString() const
{
  if (_unit != UnitTypes::Unknown)
    return _unit.toString();
  if (_tech != TechTypes::Unknown)
    return _tech.toString();
  if (_upgrade != UpgradeTypes::Unknown)
    return _upgrade.toString();
  return "error";
}

int   AnyType::getId() const
{
  return _id;
}

bool  AnyType::isTech() const
{
  return !_tech == TechTypes::Unknown;
}

bool  AnyType::isUpgrade() const
{
  return !_upgrade == UpgradeTypes::Unknown;
}

bool  AnyType::isUnit() const
{
  return !_unit == UnitTypes::Unknown;
}

UnitType  AnyType::getUnitType() const
{
  return _unit;
}

UpgradeType  AnyType::getUpgradeType() const
{
  return _upgrade;
}

TechType  AnyType::getTechType() const
{
  return _tech;
}