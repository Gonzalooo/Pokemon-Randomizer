#ifndef SVRANDOMIZERCODE_H
#define SVRANDOMIZERCODE_H

#include <QString>
#include <QMap>
#include "sv_shared_class.h"
#include "sv_trainers.h"
#include "sv_raids.h"
#include "sv_wilds.h"
#include "sv_items.h"
#include "sv_personal.h"
#include "sv_starters.h"
#include "sv_fixed.h"
#include "sv_boss.h"


class SVRandomizerCode: public QObject, public SVShared
{
    Q_OBJECT

public:
    SVRandomizerCode();
    ~SVRandomizerCode();

    //General
    bool auto_patch = true; // Added
    bool kaizo_mode = false;       // Added
    unsigned int bulk_amount = 1;
    QString seed = "";
    QRandomGenerator randNum;

    svStarters svRandomizerStarters;
    svPersonal svRandomizerPersonal;
    svItems svRandomizerItems;
    svWilds svRandomizerWilds;
    svFixed svRandomizerFixed;
    svRaids svRandomizerRaids;
    svTrainers svRandomizerTrainers;
    svBoss svRandomizerBosses;
};

#endif // SVRANDOMIZERCODE_H
