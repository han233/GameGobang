#ifndef COMMON_H
#define COMMON_H

#define HeaderWidth 45
#define Game_PieceSize 50
#define Game_ColorFlag 1
#define Game_JudgeSize 5

#include <QString>
#include <QPair>
#include <vector>

extern std::vector<QString> ColorName;
extern std::vector<QPair<int,int>> DirectionVector;

#endif // COMMON_H
