#ifndef SV_RAIDS_H
#define SV_RAIDS_H

#include "sv_shared_class.h"

class svRaids: public QObject, public SVShared{
    Q_OBJECT
    public:
        svRaids();
        ~svRaids();

        bool randomizePaldea = false;
        bool paldeaForAll = false;
        int shinyRatePaldea = 10;
        bool forceShinyRaidsPaldea = false;
        allowedPokemonLimiter paldeaPokemon;

        bool randomizeKitakami = false;
        int shinyRateKitakami = 10;
        bool forceShinyRaidsKitakami = false;
        allowedPokemonLimiter kitakamiPokemon;

        bool randomizeBlueberry = false;
        int shinyRateBlueberry = 10;
        bool forceShinyRaidsBlueberry = false;
        allowedPokemonLimiter blueberryPokemon;

        bool forNoWarning = false;

        QMap<int, QList<int>> allowedPokemon;

        void randomize(QStringList paths, QStringList clean, QStringList schema, QStringList folder, QString region,
            QRandomGenerator* r);
};

#endif // SV_RAIDS_H
