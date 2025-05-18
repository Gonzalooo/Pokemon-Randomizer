#ifndef SV_BOSS_H
#define SV_BOSS_H

#include "sv_shared_class.h"

class svBoss: public QObject, public SVShared{
    Q_OBJECT

    public:
        json cleanBossData;
        json trainerInfo;
        QRandomGenerator* localRand;

        bool randomize_bosses = false;
        allowedPokemonLimiter BossLimiter;
        QMap<int, QList<int>> allowedPokemon;
        void randomizeBosses(QRandomGenerator* r);
        void randomizeFight(unsigned long long index);
        void randomizeStellarOgerpon(unsigned long long index);
        void copyFight(unsigned long long indexSet, unsigned long long indexCopy);
        void copyStellarOgerpon(unsigned long long indexSet, unsigned long long indexCopy);
        void patchMultiBattle();
        void patchGimmighoul();
        void patchGimmighoulRoaming();
        void patchLeChonk();
        void patchHoundoom();
        void patchSunflora();
        void patchDonzoTitan();
        void patchOrthwormTitan();
        void patchBombariderTitan();
        void patchKlawfTitan();
        void patchGreatIronTitan();
        void patchTingLu();
        void patchChienPao();
        void patchWoChien();
        void patchChiYu();
        void patchLoyalThreeOgerpon();
        void patchKitakami();
        void patchStellarUnderdeepths();
        void patchTerapagos();
        void patchAreaZeroLegends();
        void patchSnacksworths();
        void obtainPokemonScene(int &dev, int &form, int& gender, int &rare);
        void saveIndividualPokemon(int index, QVector<int> &dev, QVector<int> &form, QVector<int> &gender, QVector<bool> &rare);
        void savePokemonFromTrainer(int id, QString& pokeKey,
                                    QVector<int> &dev, QVector<int> &form, QVector<int> &gender, QVector<bool> &rare);

};



#endif // SV_BOSS_H
