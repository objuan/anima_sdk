#pragma once

#include "../Modder.h"

#include "Anima_nsbegin.h"

class Animation;


class ANIMA_CORE_API PhaserModder : public Modder
{
public:

private:
	double phase_random_seed; // <1 disabled

	Animation *anim;

private:
	void Check();

public:
	PhaserModder(Animation *anim);
	virtual ~PhaserModder(void);

	virtual ModderChildType GetModderType(){return PhaserModderType;}
	virtual QColor GetMaterialColor();
	QImage Image(){return ResourceManager::Singleton.item_phaser;}
    virtual QString ClassDesc(){return "Phaser";}

	virtual void OnInit(){}
	virtual void OnDelete(){}

	double GetPhaseRandomSeed(){ return phase_random_seed;}
	void SetPhaseRandomSeed(double phase_random_seed){this->phase_random_seed=phase_random_seed;}

	virtual void Build();

	virtual void Load(QDomElement e);
	virtual void Save(QDomElement e);

};
#include "Anima_nsend.h"

