#pragma once

#include "Grid.hpp"

#include <BWAPI.h>
#include <vector>

class MapTools
{
    Grid<int> m_walkable;
    Grid<int> m_buildable;
    Grid<int> m_depotBuildable;
    Grid<int> m_lastSeen;
    int m_width = 0;
    int m_height = 0;
    int m_frame = 0;
    bool m_drawMap = false;
    bool canBuild(int tileX, int tileY) const;
    bool canWalk(int tileX, int tileY) const;
    void printMap() const;

public:
    MapTools();
    void onStart();
    void onFrame();
    void draw() const;
    void toggleDraw();
    int width() const;
    int height() const;
    bool isValidTile(int tileX, int tileY) const;
    bool isValidTile(const BWAPI::TilePosition & tile) const;
    bool isValidPosition(const BWAPI::Position & pos) const;
    bool isPowered(int tileX, int tileY) const;
    bool isExplored(int tileX, int tileY) const;
    bool isExplored(const BWAPI::Position & pos) const;
    bool isExplored(const BWAPI::TilePosition & pos) const;
    bool isVisible(int tileX, int tileY) const;
    bool isWalkable(int tileX, int tileY) const;
    bool isWalkable(const BWAPI::TilePosition& tile) const;
    bool isBuildable(int tileX, int tileY) const;
    bool isBuildable(const BWAPI::TilePosition& tile) const;
    bool isDepotBuildableTile(int tileX, int tileY) const;
    void drawTile(int tileX, int tileY, const BWAPI::Color & color) const;
    std::vector<BWAPI::Position> getUnexploredReachableTiles(BWAPI::Position start);
    BWAPI::Position getUnexploredReachableTile(BWAPI::Position start);
    BWAPI::TilePosition enemyBase;
	void SetEnemyBase(BWAPI::TilePosition base);
    BWAPI::TilePosition GetEnemyBase();
    bool foundEnemyBase;
};