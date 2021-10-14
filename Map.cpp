#include "Map.h"

using namespace std;

// Void
string space = ".......";
string spaceExtra = ".";

// Walls
string roomXaxisFull = "--------";
string roomXaxisExtra = "-";
string roomXaxis1 = "--";
string roomXaxis2 = "---";
string roomYaxis = "|";

// Doors
string roomXDoors = " ";
string roomYDoors = "   ";

// Room contents
string roomEmpty = "       ";
string roomPlayerTop = "  _o_  ";
string roomPlayerMiddle = "   |   ";
string roomPlayerBottom = "  / \\  ";

Map::Map(Dungeon &inDungeon)
{
    dungeon = &inDungeon;
    allDungeonRooms = &inDungeon.allDungeonRooms;
    allDungeonDoors = &inDungeon.allDungeonDoors;

    lowestX = 0;
    lowestY = 0;
    highestX = 0;
    highestY = 0;

    map = "";
}

void Map::calculateDungeonSize()
{
    for (auto i = allDungeonRooms->begin(); i != allDungeonRooms->end(); i++)
    {
        if (i->second.roomPos.x < lowestX && i->second.isDiscovered == true)
            lowestX = i->second.roomPos.x;
        if (i->second.roomPos.x > highestX && i->second.isDiscovered == true)
            highestX = i->second.roomPos.x;
        if (i->second.roomPos.y < lowestY && i->second.isDiscovered == true)
            lowestY = i->second.roomPos.y;
        if (i->second.roomPos.y > highestY && i->second.isDiscovered == true)
            highestY = i->second.roomPos.y;
    }
}

void Map::drawYDividers(int yLevel)
{
    Position currentPos(lowestX, yLevel);

    for (int xpos = lowestX; xpos <= highestX; xpos++)
    {
        currentPos = Position(xpos, yLevel);

        if (allDungeonRooms->find(currentPos.toString()) != allDungeonRooms->end() && allDungeonRooms->find(currentPos.toString())->second.isDiscovered == true)
        {
            Position potentialWestRoom = currentPos + Position(-1, 0);
            Position potentialNorthRoom = currentPos + Position(0, 1);
            Position potentialNWRoom = currentPos + Position(-1, 1);
            if (xpos == lowestX || (allDungeonRooms->find(potentialWestRoom.toString()) == allDungeonRooms->end() &&
                                    allDungeonRooms->find(potentialNWRoom.toString()) == allDungeonRooms->end()))
            {
                map.append(roomXaxisExtra); //One extra sign to align correctly from start
            }

            // Position potentialNorthRoom = currentPos + Position(0, 1);
            if (dungeon->areRoomsConnected(currentPos, potentialNorthRoom))
            {
                map.append(roomXaxis1);
                map.append(roomYDoors);
                map.append(roomXaxis2);
            }
            else
            {
                map.append(roomXaxisFull);
            }
        }
        else
        {
            Position potentialWestRoom = currentPos + Position(-1, 0);
            Position potentialNorthRoom = currentPos + Position(0, 1);
            Position potentialNWRoom = currentPos + Position(-1, 1);
            Position potentialEastRoom = currentPos + Position(1, 0);
            if (allDungeonRooms->find(potentialNorthRoom.toString()) != allDungeonRooms->end() && allDungeonRooms->find(potentialNorthRoom.toString())->second.isDiscovered == true)
            {
                if (xpos == lowestX || (allDungeonRooms->find(potentialWestRoom.toString()) == allDungeonRooms->end() &&
                                        allDungeonRooms->find(potentialNWRoom.toString()) == allDungeonRooms->end()))
                {
                    map.append(roomXaxisExtra); //One extra sign to align correctly from start
                }
                map.append(roomXaxisFull);
            }
            else
            {
                if (xpos == lowestX || (allDungeonRooms->find(potentialWestRoom.toString()) == allDungeonRooms->end() &&
                                        allDungeonRooms->find(potentialNWRoom.toString()) == allDungeonRooms->end()))
                {
                    map.append(spaceExtra); //one extra space for correct alignment
                }
                map.append(space);
                if (xpos == highestX)
                {
                    map.append(spaceExtra);
                }
            }
        }
    }

    map.append("\n");
}

