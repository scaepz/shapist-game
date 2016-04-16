#ifndef KICKBACK_H
#define KICKBACK_H
class CKickBack
{
public:
	CKickBack();
	~CKickBack();
	void SetKickBack(int x, int y, int rot, int time);
	void Update(int time);
	int GetX();
	int GetY();
	int GetRotation();
	void NewKickback();
private:
	int relativeX;
	int relativeY;
	int relativeRotation;
	int time;
	int timeUntilNormal;
};

#endif