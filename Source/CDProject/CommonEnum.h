#pragma once
#include "UObject/Class.h"

enum class EActions : char
{ Forward, Back, Left, Right, CameraTurnLeft, CameraTurnRight };

//游戏开始时下方为South,以此类推
enum class ECameraAbsLocations : char
{ South, West, North, East };

//公开到编辑器
UENUM()
enum class EObjectTags : uint8
{ Rule, Baba, Tree, Water };

UENUM()
enum class ERuleTags : uint8
{Baba, Tree, Water, is, You};