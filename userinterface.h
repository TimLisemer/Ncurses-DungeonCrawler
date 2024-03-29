#ifndef USERINTERFACE_H
#define USERINTERFACE_H
#include "level.h"
#include "logger.h"
#include <ncurses.h>
#include "dungeoncrawler.h"

using std::string;

class Level;
class Character;

///
/// \brief The Controller class
///
class Controller{
public:
    explicit
    Controller();
    virtual ~Controller() = default;
    virtual int move(Character* c);
    virtual bool setTile(Character* c, const int key);
};


///
/// \brief The UserInterface class
///
class UserInterface : public Controller{

public:
    UserInterface();
    virtual ~UserInterface();

    void draw(Level* lvl);

    virtual int move(Character* c);

private:

    //True = Menu Open, False = Menu Closed
    bool m_gameMenu = false;
    bool m_Inventory = false;

    //Needed to show the GameHeader at First Time Startup
    bool m_firstStartup = true;

    WINDOW* m_HeaderWindow = nullptr;

    //If m_gameMenu == true --> Show Game Menu else show Game Info and Stats
    void setGameMenu(const int menu, Character* c);

};


///
/// \brief The StationaryController class
///
class StationaryController : public Controller{
public:
    StationaryController();
    virtual int move(Character*);
};




///
/// \brief The GuardController class
///
class GuardController : public Controller{
public:
    GuardController(const string pattern);
    virtual int move(Character*);
private:
    string m_pattern;
};




///
/// \brief The AttackController class
///
class AttackController : public Controller{
public:
    AttackController(Level* level);
    virtual int move(Character* c);
private:
    Level* m_level;
};

#endif // USERINTERFACE_H
