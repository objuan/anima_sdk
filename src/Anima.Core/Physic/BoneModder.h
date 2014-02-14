#pragma once

class Bone;
class IKLeg;

class BoneModder
{
public:

	int key;
	int level;
	Bone *rootBone;
	Bone *bone;

	IKLeg *leg;

private:
	void UpdateBone();

public:
	BoneModder(Bone *bone);
	~BoneModder();

	void KeyPressEvent(QKeyEvent *e);
	void Draw(int keyIdx);
};

