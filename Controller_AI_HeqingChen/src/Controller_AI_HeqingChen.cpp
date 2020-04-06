// MIT License
// 
// Copyright(c) 2020 Arthur Bacon and Kevin Dill
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright noticeand this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Controller_AI_HeqingChen.h"

#include "Constants.h"
#include "EntityStats.h"
#include "Vec2.h"

// initial position of troop on left side
static Vec2 leftSwordsmanPos(LEFT_BRIDGE_CENTER_X, RIVER_TOP_Y - 1.0f);
static Vec2 leftGiantPos(LEFT_BRIDGE_CENTER_X, RIVER_TOP_Y - 0.5f);
static Vec2 leftArcherPos(LEFT_BRIDGE_CENTER_X, NorthPrincessY);

// initial position of troop on right side
static Vec2 rightSwordsmanPos(RIGHT_BRIDGE_CENTER_X, RIVER_TOP_Y - 1.0f);
static Vec2 rightGiantPos(RIGHT_BRIDGE_CENTER_X, RIVER_TOP_Y - 0.5f);
static Vec2 rightArcherPos(RIGHT_BRIDGE_CENTER_X, NorthPrincessY);

// these two variables are used for calculating the combat effectiveness between each camp on each lane
static int leftCostDiff = 0;
static int rightCostDiff = 0;

// return the number of own swordsman
int Controller_AI_HeqingChen::getNumOfSwordsman() {
    int numOfSwordsman = 0;
    for (int i = 0; i < m_pPlayer->getNumMobs(); i++) {
        if (m_pPlayer->getMob(i).m_Stats.getMobType() == 0) {
            numOfSwordsman++;
        }
    }
    return numOfSwordsman;
}

// return the number of own archers
int Controller_AI_HeqingChen::getNumOfArcher() {
    int numOfArcher = 0;
    for (int i = 0; i < m_pPlayer->getNumMobs(); i++) {
        if (m_pPlayer->getMob(i).m_Stats.getMobType() == 1) {
            numOfArcher++;
        }
    }
    return numOfArcher;
}

// return the number of own giants
int Controller_AI_HeqingChen::getNumOfGiant() {
    int numOfGiant = 0;
    for (int i = 0; i < m_pPlayer->getNumMobs(); i++) {
        if (m_pPlayer->getMob(i).m_Stats.getMobType() == 2) {
            numOfGiant++;
        }
    }
    return numOfGiant;
}

//get own cost on the left side
int Controller_AI_HeqingChen::getLeftCost(iPlayer*& m_pPlayer) {
    int totalCost = 0;
    for (int i = 0; i < m_pPlayer->getNumMobs(); i++) {
        if (m_pPlayer->getMob(i).m_Position.x <= KingX) {
            totalCost += m_pPlayer->getMob(i).m_Stats.getElixirCost();
        }
    }
    return totalCost;
}

//get own cost on the right side
int Controller_AI_HeqingChen::getRightCost(iPlayer*& m_pPlayer) {
    int totalCost = 0;
    for (int i = 0; i < m_pPlayer->getNumMobs(); i++) {
        if (m_pPlayer->getMob(i).m_Position.x > KingX) {
            totalCost += m_pPlayer->getMob(i).m_Stats.getElixirCost();
        }
    }
    return totalCost;
}

// calculate the total cost of opponents on the left side
int Controller_AI_HeqingChen::getLeftOpponentsCost(iPlayer* & m_pPlayer) {
    int totalCost = 0;
    for (int i = 0; i < m_pPlayer->getNumOpponentMobs(); i++) {
        if (m_pPlayer->getOpponentMob(i).m_Position.x <= KingX) {
            totalCost += m_pPlayer->getOpponentMob(i).m_Stats.getElixirCost();
        }
    }
    return totalCost;
}

// calculate the total cost of opponents on the right side
int Controller_AI_HeqingChen::getRightOpponentsCost(iPlayer*& m_pPlayer) {
    int totalCost = 0;
    for (int i = 0; i < m_pPlayer->getNumOpponentMobs(); i++) {
        if (m_pPlayer->getOpponentMob(i).m_Position.x > KingX) {
            totalCost += m_pPlayer->getOpponentMob(i).m_Stats.getElixirCost();
        }
    }
    return totalCost;
}

