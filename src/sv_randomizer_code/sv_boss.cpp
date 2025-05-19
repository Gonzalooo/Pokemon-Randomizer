#include "headers/sv_randomizer_headers/sv_boss.h"

void svBoss::saveIndividualPokemon(int index, QVector<int> &dev, QVector<int> &form, QVector<int> &genderV, QVector<bool> &rareV){
    dev.push_back(pokemonMapping["pokemons_dev"][cleanBossData["values"][index]["pokeData"]["devId"]]["devid"]);
    form.push_back(cleanBossData["values"][index]["pokeData"]["formId"]);
    QString gender = QString::fromStdString(cleanBossData["values"][index]["pokeData"]["sex"]);
    if(gender != "FEMALE"){
        genderV.push_back(0);
    }
    else{
        genderV.push_back(1);
    }

    QString rare = QString::fromStdString(cleanBossData["values"][index]["pokeData"]["rareType"]);
    if(gender != "RARE"){
        rareV.push_back(0);
    }
    else{
        rareV.push_back(1);
    }
}

void svBoss::savePokemonFromTrainer(int index, QString& pokeKey, QVector<int> &dev, QVector<int> &form, QVector<int> &genderV, QVector<bool> &rareV){
    dev.push_back(pokemonMapping["pokemons_dev"][trainerInfo["values"][index][pokeKey.toStdString()]["devId"].get<std::string>().c_str()]["devid"]);
    form.push_back(trainerInfo["values"][index][pokeKey.toStdString()]["formId"]);
    QString gender = QString::fromStdString(trainerInfo["values"][index][pokeKey.toStdString()]["sex"].get<std::string>().c_str());
    if(gender != "FEMALE"){
        genderV.push_back(0);
    }
    else{
        genderV.push_back(1);
    }

    QString rare = QString::fromStdString(trainerInfo["values"][index][pokeKey.toStdString()]["rareType"].get<std::string>().c_str());
    if(gender != "RARE"){
        rareV.push_back(0);
    }
    else{
        rareV.push_back(1);
    }
}

void svBoss::obtainPokemonScene(int &dev, int &form, int& gender, int &rare){
    dev = localRand->bounded(1, maxAllowedId);
    while(!allowedPokemon.contains(pokemonMapping["pokemons"][dev]["natdex"]))
        dev = localRand->bounded(1, maxAllowedId);

    form = localRand->bounded(0, static_cast<int>(pokemonMapping["pokemons"][dev]["forms"].size()));
    while(!allowedPokemon[dev].contains(form)){
        form = localRand->bounded(0, static_cast<int>(pokemonMapping["pokemons"][dev]["forms"].size()));
    }

    QString form_Check = QString::fromUtf8(pokemonMapping["pokemons"][dev]["name"].get<std::string>().c_str());
    if(maleOnlyPokemon.contains(form_Check) || genderlessPokemon.contains(form_Check)){
        if(genderlessPokemon.contains(form_Check)){
            gender = 0;
        }else{
            gender = 0;
        }
    }else if(femaleOnlyPokemon.contains(form_Check)){
        gender = 1;
    } else if(genderForms.contains(form_Check)){
        if(form == 0){
            gender = 0;
        }else{
            gender = 1;
        }
    }else{
        int rand_gender = localRand->bounded(1, 101);
        if(rand_gender > int(pokemonPersonalData["entry"][int(pokemonMapping["pokemons"][dev]["devid"])]["gender"]["ratio"])){
            gender = 0;
        }else{
            gender = 1;
        }
    }

    int val = localRand->bounded(1,100); // range is [1, shiny_starter_rate)
    if(val == 1){
        rare = 1;
    }
    else{
        rare = 0;
    }

    //set dev to devid
    dev = pokemonMapping["pokemons"][dev]["devid"];

}

