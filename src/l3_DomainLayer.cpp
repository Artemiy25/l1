#include <cassert>
#include <sstream>
#include <vector>

#include "hw/l3_DomainLayer.h"
#include "hw/l4_InfrastructureLayer.h"

// код типа в команде и в файле: cargo, tanker, cruise
bool parseShipType(const std::string &code, ShipType &type)
{
    if (code == "cargo")
        type = ShipType::DryCargo;
    else if (code == "tanker")
        type = ShipType::Tanker;
    else if (code == "cruise")
        type = ShipType::Cruise;
    else
        return false;
    return true;
}

std::string shipTypeCode(ShipType type)
{
    switch (type) {
    case ShipType::DryCargo:
        return "cargo";
    case ShipType::Tanker:
        return "tanker";
    case ShipType::Cruise:
        return "cruise";
    }
    return "";
}

std::string shipTypeName(ShipType type)
{
    switch (type) {
    case ShipType::DryCargo:
        return "сухогруз";
    case ShipType::Tanker:
        return "танкер";
    case ShipType::Cruise:
        return "круизный лайнер";
    }
    return "";
}

// полный тоннаж включает сухой, поэтому не может быть меньше
bool isShipDataCorrect(int dryTonnage, int fullTonnage)
{
    return dryTonnage > 0 && fullTonnage >= dryTonnage;
}


Ship::Ship(const std::string &name, int dryTonnage, int fullTonnage, ShipType type)
{
    _name = name;
    _dryTonnage = dryTonnage;
    _fullTonnage = fullTonnage;
    _type = type;

    assert(invariant());
}

std::string Ship::getName() const
{
    return _name;
}

int Ship::getDryTonnage() const
{
    return _dryTonnage;
}

int Ship::getFullTonnage() const
{
    return _fullTonnage;
}

ShipType Ship::getType() const
{
    return _type;
}

bool Ship::invariant() const
{
    return !_name.empty() && isShipDataCorrect(_dryTonnage, _fullTonnage);
}


bool ShipRegistry::addShip(const Ship &ship)
{
    if (hasShip(ship.getName()))
        return false;

    _ships.insert({ship.getName(), ship});
    return true;
}

bool ShipRegistry::updateShip(const Ship &ship)
{
    if (!hasShip(ship.getName()))
        return false;

    _ships.erase(ship.getName());
    _ships.insert({ship.getName(), ship});
    return true;
}

bool ShipRegistry::removeShip(const std::string &name)
{
    if (!hasShip(name))
        return false;
    _ships.erase(name);
    return true;
}

bool ShipRegistry::hasShip(const std::string &name) const
{
    return _ships.count(name) > 0;
}

Ship ShipRegistry::getShip(const std::string &name) const
{
    return _ships.at(name);
}

std::map<std::string, Ship> ShipRegistry::getShips() const
{
    return _ships;
}

// в файле каждое судно в отдельной строке: имя сухой_тоннаж полный_тоннаж тип
bool ShipRegistry::saveToFile(const std::string &filename) const
{
    std::vector<std::string> lines;
    for (auto s : _ships) {
        Ship ship = s.second;
        std::string line = ship.getName() + " " + std::to_string(ship.getDryTonnage()) + " "
            + std::to_string(ship.getFullTonnage()) + " " + shipTypeCode(ship.getType());
        lines.push_back(line);
    }

    FileStorage storage;
    return storage.saveLines(filename, lines);
}

bool ShipRegistry::loadFromFile(const std::string &filename)
{
    FileStorage storage;
    std::vector<std::string> lines;
    if (!storage.loadLines(filename, lines))
        return false;

    // если в файле ошибка то старый список не трогаем
    std::map<std::string, Ship> loaded;

    for (std::string line : lines)
    {
        std::istringstream in(line);
        std::string name, typeCode;
        int dryTonnage = 0, fullTonnage = 0;

        in >> name;
        if (name == "")
            continue;

        in >> dryTonnage >> fullTonnage >> typeCode;
        if (in.fail())
            return false;
        if (!isShipDataCorrect(dryTonnage, fullTonnage))
            return false;

        ShipType type;
        if (!parseShipType(typeCode, type))
            return false;

        loaded.insert({name, Ship(name, dryTonnage, fullTonnage, type)});
    }

    _ships = loaded;
    return true;
}
