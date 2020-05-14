#include "level.h"

Level::Level() : m_height(20), m_width(20) {
    //create rows
    m_world = new Tile**[m_height];

    //create cols
    for(int i = 0; i < m_height; i++) {
        m_world[i] = new Tile*[m_width];
    }



    //create elements
    for(int i = 0; i < m_height; i++) {
        for(int d = 0; d < m_width; d++) {

            if(i == 0 || i == m_height - 1 || d == 0 || d == m_width - 1){
                m_world[i][d] = new Wall(i, d);
            }else{
                m_world[i][d] = new Floor(i, d);
            }
        }
    }


}

//free allocated  space
Level::~Level() {
    for(int i = 0; i < m_height; i++) {
        for(int j = 0; j < m_width; j++) {
            delete m_world[i][j];
        }
        delete m_world[i];
    }
    delete m_world;
}

Level::Level(int hoehe, int breite) : m_height(hoehe), m_width(breite) {

}

Tile* Level::getTile(int row, int col) {
    return m_world[row][col];
}

int Level::getHeight() const {
    return m_height;
}

int Level::getWidth() const {
    return m_width;
}

const Tile* Level::getTile(int row, int col) const {
    return m_world[row][col];
}

void Level::placeCharacter(Character *c, int row, int col) {
    m_world[row][col]->setCharacter(c);
}
