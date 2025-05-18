#ifndef SV_FIXED_H
#define SV_FIXED_H

#include "sv_shared_class.h"

class svFixed: public QObject, SVShared{
    Q_OBJECT
public:
    svFixed();
    ~svFixed();

    json fixedEncounters;
    QRandomGenerator* localRand;

    bool randomizeFixedEncounters = false;
    bool similarBST = false;
    bool keepSameTera = false;
    int shinyRate = 10;

    allowedPokemonLimiter fixedEncountersPokemon;
    QMap<int, QList<int>> allowedPokemon;


    void randomize(QRandomGenerator* r);

};

#endif // SV_FIXED_H
