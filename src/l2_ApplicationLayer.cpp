#include "hw/l2_ApplicationLayer.h"

bool isNumber(std::string s)
{
    if (s.length() == 0 || s.length() > 9)
        return false;

    unsigned int start = 0;
    if (s[0] == '-')
        start = 1;
    if (start == s.length())
        return false;

    for (unsigned int i = start; i < s.length(); i++) {
        if (s[i] < '0' || s[i] > '9')
            return false;
    }
    return true;
}

Application::Application(ShipRegistry &registry) : _registry(registry)
{
}

std::string Application::execute(std::vector<std::string> args)
{
    std::string command = args[0];

    if (command == "add")
        return add(args);
    else if (command == "remove")
        return remove(args);
    else if (command == "update")
        return update(args);
    else if (command == "view")
        return view();
    else if (command == "save")
        return save(args);
    else if (command == "load")
        return load(args);

    return "Неизвестная команда '" + command + "'\n";
}

std::string Application::add(std::vector<std::string> args)
{
    if (args.size() != 5)
        return "Неверный формат команды. Нужно: add <наименование> <сухой тоннаж> <полный тоннаж> <тип cargo|tanker|cruise>\n";

    std::string name = args[1];

    if (!isNumber(args[2]) || !isNumber(args[3]))
        return "Ошибка: тоннаж должен быть целым числом\n";
    int dryTonnage = std::stoi(args[2]);
    int fullTonnage = std::stoi(args[3]);

    if (!isShipDataCorrect(dryTonnage, fullTonnage))
        return "Ошибка: сухой тоннаж должен быть больше 0, полный тоннаж - не меньше сухого\n";

    ShipType type;
    if (!parseShipType(args[4], type))
        return "Ошибка: тип судна должен быть cargo (сухогруз), tanker (танкер) или cruise (круизный лайнер)\n";

    Ship ship(name, dryTonnage, fullTonnage, type);
    if (!_registry.addShip(ship))
        return "Ошибка: судно '" + name + "' уже наблюдается\n";

    return "Судно '" + name + "' добавлено\n";
}

std::string Application::remove(std::vector<std::string> args)
{
    if (args.size() != 2)
        return "Неверный формат команды. Нужно: remove <наименование>\n";

    std::string name = args[1];
    if (_registry.removeShip(name) == false)
        return "Ошибка: судно '" + name + "' не найдено\n";
    return "Судно '" + name + "' удалено\n";
}

std::string Application::update(std::vector<std::string> args)
{
    if (args.size() != 4) {
        return "Неверный формат команды. Нужно: update <наименование> <сухой тоннаж> <полный тоннаж>\n";
    }

    std::string name = args[1];
    if (!isNumber(args[2]) || !isNumber(args[3]))
        return "Ошибка: тоннаж должен быть целым числом\n";

    int dryTonnage = std::stoi(args[2]);
    int fullTonnage = std::stoi(args[3]);

    if (!_registry.hasShip(name))
        return "Ошибка: судно '" + name + "' не найдено\n";

    if (!isShipDataCorrect(dryTonnage, fullTonnage))
        return "Ошибка: сухой тоннаж должен быть больше 0, полный тоннаж - не меньше сухого\n";

    ShipType type = _registry.getShip(name).getType();
    _registry.updateShip(Ship(name, dryTonnage, fullTonnage, type));
    return "Судно '" + name + "' изменено\n";
}

std::string Application::view()
{
    std::map<std::string, Ship> ships = _registry.getShips();
    if (ships.size() == 0)
        return "Список наблюдаемых судов пуст\n";

    std::string result = "Наблюдаемых судов: " + std::to_string(ships.size()) + "\n";
    for (auto s : ships)
    {
        Ship ship = s.second;
        result += ship.getName() + " | тип: " + shipTypeName(ship.getType())
            + " | сухой тоннаж: " + std::to_string(ship.getDryTonnage()) + " т"
            + " | полный тоннаж: " + std::to_string(ship.getFullTonnage()) + " т\n";
    }

    return result;
}

std::string Application::save(std::vector<std::string> args)
{
    if (args.size() != 2)
        return "Неверный формат команды. Нужно: save <файл>\n";

    std::string filename = args[1];
    if (!_registry.saveToFile(filename))
        return "Ошибка: не удалось сохранить список судов в файл " + filename + "\n";
    return "Список судов сохранён в файл " + filename + "\n";
}

std::string Application::load(std::vector<std::string> args)
{
    if (args.size() != 2)
        return "Неверный формат команды. Нужно: load <файл>\n";

    std::string filename = args[1];
    if (!_registry.loadFromFile(filename))
        return "Ошибка: не удалось загрузить список судов из файла " + filename + "\n";

    return "Список судов загружен из файла " + filename + "\n";
}
