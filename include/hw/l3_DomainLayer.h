#ifndef HW_L3_DOMAIN_LAYER_H
#define HW_L3_DOMAIN_LAYER_H

#include <string>
#include <map>

// типы судов
enum class ShipType
{
    DryCargo, // сухогруз
    Tanker,   // танкер
    Cruise    // круизный лайнер
};

bool parseShipType(const std::string &code, ShipType &type);
std::string shipTypeCode(ShipType type);
std::string shipTypeName(ShipType type);
bool isShipDataCorrect(int dryTonnage, int fullTonnage);

// карточка судна
class Ship
{
public:
    Ship(const std::string &name, int dryTonnage, int fullTonnage, ShipType type);

    std::string getName() const;
    int getDryTonnage() const;
    int getFullTonnage() const;
    ShipType getType() const;

    bool invariant() const;

private:
    std::string _name;
    int _dryTonnage;  // сухой тоннаж, т
    int _fullTonnage; // полный тоннаж, т (не меньше сухого)
    ShipType _type;
};

// все наблюдаемые суда
class ShipRegistry {
public:
    bool addShip(const Ship &ship);
    bool updateShip(const Ship &ship);
    bool removeShip(const std::string &name);

    bool hasShip(const std::string &name) const;
    Ship getShip(const std::string &name) const;
    std::map<std::string, Ship> getShips() const;

    bool saveToFile(const std::string &filename) const;
    bool loadFromFile(const std::string &filename);

private:
    std::map<std::string, Ship> _ships;
};

#endif
