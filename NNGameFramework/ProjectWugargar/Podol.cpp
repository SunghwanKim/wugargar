#include "Podol.h"


CPodol::CPodol(void)
{
}


CPodol::~CPodol(void)
{
}

CZombie * CPodol::GetCloserEnemy()
{
	int x = this->GetPosition_x();
	int y = this->GetPosition_y();
	CZombie *tmp_target;

	//tmp_target�� ��ġ �ʱ�ȭ

	for(int idx=0;idx<num_zombie_unit; ++idx)
	{
		if((podol_list[idx].GetPosition_x() - x) /*<= (tmp_target->GetPosition_x() - x)*/)
			tmp_target = podol_list[idx];
	}

	return tmp_target;

}
