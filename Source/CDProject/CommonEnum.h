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
{ Rule, Baba, Tree, Water };

UENUM()
enum class ERuleTags : uint8
{Baba, Tree, Water, is, You};