void Controller_AI_HeqingChen::tick(float deltaTSec)
{
    assert(m_pPlayer);

    // behavior while there is no opponents
    if (m_pPlayer->getNumOpponentMobs() == 0) {
        if (getNumOfGiant() == 0) {
            if (m_pPlayer->getElixir() > 8)
            {
                // convert the positions from player space to game space
                bool isNorth = m_pPlayer->isNorth();
                Vec2 giantPos_Game = leftGiantPos.Player2Game(isNorth);
                Vec2 archerPos_Game = leftArcherPos.Player2Game(isNorth);
                // Create two archers and a giant
                m_pPlayer->placeMob(iEntityStats::Giant, giantPos_Game);
                m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
                m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);

            }
            if (m_pPlayer->getElixir() > 7 && m_pPlayer->getElixir() <= 8 && getNumOfArcher() != 0 && getNumOfSwordsman() != 0) {
                bool isNorth = m_pPlayer->isNorth();
                Vec2 swordsmanPos_Game = leftSwordsmanPos.Player2Game(isNorth);
                Vec2 archerPos_Game = leftArcherPos.Player2Game(isNorth);

                m_pPlayer->placeMob(iEntityStats::Swordsman, swordsmanPos_Game);
                m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
                m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
            }
            if (m_pPlayer->getElixir() > 4 && m_pPlayer->getElixir() <= 5 && getNumOfArcher() == 0) {
                bool isNorth = m_pPlayer->isNorth();
                Vec2 archerPos_Game = leftArcherPos.Player2Game(isNorth);

                m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
                m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
            }
            if (m_pPlayer->getElixir() > 3 && m_pPlayer->getElixir() <= 5 && getNumOfSwordsman() == 0) {
                bool isNorth = m_pPlayer->isNorth();
                Vec2 swordsmanPos_Game = leftSwordsmanPos.Player2Game(isNorth);
                m_pPlayer->placeMob(iEntityStats::Swordsman, swordsmanPos_Game);
            }
        }
        else {
            if (m_pPlayer->getElixir() > 7 && m_pPlayer->getElixir() <= 8 && getNumOfArcher() != 0 && getNumOfSwordsman() != 0) {
                bool isNorth = m_pPlayer->isNorth();
                Vec2 swordsmanPos_Game = leftSwordsmanPos.Player2Game(isNorth);
                Vec2 archerPos_Game = leftArcherPos.Player2Game(isNorth);

                m_pPlayer->placeMob(iEntityStats::Swordsman, swordsmanPos_Game);
                m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
                m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
            }
            if (m_pPlayer->getElixir() > 5 && getNumOfGiant() <= 1) {
                bool isNorth = m_pPlayer->isNorth();
                Vec2 giantPos_Game = leftGiantPos.Player2Game(isNorth);
                m_pPlayer->placeMob(iEntityStats::Giant, giantPos_Game);
            }
            if (m_pPlayer->getElixir() > 4 && m_pPlayer->getElixir() <= 5 && getNumOfArcher() == 0) {
                bool isNorth = m_pPlayer->isNorth();
                Vec2 archerPos_Game = leftArcherPos.Player2Game(isNorth);

                m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
                m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
            }
            if (m_pPlayer->getElixir() > 3 && m_pPlayer->getElixir() <= 5 && getNumOfSwordsman() == 0) {
                bool isNorth = m_pPlayer->isNorth();
                Vec2 swordsmanPos_Game = leftSwordsmanPos.Player2Game(isNorth);
                m_pPlayer->placeMob(iEntityStats::Swordsman, swordsmanPos_Game);
            }
        }     
    }
    // behavior while there are opponents
    else {

        // first check if opponents are close to the king tower and update the initial position of swordman and archer
        int numOfOpponentsNearKing = 0;
        for (int i = 0; i < m_pPlayer->getNumOpponentMobs(); i++) {
            if (m_pPlayer->getOpponentMob(i).m_Position.y <= NorthPrincessY + 2.f) {
                // left side
                if (m_pPlayer->getOpponentMob(i).m_Position.x <= KingX) {
                    leftSwordsmanPos.x = KingX - 1.f;
                    leftSwordsmanPos.y = NorthPrincessY - 2.f;
                    leftArcherPos.x = KingX - 1.f;
                    leftArcherPos.y = NorthPrincessY - 2.f;
                }
                // right side
                else {
                    rightSwordsmanPos.x = KingX + 2.f;
                    rightSwordsmanPos.y = NorthPrincessY - 2.f;
                    rightArcherPos.x = KingX + 2.f;
                    rightArcherPos.y = NorthPrincessY - 2.f;
                }
                numOfOpponentsNearKing++;
            }
        }
        // if there is no opponent near the king, redefine the initial position
        if (numOfOpponentsNearKing == 0) {
            leftSwordsmanPos.x = LEFT_BRIDGE_CENTER_X;
            leftSwordsmanPos.y = RIVER_TOP_Y - 1.0f;
            leftArcherPos.x = LEFT_BRIDGE_CENTER_X;
            leftArcherPos.y = NorthPrincessY;
            rightSwordsmanPos.x = RIGHT_BRIDGE_CENTER_X;
            rightSwordsmanPos.y = RIVER_TOP_Y - 1.0f;
            rightArcherPos.x = RIGHT_BRIDGE_CENTER_X;
            rightArcherPos.y = NorthPrincessY;
        }

        // calculate the cost difference to see which side has more enemies
        leftCostDiff = getLeftOpponentsCost(m_pPlayer) - getLeftCost(m_pPlayer);
        rightCostDiff = getRightOpponentsCost(m_pPlayer) - getRightCost(m_pPlayer);
        if (leftCostDiff >= rightCostDiff) {
            // the left side has less military power, deploy troop for defense
            if (leftCostDiff >= 0) {
                if (m_pPlayer->getElixir() > 7 && m_pPlayer->getElixir() <= 8 && getNumOfArcher() != 0 && getNumOfSwordsman() != 0) {
                    bool isNorth = m_pPlayer->isNorth();
                    Vec2 swordsmanPos_Game = leftSwordsmanPos.Player2Game(isNorth);
                    Vec2 archerPos_Game = leftArcherPos.Player2Game(isNorth);

                    m_pPlayer->placeMob(iEntityStats::Swordsman, swordsmanPos_Game);
                    m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
                    m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
                }
                if (m_pPlayer->getElixir() > 5 && getNumOfGiant() <= 1) {
                    bool isNorth = m_pPlayer->isNorth();
                    Vec2 giantPos_Game = leftGiantPos.Player2Game(isNorth);
                    m_pPlayer->placeMob(iEntityStats::Giant, giantPos_Game);
                }
                if (m_pPlayer->getElixir() > 4 && m_pPlayer->getElixir() <= 5 && getNumOfArcher() == 0) {
                    bool isNorth = m_pPlayer->isNorth();
                    Vec2 archerPos_Game = leftArcherPos.Player2Game(isNorth);

                    m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
                    m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
                }
                if (m_pPlayer->getElixir() > 3 && m_pPlayer->getElixir() <= 5 && getNumOfSwordsman() == 0) {
                    bool isNorth = m_pPlayer->isNorth();
                    Vec2 swordsmanPos_Game = leftSwordsmanPos.Player2Game(isNorth);
                    m_pPlayer->placeMob(iEntityStats::Swordsman, swordsmanPos_Game);
                }
            }
            // each lane has advantage, then deploy more troop on the better lane (right side) for pushing
            else {
                if (m_pPlayer->getElixir() > 7 && m_pPlayer->getElixir() <= 8 && getNumOfArcher() != 0 && getNumOfSwordsman() != 0) {
                    bool isNorth = m_pPlayer->isNorth();
                    Vec2 swordsmanPos_Game = rightSwordsmanPos.Player2Game(isNorth);
                    Vec2 archerPos_Game = rightArcherPos.Player2Game(isNorth);

                    m_pPlayer->placeMob(iEntityStats::Swordsman, swordsmanPos_Game);
                    m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
                    m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
                }
                if (m_pPlayer->getElixir() > 5 && getNumOfGiant() <= 1) {
                    bool isNorth = m_pPlayer->isNorth();
                    Vec2 giantPos_Game = rightGiantPos.Player2Game(isNorth);
                    m_pPlayer->placeMob(iEntityStats::Giant, giantPos_Game);
                }
                if (m_pPlayer->getElixir() > 4 && m_pPlayer->getElixir() <= 5 && getNumOfArcher() == 0) {
                    bool isNorth = m_pPlayer->isNorth();
                    Vec2 archerPos_Game = rightArcherPos.Player2Game(isNorth);

                    m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
                    m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
                }
                if (m_pPlayer->getElixir() > 3 && m_pPlayer->getElixir() <= 5 && getNumOfSwordsman() == 0) {
                    bool isNorth = m_pPlayer->isNorth();
                    Vec2 swordsmanPos_Game = rightSwordsmanPos.Player2Game(isNorth);
                    m_pPlayer->placeMob(iEntityStats::Swordsman, rightSwordsmanPos);
                }
            }
           
        }
        else {
            if (rightCostDiff >= 0) {
                if (m_pPlayer->getElixir() > 7 && m_pPlayer->getElixir() <= 8 && getNumOfArcher() != 0 && getNumOfSwordsman() != 0) {
                    bool isNorth = m_pPlayer->isNorth();
                    Vec2 swordsmanPos_Game = rightSwordsmanPos.Player2Game(isNorth);
                    Vec2 archerPos_Game = rightArcherPos.Player2Game(isNorth);

                    m_pPlayer->placeMob(iEntityStats::Swordsman, swordsmanPos_Game);
                    m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
                    m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
                }
                if (m_pPlayer->getElixir() > 5 && getNumOfGiant() <= 1) {
                    bool isNorth = m_pPlayer->isNorth();
                    Vec2 giantPos_Game = rightGiantPos.Player2Game(isNorth);
                    m_pPlayer->placeMob(iEntityStats::Giant, giantPos_Game);
                }
                if (m_pPlayer->getElixir() > 4 && m_pPlayer->getElixir() <= 5 && getNumOfArcher() == 0) {
                    bool isNorth = m_pPlayer->isNorth();
                    Vec2 archerPos_Game = rightArcherPos.Player2Game(isNorth);

                    m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
                    m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
                }
                if (m_pPlayer->getElixir() > 3 && m_pPlayer->getElixir() <= 5 && getNumOfSwordsman() == 0) {
                    bool isNorth = m_pPlayer->isNorth();
                    Vec2 swordsmanPos_Game = rightSwordsmanPos.Player2Game(isNorth);
                    m_pPlayer->placeMob(iEntityStats::Swordsman, rightSwordsmanPos);
                }
            }
            else {
                if (m_pPlayer->getElixir() > 7 && m_pPlayer->getElixir() <= 8 && getNumOfArcher() != 0 && getNumOfSwordsman() != 0) {
                    bool isNorth = m_pPlayer->isNorth();
                    Vec2 swordsmanPos_Game = leftSwordsmanPos.Player2Game(isNorth);
                    Vec2 archerPos_Game = leftArcherPos.Player2Game(isNorth);

                    m_pPlayer->placeMob(iEntityStats::Swordsman, swordsmanPos_Game);
                    m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
                    m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
                }
                if (m_pPlayer->getElixir() > 5 && getNumOfGiant() <= 1) {
                    bool isNorth = m_pPlayer->isNorth();
                    Vec2 giantPos_Game = leftGiantPos.Player2Game(isNorth);
                    m_pPlayer->placeMob(iEntityStats::Giant, giantPos_Game);
                }
                if (m_pPlayer->getElixir() > 4 && m_pPlayer->getElixir() <= 5 && getNumOfArcher() == 0) {
                    bool isNorth = m_pPlayer->isNorth();
                    Vec2 archerPos_Game = leftArcherPos.Player2Game(isNorth);

                    m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
                    m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
                }
                if (m_pPlayer->getElixir() > 3 && m_pPlayer->getElixir() <= 5 && getNumOfSwordsman() == 0) {
                    bool isNorth = m_pPlayer->isNorth();
                    Vec2 swordsmanPos_Game = leftSwordsmanPos.Player2Game(isNorth);
                    m_pPlayer->placeMob(iEntityStats::Swordsman, swordsmanPos_Game);
                }
            }
        }
    }

}

