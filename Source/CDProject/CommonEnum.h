#pragma once
#include "UObject/Class.h"
#include "CommonEnum.generated.h"

const float Layer1Z = 100.0f;
const float Layer2Z = 500.0f;

enum class EActions : char
{ Forward, Back, Left, Right, Wait, FlyOrFall, CameraTurnLeft, CameraTurnRight };

//游戏开始时下方为South,以此类推
enum class ECameraAbsLocations : char
{ South, West, North, East };

//公开到编辑器
UENUM()
enum class EObjectTags : uint8
{ Rule=0, Baba=1, Tree=2, Water=3 ,Bush=4, Gold=5, Trap=6, Box =7, Fox=8, Cloud=9, Fire=10, Heart=11};

UENUM()
enum class ERuleTags : uint8
{Null=0, Baba=1, Tree=2, Water=3, Bush=4, Gold=5, Trap = 6, Box = 7, Fox = 8, Cloud = 9, Fire = 10, Heart = 11, is=100, You=101, Push=102, Win=103, Defeat=104, Move=105, Sink=106, Slide=107, Fly=108};

inline bool isObject(ERuleTags tag) {
	return static_cast<int>(tag) < 100 && static_cast<int>(tag) > 0;
}

inline bool isAttribute(ERuleTags tag) {
	return static_cast<int>(tag) > 100;
}

inline bool isIs(ERuleTags tag) {
	return static_cast<int>(tag) == 100;
}