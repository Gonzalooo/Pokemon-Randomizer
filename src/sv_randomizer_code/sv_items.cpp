#include "headers/sv_randomizer_headers/sv_items.h"

svItems::svItems(){
    itemDevs = readJsonQFile("SV_FLATBUFFERS/SV_ITEMS/pokemon_items_dev.json");
}

svItems::~svItems(){

}

void svItems::randomize(QRandomGenerator* r){
    pokemonDrops = readJsonQFile("SV_FLATBUFFERS/SV_ITEMS/dropitemdata_array_clean.json");
    letsGoItems = readJsonQFile("SV_FLATBUFFERS/SV_ITEMS/rummagingItemDataTable_array_clean.json");
    pickUpItems = readJsonQFile("SV_FLATBUFFERS/SV_ITEMS/monohiroiItemData_array_clean.json");
    localRand = r;
    setRandNum(localRand);

    if(randomizeHiddenItems == true){
        QList<QPair<QString, QString>> filesJson = {
            {"SV_ITEMS/hiddenItemDataTable_array_clean.json", "SV_ITEMS/hiddenItemDataTable_array.json"},
            {"SV_ITEMS/hiddenItemDataTable_su1_array_clean.json", "SV_ITEMS/hiddenItemDataTable_su1_array.json"},
            {"SV_ITEMS/hiddenItemDataTable_su2_array_clean.json", "SV_ITEMS/hiddenItemDataTable_su2_array.json"},
            {"SV_ITEMS/hiddenItemDataTable_lc_array_clean.json", "SV_ITEMS/hiddenItemDataTable_lc_array.json"},
        };

        QList<QPair<QString, QString>> filesSchema = {
            {"SV_ITEMS/hiddenItemDataTable_array.bfbs", "world/data/item/hiddenItemDataTable/"},
            {"SV_ITEMS/hiddenItemDataTable_su1_array.bfbs", "world/data/item/hiddenItemDataTable_su1/"},
            {"SV_ITEMS/hiddenItemDataTable_su2_array.bfbs", "world/data/item/hiddenItemDataTable_su2/"},
            {"SV_ITEMS/hiddenItemDataTable_lc_array.bfbs", "world/data/item/hiddenItemDataTable_lc/"},
        };

        for(int i=0; i<filesJson.size(); i++){
            qDebug()<<"Randomizing Hidden Items - "<<filesJson[i].first;
            hiddenItems = readJsonQFile("SV_FLATBUFFERS/"+filesJson[i].first);
            for(unsigned long long i =0; i<hiddenItems["values"].size(); i++){
                for(int j = 1; j<=10; j++){
                    int itemChoice = selectItem();

                    std::string itemName = itemDevs["items"][itemChoice]["devName"];
                    std::string itemKey = "item_" + std::to_string(j);
                    hiddenItems["values"][i][itemKey]["itemId"] = itemName;
                    hiddenItems["values"][i][itemKey]["emergePercent"] = localRand->bounded(100, 901);
                    hiddenItems["values"][i][itemKey]["dropCount"] = localRand->bounded(1, 21);
                }
            }

            closeFileAndDelete(filesJson[i].second,
                               filesSchema[i].first,
                               filesSchema[i].second,
                               hiddenItems, true);
        }
    }

    if(randomizePokemonDrops == true){
         qDebug()<<"Randomizing Pokemon Drops";
        for(unsigned long long i =0; i<pokemonDrops["values"].size(); i++){
            for(int j = 1; j<=10; j++){
                int itemChoice = selectItem();

                std::string itemName = itemDevs["items"][itemChoice]["devName"];
                pokemonDrops["values"][i]["item1"]["itemid"] = itemName;
            }
        }

        closeFileAndDelete("SV_ITEMS/dropitemdata_array.json",
                           "SV_ITEMS/dropitemdata_array.bfbs",
                           "world/data/item/dropitemdata/",
                           pokemonDrops, true);
    }

    if(randomizePickUpItems == true){
        qDebug()<<"Randomizing Pick Up Items";

        for(unsigned long long i =0; i<pickUpItems["values"].size(); i++){
            for(int j = 1; j<=30; j++){
                int itemChoice = selectItem();

                std::string itemName = itemDevs["items"][itemChoice]["devName"];
                std::string itemKey;
                if(j < 10){
                    itemKey = "item_0" + std::to_string(j);
                }else{
                    itemKey = "item_" + std::to_string(j);
                }
                pickUpItems["values"][i][itemKey]["itemId"] = itemName;
            }
        }

        closeFileAndDelete("SV_ITEMS/monohiroiItemData_array.json",
                           "SV_ITEMS/monohiroiItemData_array.bfbs",
                           "world/data/item/monohiroiItemData/",
                           pickUpItems, true);
    }

    if(randomizeLetsGoItems == true){
        qDebug()<<"Randomizing Synchro/Let's Go Items";
        for(unsigned long long i =0; i<letsGoItems["values"].size(); i++){
            for(int j = 0; j<=4; j++){
                int itemChoice = selectItem();

                std::string itemName = itemDevs["items"][itemChoice]["devName"];
                std::string itemKey;
                if(j < 10){
                    itemKey = "Item0" + std::to_string(j);
                }else{
                    itemKey = "Item" + std::to_string(j);
                }
                letsGoItems["values"][i][itemKey] = itemName;
            }
        }

        closeFileAndDelete("SV_ITEMS/rummagingItemDataTable_array.json",
                           "SV_ITEMS/rummagingItemDataTable_array.bfbs",
                           "world/data/item/rummagingItemDataTable/",
                           letsGoItems, true);
    }

    if(randomizeBallItem == true){
        qDebug()<<"Randomizing Poke Ball Items";
    }

}

int svItems::selectItem(){
    int itemChoice = localRand->bounded(1, 1091);
    while(itemDevs["items"][itemChoice]["ItemType"] == "ITEMTYPE_MATERIAL" ||
           itemDevs["items"][itemChoice]["ItemType"] == "ITEMTYPE_EVENT" ||
           itemDevs["items"][itemChoice]["ItemType"] == "ITEMTYPE_BATTLE" ||
           itemDevs["items"][itemChoice]["ItemType"] == "ITEMTYPE_POCKET" ||
           bannedItems.contains(itemDevs["items"][itemChoice]["id"])){
        itemChoice = localRand->bounded(1, 1091);
    }

    return itemChoice;
}