void svBoss::patchMultiBattle(){
    QVector<int> devIdBoss;
    QVector<int> formIdBoss;
    QVector<int> genderBoss;
    QVector<bool> rareBoss;
    // Glimmora - 0 - 1055
    saveIndividualPokemon(0, devIdBoss, formIdBoss, genderBoss, rareBoss);

    QList<QPair<QString, QString>> files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_AZ/WayHome/common_1055_main")},
    };

    QList<QString> outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/common_1055_/",
    };

    qDebug()<<"Patching Glimmora - AZ(1) Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    files.clear();

    // Scream Tails - 1 - 1075
    // Iron Bundle - 2 - 1075
    saveIndividualPokemon(1, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(2, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_AZ/WayHome/common_1075_main")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/common_1075_/",
    };

    qDebug()<<"Patching Scream Tails/Iron Bundle Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    files.clear();

    // Great Tusk - 3 - 1095
    // Iron Treads - 4 - 1095
    saveIndividualPokemon(3, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(4, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files = {
             {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_AZ/WayHome/common_1095_main")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/common_1095_/",
    };

    qDebug()<<"Patching Great Tusk/Iron Treads Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    files.clear();


    // 6 6 5 6 6 7 6 5 - 1170
    saveIndividualPokemon(5, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(6, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(7, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(6, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(6, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(5, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(6, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(6, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files = {
             {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_AZ/WayHome/common_1170_always")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/common_1170_/",
    };

    qDebug()<<"Patching AZ Big Fight - Scarlet Scene Files";
    patchOneScene(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss, "0");

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    files.clear();

    // 9 9 8 9 9 10 8 9 -- 1170
    saveIndividualPokemon(8, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(9, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(10, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(9, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(9, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(8, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(9, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(9, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files = {
             {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_AZ/WayHome/common_1170_always")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/common_1170_/",
    };

    qDebug()<<"Patching AZ Big Fight - Violet Scene Files";
    patchOneScene(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss, "1");

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    files.clear();

}

void svBoss::patchGimmighoulRoaming(){
    QVector<int> devIdBoss;
    QVector<int> formIdBoss;
    QVector<int> genderBoss;
    QVector<bool> rareBoss;
    // Gimmighoul - 11
    saveIndividualPokemon(11, devIdBoss, formIdBoss, genderBoss, rareBoss);

    QList<QPair<QString, QString>> files = {
             {qBaseDir.filePath("SV_SCENES/trsot.fbs"), qBaseDir.filePath("SV_SCENES/MISC/Gimmighoul/coin_symbol_walk")},
    };

    QList<QString> outputs = {
        "world/obj_template/parts/coin_symbol/coin_symbol_walk_/",
    };

    qDebug()<<"Patching Gimmighoul Walk Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

}

void svBoss::patchGimmighoul(){
    QVector<int> devIdBoss;
    QVector<int> formIdBoss;
    QVector<int> genderBoss;
    QVector<bool> rareBoss;
    // Gimmighoul - 11
    saveIndividualPokemon(11, devIdBoss, formIdBoss, genderBoss, rareBoss);

    QList<QPair<QString, QString>> files = {
             {qBaseDir.filePath("SV_SCENES/trsot.fbs"), qBaseDir.filePath("SV_SCENES/MISC/Gimmighoul/coin_symbol_box")},
             };

    QList<QString> outputs = {
        "world/obj_template/parts/coin_symbol/coin_symbol_box_/",
    };

    qDebug()<<"Patching Gimmighoul Chest Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
}

void svBoss::patchLeChonk(){
    QVector<int> devIdBoss;
    QVector<int> formIdBoss;
    QVector<int> genderBoss;
    QVector<bool> rareBoss;

    // Set Lechonk
    for(int i =0; i<=4; i++){
        devIdBoss.push_back(pokemonMapping["pokemons_dev"][cleanBossData["values"][24]["pokeData"]["devId"]]["devid"]);
        formIdBoss.push_back(cleanBossData["values"][24]["pokeData"]["formId"]);
        QString gender = QString::fromStdString(cleanBossData["values"][24]["pokeData"]["sex"]);
        if(gender != "FEMALE"){
            genderBoss.push_back(0);
        }
        else{
            genderBoss.push_back(1);
        }

        QString rare = QString::fromStdString(cleanBossData["values"][24]["pokeData"]["rareType"]);
        if(gender != "RARE"){
            rareBoss.push_back(0);
        }
        else{
            rareBoss.push_back(1);
        }
    }

    // Tarantoula
    int devid = 1;
    int form = 0;
    int gender = 0;
    int rare = 0;
    obtainPokemonScene(devid, form, gender, rare);

    for(int i = 5; i<=8; i++){
        devIdBoss.push_back(devid);
        formIdBoss.push_back(form);
        genderBoss.push_back(gender);
        rareBoss.push_back(rare);
    }

    //Pawmi - 9
    obtainPokemonScene(devid, form, gender, rare);
    devIdBoss.push_back(devid);
    formIdBoss.push_back(form);
    genderBoss.push_back(gender);
    rareBoss.push_back(rare);

    // Fletchling
    obtainPokemonScene(devid, form, gender, rare);
    for(int i = 10; i<=11; i++){
        devIdBoss.push_back(devid);
        formIdBoss.push_back(form);
        genderBoss.push_back(gender);
        rareBoss.push_back(rare);
    }

    QList<QPair<QString, QString>> files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_FIRST_ROUTE/common_0100_main")},
    };

    QList<QString> outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/common_0100_/",
    };

    qDebug()<<"Patching Opening Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

}

void svBoss::patchHoundoom(){
    QVector<int> devIdBoss;
    QVector<int> formIdBoss;
    QVector<int> genderBoss;
    QVector<bool> rareBoss;
    // Houndoom - 25 - 1050
    saveIndividualPokemon(25, devIdBoss, formIdBoss, genderBoss, rareBoss);

    QList<QPair<QString, QString>> files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/MISC/Houndoom/common_0150_main")},
    };

    QList<QString> outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/common_0150_/",
    };

    qDebug()<<"Patching Houndoom Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    files.clear();
}

void svBoss::patchSunflora(){
    QVector<int> devIdBoss;
    QVector<int> formIdBoss;
    QVector<int> genderBoss;
    QVector<bool> rareBoss;

    // Sunflora - 26
    for(int i =0; i<=29; i ++)
        saveIndividualPokemon(26, devIdBoss, formIdBoss, genderBoss, rareBoss);

    QList<QPair<QString, QString>> files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/MISC/Sunflora/pokes")},
    };

    QList<QString> outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/gym_kusa_poke_finding_/",
    };

    qDebug()<<"Patching Sunflora Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    files.clear();

}

void svBoss::patchDonzoTitan(){
    QVector<int> devIdBoss;
    QVector<int> formIdBoss;
    QVector<int> genderBoss;
    QVector<bool> rareBoss;

    // Donzo - 33
    saveIndividualPokemon(33, devIdBoss, formIdBoss, genderBoss, rareBoss);

    QList<QPair<QString, QString>> files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/nushi_dragon_010_always")},
        {qBaseDir.filePath("SV_SCENES/trsot.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1035_010")},
        {qBaseDir.filePath("SV_SCENES/trsot.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1035_020")}
    };

    QList<QString> outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/nushi_dragon_010_/",
        "world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1035_010_/",
        "world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1035_020_/"
    };

    qDebug()<<"Patching Dondozo Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    files.clear();

    // Tatsugiri - 37
    saveIndividualPokemon(37, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files = {
        {qBaseDir.filePath("SV_SCENES/trsot.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1056_010")},
        {qBaseDir.filePath("SV_SCENES/trsot.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1056_020")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/sub_040_pre_start")}
    };

    outputs = {
        "world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1056_010_/",
        "world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1056_020_/",
        "world/scene/parts/event/event_scenario/sub_scenario/sub_040_/"
    };

    qDebug()<<"Patching Tatsugiri-1 Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    for(int i =0; i<=25; i++)
        saveIndividualPokemon(37, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/nushi_dragon_010_pre_start")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/nushi_dragon_010_/",
    };

    qDebug()<<"Patching Tatsugiri-2 Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    saveIndividualPokemon(33, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(37, devIdBoss, formIdBoss, genderBoss, rareBoss);
    QString pokeKy = "poke1";
    savePokemonFromTrainer(435, pokeKy, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files = {
             {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/nushi_dragon_020_pre_start")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/nushi_dragon_020_/",
    };

    qDebug()<<"Patching Tatsugiri-3 Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    files.clear();
}

void svBoss::patchOrthwormTitan(){
    QVector<int> devIdBoss;
    QVector<int> formIdBoss;
    QVector<int> genderBoss;
    QVector<bool> rareBoss;

    // Orthworm - 35
    saveIndividualPokemon(35, devIdBoss, formIdBoss, genderBoss, rareBoss);

    QList<QPair<QString, QString>> files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Orthworm/nushi_hagane_010_pre_start")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Orthworm/nushi_hagane_020_pre_start")},
        {qBaseDir.filePath("SV_SCENES/trsot.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Orthworm/nushi_hagane_fp_1048_010")},
        {qBaseDir.filePath("SV_SCENES/trsot.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Orthworm/nushi_hagane_fp_1048_020")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Orthworm/sub_039_pre_start")},
    };

    QList<QString> outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/nushi_hagane_010_/",
        "world/scene/parts/event/event_scenario/main_scenario/nushi_hagane_020_/",
        "world/obj_template/parts/nushi/hagane/nushi_hagane_fp_1048_010_/",
        "world/obj_template/parts/nushi/hagane/nushi_hagane_fp_1048_020_/",
        "world/scene/parts/event/event_scenario/sub_scenario/sub_039_/",
    };

    qDebug()<<"Patching Orthworm Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    files.clear();
}

void svBoss::patchBombariderTitan(){
    QVector<int> devIdBoss;
    QVector<int> formIdBoss;
    QVector<int> genderBoss;
    QVector<bool> rareBoss;

    // Bombardier - 41
    saveIndividualPokemon(41, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(41, devIdBoss, formIdBoss, genderBoss, rareBoss);

    QList<QPair<QString, QString>> files = {
        {qBaseDir.filePath("SV_SCENES/trscn.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Bombirdier/HikoNushi")},
    };

    QList<QString> outputs = {
        "world/scene/parts/field/field_contents/nushi/hiko/HikoNushi_/",
    };

    qDebug()<<"Patching Bombardier - 1 Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    saveIndividualPokemon(41, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Bombirdier/nushi_hikou_010_pre_start")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Bombirdier/nushi_hikou_020_pre_start")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Bombirdier/nushi_hikou_020_main")},
        {qBaseDir.filePath("SV_SCENES/trsot.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Bombirdier/nushi_hiko_fp_1063_010")},
        {qBaseDir.filePath("SV_SCENES/trsot.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Bombirdier/nushi_hiko_fp_1063_020")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Bombirdier/sub_038_pre_start")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/nushi_hikou_010_/",
        "world/scene/parts/event/event_scenario/main_scenario/nushi_hikou_020_/",
        "world/scene/parts/event/event_scenario/main_scenario/nushi_hikou_020_/",
        "world/obj_template/parts/nushi/hiko/nushi_hiko_fp_1063_010_/",
        "world/obj_template/parts/nushi/hiko/nushi_hiko_fp_1063_020_/",
        "world/scene/parts/event/event_scenario/sub_scenario/sub_038_/"
    };

    qDebug()<<"Patching Bombardier - 2 Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    files.clear();
}

void svBoss::patchKlawfTitan(){
    QVector<int> devIdBoss;
    QVector<int> formIdBoss;
    QVector<int> genderBoss;
    QVector<bool> rareBoss;

    // Klawf - 43
    saveIndividualPokemon(43, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(43, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(43, devIdBoss, formIdBoss, genderBoss, rareBoss);

    QList<QPair<QString, QString>> files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Klawf/nushi_iwa_010_pre_start")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Klawf/nushi_iwa_020_pre_start")},
        {qBaseDir.filePath("SV_SCENES/trsot.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Klawf/nushi_iwa_fp_1066")},
        {qBaseDir.filePath("SV_SCENES/trsot.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Klawf/nushi_iwa_fp_1066_020")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Klawf/sub_037_pre_start")},
    };

    QList<QString> outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/nushi_iwa_010_/",
        "world/scene/parts/event/event_scenario/main_scenario/nushi_iwa_020_/",
        "world/obj_template/parts/nushi/iwa/nushi_iwa_fp_1066_/",
        "world/obj_template/parts/nushi/iwa/nushi_iwa_fp_1066_020_/",
        "world/scene/parts/event/event_scenario/sub_scenario/sub_037_/"
    };

    qDebug()<<"Patching Klawf Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    files.clear();
}

void svBoss::patchGreatIronTitan(){
    QVector<int> devIdBoss;
    QVector<int> formIdBoss;
    QVector<int> genderBoss;
    QVector<bool> rareBoss;

    // Great Tusk - 45
    // Iron Treads - 47
    saveIndividualPokemon(47, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(45, devIdBoss, formIdBoss, genderBoss, rareBoss);

    QList<QPair<QString, QString>> files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Great_Iron/nushi_jimen_010_pre_start")},
    };

    QList<QString> outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/nushi_jimen_010_/",
    };

    qDebug()<<"Patching Great Tusk/Iron Treads Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    saveIndividualPokemon(45, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(47, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Great_Iron/nushi_jimen_020_pre_start")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/nushi_jimen_020_/",
    };

    qDebug()<<"Patching Great Tusk/Iron Treads Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    // Great Tusk Only
    saveIndividualPokemon(45, devIdBoss, formIdBoss, genderBoss, rareBoss);
    files = {
        {qBaseDir.filePath("SV_SCENES/trsot.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Great_Iron/nushi_jimen_fp_1082_010")},
        {qBaseDir.filePath("SV_SCENES/trsot.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Great_Iron/nushi_jimen_fp_1082_020")},
    };

    outputs = {
        "world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1082_010_/",
        "world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1082_020_/",
    };

    qDebug()<<"Patching Great Tusk Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files.clear();

    files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Great_Iron/sub_041_pre_start")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/sub_scenario/sub_041_/",
    };

    qDebug()<<"Patching Great Tusk - Scarlet Scene Files";
    patchOneScene(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss, "0");

    // Iron Treads Only
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    saveIndividualPokemon(47, devIdBoss, formIdBoss, genderBoss, rareBoss);
    files = {
             {qBaseDir.filePath("SV_SCENES/trsot.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Great_Iron/nushi_jimen_fp_1090_010")},
             {qBaseDir.filePath("SV_SCENES/trsot.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Great_Iron/nushi_jimen_fp_1090_020")},
             };

    outputs = {
        "world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1090_010_/",
        "world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1090_020_/",
    };

    qDebug()<<"Patching Iron Tread Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files.clear();

    files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_TITANS/Great_Iron/sub_041_pre_start")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/sub_scenario/sub_041_/",
    };

    qDebug()<<"Patching Iron Tread - Violet Scene Files";
    patchOneScene(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss, "1");

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    files.clear();
}

void svBoss::patchTingLu(){
    QVector<int> devIdBoss;
    QVector<int> formIdBoss;
    QVector<int> genderBoss;
    QVector<bool> rareBoss;

    // Bombardier - 49
    saveIndividualPokemon(49, devIdBoss, formIdBoss, genderBoss, rareBoss);
    QList<QPair<QString, QString>> files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_LEGENDS/Treasures/sub_014_main")},
    };

    QList<QString> outputs = {
        "world/scene/parts/event/event_scenario/sub_scenario/sub_014_/",
    };

    qDebug()<<"Patching Ting Lu Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    files.clear();
}

void svBoss::patchChienPao(){
    QVector<int> devIdBoss;
    QVector<int> formIdBoss;
    QVector<int> genderBoss;
    QVector<bool> rareBoss;

    // Bombardier - 49
    saveIndividualPokemon(50, devIdBoss, formIdBoss, genderBoss, rareBoss);
    QList<QPair<QString, QString>> files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_LEGENDS/Treasures/sub_015_main")},
    };

    QList<QString> outputs = {
        "world/scene/parts/event/event_scenario/sub_scenario/sub_015_/",
    };

    qDebug()<<"Patching Chien Pao Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    files.clear();
}

void svBoss::patchWoChien(){
    QVector<int> devIdBoss;
    QVector<int> formIdBoss;
    QVector<int> genderBoss;
    QVector<bool> rareBoss;

    // Bombardier - 49
    saveIndividualPokemon(51, devIdBoss, formIdBoss, genderBoss, rareBoss);
    QList<QPair<QString, QString>> files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_LEGENDS/Treasures/sub_016_main")},
    };

    QList<QString> outputs = {
        "world/scene/parts/event/event_scenario/sub_scenario/sub_016_/",
    };

    qDebug()<<"Patching Wo Chien Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    files.clear();
}

void svBoss::patchChiYu(){
    QVector<int> devIdBoss;
    QVector<int> formIdBoss;
    QVector<int> genderBoss;
    QVector<bool> rareBoss;

    // Bombardier - 49
    saveIndividualPokemon(52, devIdBoss, formIdBoss, genderBoss, rareBoss);
    QList<QPair<QString, QString>> files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_LEGENDS/Treasures/sub_017_main")},
    };

    QList<QString> outputs = {
        "world/scene/parts/event/event_scenario/sub_scenario/sub_017_/",
    };

    qDebug()<<"Patching Chien Pao Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    files.clear();
}

void svBoss::copyStellarOgerpon(unsigned long long indexSet, unsigned long long indexCopy){
    cleanBossData["values"][indexSet]["pokeData"]["devId"] = cleanBossData["values"][indexCopy]["pokeData"]["devId"];
    cleanBossData["values"][indexSet]["pokeData"]["formId"] = cleanBossData["values"][indexCopy]["pokeData"]["formId"];
    cleanBossData["values"][indexSet]["pokeData"]["sex"] = cleanBossData["values"][indexCopy]["pokeData"]["sex"];
    cleanBossData["values"][indexSet]["pokeData"]["rareType"] = cleanBossData["values"][indexCopy]["pokeData"]["rareType"];
    cleanBossData["values"][indexSet]["pokeData"]["item"] = cleanBossData["values"][indexCopy]["pokeData"]["item"];
    cleanBossData["values"][indexSet]["pokeData"]["wazaType"] = "DEFAULT";
    for(int i = 1; i<=4; i++){
        QString key = "waza"+QString::fromStdString(std::to_string(i));
        cleanBossData["values"][indexSet]["pokeData"][key.toStdString()]["wazaId"] = "WAZA_NULL";
    }
}

void svBoss::copyFight(unsigned long long indexSet, unsigned long long indexCopy){
    cleanBossData["values"][indexSet]["pokeData"]["devId"] = cleanBossData["values"][indexCopy]["pokeData"]["devId"];
    cleanBossData["values"][indexSet]["pokeData"]["formId"] = cleanBossData["values"][indexCopy]["pokeData"]["formId"];
    cleanBossData["values"][indexSet]["pokeData"]["sex"] = cleanBossData["values"][indexCopy]["pokeData"]["sex"];
    cleanBossData["values"][indexSet]["pokeData"]["rareType"] = cleanBossData["values"][indexCopy]["pokeData"]["rareType"];
    cleanBossData["values"][indexSet]["pokeData"]["item"] = cleanBossData["values"][indexCopy]["pokeData"]["item"];
    cleanBossData["values"][indexSet]["pokeData"]["wazaType"] = "DEFAULT";
    for(int i = 1; i<=4; i++){
        QString key = "waza"+QString::fromStdString(std::to_string(i));
        cleanBossData["values"][indexSet]["pokeData"][key.toStdString()]["wazaId"] = "WAZA_NULL";
    }
    cleanBossData["values"][indexSet]["pokeData"]["gemType"] = cleanBossData["values"][indexCopy]["pokeData"]["gemType"];
}

void svBoss::randomizeFight(unsigned long long index){
    if(index == 31 || index == 32 ||cleanBossData["values"][index]["label"] == "SDC02_0310_kodaikame" || cleanBossData["values"][index]["label"] == "SDC02_0330_kodaikame"){
        int random = localRand->bounded(1, maxAllowedId);
        while(!allowedPokemon.contains(pokemonMapping["pokemons"][random]["natdex"]))
            random = localRand->bounded(1, maxAllowedId);;

        int formRandom = 0;

        if(random != 1017){
            formRandom = localRand->bounded(0, static_cast<int>(pokemonMapping["pokemons"][random]["forms"].size()));
            while(!allowedPokemon[random].contains(formRandom)){
                formRandom = localRand->bounded(0, static_cast<int>(pokemonMapping["pokemons"][random]["forms"].size()));
            }

            cleanBossData["values"][index]["pokeData"]["devId"] = pokemonMapping["pokemons"][random]["devName"];

            // Set Starter Form
            cleanBossData["values"][index]["pokeData"]["formId"] = formRandom;
        }else{
            cleanBossData["values"][index]["pokeData"]["devId"] = 1011;
            cleanBossData["values"][index]["pokeData"]["formId"] = 3;
        }

        QString genderStd = "";

        QString form_Check = QString::fromUtf8(pokemonMapping["pokemons"][random]["name"].get<std::string>().c_str());
        if(maleOnlyPokemon.contains(form_Check) || genderlessPokemon.contains(form_Check)){

            if(genderlessPokemon.contains(form_Check)){
                genderStd = "DEFAULT";
            }else{
                genderStd = "MALE";
            }
        }else if(femaleOnlyPokemon.contains(form_Check)){
            genderStd = "FEMALE";
        } else if(genderForms.contains(form_Check)){
            if(formRandom == 0){

                genderStd = "MALE";
            }else{

                genderStd = "FEMALE";
            }
        }else{
            int rand_gender = localRand->bounded(1, 101);
            if(rand_gender > int(pokemonPersonalData["entry"][int(pokemonMapping["pokemons"][random]["devid"])]["gender"]["ratio"])){
                genderStd = "MALE";
            }else{

                genderStd = "FEMALE";
            }
        }

        cleanBossData["values"][index]["pokeData"]["sex"] = genderStd.toStdString();
        int val = localRand->bounded(1,100); // range is [1, shiny_starter_rate)
        if(val == 1){
            cleanBossData["values"][index]["pokeData"]["rareType"] = "RARE";
        }
        else{
            cleanBossData["values"][index]["pokeData"]["rareType"] = "NO_RARE";
        }
        std::string itemForPokemon = getPokemonItemId(pokemonMapping["pokemons"][random]["natdex"], formRandom);
        cleanBossData["values"][index]["pokeData"]["item"] = itemForPokemon;


        cleanBossData["values"][index]["pokeData"]["wazaType"] = "MANUAL";
        cleanBossData["values"][index]["pokeData"]["waza1"]["wazaId"] = "WAZA_TURUGINOMAI";
        cleanBossData["values"][index]["pokeData"]["waza2"]["wazaId"] = "WAZA_WARUDAKUMI";
        cleanBossData["values"][index]["pokeData"]["waza3"]["wazaId"] = "WAZA_ONIKANABOO";
        if(random == 1024){
            cleanBossData["values"][index]["pokeData"]["waza4"]["wazaId"] = "WAZA_TERAKURASUTAA";
        }else{
            cleanBossData["values"][index]["pokeData"]["waza4"]["wazaId"] = "WAZA_TERABAASUTO";
        }

        if(random != 1017){
            cleanBossData["values"][index]["pokeData"]["gemType"] = "NIJI";
        }else{
            cleanBossData["values"][index]["pokeData"]["gemType"] = "IWA";
        }
    }
    else{
        int random = localRand->bounded(1, maxAllowedId);
        while(!allowedPokemon.contains(pokemonMapping["pokemons"][random]["natdex"]))
            random =localRand->bounded(1, maxAllowedId);

        int formRandom = localRand->bounded(0, static_cast<int>(pokemonMapping["pokemons"][random]["forms"].size()));
        while(!allowedPokemon[random].contains(formRandom)){
            formRandom = localRand->bounded(0, static_cast<int>(pokemonMapping["pokemons"][random]["forms"].size()));
        }

        cleanBossData["values"][index]["pokeData"]["devId"] = pokemonMapping["pokemons"][random]["devName"];

        // Set Starter Form
        cleanBossData["values"][index]["pokeData"]["formId"] = formRandom;

        QString genderStd = "";

        QString form_Check = QString::fromUtf8(pokemonMapping["pokemons"][random]["name"].get<std::string>().c_str());
        if(maleOnlyPokemon.contains(form_Check) || genderlessPokemon.contains(form_Check)){

            if(genderlessPokemon.contains(form_Check)){
                genderStd = "DEFAULT";
            }else{
                genderStd = "MALE";
            }
        }else if(femaleOnlyPokemon.contains(form_Check)){
            genderStd = "FEMALE";
        } else if(genderForms.contains(form_Check)){
            if(formRandom == 0){

                genderStd = "MALE";
            }else{

                genderStd = "FEMALE";
            }
        }else{
            int rand_gender = 1+std::rand()%100;
            if(rand_gender > int(pokemonPersonalData["entry"][int(pokemonMapping["pokemons"][random]["devid"])]["gender"]["ratio"])){
                genderStd = "MALE";
            }else{

                genderStd = "FEMALE";
            }
        }

        cleanBossData["values"][index]["pokeData"]["sex"] = genderStd.toStdString();
        int val = 1+std::rand()%100; // range is [1, shiny_starter_rate)
        if(val == 1){
            cleanBossData["values"][index]["pokeData"]["rareType"] = "RARE";
        }
        else{
            cleanBossData["values"][index]["pokeData"]["rareType"] = "NO_RARE";
        }
        std::string itemForPokemon = getPokemonItemId(pokemonMapping["pokemons"][random]["natdex"], formRandom);
        cleanBossData["values"][index]["pokeData"]["item"] = itemForPokemon;


        cleanBossData["values"][index]["pokeData"]["wazaType"] = "DEFAULT";
        for(int i = 1; i<=4; i++){
            QString key = "waza"+QString::fromStdString(std::to_string(i));
            cleanBossData["values"][index]["pokeData"][key.toStdString()]["wazaId"] = "WAZA_NULL";
        }

        randomizeTeraType(cleanBossData["values"][index]["pokeData"], true, random, formRandom);
    }
}

void svBoss::randomizeStellarOgerpon(unsigned long long index){
    int random = localRand->bounded(1, maxAllowedId);
    while(!allowedPokemon.contains(pokemonMapping["pokemons"][random]["natdex"]))
        random = localRand->bounded(1, maxAllowedId);

    int formRandom = localRand->bounded(0, static_cast<int>(pokemonMapping["pokemons"][random]["forms"].size()));
    while(!allowedPokemon[random].contains(formRandom)){
        formRandom = localRand->bounded(0, static_cast<int>(pokemonMapping["pokemons"][random]["forms"].size()));
    }

    cleanBossData["values"][index]["pokeData"]["devId"] = pokemonMapping["pokemons"][random]["devName"];

    // Set Starter Form
    cleanBossData["values"][index]["pokeData"]["formId"] = formRandom;

    QString genderStd = "";

    QString form_Check = QString::fromUtf8(pokemonMapping["pokemons"][random]["name"].get<std::string>().c_str());
    if(maleOnlyPokemon.contains(form_Check) || genderlessPokemon.contains(form_Check)){

        if(genderlessPokemon.contains(form_Check)){
            genderStd = "DEFAULT";
        }else{
            genderStd = "MALE";
        }
    }else if(femaleOnlyPokemon.contains(form_Check)){
        genderStd = "FEMALE";
    } else if(genderForms.contains(form_Check)){
        if(formRandom == 0){

            genderStd = "MALE";
        }else{

            genderStd = "FEMALE";
        }
    }else{
        int rand_gender = localRand->bounded(1, 101);
        if(rand_gender > int(pokemonPersonalData["entry"][int(pokemonMapping["pokemons"][random]["devid"])]["gender"]["ratio"])){
            genderStd = "MALE";
        }else{

            genderStd = "FEMALE";
        }
    }

    cleanBossData["values"][index]["pokeData"]["sex"] = genderStd.toStdString();
    int val = localRand->bounded(1,100); // range is [1, shiny_starter_rate)
    if(val == 1){
        cleanBossData["values"][index]["pokeData"]["rareType"] = "RARE";
    }
    else{
        cleanBossData["values"][index]["pokeData"]["rareType"] = "NO_RARE";
    }
    std::string itemForPokemon = getPokemonItemId(pokemonMapping["pokemons"][random]["natdex"], formRandom);
    cleanBossData["values"][index]["pokeData"]["item"] = itemForPokemon;

    cleanBossData["values"][index]["pokeData"]["wazaType"] = "DEFAULT";
    for(int i = 1; i<=4; i++){
        QString key = "waza"+QString::fromStdString(std::to_string(i));
        cleanBossData["values"][index]["pokeData"][key.toStdString()]["wazaId"] = "WAZA_NULL";
    }
}

void svBoss::patchLoyalThreeOgerpon(){
    QString outputKey = "output";
    // Ogerpon
    QVector<int> devIdBoss;
    QVector<int> formIdBoss;
    QVector<int> genderBoss;
    QVector<bool> rareBoss;

    saveIndividualPokemon(58, devIdBoss, formIdBoss, genderBoss, rareBoss);

    QList<QPair<QString, QString>> files = {
        {qBaseDir.filePath("SV_SCENES/trscn.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Ogerpon/d610")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Ogerpon/sdc01_0105_main")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Ogerpon/sdc01_0130_always")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Ogerpon/sdc01_0160_always")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Ogerpon/sdc01_0370_post_end")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Ogerpon/sdc01_0390_main")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Ogerpon/sdc01_0430_main")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Ogerpon/sdc01_0440_always")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Ogerpon/sdc01_0460_main")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Ogerpon/sdc01_0480_main")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Ogerpon/sdc01_3gods_a_main")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Ogerpon/sdc01_3gods_b_main")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Ogerpon/sdc01_3gods_c_main")},
    };

    QList<QString> outputs = {
        "world/scene/parts/demo/ev/d610_/",
        "world/scene/parts/event/event_scenario/main_scenario/sdc01_0105_/",
        "world/scene/parts/event/event_scenario/main_scenario/sdc01_0130_/",
        "world/scene/parts/event/event_scenario/main_scenario/sdc01_0160_/",
        "world/scene/parts/event/event_scenario/main_scenario/sdc01_0370_/",
        "world/scene/parts/event/event_scenario/main_scenario/sdc01_0390_/",
        "world/scene/parts/event/event_scenario/main_scenario/sdc01_0430_/",
        "world/scene/parts/event/event_scenario/main_scenario/sdc01_0440_/",
        "world/scene/parts/event/event_scenario/main_scenario/sdc01_0460_/",
        "world/scene/parts/event/event_scenario/main_scenario/sdc01_0480_/",
        "world/scene/parts/event/event_scenario/main_scenario/sdc01_3gods_a_/",
        "world/scene/parts/event/event_scenario/main_scenario/sdc01_3gods_b_/",
        "world/scene/parts/event/event_scenario/main_scenario/sdc01_3gods_c_/",
    };

    qDebug()<<"Patching Ogerpon Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    // Okidogi

    saveIndividualPokemon(55, devIdBoss, formIdBoss, genderBoss, rareBoss);
    files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Okidogi/sdc01_3gods_a_pre_start")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Okidogi/s1_sub_011_pre_start")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/sdc01_3gods_a_/",
        "world/scene/parts/event/event_scenario/sub_scenario/s1_sub_011_/",
    };

    qDebug()<<"Patching Okidogi Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    // Monkidori
    saveIndividualPokemon(53, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Monkidori/sdc01_3gods_b_pre_start")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Monkidori/s1_sub_012_pre_start")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/sdc01_3gods_b_/",
        "world/scene/parts/event/event_scenario/sub_scenario/s1_sub_012_/",
    };

    qDebug()<<"Patching Monkidori Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);


    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    // Fezandipiti
    saveIndividualPokemon(56, devIdBoss, formIdBoss, genderBoss, rareBoss);
    files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Fezandipiti/sdc01_3gods_c_pre_start")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Fezandipiti/sdc01_3gods_c_before_pre_start")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Fezandipiti/s1_sub_016_pre_start")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/sdc01_3gods_c_/",
        "world/scene/parts/event/event_scenario/main_scenario/sdc01_3gods_c_before_/",
        "world/scene/parts/event/event_scenario/sub_scenario/s1_sub_016_/"
    };

    qDebug()<<"Patching Fezandipiti Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);


    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    // Loyal Three
    QVector<int> devIdBoss_0;
    QVector<int> formIdBoss_0;
    QVector<int> genderBoss_0;
    QVector<bool> rareBoss_0;
    saveIndividualPokemon(55, devIdBoss_0, formIdBoss_0, genderBoss_0, rareBoss_0);
    saveIndividualPokemon(53, devIdBoss_0, formIdBoss_0, genderBoss_0, rareBoss_0);
    saveIndividualPokemon(56, devIdBoss_0, formIdBoss_0, genderBoss_0, rareBoss_0);

    files = {
             {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/LoyalThree/sdc01_0330_always")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/sdc01_0330_/",
    };

    qDebug()<<"Patching Loyal Three - 1 Scene Files";
    patchScenes(files, outputs, devIdBoss_0, formIdBoss_0, genderBoss_0, rareBoss_0);

    QVector<int> devIdBoss_1;
    QVector<int> formIdBoss_1;
    QVector<int> genderBoss_1;
    QVector<bool> rareBoss_1;
    saveIndividualPokemon(58, devIdBoss_1, formIdBoss_1, genderBoss_1, rareBoss_1);
    saveIndividualPokemon(55, devIdBoss_1, formIdBoss_1, genderBoss_1, rareBoss_1);
    saveIndividualPokemon(53, devIdBoss_1, formIdBoss_1, genderBoss_1, rareBoss_1);
    saveIndividualPokemon(56, devIdBoss_1, formIdBoss_1, genderBoss_1, rareBoss_1);

    files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/LoyalThree/sdc01_0360_pre_start")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/sdc01_0360_/",
    };

    qDebug()<<"Patching Loyal Three - 2 Scene Files";
    patchScenes(files, outputs, devIdBoss_1, formIdBoss_1, genderBoss_1, rareBoss_1);

    QVector<int> devIdBoss_2;
    QVector<int> formIdBoss_2;
    QVector<int> genderBoss_2;
    QVector<bool> rareBoss_2;
    saveIndividualPokemon(58, devIdBoss_2, formIdBoss_2, genderBoss_2, rareBoss_2);
    saveIndividualPokemon(53, devIdBoss_2, formIdBoss_2, genderBoss_2, rareBoss_2);
    saveIndividualPokemon(56, devIdBoss_2, formIdBoss_2, genderBoss_2, rareBoss_2);
    saveIndividualPokemon(55, devIdBoss_2, formIdBoss_2, genderBoss_2, rareBoss_2);

    files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/LoyalThree/sdc01_0400_main")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/LoyalThree/sdc01_0410_main")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/LoyalThree/sdc01_0420_main")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/sdc01_0400_/",
        "world/scene/parts/event/event_scenario/main_scenario/sdc01_0410_/",
        "world/scene/parts/event/event_scenario/main_scenario/sdc01_0420_/"
    };

    qDebug()<<"Patching Loyal Three - 3 Scene Files";
    qDebug()<<outputs.size();
    qDebug()<<files.size();
    patchScenes(files, outputs, devIdBoss_2, formIdBoss_2, genderBoss_2, rareBoss_2);

}

