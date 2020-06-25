#include "level.h"

template<typename Base, typename T>
inline bool instanceof(const T*) {
   return std::is_base_of<Base, T>::value;
}

Level::Level(UserInterface* ui) {

    //Map Insert
    string LevelPath = "../Do12x-Team5MASTER-master/4.map";
    ifstream file(LevelPath);
    if (!file.good()) {
        throw std::invalid_argument("Level File not found! --- Path = " + LevelPath);
    }
    vector<Node> nodes;
    while (true) {
        Node node;
        file >> node;
        nodes.push_back(node);
        if (file.eof())
        break;
    }

    //Load Map Information
    for (const auto &n : nodes) {
        if (n.name == "Map Information") {
            m_height = n.get<int>("rows");
            m_width = n.get<int>("cols");
        }
    }

    //create rows
    m_world = new Tile**[m_height];

    //create cols
    for(int i = 0; i < m_height; i++) {
        m_world[i] = new Tile*[m_width];
    }

    //create static elements
    for (size_t i = 0; i < nodes.size(); i++) {
        Node n = nodes.at(i);
        if (n.name == "Floor") {
            int row = n.get<int>("row");
            int col = n.get<int>("col");
            m_world[row][col] = new Floor(row, col, this);
        }else if (n.name == "Wall") {
            int row = n.get<int>("row");
            int col = n.get<int>("col");
            m_world[row][col] = new Wall(row, col, this);
        }else if (n.name == "Portal") {
            int row = n.get<int>("row");
            int col = n.get<int>("col");
            m_world[row][col] = new Portal(row, col, this);
        }else if (n.name == "Door") {
            int row = n.get<int>("row");
            int col = n.get<int>("col");
            m_world[row][col] = new Door(row, col, this);
        }else if (n.name == "Trap") {
            int row = n.get<int>("row");
            int col = n.get<int>("col");
            m_world[row][col] = new Trap(row, col, this, 5);
        }
    }

    //Create Dynamic Elements (Switch / Lever / Portal)
    for (size_t i = 0; i < nodes.size(); i++) {
        Node n = nodes.at(i);
        if (n.name == "Portal") {
            int row = n.get<int>("row");
            int col = n.get<int>("col");
            int destrow = n.get<int>("destrow");
            int destcol = n.get<int>("destcol");
            m_world[row][col] = new Portal(row, col, destrow, destcol, this);
        }else if (n.name == "Switch") {
            int row = n.get<int>("row");
            int col = n.get<int>("col");
            vector<int> destrows = n.get<vector<int>>("destrows");
            vector<int> destcols = n.get<vector<int>>("destcols");
            m_world[row][col] = new Switch(row, col, destrows, destcols, this);
        }else if (n.name == "Lever") {
            int row = n.get<int>("row");
            int col = n.get<int>("col");
            vector<int> destrows = n.get<vector<int>>("destrows");
            vector<int> destcols = n.get<vector<int>>("destcols");
            m_world[row][col] = new Lever(row, col, destrows, destcols, this);

        //Items
        }else if (n.name == "Item") {
            int row = n.get<int>("row");
            int col = n.get<int>("col");
            if(n.get<string>("type") == "Weapon"){
                m_world[row][col]->setItem(new Weapon(n.get<string>("name"), n.get<int>("strbonus")));
            }else if(n.get<string>("type") == "Armor"){
                m_world[row][col]->setItem(new Armor(n.get<string>("name"), n.get<int>("stabonus")));
            }else if(n.get<string>("type") == "Potion"){
                m_world[row][col]->setItem(new Potion(n.get<string>("name"), n.get<int>("amount"), n.get<int>("hp")));
            }else if(n.get<string>("type") == "Elixir"){
                m_world[row][col]->setItem(new Elixir(n.get<string>("name"), n.get<int>("amount"), n.get<int>("hp")));
            }
        }
    }


    //Load Characters
    m_characters = std::vector<Character*>();
    for (const auto &n : nodes) {
        if (n.name == "Character") {
            Character* c = nullptr;
            if(n.get<string>("controller") == "ConsoleController"){
                c = new Character(ui, this, n.get<char>("icon"), 5, 5);
            }else if(n.get<string>("controller") == "StationaryController"){
                c = new Character(new StationaryController(), this, n.get<char>("icon"), 5, 5);
            }else if(n.get<string>("controller") == "GuardController"){
                c = new Character(new GuardController(n.get<int>("pattern")), this, n.get<char>("icon"), 5, 5);
            }
            placeCharacter(c, n.get<int>("row"), n.get<int>("col"));
        }
    }

}


//free allocated  space
Level::~Level() {
    //delete single tiles
    for(int i = 0; i < m_height; i++) {
        for(int j = 0; j < m_width; j++) {
            delete m_world[i][j];
        }
        //delete single array
        delete[] m_world[i];
    }
    //delete array of arrays
    delete[] m_world;

    //delete all characters
    for(auto* c: m_characters){
        delete c;
    }
}

int Level::getHeight() const {
    return m_height;
}

int Level::getWidth() const {
    return m_width;
}

Tile* Level::getTile(const int row, const int col) {
    return m_world[row][col];
}

const Tile* Level::getTile(const int row, const int col) const {
    return m_world[row][col];
}

void Level::placeCharacter(Character *c, int row, int col) {
    m_world[row][col]->setCharacter(c);
    c->setTile(getTile(row, col));
    m_characters.push_back(c);
}

vector<Character*> Level::getCharacters() const{
    return m_characters;
}
