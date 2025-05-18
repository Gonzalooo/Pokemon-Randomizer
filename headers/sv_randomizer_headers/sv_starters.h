#ifndef SV_STARTERS_H
#define SV_STARTERS_H

#include "sv_shared_class.h"

class svStarters: public QObject, SVShared{
    Q_OBJECT
public:
    svStarters();
    ~svStarters();

    json startersMaps;
    QRandomGenerator* localRand;

    allowedPokemonLimiter startersPokemon; // connected
    QMap<int, QList<int>> usableStarterPokemon; // setUp
    bool randomizeStarters = false; // connected
    bool forceShinyStarter = false; // connected
    bool allStartersShiny = false; // connected
    bool randomizeStartersTeraTypes = false; // connected
    int startersRates = 10;
    QVector<QString> starters = {"", "", ""}; // connected
    QVector<int> startersForms = {0, 0, 0}; // connected
    QVector<QString> startersGenders = {"MALE", "MALE", "MALE"}; // connected
    QVector<QString> startersPokeballs = {"Poke Ball", "Poke Ball", "Poke Ball"}; // connected
    QVector<bool> startersShiny = {false, false, false}; // connected
    QVector<int> devIds;
    QVector<int> forms;
    QVector<int> genders;
    QVector<bool> rares;

    allowedPokemonLimiter giftsPokemon;
    QMap<int, QList<int>> usableGiftPokemon;
    bool randomizeGifts = false;
    bool randomizeGiftsTeraTypes = false;
    int giftsRates = 10;

    void randomize(QRandomGenerator* r);
    void obtainSelectedStarters(int index, QString starterName, int form, QString gender, bool shiny, QString ball);
};

#endif // SV_STARTERS_H
