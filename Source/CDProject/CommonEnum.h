#pragma once
#include "UObject/Class.h"

enum class EActions : char
{ Forward, Back, Left, Right, CameraTurnLeft, CameraTurnRight };

//��Ϸ��ʼʱ�·�ΪSouth,�Դ�����
enum class ECameraAbsLocations : char
{ South, West, North, East };

//�������༭��
UENUM()
enum class EObjectTags : uint8
{ Rule=0, Baba=1, Tree=2, Water=3 };

UENUM()
enum class ERuleTags : uint8
{Baba=1, Tree=2, Water=3, is=200, You=101, Push=102, Win=103, Defeat=104};