void svBoss::patchKitakami(){
    QString outputKey = "output";
    // Ogerpon
    QVector<int> devIdBoss;
    QVector<int> formIdBoss;
    QVector<int> genderBoss;
    QVector<bool> rareBoss;

    saveIndividualPokemon(67, devIdBoss, formIdBoss, genderBoss, rareBoss);
    QList<QPair<QString, QString>> files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Other/sdc01_0300_main")},
    };

    QList<QString> outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/sdc01_0300_/",
    };

    qDebug()<<"Patching Milotic Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    saveIndividualPokemon(74, devIdBoss, formIdBoss, genderBoss, rareBoss);
    files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Other/s1_side02_0030_main")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/sub_scenario/s1_side02_0030_/",
    };

    qDebug()<<"Patching Ariados Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    saveIndividualPokemon(75, devIdBoss, formIdBoss, genderBoss, rareBoss);
    files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_Kitakami/Other/s1_side02_0050_main")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/sub_scenario/s1_side02_0050_/",
    };

    qDebug()<<"Patching Ursaluna-Bloodmoon Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
}

void svBoss::patchStellarUnderdeepths(){
    QString outputKey = "output";
    QVector<int> devIdBoss;
    QVector<int> formIdBoss;
    QVector<int> genderBoss;
    QVector<bool> rareBoss;

    saveIndividualPokemon(78, devIdBoss, formIdBoss, genderBoss, rareBoss);
    QList<QPair<QString, QString>> files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_AZ/Underdepths/s2_sub_005_pre_start")},
    };

    QList<QString> outputs = {
        "world/scene/parts/event/event_scenario/sub_scenario/s2_sub_005_/",
    };

    qDebug()<<"Patching Underdepths - Glimmora Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    saveIndividualPokemon(79, devIdBoss, formIdBoss, genderBoss, rareBoss);
    files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_AZ/Underdepths/sdc02_0262_main")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/sdc02_0262_/",
    };

    qDebug()<<"Patching Underdepths - Noivern Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    saveIndividualPokemon(80, devIdBoss, formIdBoss, genderBoss, rareBoss);
    files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_AZ/Underdepths/sdc02_0263_pre_start")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/sdc02_0263_/",
    };

    qDebug()<<"Patching Underdepths - Nargancl Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    saveIndividualPokemon(81, devIdBoss, formIdBoss, genderBoss, rareBoss);
    files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_AZ/Underdepths/sdc02_0267_pre_start")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/sdc02_0267_/",
    };

    qDebug()<<"Patching Underdepths - Garchomp Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    saveIndividualPokemon(82, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_AZ/Underdepths/sdc02_0265_pre_start")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/sdc02_0265_/",
    };

    qDebug()<<"Patching Underdepths - Sandy Shocks Scene Files";
    patchOneScene(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss, "0");

    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    saveIndividualPokemon(83, devIdBoss, formIdBoss, genderBoss, rareBoss);
    files = {
             {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_AZ/Underdepths/sdc02_0265_pre_start")},
             };

    outputs = {
        "world/scene/parts/event/event_scenario/main_scenario/sdc02_0265_/",
    };

    qDebug()<<"Patching Underdepths - Iron Thorns Scene Files";
    patchOneScene(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss, "1");

    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
}

