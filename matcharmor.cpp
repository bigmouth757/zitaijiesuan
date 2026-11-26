#include"core.h"
vector<Armor>MatchArmor(vector<Light>& all_light)
{
	vector<Armor> armorlist;
	for (int i = 0;i < all_light.size();i++)
	{
		for (int j = i + 1;j < all_light.size();j++)
		{
			Light L1 = all_light[i];
			Light L2 = all_light[j];
			if (L1.color != L2.color)
				continue;
			//if (L1.angle > 70)
				//continue;
			float angle3 = fabs(L1.angle - L2.angle);
			if (angle3 > 30)
				continue;
			float lengthmax = max(L1.length, L2.length);
			float lengthmini = min(L1.length, L2.length);
			float radio = lengthmax / lengthmini;
			if (radio > 1.2)
				continue;
			float dy = fabs(L1.center.y - L2.center.y);
			float avglen = (L1.length + L2.length) / 2;
			if (dy > avglen * 0.5)
				continue;
			float dx = fabs(L1.center.x - L2.center.x);
			if (dx < avglen * 1 || dx > avglen * 4.0f)
				continue;
			Light leftbar, rightbar;
			if (L1.center.x < L2.center.x)
			{
				leftbar = L1;
				rightbar = L2;
			}
			else
			{
				leftbar = L2;
				rightbar = L1;
			}
			Point2f leftpt[4];
			Point2f rightpt[4];
			leftbar.rotate_rect.points(leftpt);
			rightbar.rotate_rect.points(rightpt);
			Armor armor;
			armor.color = leftbar.color;
			armor.is_valid = true;
			armor.corner[0] = leftpt[1];
			armor.corner[1] = rightpt[0];
			armor.corner[2] = rightpt[3];
			armor.corner[3] = leftpt[2];
			armor.center.x = (armor.corner[0].x + armor.corner[1].x + armor.corner[2].x + armor.corner[3].x) / 4;
			armor.center.y = (armor.corner[0].y + armor.corner[1].y + armor.corner[2].y + armor.corner[3].y) / 4;
			vector < Point2f >corpt;
			for (int k = 0;k < 4;k++)
			{
				corpt.push_back(armor.corner[k]);
			}
			armor.bounding = boundingRect(corpt);
			armorlist.push_back(armor);

		}
	}
	return armorlist;
}