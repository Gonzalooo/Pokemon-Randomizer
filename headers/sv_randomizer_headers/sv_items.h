#ifndef SV_ITEMS_H
#define SV_ITEMS_H

#include "sv_shared_class.h"

class svItems: public QObject, SVShared{
    Q_OBJECT

    public:
        svItems();
        ~svItems();

        json pokemonDrops;
        json letsGoItems;
        json pickUpItems;
        json hiddenItems;
        json itemDevs;
        QRandomGenerator* localRand;

        bool randomizeItems = false;
        bool randomizeHiddenItems = false;
        bool randomizePokemonDrops = false;
        bool randomizePickUpItems = false;
        bool randomizeLetsGoItems = false;
        bool randomizeBallItem = false;

        void randomize(QRandomGenerator* r);
        int selectItem();
    private:
        // void randomizeBallItems();
};

#endif // SV_ITEMS_H