void svBoss::patchAreaZeroLegends(){
    QVector<int> devIdBoss;
    QVector<int> formIdBoss;
    QVector<int> genderBoss;
    QVector<bool> rareBoss;
    QString outputKey = "output";

    saveIndividualPokemon(76, devIdBoss, formIdBoss, genderBoss, rareBoss);

    QList<QPair<QString, QString>> files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_AZ/Legends/s2_side02_0010_pre_start")},
    };

    QList<QString> outputs = {
        "world/scene/parts/event/event_scenario/sub_scenario/s2_side02_0010_/",
    };

    qDebug()<<"Patching Paradox Legends Scene Files";
    patchOneScene(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss, "0");

    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    saveIndividualPokemon(84, devIdBoss, formIdBoss, genderBoss, rareBoss);
    files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_AZ/Legends/s2_side02_0010_pre_start")},
    };

    outputs = {
        "world/scene/parts/event/event_scenario/sub_scenario/s2_side02_0010_/",
    };

    qDebug()<<"Patching Paradox Legends - 2 Scene Files";
    patchOneScene(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss, "1");

    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    saveIndividualPokemon(77, devIdBoss, formIdBoss, genderBoss, rareBoss);
    files = {
             {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_AZ/Legends/s2_side02_0020_pre_start")},
             };

    outputs = {
        "world/scene/parts/event/event_scenario/sub_scenario/s2_side02_0020_/",
    };

    qDebug()<<"Patching Paradox Legends - 3 Scene Files";
    patchOneScene(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss, "0");

    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    saveIndividualPokemon(85, devIdBoss, formIdBoss, genderBoss, rareBoss);
    files = {
             {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_AZ/Legends/s2_side02_0020_pre_start")},
             };

    outputs = {
        "world/scene/parts/event/event_scenario/sub_scenario/s2_side02_0020_/",
    };

    qDebug()<<"Patching Paradox Legends - 4 Scene Files";
    patchOneScene(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss, "1");
    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
}

void svBoss::patchTerapagos(){
    QVector<int> devIdBoss;
    QVector<int> formIdBoss;
    QVector<int> genderBoss;
    QVector<bool> rareBoss;
    QString outputKey = "output";

    saveIndividualPokemon(88, devIdBoss, formIdBoss, genderBoss, rareBoss);

    QList<QPair<QString, QString>> files = {
        {qBaseDir.filePath("SV_SCENES/trscn.fbs"), qBaseDir.filePath("SV_SCENES/SV_AZ/Terapagos/d730")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_AZ/Terapagos/sdc02_0320_always")},
    };

    QList<QString> outputs = {
        "world/scene/parts/demo/ev/d730_/",
        "world/scene/parts/event/event_scenario/main_scenario/sdc02_0320_/"
    };

    qDebug()<<"Patching Terapagos Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

}

void svBoss::patchSnacksworths(){
    QVector<int> devIdBoss;
    QVector<int> formIdBoss;
    QVector<int> genderBoss;
    QVector<bool> rareBoss;
    QString outputKey = "output";

    saveIndividualPokemon(86, devIdBoss, formIdBoss, genderBoss, rareBoss);

    // Pecharunt
    QList<QPair<QString, QString>> files = {
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_LEGENDS/Pecharunt/s2_side01_0160_always")},
        {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_LEGENDS/Pecharunt/s2_side01_0180_always")},
    };

    QList<QString> outputs = {
        "world/scene/parts/event/event_scenario/sub_scenario/s2_side01_0160_/",
        "world/scene/parts/event/event_scenario/sub_scenario/s2_side01_0180_/"
    };

    qDebug()<<"Patching Pecharunt Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    // Meloetta
    saveIndividualPokemon(114, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files = {
        {qBaseDir.filePath("SV_SCENES/trscn.fbs"), qBaseDir.filePath("SV_SCENES/SV_LEGENDS/Meloetta/s2_sub_003_pop")}
    };

    outputs = {
        "world/scene/parts/event/event_scenario/sub_scenario/s2_sub_003_pop_/"
    };

    qDebug()<<"Patching Pecharunt Scene Files";
    patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

    files.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    // Snacksworth in order
    QList<int> snacksworthLegends = {92, 112, 102, 104, 91, 109, 100, 95, 98, 99, 96, 94, 105, 89, 110, 111, 106, 113, 97, 107, 101, 103, 90, 93, 108};
    for(int i = 13, k=0; i<=37; i++, k++){
        saveIndividualPokemon(snacksworthLegends[k], devIdBoss, formIdBoss, genderBoss, rareBoss);
        QString fileName = "s2_sub_0"+QString::fromStdString(std::to_string(i))+"_pre_start";
        QString folderName = "world/scene/parts/event/event_scenario/sub_scenario/s2_sub_0"+QString::fromStdString(std::to_string(i))+"_/";

        files = {
            {qBaseDir.filePath("SV_SCENES/trsog.fbs"), qBaseDir.filePath("SV_SCENES/SV_LEGENDS/Snacksworth/"+fileName)}
        };

        outputs = {
            folderName
        };

        qDebug()<<"Patching Snacksworths Scene Files";
        patchScenes(files, outputs, devIdBoss, formIdBoss, genderBoss, rareBoss);

        files.clear();
        devIdBoss.clear();
        formIdBoss.clear();
        genderBoss.clear();
        rareBoss.clear();

    }
}

void svBoss::randomizeBosses(QRandomGenerator* r){
    localRand = r;
    setRandNum(localRand);
    qDebug()<<"Starting Boss Randomizer";
    cleanBossData = readJsonQFile("SV_FLATBUFFERS/SV_SCENES/eventBattlePokemon_array_clean.json");
    QDir check;
    if(check.exists(QDir::current().filePath("SV_FLATBUFFERS/SV_TRAINERS/trdata_array.json"))){
        trainerInfo = readJsonQFile("SV_FLATBUFFERS/SV_TRAINERS/trdata_array.json");
        QDir().remove(qBaseDir.filePath("SV_FLATBUFFERS/SV_TRAINERS/trdata_array.json"));
        qDebug()<<"Getting existing trainer info";
    }else{
        trainerInfo = readJsonQFile("SV_FLATBUFFERS/SV_TRAINERS/trdata_array_clean.json");
        qDebug()<<"Getting clean trainer info";
    }

    bool sizeCheck = getAllowedPokemon(BossLimiter, allowedPokemon, "Bosses");

    for(unsigned long long i =0; i<cleanBossData["values"].size(); i++){
        if(i == 115){
            copyFight(i, 31);
        }else if(i == 116){
            copyFight(i, 32);
        }
        else if (i >= 12 && i <= 23) {
            copyFight(i, 11);
        }
        else if (i >= 27 && i <= 30) {
            copyFight(i, 26);
        }
        else if (i >= 33 && i <= 48) {
            switch(i){
            case 34:
                copyFight(i, 33);
                break;
            case 36:
                copyFight(i, 35);
                break;
            case 38:
            case 39:
            case 40:
                copyFight(i, 37);
                break;
            case 42:
                copyFight(i, 41);
                break;
            case 44:
                copyFight(i, 43);
                break;
            case 46:
                copyFight(i, 45);
                break;
            case 48:
                copyFight(i, 47);
                break;
            default:
                randomizeFight(i);
            }
        }
        else if(i == 54 || i == 57 || i == 72 || i == 73){
            copyFight(i, 53);
        }
        else if (i == 70 || i == 71){
            copyFight(i, 56);
        }
        else if(i == 68 || i == 69){
            copyFight(i, 55);
        }
        else if(i == 59 || i == 60 || i == 61 || i == 62 || i == 63 || i ==64 || i ==65){
            copyStellarOgerpon(i, 58);
        }
        else if(i == 67){
            copyFight(i, 66);
        }
        else if(i == 88){
            copyStellarOgerpon(i, 87);
        }
        else{
            if(i == 58 || i == 78 || i == 79 || i == 80 || i == 81 || i == 82 || i == 83 || i == 87){
                randomizeStellarOgerpon(i);
            }else{
                randomizeFight(i);
            }
        }
    }
    qDebug()<<"Randomizing AZ Fights";
    patchMultiBattle();

    qDebug()<<"Randomizing Gimmighoul";
    patchGimmighoul();
    patchGimmighoulRoaming();

    qDebug()<<"Randomizing LeChonk and First Route";
    patchLeChonk();

    qDebug()<<"Randomizing Houndoom";
    patchHoundoom();

    qDebug()<<"Randomizing Sunflora";
    patchSunflora();

    qDebug()<<"Randomizing Dondozo and Tatsugiri Titan";
    patchDonzoTitan();

    qDebug()<<"Randomizing Orthworm Titan";
    patchOrthwormTitan();

    qDebug()<<"Randomizing Bombardier Titan";
    patchBombariderTitan();

    qDebug()<<"Randomizing Klawf Titan";
    patchKlawfTitan();

    qDebug()<<"Randomizing Great Tustk and Iron Treads Titan";
    patchGreatIronTitan();

    qDebug()<<"Randomizing Ting-Lu";
    patchTingLu();

    qDebug()<<"Randomizing Chien-Pao";
    patchChienPao();

    qDebug()<<"Randomizing Wo-Chien";
    patchWoChien();

    qDebug()<<"Randomizing Chi-Yu";
    patchChiYu();

    qDebug()<<"Randomizing Ogerpon and Loyal 3";
    patchLoyalThreeOgerpon();

    qDebug()<<"Randomizing Kitakami sub-bosses";
    patchKitakami();

    qDebug()<<"Randomizing AZ Underdeepths";
    patchStellarUnderdeepths();

    qDebug()<<"Randomizing Paradox Legends";
    patchAreaZeroLegends();

    qDebug()<<"Randomizing Terapagos";
    patchTerapagos();

    qDebug()<<"Randomizing Snacksworths Legends";
    patchSnacksworths();

    qDebug()<<"Closing and finalizing file";
    closeFileAndDelete("SV_SCENES/eventBattlePokemon_array.json",
                       "SV_SCENES/eventBattlePokemon_array.bfbs",
                       "world/data/battle/eventBattlePokemon/",
                       cleanBossData, true);
    qDebug()<<"Returning file";
}