void Map::drawRowRooms(int yLevel, ROOMPART part)
{
    Position currentPos(lowestX, yLevel);

    for (int xpos = lowestX; xpos <= highestX; xpos++)
    {
        currentPos = Position(xpos, yLevel);

        switch (part)
        {
        case TOP:
        {
            if (allDungeonRooms->find(currentPos.toString()) != allDungeonRooms->end() && allDungeonRooms->find(currentPos.toString())->second.isDiscovered == true)
            {
                Position potentialWestRoom = currentPos + Position(-1, 0);
                if (xpos == lowestX || allDungeonRooms->find(potentialWestRoom.toString()) == allDungeonRooms->end())
                {
                    map.append(roomYaxis);
                }
                if (playerPos == currentPos)
                {
                    map.append(roomPlayerTop);
                }
                else
                    map.append(roomEmpty);
                map.append(roomYaxis);
            }
            else
            {
                Position potentialWestRoom = currentPos + Position(-1, 0);
                if (xpos == lowestX || allDungeonRooms->find(potentialWestRoom.toString()) == allDungeonRooms->end())
                {
                    map.append(spaceExtra); //one extra space for correct alignment
                }
                map.append(space);
                if (xpos == highestX)
                {
                    map.append(spaceExtra);
                }
            }
            break;
        }
        case MIDDLE:
        {
            if (allDungeonRooms->find(currentPos.toString()) != allDungeonRooms->end() && allDungeonRooms->find(currentPos.toString())->second.isDiscovered == true)
            {
                Position potentialWestRoom = currentPos + Position(-1, 0);
                Position potentialEastRoom = currentPos + Position(1, 0);

                if (dungeon->areRoomsConnected(currentPos, potentialWestRoom))
                {
                    // If the room is connected to a room to the left AND
                    // If it's the first room or there is no room to the left
                    if (xpos == lowestX && allDungeonRooms->find(potentialWestRoom.toString()) != allDungeonRooms->end())
                    {
                        map.append(roomXDoors);
                    }
                }
                else
                {
                    // If the room is not connected to a room to the left AND
                    // If it's the first room or there is no room to the left
                    if (xpos == lowestX || allDungeonRooms->find(potentialWestRoom.toString()) == allDungeonRooms->end())
                    {
                        map.append(roomYaxis);
                    }
                }

                if (playerPos == currentPos)
                {
                    map.append(roomPlayerMiddle);
                }
                else
                    map.append(roomEmpty);

                if (dungeon->areRoomsConnected(currentPos, potentialEastRoom))
                {
                    if (allDungeonRooms->find(potentialEastRoom.toString()) != allDungeonRooms->end())
                    {
                        map.append(roomXDoors);
                    }
                }
                else
                    map.append(roomYaxis);
            }
            else
            {
                Position potentialWestRoom = currentPos + Position(-1, 0);
                if (xpos == lowestX || allDungeonRooms->find(potentialWestRoom.toString()) == allDungeonRooms->end())
                {
                    map.append(spaceExtra); //one extra space for correct alignment
                }
                map.append(space);
                if (xpos == highestX)
                {
                    map.append(spaceExtra);
                }
            }
            break;
        }
        case BOTTOM:
        {
            if (allDungeonRooms->find(currentPos.toString()) != allDungeonRooms->end() && allDungeonRooms->find(currentPos.toString())->second.isDiscovered == true)
            {
                Position potentialWestRoom = currentPos + Position(-1, 0);
                if (xpos == lowestX || allDungeonRooms->find(potentialWestRoom.toString()) == allDungeonRooms->end())
                {
                    map.append(roomYaxis);
                }
                if (playerPos == currentPos)
                {
                    map.append(roomPlayerBottom);
                }
                else
                    map.append(roomEmpty);
                map.append(roomYaxis);
            }
            else
            {
                Position potentialWestRoom = currentPos + Position(-1, 0);
                if (xpos == lowestX || allDungeonRooms->find(potentialWestRoom.toString()) == allDungeonRooms->end())
                {
                    map.append(spaceExtra); //one extra space for correct alignment
                }
                map.append(space);
                if (xpos == highestX)
                {
                    map.append(spaceExtra);
                }
            }
            break;
        }
        }
    }

    map.append("\n");
}

void Map::setPlayerPos(Position &pos)
{
    playerPos = pos;
}

void Map::printMap()
{
    calculateDungeonSize();

    int xLength = (highestX - lowestX) + 1;
    int yLength = (highestY - lowestY) + 1;

    system("CLS");

    for (int index = highestY; index >= lowestY; index--)
    {
        drawYDividers(index);
        drawRowRooms(index, TOP);
        drawRowRooms(index, MIDDLE);
        drawRowRooms(index, BOTTOM);

        if (index == lowestY)
            drawYDividers(index - 1);
    }

    cout << endl
         << map << flush;
}