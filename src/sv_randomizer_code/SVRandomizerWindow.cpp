#include "headers/sv_randomizer_headers/SVRandomizerWindow.h"

SVRandomizerWindow::SVRandomizerWindow(QWidget *parent): QWidget(parent) {
    starterNames = {"Sprigatito", "Fuecoco", "Quaxly"};
}

SVRandomizerWindow::~SVRandomizerWindow(){}

// Main Layout
void SVRandomizerWindow::createLayout()
{
    // Initialize Settings from Virtual Functions
    initializeSettings();

    // Create the main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // ------------------------General Settings Section------------------------
        QGroupBox *generalGroup = new QGroupBox("General Settings Section");
        QVBoxLayout *generalSettings = new QVBoxLayout(generalGroup);

        // Horizontal layout for side-by-side questions and inputs in singleCheckboxGroup
        QHBoxLayout *mainSettings = new QHBoxLayout();
        QHBoxLayout *extraSettings = new QHBoxLayout();
        QHBoxLayout *seedSettings = new QHBoxLayout();
        QHBoxLayout *settingsLocal = new QHBoxLayout();

        extraSettings->setContentsMargins(0, 0, 0, 0);
        extraSettings->setSpacing(0);

        QCheckBox * auto_patch = new QCheckBox("Auto Patch Randomizer ", generalGroup);
        auto_patch->setChecked(true);
        mainSettings->addWidget(auto_patch);
        connect(auto_patch, &QCheckBox::toggled, this, [this](bool checked) mutable{
            randomizer.auto_patch = checked;
        });

        QCheckBox *kaizo_mode = new QCheckBox("Kaizo Randomizer", generalGroup);
        mainSettings->addWidget(kaizo_mode);
        connect(auto_patch, &QCheckBox::toggled, this, [this](bool checked) mutable{
            randomizer.kaizo_mode = checked;
        });

        QLabel *bulk_question = new QLabel("Randomizers to Create ", generalGroup);
        bulk_question->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        QSpinBox * bulk_amount= new QSpinBox(generalGroup);  // Number input
        bulk_amount->setRange(1, 100);  // Set range for the number input
        bulk_amount->setValue(1);
        connect(bulk_amount, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) mutable{
            randomizer.bulk_amount = static_cast<unsigned int>(value);
        });
        bulk_amount->setMaximumWidth(100);

        extraSettings->addWidget(bulk_question);
        extraSettings->addWidget(bulk_amount);

        // Add the horizontal layout to the group
        extraSettings->addStretch(1);  // Pushes widgets to the left

        // QPushButton * importButton = new QPushButton("Import Settings and Seed", generalGroup);
        // connect(importButton, &QPushButton::clicked, this, [=, this]() mutable{
        //     emit importSettings();
        // });

        // settingsLocal->addWidget(importButton);

        // QPushButton * exportButton = new QPushButton("Export Settings and Seed", generalGroup);
        // connect(exportButton, &QPushButton::clicked, this, [=, this]() mutable{
        //     QString defaultDir = QDir::currentPath();  // Get the current working directory
        //     QString fileName = QFileDialog::getSaveFileName(nullptr,
        //                                                     "Choose File Name",
        //                                                     defaultDir + "/SVRandomizerSettings-Test.json",
        //                                                     "JSON Files (*.json)");

        //     QJsonDocument settingsJson = exportSettings(settings);

        //     QFile file(fileName);
        //     if (file.open(QIODevice::WriteOnly)) {
        //         file.write(settingsJson.toJson());
        //         file.close();
        //     } else {
        //         qDebug() << "Failed to open file for writing.";
        //     }
        // });

        // settingsLocal->addWidget(exportButton);

        QLineEdit *seed = new QLineEdit(generalGroup);
        //connect(seed, &QLineEdit::textChanged, this, &SVRandomizerWindow::saveStringInput);

        QLabel *seedsets = new QLabel("Seed ", generalGroup);
        seedsets->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        seedSettings->addWidget(seedsets);

        seed->setFixedWidth(348);
        seed->setPlaceholderText("Leave Blank to use time... (no limit)");
        seedSettings->addWidget(seed);

        QPushButton * startRandomizer = new QPushButton("Start Randomizing", generalGroup);
        connect(startRandomizer, &QPushButton::clicked, this, &SVRandomizerWindow::runRandomizer);
        seedSettings->addWidget(startRandomizer);

        generalSettings->addLayout(mainSettings);
        generalSettings->addLayout(extraSettings);
        generalSettings->addLayout(settingsLocal);
        generalSettings->addLayout(seedSettings);

    mainLayout->addWidget(generalGroup);
    // Configure the tab bar
    QTabBar * topBar = new QTabBar(this);
    topBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed); // Expand horizontally
    topBar->addTab("Starter/Gift Pokemon");
    topBar->addTab("Stats/TMs/Items");
    topBar->addTab("Wild/Fixed Pokemon");
    topBar->addTab("Raids");
    topBar->addTab("Trainers");
    topBar->addTab("Bosses");
    //topBar->addTab("Shop");

    // // Adjust the minimum width to a specific value to ensure it expands
    topBar->setMinimumWidth(800); // Set a minimum width (adjust as necessary)

    mainLayout->addWidget(topBar); // Add the tab bar to the layout

    // // Configure the stacked widget
    QStackedWidget * stackedWidget = new QStackedWidget(this);
    stackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Allow full expansion
    mainLayout->addWidget(stackedWidget);

    // Add each widget to the stacked widget
    stackedWidget->addWidget(setupStartersWidget());
    stackedWidget->addWidget(setupPersonalWidget());
    stackedWidget->addWidget(setupWildWidget());
    stackedWidget->addWidget(setupRaidsWidget());
    stackedWidget->addWidget(setupTrainersWidget());
    stackedWidget->addWidget(setupBossWidget());
    // //stackedWidget->addWidget(setupShopWidget());

    connect(topBar, &QTabBar::currentChanged, this, [=, this](int index) {
        switchTabs(stackedWidget, index);
    });

    // Set the stretch factors
    mainLayout->setStretch(2, 0); // Prevent tab bar from expanding vertically
    mainLayout->setStretch(3, 1); // Make the stacked widget take up the remaining space

    // Apply the main layout to RandomizerWindow
    setLayout(mainLayout);
}

void SVRandomizerWindow::runRandomizer(){
    // setup randNum and seed
    qDebug()<<"Running Randomizer";
    qDebug()<<"Generating seed value for: "<<randomizer.seed;

    quint64 seed;

    if(randomizer.seed.isEmpty()){
        auto now_ns = std::chrono::steady_clock::now().time_since_epoch().count();
        seed = static_cast<quint64>(now_ns);
    }else{
        QByteArray seedArray = randomizer.seed.toUtf8();
        QByteArray hash = QCryptographicHash::hash(seedArray, QCryptographicHash::Sha256);
        seed = *reinterpret_cast<const quint64*>(hash.constData());
    }

    quint32 seed_low = static_cast<quint32>(seed & 0xFFFFFFFF);
    quint32 seed_high = static_cast<quint32>(seed >> 32);
    std::seed_seq seq{ seed_low, seed_high };

    qDebug()<<seed;
    qDebug()<<seed_low;
    qDebug()<<seed_high;
    randomizer.randNum = QRandomGenerator(seq);

    QString outputFolderName = "Randomizers-Output";
    QString outputFolderPath = QDir::current().filePath(outputFolderName);
    QDir outputDir(outputFolderPath);

    // If the folder exists, delete it
    if (outputDir.exists()) {
        if (!outputDir.removeRecursively()) {
            qFatal() << "Failed to delete existing folder:" << outputFolderPath;
        }
        qDebug() << "Deleted existing folder:" << outputFolderPath;
    }

    // Recreate the folder
    if (!QDir().mkdir(outputFolderPath)) {
        qFatal() << "Failed to create directory:" << outputFolderPath;
    }

    for(unsigned int i = 0; i<randomizer.bulk_amount; i++){
        QString currentRunMessage = "Doing Randomizer Run: " + QString::fromStdString(std::to_string(i+1)) + "/" + QString::fromStdString(std::to_string(randomizer.bulk_amount));
        showMessage(currentRunMessage, i);
        qDebug()<<"Starting Randomizer!";

        QString newOutputFolder = QString("Randomizers-Output/Randomizer-%1").arg(i + 1);

        if(randomizer.svRandomizerStarters.randomizeStarters == true ||
            randomizer.svRandomizerStarters.randomizeGifts == true){

            randomizer.svRandomizerStarters.randomize(&randomizer.randNum);
        }

        if(randomizer.svRandomizerPersonal.randomizeAbilities == true||
            randomizer.svRandomizerPersonal.randomizeBST == true ||
            randomizer.svRandomizerPersonal.randomizeEvolutions == true ||
            randomizer.svRandomizerPersonal.randomizeMoveset == true ||
            randomizer.svRandomizerPersonal.randomizeTypes == true ||
            randomizer.svRandomizerPersonal.randomizeTMs == true ||
            randomizer.svRandomizerPersonal.fixEvolutions == true){

            randomizer.svRandomizerPersonal.randomize(&randomizer.randNum);
        }

        if(randomizer.svRandomizerItems.randomizeItems == true){
            randomizer.svRandomizerItems.randomize(&randomizer.randNum);
        }

        if(randomizer.svRandomizerFixed.randomizeFixedEncounters == true){
            randomizer.svRandomizerFixed.randomize(&randomizer.randNum);
        }

        if(randomizer.svRandomizerWilds.randomizePaldeaWild == true ||
            randomizer.svRandomizerWilds.randomizeKitakamiWild == true ||
            randomizer.svRandomizerWilds.randomizeBlueberryWild == true){
            randomizer.svRandomizerWilds.randomize(&randomizer.randNum);
        }

        if(randomizer.svRandomizerRaids.randomizePaldea == true){
            QStringList paths = {
                "SV_RAIDS/raid_enemy_01_array.json",
                "SV_RAIDS/raid_enemy_02_array.json",
                "SV_RAIDS/raid_enemy_03_array.json",
                "SV_RAIDS/raid_enemy_04_array.json",
                "SV_RAIDS/raid_enemy_05_array.json",
                "SV_RAIDS/raid_enemy_06_array.json",
            };

            QStringList cleans = {
                "SV_RAIDS/raid_enemy_01_array_clean.json",
                "SV_RAIDS/raid_enemy_02_array_clean.json",
                "SV_RAIDS/raid_enemy_03_array_clean.json",
                "SV_RAIDS/raid_enemy_04_array_clean.json",
                "SV_RAIDS/raid_enemy_05_array_clean.json",
                "SV_RAIDS/raid_enemy_06_array_clean.json",
            };

            QStringList folder = {
                "world/data/raid/raid_enemy_01/",
                "world/data/raid/raid_enemy_02/",
                "world/data/raid/raid_enemy_03/",
                "world/data/raid/raid_enemy_04/",
                "world/data/raid/raid_enemy_05/",
                "world/data/raid/raid_enemy_06/",
            };

            QStringList schemas = {
                "SV_RAIDS/raid_enemy_01_array.bfbs",
                "SV_RAIDS/raid_enemy_02_array.bfbs",
                "SV_RAIDS/raid_enemy_03_array.bfbs",
                "SV_RAIDS/raid_enemy_04_array.bfbs",
                "SV_RAIDS/raid_enemy_05_array.bfbs",
                "SV_RAIDS/raid_enemy_06_array.bfbs",
            };

            QString region = "Paldea";
            randomizer.svRandomizerRaids.randomize(paths, cleans, schemas, folder, region, &randomizer.randNum);
        }

        if(randomizer.svRandomizerRaids.randomizeKitakami == true){
            QStringList paths = {
                "SV_RAIDS/su1_raid_enemy_01_array.json",
                "SV_RAIDS/su1_raid_enemy_02_array.json",
                "SV_RAIDS/su1_raid_enemy_03_array.json",
                "SV_RAIDS/su1_raid_enemy_04_array.json",
                "SV_RAIDS/su1_raid_enemy_05_array.json",
                "SV_RAIDS/su1_raid_enemy_06_array.json",
            };

            QStringList cleans = {
                "SV_RAIDS/su1_raid_enemy_01_array_clean.json",
                "SV_RAIDS/su1_raid_enemy_02_array_clean.json",
                "SV_RAIDS/su1_raid_enemy_03_array_clean.json",
                "SV_RAIDS/su1_raid_enemy_04_array_clean.json",
                "SV_RAIDS/su1_raid_enemy_05_array_clean.json",
                "SV_RAIDS/su1_raid_enemy_06_array_clean.json",
            };

            QStringList folder = {
                "world/data/raid/su1_raid_enemy_01/",
                "world/data/raid/su1_raid_enemy_02/",
                "world/data/raid/su1_raid_enemy_03/",
                "world/data/raid/su1_raid_enemy_04/",
                "world/data/raid/su1_raid_enemy_05/",
                "world/data/raid/su1_raid_enemy_06/",
            };

            QStringList schemas = {
                "SV_RAIDS/su1_raid_enemy_01_array.bfbs",
                "SV_RAIDS/su1_raid_enemy_02_array.bfbs",
                "SV_RAIDS/su1_raid_enemy_03_array.bfbs",
                "SV_RAIDS/su1_raid_enemy_04_array.bfbs",
                "SV_RAIDS/su1_raid_enemy_05_array.bfbs",
                "SV_RAIDS/su1_raid_enemy_06_array.bfbs",
            };

            QString region = "Kitakami";
            randomizer.svRandomizerRaids.randomize(paths, cleans, schemas, folder, region, &randomizer.randNum);
        }

        if(randomizer.svRandomizerRaids.randomizeBlueberry == true){
            QStringList paths = {
                "SV_RAIDS/su2_raid_enemy_01_array.json",
                "SV_RAIDS/su2_raid_enemy_02_array.json",
                "SV_RAIDS/su2_raid_enemy_03_array.json",
                "SV_RAIDS/su2_raid_enemy_04_array.json",
                "SV_RAIDS/su2_raid_enemy_05_array.json",
                "SV_RAIDS/su2_raid_enemy_06_array.json",
            };

            QStringList cleans = {
                "SV_RAIDS/su2_raid_enemy_01_array_clean.json",
                "SV_RAIDS/su2_raid_enemy_02_array_clean.json",
                "SV_RAIDS/su2_raid_enemy_03_array_clean.json",
                "SV_RAIDS/su2_raid_enemy_04_array_clean.json",
                "SV_RAIDS/su2_raid_enemy_05_array_clean.json",
                "SV_RAIDS/su2_raid_enemy_06_array_clean.json",
            };

            QStringList folder = {
                "world/data/raid/su2_raid_enemy_01/",
                "world/data/raid/su2_raid_enemy_02/",
                "world/data/raid/su2_raid_enemy_03/",
                "world/data/raid/su2_raid_enemy_04/",
                "world/data/raid/su2_raid_enemy_05/",
                "world/data/raid/su2_raid_enemy_06/",
            };

            QStringList schemas = {
                "SV_RAIDS/su2_raid_enemy_01_array.bfbs",
                "SV_RAIDS/su2_raid_enemy_02_array.bfbs",
                "SV_RAIDS/su2_raid_enemy_03_array.bfbs",
                "SV_RAIDS/su2_raid_enemy_04_array.bfbs",
                "SV_RAIDS/su2_raid_enemy_05_array.bfbs",
                "SV_RAIDS/su2_raid_enemy_06_array.bfbs",
            };

            QString region = "Blueberry";
            randomizer.svRandomizerRaids.randomize(paths, cleans, schemas, folder, region, &randomizer.randNum);
        }

        bool paldeaTrainers = false;
        bool kitakamiTrainers = false;
        bool blueberryTrainers = false;
        bool trainerBosses = randomizer.svRandomizerBosses.randomize_bosses;
        bool randomizeTrainers = randomizer.svRandomizerTrainers.checkRandomization(paldeaTrainers, kitakamiTrainers, blueberryTrainers);

        if(randomizeTrainers == true){
            randomizer.svRandomizerTrainers.randomize(&randomizer.randNum, paldeaTrainers, kitakamiTrainers, blueberryTrainers, trainerBosses);
        }

        if (trainerBosses == true){
            randomizer.svRandomizerBosses.randomizeBosses(&randomizer.randNum);
        }

        if(randomizer.auto_patch == true){
            qDebug()<<"Auto Patching Randomizer";
            randomizer.patchFileDescriptor();

            generateBinary(qBaseDir.filePath("SV_DATA_FLATBUFFERS/data.fbs").toStdString(),
                           qBaseDir.filePath("SV_DATA_FLATBUFFERS/data.json").toStdString(),
                           "arc/", true);

            QDir().remove(qBaseDir.filePath("SV_DATA_FLATBUFFERS/data.json"));
        }

        QString basePath = QDir::currentPath();  // or use QCoreApplication::applicationDirPath()
        QString outputFolder = basePath + "/output";

        if (QDir(outputFolder).exists()) {
            if (!QDir().rename(outputFolder, newOutputFolder)) {
                qFatal() << "Failed to move and rename output folder from" << outputFolder << "to" << newOutputFolder;
            }
        } else {
            qFatal() << "Output folder does not exist at" << outputFolder;
        }
    }

    QString currentRunMessage = "Finalized Randomizer! - Click Ok to be done";
    showMessage(currentRunMessage, ENDRANDOMIZER);
}

void SVRandomizerWindow::showMessage(const QString &message, unsigned int id) {
    static QMessageBox* msgBox = nullptr;

    if (!msgBox) {
        msgBox = new QMessageBox;
        msgBox->setWindowTitle("Current Run Information");
        msgBox->setAttribute(Qt::WA_DeleteOnClose, false); // Do not delete when closed
    }

    msgBox->setText(message);

    // Set buttons based on id
    if (id == ENDRANDOMIZER) {
        msgBox->setStandardButtons(QMessageBox::Ok);
    } else {
        msgBox->setStandardButtons(QMessageBox::NoButton); // No buttons
    }

    // Bring to front
    msgBox->raise();
    msgBox->activateWindow();
    QApplication::beep();

    if (!msgBox->isVisible()) {
        msgBox->show(); // Non-blocking
    }

    msgBox->update();
    QApplication::processEvents();
}

// Specific Widgets Codes
QScrollArea* SVRandomizerWindow::setupStartersWidget(){
    QWidget *startersWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(startersWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    // Add Actual Buttons
    formLayout->addRow(createStartersWidget());
    formLayout->addRow(createGiftsWidget());

    // Create Layouts
    mainLayout->addLayout(formLayout);
    scrollArea->setWidget(startersWidget);

    return scrollArea;
}

QScrollArea* SVRandomizerWindow::setupPersonalWidget(){
    QWidget *personalsWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(personalsWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    // Add Actual Buttons
    formLayout->addRow(createAbilitiesWidget());
    formLayout->addRow(createTypesWidget());
    formLayout->addRow(createMovesetWidget());
    formLayout->addRow(createBSTWidget());
    formLayout->addRow(fixEvolutionsWidget());
    formLayout->addRow(createEvolutionsWidget());
    formLayout->addRow(createTMWidget());
    formLayout->addRow(createItemWidget());

    // Create Layouts
    mainLayout->addLayout(formLayout);
    scrollArea->setWidget(personalsWidget);

    return scrollArea;
}

QScrollArea* SVRandomizerWindow::setupWildWidget(){
    QWidget *wildsWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(wildsWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    // Configure the tab bar
    QTabBar *innerWildBar = new QTabBar(this);
    innerWildBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed); // Expand horizontally
    innerWildBar->addTab("Fixed Encounters");
    innerWildBar->addTab("Paldea Wilds");
    innerWildBar->addTab("Kitakami Wilds");
    innerWildBar->addTab("Blueberry Academy Wilds");

    // Adjust the minimum width to a specific value to ensure it expands
    innerWildBar->setMinimumWidth(750); // Set a minimum width (adjust as necessary)

    mainLayout->addWidget(innerWildBar); // Add the tab bar to the layout

    // Configure the stacked widget
    QStackedWidget* innerWildStackWidget = new QStackedWidget(this);
    innerWildStackWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Allow full expansion
    mainLayout->addWidget(innerWildStackWidget);

    // Add each widget to the stacked widget
    innerWildStackWidget->addWidget(createFixedEncountersWidget());
    innerWildStackWidget->addWidget(createPaldeaWildWidget());
    innerWildStackWidget->addWidget(createKitakamiWildWidget());
    innerWildStackWidget->addWidget(createBlueberryWildWidget());

    connect(innerWildBar, &QTabBar::currentChanged, this, [=, this](int index) {
        switchTabs(innerWildStackWidget, index);
    });

    // Set the stretch factors
    mainLayout->setStretch(2, 0); // Prevent tab bar from expanding vertically
    mainLayout->setStretch(3, 1); // Make the stacked widget take up the remaining space

    // Create Layouts
    mainLayout->addLayout(formLayout);
    scrollArea->setWidget(wildsWidget);

    return scrollArea;
}

QWidget* SVRandomizerWindow::createFixedEncountersWidget(){
    QWidget *fixedWildWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(fixedWildWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //add rows to formLayout
    formLayout->addRow(createFixedEncounters());

    // Set form layout to main layout
    mainLayout->addLayout(formLayout);
    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(fixedWildWidget);
    return scrollArea;
}

QWidget* SVRandomizerWindow::createPaldeaWildWidget(){
    QWidget *paldeaWildWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(paldeaWildWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //add rows to formLayout
    formLayout->addRow(createPaldeaWild());

    // Set form layout to main layout
    mainLayout->addLayout(formLayout);
    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(paldeaWildWidget);
    return scrollArea;
}

QWidget* SVRandomizerWindow::createKitakamiWildWidget(){
    QWidget *kitakamiWildWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(kitakamiWildWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //add rows to formLayout
    formLayout->addRow(createKitakamiWild());

    // Set form layout to main layout
    mainLayout->addLayout(formLayout);
    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(kitakamiWildWidget);
    return scrollArea;
}

QWidget* SVRandomizerWindow::createBlueberryWildWidget(){
    QWidget *blueberryWildWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(blueberryWildWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //add rows to formLayout
    formLayout->addRow(createBlueberryWild());

    // Set form layout to main layout
    mainLayout->addLayout(formLayout);
    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(blueberryWildWidget);
    return scrollArea;
}

QScrollArea* SVRandomizerWindow::setupRaidsWidget(){
    QWidget *raidsWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(raidsWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    // Configure the tab bar
    QTabBar *innerRaidBar = new QTabBar(this);
    innerRaidBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed); // Expand horizontally
    innerRaidBar->addTab("Paldea Raids");
    innerRaidBar->addTab("Kitakami Raids");
    innerRaidBar->addTab("Blueberry Academy Raids");

    // Adjust the minimum width to a specific value to ensure it expands
    innerRaidBar->setMinimumWidth(750); // Set a minimum width (adjust as necessary)

    mainLayout->addWidget(innerRaidBar); // Add the tab bar to the layout

    // Configure the stacked widget
    QStackedWidget* innerRaidStackWidget = new QStackedWidget(this);
    innerRaidStackWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Allow full expansion
    mainLayout->addWidget(innerRaidStackWidget);

    // Add each widget to the stacked widget
    innerRaidStackWidget->addWidget(createPaldeaRaidWidget());
    innerRaidStackWidget->addWidget(createKitakamiRaidWidget());
    innerRaidStackWidget->addWidget(createBlueberryRaidWidget());

    connect(innerRaidBar, &QTabBar::currentChanged, this, [=, this](int index) {
        switchTabs(innerRaidStackWidget, index);
    });

    // Set the stretch factors
    mainLayout->setStretch(2, 0); // Prevent tab bar from expanding vertically
    mainLayout->setStretch(3, 1); // Make the stacked widget take up the remaining space

    // Create Layouts
    mainLayout->addLayout(formLayout);
    scrollArea->setWidget(raidsWidget);

    return scrollArea;
}

QWidget* SVRandomizerWindow::createPaldeaRaidWidget(){
    QWidget *paldeaRaidWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(paldeaRaidWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //add rows to formLayout
    formLayout->addRow(createPaldeaRaid());

    // Set form layout to main layout
    mainLayout->addLayout(formLayout);
    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(paldeaRaidWidget);
    return scrollArea;
}

QWidget* SVRandomizerWindow::createKitakamiRaidWidget(){
    QWidget *kitakamiRaidWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(kitakamiRaidWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //add rows to formLayout
    formLayout->addRow(createKitakamiRaid());

    // Set form layout to main layout
    mainLayout->addLayout(formLayout);
    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(kitakamiRaidWidget);
    return scrollArea;
}

QWidget* SVRandomizerWindow::createBlueberryRaidWidget(){
    QWidget *blueberryRaidWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(blueberryRaidWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //add rows to formLayout
    formLayout->addRow(createBlueberryRaid());

    // Set form layout to main layout
    mainLayout->addLayout(formLayout);
    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(blueberryRaidWidget);
    return scrollArea;
}

QScrollArea* SVRandomizerWindow::setupTrainersWidget(){
    QWidget *trainersWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(trainersWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    // Configure the tab bar
    QTabBar *innerTrainersBar = new QTabBar(this);
    innerTrainersBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed); // Expand horizontally
    innerTrainersBar->addTab("Paldea Trainers");
    innerTrainersBar->addTab("Kitakami Trainers");
    innerTrainersBar->addTab("Blueberry Academy Trainers");

    // Adjust the minimum width to a specific value to ensure it expands
    innerTrainersBar->setMinimumWidth(750); // Set a minimum width (adjust as necessary)

    mainLayout->addWidget(innerTrainersBar); // Add the tab bar to the layout

    // Configure the stacked widget
    QStackedWidget* innerTrainerStackWidget = new QStackedWidget(this);
    innerTrainerStackWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Allow full expansion
    mainLayout->addWidget(innerTrainerStackWidget);

    // Add each widget to the stacked widget
    innerTrainerStackWidget->addWidget(createPaldeaTrainersWidget());
    innerTrainerStackWidget->addWidget(createKitakamiTrainersWidget());
    innerTrainerStackWidget->addWidget(createBlueberryTrainersWidget());

    connect(innerTrainersBar, &QTabBar::currentChanged, this, [=, this](int index) {
        switchTabs(innerTrainerStackWidget, index);
    });

    // Set the stretch factors
    mainLayout->setStretch(2, 0); // Prevent tab bar from expanding vertically
    mainLayout->setStretch(3, 1); // Make the stacked widget take up the remaining space

    // Create Layouts
    mainLayout->addLayout(formLayout);
    scrollArea->setWidget(trainersWidget);

    return scrollArea;
}

QWidget* SVRandomizerWindow::createPaldeaTrainersWidget(){
    QWidget *paldeaTrainersWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(paldeaTrainersWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //add rows to formLayout
    QCheckBox* paldea_all_regions = new QCheckBox("Use Paldea Settings for all Regions");
    formLayout->addWidget(paldea_all_regions);
    // Connect for Setting the values
    connect(paldea_all_regions, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerTrainers.paldeaForAll = checked;
    });

    formLayout->addRow(createTrainerSettings("Paldea", "All (excluding Koraidon/Miraidon)",  randomizer.svRandomizerTrainers.allTrainers, true));
    formLayout->addRow(createTrainerSettings("Paldea", "Koraidon/Miraidon", randomizer.svRandomizerTrainers.paradisePokemon, true));
    formLayout->addRow(createTrainerSettings("Paldea", "Rival", randomizer.svRandomizerTrainers.rivalTrainers, true));
    formLayout->addRow(createTrainerSettings("Paldea", "Gym", randomizer.svRandomizerTrainers.gymTrainers, true));
    formLayout->addRow(createTrainerSettings("Paldea", "Elite 4", randomizer.svRandomizerTrainers.e4Trainers, true));
    formLayout->addRow(createTrainerSettings("Paldea", "Champion", randomizer.svRandomizerTrainers.championTrainers, true));
    formLayout->addRow(createTrainerSettings("Paldea", "Route", randomizer.svRandomizerTrainers.routeTrainers, true));
    formLayout->addRow(createTrainerSettings("Paldea", "Raid", randomizer.svRandomizerTrainers.raidTrainers, true));

    // Set form layout to main layout
    mainLayout->addLayout(formLayout);
    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(paldeaTrainersWidget);
    return scrollArea;
}

QWidget* SVRandomizerWindow::createKitakamiTrainersWidget(){
    QWidget *kitakamiTrainersWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(kitakamiTrainersWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //add rows to formLayout
    formLayout->addRow(createTrainerSettings("Kitakami", "All", randomizer.svRandomizerTrainers.allKitakamiTrainers, false));
    formLayout->addRow(createTrainerSettings("Kitakami", "Rival", randomizer.svRandomizerTrainers.kitakamiRivals, false));
    formLayout->addRow(createTrainerSettings("Kitakami", "Ogre Clan", randomizer.svRandomizerTrainers.ogreClanTrainers, false));
    formLayout->addRow(createTrainerSettings("Kitakami", "Route", randomizer.svRandomizerTrainers.kitakamiRouteTrainers, false));
    formLayout->addRow(createTrainerSettings("Kitakami", "Raid", randomizer.svRandomizerTrainers.kitakamiRaidTrainers, false));

    // Set form layout to main layout
    mainLayout->addLayout(formLayout);
    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(kitakamiTrainersWidget);
    return scrollArea;
}

QWidget* SVRandomizerWindow::createBlueberryTrainersWidget(){
    QWidget *blueberryTrainersWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(blueberryTrainersWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //add rows to formLayout
    formLayout->addRow(createTrainerSettings("Blueberry", "All (excluding Terapagos)", randomizer.svRandomizerTrainers.allBlueberryTrainers, false));
    formLayout->addRow(createTrainerSettings("Blueberry", "Terapagos", randomizer.svRandomizerTrainers.hiddenTreasure, false));
    formLayout->addRow(createTrainerSettings("Blueberry", "Rival", randomizer.svRandomizerTrainers.blueberryRivals, false));
    formLayout->addRow(createTrainerSettings("Blueberry", "BB4", randomizer.svRandomizerTrainers.bb4Trainers, false));
    formLayout->addRow(createTrainerSettings("Blueberry", "Route", randomizer.svRandomizerTrainers.blueberryRouteTrainers, false));
    formLayout->addRow(createTrainerSettings("Blueberry", "Raid", randomizer.svRandomizerTrainers.blueberryRaidTrainers, false));

    // Set form layout to main layout
    mainLayout->addLayout(formLayout);
    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(blueberryTrainersWidget);
    return scrollArea;
}

QScrollArea* SVRandomizerWindow::setupBossWidget(){
    QWidget *bossWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(bossWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    // Configure the tab bar
    QTabBar *innerTrainersBar = new QTabBar(this);
    innerTrainersBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed); // Expand horizontally
    innerTrainersBar->addTab("Paldea Boss");
    // innerTrainersBar->addTab("Kitakami Boss");
    // innerTrainersBar->addTab("Blueberry Academy Boss");

    // Adjust the minimum width to a specific value to ensure it expands
    innerTrainersBar->setMinimumWidth(750); // Set a minimum width (adjust as necessary)

    mainLayout->addWidget(innerTrainersBar); // Add the tab bar to the layout

    // Configure the stacked widget
    QStackedWidget* innerTrainerStackWidget = new QStackedWidget(this);
    innerTrainerStackWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Allow full expansion
    mainLayout->addWidget(innerTrainerStackWidget);

    // Add each widget to the stacked widget
    innerTrainerStackWidget->addWidget(createPaldeaBossWidget());
    // innerTrainerStackWidget->addWidget(createKitakamiBossWidget());
    // innerTrainerStackWidget->addWidget(createBlueberryBossWidget());

    connect(innerTrainersBar, &QTabBar::currentChanged, this, [=, this](int index) {
        switchTabs(innerTrainerStackWidget, index);
    });

    // Set the stretch factors
    mainLayout->setStretch(2, 0); // Prevent tab bar from expanding vertically
    mainLayout->setStretch(3, 1); // Make the stacked widget take up the remaining space

    // Create Layouts
    mainLayout->addLayout(formLayout);
    scrollArea->setWidget(bossWidget);

    return scrollArea;
}

QWidget* SVRandomizerWindow::createPaldeaBossWidget(){
    QWidget *paldeaTrainersWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(paldeaTrainersWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //add rows to formLayout
    formLayout->addRow(createBossSettings("All", randomizer.svRandomizerBosses.BossLimiter, true));
    // formLayout->addRow(createBossSettings("Koraidon/Miraidon (Not Recommended)", randomizer.svRandomizerBoss.BossLimiter,true));
    // formLayout->addRow(createBossSettings("LeChonk", randomizer.svRandomizerBoss.BossLimiter,true));
    // formLayout->addRow(createBossSettings("Houndoom", randomizer.svRandomizerBoss.BossLimiter,true));
    // formLayout->addRow(createBossSettings("Sunflora", randomizer.svRandomizerBoss.BossLimiter,true));
    // formLayout->addRow(createBossSettings("Gimmighoul", randomizer.svRandomizerBoss.BossLimiter,true));
    // formLayout->addRow(createBossSettings("Titans", randomizer.svRandomizerBoss.BossLimiter,true));
    // formLayout->addRow(createBossSettings("Treasure of Ruin", randomizer.svRandomizerBoss.BossLimiter,true));
    // formLayout->addRow(createBossSettings("Area Zero", randomizer.svRandomizerBoss.BossLimiter,false));
    // formLayout->addRow(createBossSettings("Snacksworth", randomizer.svRandomizerBoss.BossLimiter,true));

    // Set form layout to main layout
    mainLayout->addLayout(formLayout);
    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(paldeaTrainersWidget);
    return scrollArea;
}

QWidget* SVRandomizerWindow::createKitakamiBossWidget(){
    QWidget *kitakamiTrainersWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(kitakamiTrainersWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //add rows to formLayout
    // formLayout->addRow(createBossSettings("All", randomizer.svRandomizerBoss.BossLimiter,false));
    // formLayout->addRow(createBossSettings("Ogerpon", randomizer.svRandomizerBoss.BossLimiter,false));
    // formLayout->addRow(createBossSettings("Loyal Three", randomizer.svRandomizerBoss.BossLimiter,false));
    // formLayout->addRow(createBossSettings("Milotic", randomizer.svRandomizerBoss.BossLimiter,false));
    // formLayout->addRow(createBossSettings("Ariados", randomizer.svRandomizerBoss.BossLimiter,false));
    // formLayout->addRow(createBossSettings("Ursaluna", randomizer.svRandomizerBoss.BossLimiter,false));
    // formLayout->addRow(createBossSettings("Pecharunt", randomizer.svRandomizerBoss.BossLimiter,false));

    // Set form layout to main layout
    mainLayout->addLayout(formLayout);
    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(kitakamiTrainersWidget);
    return scrollArea;
}

QWidget* SVRandomizerWindow::createBlueberryBossWidget(){
    QWidget *blueberryTrainersWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(blueberryTrainersWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //add rows to formLayout
    // formLayout->addRow(createBossSettings("All", randomizer.svRandomizerBoss.BossLimiter,false));
    // formLayout->addRow(createBossSettings("Area Zero (Underdepths)", randomizer.svRandomizerBoss.BossLimiter,false));
    // formLayout->addRow(createBossSettings("Terapagos", randomizer.svRandomizerBoss.BossLimiter,false));
    // formLayout->addRow(createBossSettings("Meloetta", randomizer.svRandomizerBoss.BossLimiter,false));
    // formLayout->addRow(createBossSettings("Paradox Legends", randomizer.svRandomizerBoss.BossLimiter,false));

    // Set form layout to main layout
    mainLayout->addLayout(formLayout);
    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(blueberryTrainersWidget);
    return scrollArea;
}

// Creation of Widget
QVBoxLayout* SVRandomizerWindow::createStartersWidget(){
    QVBoxLayout *starterSettingLayout = new QVBoxLayout();

    // Creates the Main Button
    QCheckBox* enable_starters = new QCheckBox("Randomize Starters");
    starterSettingLayout->addWidget(enable_starters);
    // Connect for Setting the values
    connect(enable_starters, &QCheckBox::toggled, this, [this](bool checked) mutable{
        QMap<QString, QVariant> startersSettings = settings["Starters"].toMap();

        startersSettings["Randomize"] = checked;
        randomizer.svRandomizerStarters.randomizeStarters = checked;
        settings["Starters"] = startersSettings;
    });

    // Creates Hidden Group based on button
    QGroupBox *startersGroupSettings= new QGroupBox("Starter Settings Section");
    QVBoxLayout *startersSettingsLayout = new QVBoxLayout(startersGroupSettings);
    QHBoxLayout *startersRow_Q0 = new QHBoxLayout();
    QHBoxLayout *startersRow_Q1 = new QHBoxLayout();
    QHBoxLayout *startersRow_Q2 = new QHBoxLayout();
    QHBoxLayout *startersRow_Q3 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    startersGroupSettings->setVisible(false);
    connect(enable_starters, &QCheckBox::toggled, startersGroupSettings, &QGroupBox::setVisible);

    starterSettingLayout->addWidget(startersGroupSettings);

    // Row 0
    QCheckBox* randomize_starters_tera_types = new QCheckBox("Randomize Tera Types ", startersGroupSettings);
    startersRow_Q0->addWidget(randomize_starters_tera_types);
    // Connect for Setting the values
    connect(randomize_starters_tera_types, &QCheckBox::toggled, this, [this](bool checked) mutable{
        QMap<QString, QVariant> startersSettings = settings["Starters"].toMap();

        startersSettings["Tera Types"] = checked;
        randomizer.svRandomizerStarters.randomizeStartersTeraTypes = checked;
        settings["Starters"] = startersSettings;
    });

    QCheckBox* all_starters_shiny = new QCheckBox("Force All Starters Shiny ", startersGroupSettings);
    startersRow_Q0->addWidget(all_starters_shiny);
    // Connect for Setting the values
    connect(all_starters_shiny, &QCheckBox::toggled, this, [this](bool checked) mutable{
        QMap<QString, QVariant> startersSettings = settings["Starters"].toMap();

        startersSettings["All Shiny"] = checked;
        randomizer.svRandomizerStarters.allStartersShiny = checked;
        settings["Starters"] = startersSettings;
    });

    QCheckBox* force_shiny_starter = new QCheckBox("Force One Starter Shiny ", startersGroupSettings);
    startersRow_Q0->addWidget(force_shiny_starter);
    connect(force_shiny_starter, &QCheckBox::toggled, this, [this](bool checked) mutable{
        QMap<QString, QVariant> startersSettings = settings["Starters"].toMap();

        startersSettings["One Shiny"] = checked;
        randomizer.svRandomizerStarters.forceShinyStarter = checked; // connected
        settings["Starters"] = startersSettings;
    });

    startersSettingsLayout->addLayout(startersRow_Q0);

    createStarterPokemonSelection("Sprigatito", randomizer.pokemonInGame, randomizer.pokeballNames, startersGroupSettings, startersRow_Q1, startersSettingsLayout, 0);
    createStarterPokemonSelection("Fuecoco", randomizer.pokemonInGame, randomizer.pokeballNames, startersGroupSettings, startersRow_Q2, startersSettingsLayout, 1);
    createStarterPokemonSelection("Quaxly", randomizer.pokemonInGame, randomizer.pokeballNames, startersGroupSettings, startersRow_Q3, startersSettingsLayout, 2);

    // Pokemon Limiter
    setupAllowedPokemon(startersSettingsLayout, randomizer.svRandomizerStarters.startersPokemon);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return starterSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createGiftsWidget(){
    QVBoxLayout *giftsSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> giftsSettings;

    // Creates the Main Button
    QCheckBox* enable_gifts = new QCheckBox("Randomize Gifts");
    giftsSettingLayout->addWidget(enable_gifts);
    // Connect for Setting the values
    connect(enable_gifts, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerStarters.randomizeGifts = checked;
    });

    // Creates Hidden Group based on button
    QGroupBox *giftsGroupSettings= new QGroupBox("Gifts Settings Section");
    QVBoxLayout *giftSettingsLayout = new QVBoxLayout(giftsGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    giftsGroupSettings->setVisible(false);
    connect(enable_gifts, &QCheckBox::toggled, giftsGroupSettings, &QGroupBox::setVisible);

    giftsSettingLayout->addWidget(giftsGroupSettings);

    // Row 0
    QCheckBox* randomize_gifts_tera_types = new QCheckBox("Randomize Tera Types ", giftsGroupSettings);
    row0->addWidget(randomize_gifts_tera_types);
    // Connect for Setting the values
    connect(randomize_gifts_tera_types, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerStarters.randomizeGiftsTeraTypes = checked;
    });

    giftSettingsLayout->addLayout(row0);

    // Pokemon Limiter
    setupAllowedPokemon(giftSettingsLayout, randomizer.svRandomizerStarters.giftsPokemon);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return giftsSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createAbilitiesWidget(){
    QVBoxLayout *abilitiesSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> abilitiesSettings;

    // Creates the Main Button
    QCheckBox* enable_abilities = new QCheckBox("Randomize Abilities");
    abilitiesSettingLayout->addWidget(enable_abilities);
    // Connect for Setting the values
    connect(enable_abilities, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerPersonal.randomizeAbilities = checked;
    });

    // Creates Hidden Group based on button
    QGroupBox *abilitiesGroupSettings= new QGroupBox("Abilities Settings Section");
    QVBoxLayout *abilitiesSettingsLayout = new QVBoxLayout(abilitiesGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    abilitiesGroupSettings->setVisible(false);
    connect(enable_abilities, &QCheckBox::toggled, abilitiesGroupSettings, &QGroupBox::setVisible);

    abilitiesSettingLayout->addWidget(abilitiesGroupSettings);

    // Row 0
    QCheckBox* ban_wonder_guard = new QCheckBox("Ban Wonder Guard", abilitiesGroupSettings);
    row0->addWidget(ban_wonder_guard);
    // Connect for Setting the values
    connect(ban_wonder_guard, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerPersonal.banWonderGuard = checked;
    });

    QCheckBox* ban_exit_abilities = new QCheckBox("Ban Exit Abilities", abilitiesGroupSettings);
    row0->addWidget(ban_exit_abilities);
    // Connect for Setting the values
    connect(ban_exit_abilities, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerPersonal.banExitAbilities = checked;
    });

    abilitiesSettingsLayout->addLayout(row0);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return abilitiesSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createTypesWidget(){
    QVBoxLayout *typesSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> typesSettings;

    // Creates the Main Button
    QCheckBox* enable_types = new QCheckBox("Randomize Types");
    typesSettingLayout->addWidget(enable_types);
    // Connect for Setting the values
    connect(enable_types, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerPersonal.randomizeTypes = checked;
        randomizer.svRandomizerWilds.typesChanged = checked;
    });

    // Creates Hidden Group based on button
    QGroupBox *typesGroupSettings= new QGroupBox("Types Settings Section");
    QVBoxLayout *typesSettingsLayout = new QVBoxLayout(typesGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    typesGroupSettings->setVisible(false);
    connect(enable_types, &QCheckBox::toggled, typesGroupSettings, &QGroupBox::setVisible);

    typesSettingLayout->addWidget(typesGroupSettings);

    // Row 0
    QCheckBox* extra_types = new QCheckBox("Grant Extra Types", typesGroupSettings);
    row0->addWidget(extra_types);
    // Connect for Setting the values
    connect(extra_types, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerPersonal.grantExtraTypes = checked;
    });

    typesSettingsLayout->addLayout(row0);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return typesSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createMovesetWidget(){
    QVBoxLayout *movesSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> movesSettings;

    // Creates the Main Button
    QCheckBox* enable_moves = new QCheckBox("Randomize Movesets");
    movesSettingLayout->addWidget(enable_moves);
    // Connect for Setting the values
    connect(enable_moves, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerPersonal.randomizeMoveset = checked;
    });

    // Creates Hidden Group based on button
    QGroupBox *movesGroupSettings= new QGroupBox("Moveset Settings Section");
    QVBoxLayout *movesSettingsLayout = new QVBoxLayout(movesGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    movesGroupSettings->setVisible(false);
    connect(enable_moves, &QCheckBox::toggled, movesGroupSettings, &QGroupBox::setVisible);

    movesSettingLayout->addWidget(movesGroupSettings);

    // Row 0
    QCheckBox* same_moves_as_type = new QCheckBox("Same Moves as Type (Useless)", movesGroupSettings);
    row0->addWidget(same_moves_as_type);
    // Connect for Setting the values
    connect(same_moves_as_type, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerPersonal.setSameMovesAsType = checked;
    });

    movesSettingsLayout->addLayout(row0);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return movesSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createBSTWidget(){
    QVBoxLayout *bstSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> bstSettings;

    // Creates the Main Button
    QCheckBox* enable_bst = new QCheckBox("Randomize BST");
    bstSettingLayout->addWidget(enable_bst);
    // Connect for Setting the values
    connect(enable_bst, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerPersonal.randomizeBST = checked;
    });

    // Creates Hidden Group based on button
    QGroupBox *bstGroupSettings= new QGroupBox("BST Settings Section");
    QVBoxLayout *bstSettingsLayout = new QVBoxLayout(bstGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    bstGroupSettings->setVisible(false);
    connect(enable_bst, &QCheckBox::toggled, bstGroupSettings, &QGroupBox::setVisible);

    bstSettingLayout->addWidget(bstGroupSettings);

    // Row 0
    QCheckBox* same_bst = new QCheckBox("Keep Same BST", bstGroupSettings);
    row0->addWidget(same_bst);
    // Connect for Setting the values
    connect(same_bst, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerPersonal.keepSameBST = checked;
    });

    bstSettingsLayout->addLayout(row0);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return bstSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::fixEvolutionsWidget(){
    QVBoxLayout *fixEvolutionsSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> fixEvolutionsSettings;

    // Creates the Main Button
    QCheckBox* fix_evolutions = new QCheckBox("Fix Evolutions (Regional & Trade)");
    fixEvolutionsSettingLayout->addWidget(fix_evolutions);
    // Connect for Setting the values
    connect(fix_evolutions, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerPersonal.fixEvolutions = checked;
    });

    // Creates Hidden Group based on button
    QGroupBox *fixEvolutionsGroupSettings= new QGroupBox("Fix Evolutions Settings Section");
    QVBoxLayout *fixEvolutionsSettingsLayout = new QVBoxLayout(fixEvolutionsGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();
    QHBoxLayout *row1 = new QHBoxLayout();
    QHBoxLayout *row2 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    fixEvolutionsGroupSettings->setVisible(false);
    connect(fix_evolutions, &QCheckBox::toggled, fixEvolutionsGroupSettings, &QGroupBox::setVisible);

    fixEvolutionsSettingLayout->addWidget(fixEvolutionsGroupSettings);

    // Row 0
    QLabel* explanation1 = new QLabel("All Regionals Evolutions Evolve At Nighttime - Level 36", fixEvolutionsGroupSettings);
    row0->addWidget(explanation1);

    QLabel* explanation2 = new QLabel("All Trade Evolutions Evolve At Level 36", fixEvolutionsGroupSettings);
    row1->addWidget(explanation2);

    QLabel* explanation3 = new QLabel("Split Trades/Regional Evolve At Level 36 in Day and Nighttime", fixEvolutionsGroupSettings);
    row2->addWidget(explanation3);

    fixEvolutionsSettingsLayout->addLayout(row0);
    fixEvolutionsSettingsLayout->addLayout(row1);
    fixEvolutionsSettingsLayout->addLayout(row2);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return fixEvolutionsSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createEvolutionsWidget(){
    QVBoxLayout *evolutionsSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> evolutionsSettings;

    // Creates the Main Button
    QCheckBox* enable_evos = new QCheckBox("Randomize Evolutions");
    evolutionsSettingLayout->addWidget(enable_evos);
    // Connect for Setting the values
    connect(enable_evos, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerPersonal.randomizeEvolutions = checked;
    });

    // Creates Hidden Group based on button
    QGroupBox *evolutionsGroupSettings= new QGroupBox("Evolutions Settings Section");
    QVBoxLayout *evolutionsSettingsLayout = new QVBoxLayout(evolutionsGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    evolutionsGroupSettings->setVisible(false);
    connect(enable_evos, &QCheckBox::toggled, evolutionsGroupSettings, &QGroupBox::setVisible);

    evolutionsSettingLayout->addWidget(evolutionsGroupSettings);

    // Row 0
    QCheckBox* evolve_every_level = new QCheckBox("Evolve Every Level", evolutionsGroupSettings);
    row0->addWidget(evolve_every_level);
    // Connect for Setting the values
    connect(evolve_every_level, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerPersonal.evolveEveryLevel = checked;
    });

    evolutionsSettingsLayout->addLayout(row0);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return evolutionsSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createTMWidget(){
    QVBoxLayout *tmSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> tmSettings;

    // Creates the Main Button
    QCheckBox* enable_tms = new QCheckBox("Randomize TMs");
    tmSettingLayout->addWidget(enable_tms);
    // Connect for Setting the values
    connect(enable_tms, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerPersonal.randomizeTMs = checked;
    });

    // Creates Hidden Group based on button
    QGroupBox *tmGroupSettings= new QGroupBox("TMs Settings Section");
    QVBoxLayout *tmSettingsLayout = new QVBoxLayout(tmGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    tmGroupSettings->setVisible(false);
    connect(enable_tms, &QCheckBox::toggled, tmGroupSettings, &QGroupBox::setVisible);

    tmSettingLayout->addWidget(tmGroupSettings);

    // Row 0
    QCheckBox* moves_without_animation = new QCheckBox("Allow Moves without Animation", tmGroupSettings);
    row0->addWidget(moves_without_animation);
    // Connect for Setting the values
    connect(moves_without_animation, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerPersonal.noAnimationTMs = checked;
    });

    tmSettingsLayout->addLayout(row0);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return tmSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createItemWidget(){
    QVBoxLayout *itemSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> itemSettings;

    // Creates the Main Button
    QCheckBox* enable_items = new QCheckBox("Randomize Items");
    itemSettingLayout->addWidget(enable_items);
    // Connect for Setting the values
    connect(enable_items, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerItems.randomizeItems = checked;
    });

    // Creates Hidden Group based on button
    QGroupBox *itemGroupSettings= new QGroupBox("Items Settings Section");
    QVBoxLayout *itemSettingsLayout = new QVBoxLayout(itemGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();
    QHBoxLayout *row1 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    itemGroupSettings->setVisible(false);
    connect(enable_items, &QCheckBox::toggled, itemGroupSettings, &QGroupBox::setVisible);

    itemSettingLayout->addWidget(itemGroupSettings);

    // Row 0
    QCheckBox* hidden_items = new QCheckBox("Randomize Hidden Items", itemGroupSettings);
    row0->addWidget(hidden_items);
    // Connect for Setting the values
    connect(hidden_items, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerItems.randomizeHiddenItems = checked;
    });

    QCheckBox* pickup_items = new QCheckBox("Randomize Pickup Items", itemGroupSettings);
    row0->addWidget(pickup_items);
    // Connect for Setting the values
    connect(pickup_items, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerItems.randomizePickUpItems = checked;
    });

    itemSettingsLayout->addLayout(row0);

    // Row 1
    QCheckBox* drops = new QCheckBox("Randomize Pokemon Drops", itemGroupSettings);
    row1->addWidget(drops);
    // Connect for Setting the values
    connect(drops, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerItems.randomizePokemonDrops = checked;
    });

    QCheckBox* synchro = new QCheckBox("Randomize Synchro Items", itemGroupSettings);
    row1->addWidget(synchro);
    // Connect for Setting the values
    connect(synchro, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerItems.randomizeLetsGoItems = checked;
    });

    itemSettingsLayout->addLayout(row1);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return itemSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createFixedEncounters(){
    QVBoxLayout *fixedWildSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> fixedWildSettings;

    // Creates the Main Button
    QCheckBox* enable_fixed = new QCheckBox("Randomize Fixed Encounters");
    fixedWildSettingLayout->addWidget(enable_fixed);
    // Connect for Setting the values
    connect(enable_fixed, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerFixed.randomizeFixedEncounters = checked;
    });

    // Creates Hidden Group based on button
    QGroupBox *fixedWildGroupSettings= new QGroupBox("Fixed Encounter Settings Section");
    QVBoxLayout *fixedWildSettingsLayout = new QVBoxLayout(fixedWildGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    fixedWildGroupSettings->setVisible(false);
    connect(enable_fixed, &QCheckBox::toggled, fixedWildGroupSettings, &QGroupBox::setVisible);

    fixedWildSettingLayout->addWidget(fixedWildGroupSettings);

    // Row 0
    QCheckBox* bst_balance = new QCheckBox("Balance Encounter on BST (Useless)", fixedWildGroupSettings);
    row0->addWidget(bst_balance);
    // Connect for Setting the values
    connect(bst_balance, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerFixed.similarBST = checked;
    });

    QCheckBox* sameTera = new QCheckBox("Keep Same Teras", fixedWildGroupSettings);
    row0->addWidget(sameTera);
    // Connect for Setting the values
    connect(sameTera, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerFixed.keepSameTera = checked;
    });

    fixedWildSettingsLayout->addLayout(row0);

    setupAllowedPokemon(fixedWildSettingsLayout, randomizer.svRandomizerFixed.fixedEncountersPokemon);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return fixedWildSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createPaldeaWild(){
    QVBoxLayout *paldeaWildSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> paldeaWildSettings;

    // Creates the Main Button
    QCheckBox* enable_paldea_wild = new QCheckBox("Randomize Paldea's Wild");
    paldeaWildSettingLayout->addWidget(enable_paldea_wild);
    // Connect for Setting the values
    connect(enable_paldea_wild, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerWilds.randomizePaldeaWild = checked;
    });

    // Creates Hidden Group based on button
    QGroupBox *paldeaWildGroupSettings= new QGroupBox("Paldea's Wild Settings Section");
    QVBoxLayout *paldeaWildSettingsLayout = new QVBoxLayout(paldeaWildGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();
    QHBoxLayout *row1 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    paldeaWildGroupSettings->setVisible(false);
    connect(enable_paldea_wild, &QCheckBox::toggled, paldeaWildGroupSettings, &QGroupBox::setVisible);

    paldeaWildSettingLayout->addWidget(paldeaWildGroupSettings);

    // Row 0
    QCheckBox* ogerpone_and_terapagos = new QCheckBox("Let Ogerpon and Terapagos Spawn (Do not Tera if caught)", paldeaWildGroupSettings);
    row0->addWidget(ogerpone_and_terapagos);
    // Connect for Setting the values
    connect(ogerpone_and_terapagos, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerWilds.ogerponTerapagosPaldea = checked;
    });

    QCheckBox* balance_bst_area = new QCheckBox("Balace Area per BST (Useless for now)", paldeaWildGroupSettings);
    row0->addWidget(balance_bst_area);
    // Connect for Setting the values
    connect(balance_bst_area, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerWilds.balanceBSTPaldea = checked;
    });

    paldeaWildSettingsLayout->addLayout(row0);


    QCheckBox* paldea_wild_for_all = new QCheckBox("Use Paldea Wild Settings for All", paldeaWildGroupSettings);
    row1->addWidget(paldea_wild_for_all);
    // Connect for Setting the values
    connect(paldea_wild_for_all, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerWilds.paldeaForAll = checked;
    });

    paldeaWildSettingsLayout->addLayout(row1);

    setupAllowedPokemon(paldeaWildSettingsLayout, randomizer.svRandomizerWilds.paldeaWilds);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return paldeaWildSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createKitakamiWild(){
    QVBoxLayout *kitakamiWildSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> kitakamiWildSettings;

    // Creates the Main Button
    QCheckBox* enable_kitakami_wild = new QCheckBox("Randomize Kitakami's Wild");
    kitakamiWildSettingLayout->addWidget(enable_kitakami_wild);
    // Connect for Setting the values
    connect(enable_kitakami_wild, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerWilds.randomizeKitakamiWild = checked;
    });

    // Creates Hidden Group based on button
    QGroupBox *kitakamiWildGroupSettings= new QGroupBox("Kitakami Wild Settings Section");
    QVBoxLayout *kitakamiWildSettingsLayout = new QVBoxLayout(kitakamiWildGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    kitakamiWildGroupSettings->setVisible(false);
    connect(enable_kitakami_wild, &QCheckBox::toggled, kitakamiWildGroupSettings, &QGroupBox::setVisible);

    kitakamiWildSettingLayout->addWidget(kitakamiWildGroupSettings);

    // Row 0
    QCheckBox* ogerpone_and_terapagos = new QCheckBox("Let Ogerpon and Terapagos Spawn (Do not Tera if caught)", kitakamiWildGroupSettings);
    row0->addWidget(ogerpone_and_terapagos);
    // Connect for Setting the values
    connect(ogerpone_and_terapagos, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerWilds.ogerponTerapagosKitakami = checked;
    });

    QCheckBox* bst_balance = new QCheckBox("Balance Area on BST (Useless for now)", kitakamiWildGroupSettings);
    row0->addWidget(bst_balance);
    // Connect for Setting the values
    connect(bst_balance, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerWilds.balanceBSTKitakami = checked;
    });

    kitakamiWildSettingsLayout->addLayout(row0);

    setupAllowedPokemon(kitakamiWildSettingsLayout, randomizer.svRandomizerWilds.kitakamiWilds);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return kitakamiWildSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createBlueberryWild(){
    QVBoxLayout *blueberryWildSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> blueberryWildSettings;

    // Creates the Main Button
    QCheckBox* enable_blueberry_wild = new QCheckBox("Randomize Blueberry's Wild");
    blueberryWildSettingLayout->addWidget(enable_blueberry_wild);
    // Connect for Setting the values
    connect(enable_blueberry_wild, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerWilds.randomizeBlueberryWild = checked;
    });

    // Creates Hidden Group based on button
    QGroupBox *blueberryWildGroupSettings= new QGroupBox("Blueberry Wild Settings Section");
    QVBoxLayout *blueberryWildSettingsLayout = new QVBoxLayout(blueberryWildGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    blueberryWildGroupSettings->setVisible(false);
    connect(enable_blueberry_wild, &QCheckBox::toggled, blueberryWildGroupSettings, &QGroupBox::setVisible);

    blueberryWildSettingLayout->addWidget(blueberryWildGroupSettings);

    // Row 0
    QCheckBox* ogerpone_and_terapagos = new QCheckBox("Let Ogerpon and Terapagos Spawn (Do not Tera if caught)", blueberryWildGroupSettings);
    row0->addWidget(ogerpone_and_terapagos);
    // Connect for Setting the values
    connect(ogerpone_and_terapagos, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerWilds.ogerponTearapagosBlueberry = checked;
    });

    QCheckBox* bst_balance = new QCheckBox("Balance Area on BST (Useless for now)", blueberryWildGroupSettings);
    row0->addWidget(bst_balance);
    // Connect for Setting the values
    connect(bst_balance, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerWilds.balanceBSTBlueberry = checked;
    });

    blueberryWildSettingsLayout->addLayout(row0);

    setupAllowedPokemon(blueberryWildSettingsLayout, randomizer.svRandomizerWilds.blueberrWilds);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return blueberryWildSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createPaldeaRaid(){
    QVBoxLayout *paldeaRaidSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> paldeaRaidSettings;

    // Creates the Main Button
    QCheckBox* enable_paldea_raid = new QCheckBox("Randomize All Paldea's Raid");
    paldeaRaidSettingLayout->addWidget(enable_paldea_raid);
    // Connect for Setting the values
    connect(enable_paldea_raid, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerRaids.randomizePaldea = checked;
    });

    // Creates Hidden Group based on button
    QGroupBox *paldeaRaidGroupSettings= new QGroupBox("All Paldea's Raids Settings Section");
    QVBoxLayout *paldeaRaidSettingsLayout = new QVBoxLayout(paldeaRaidGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();
    QHBoxLayout *row1 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    paldeaRaidGroupSettings->setVisible(false);
    connect(enable_paldea_raid, &QCheckBox::toggled, paldeaRaidGroupSettings, &QGroupBox::setVisible);

    paldeaRaidSettingLayout->addWidget(paldeaRaidGroupSettings);

    // Row 0
    QCheckBox* shiny_raids = new QCheckBox("Force Shiny Raids", paldeaRaidGroupSettings);
    row0->addWidget(shiny_raids);
    // Connect for Setting the values
    connect(shiny_raids, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerRaids.forceShinyRaidsPaldea = checked;
    });

    QCheckBox* balance_bst_area = new QCheckBox("Balace Raid per BST (Useless for now)", paldeaRaidGroupSettings);
    row0->addWidget(balance_bst_area);
    // Connect for Setting the values
    connect(balance_bst_area, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerRaids.forNoWarning = checked;
    });

    paldeaRaidSettingsLayout->addLayout(row0);


    QCheckBox* paldea_raid_for_all = new QCheckBox("Use Paldea Raid Settings for All", paldeaRaidGroupSettings);
    row1->addWidget(paldea_raid_for_all);
    // Connect for Setting the values
    connect(paldea_raid_for_all, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerRaids.paldeaForAll = checked;
    });

    paldeaRaidSettingsLayout->addLayout(row1);

    setupAllowedPokemon(paldeaRaidSettingsLayout, randomizer.svRandomizerRaids.paldeaPokemon);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return paldeaRaidSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createKitakamiRaid(){
    QVBoxLayout *kitakamiRaidSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> kitakamiRaidSettings;

    // Creates the Main Button
    QCheckBox* enable_kitakami_raid = new QCheckBox("Randomize All Kitakami's Raid");
    kitakamiRaidSettingLayout->addWidget(enable_kitakami_raid);
    // Connect for Setting the values
    connect(enable_kitakami_raid, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerRaids.randomizeKitakami = checked;
    });

    // Creates Hidden Group based on button
    QGroupBox *kitakamiRaidGroupSettings= new QGroupBox("All Kitakami's Raids Settings Section");
    QVBoxLayout *kitakamiRaidSettingsLayout = new QVBoxLayout(kitakamiRaidGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    kitakamiRaidGroupSettings->setVisible(false);
    connect(enable_kitakami_raid, &QCheckBox::toggled, kitakamiRaidGroupSettings, &QGroupBox::setVisible);

    kitakamiRaidSettingLayout->addWidget(kitakamiRaidGroupSettings);

    // Row 0
    QCheckBox* shiny_raids = new QCheckBox("Force Shiny Raids", kitakamiRaidGroupSettings);
    row0->addWidget(shiny_raids);
    // Connect for Setting the values
    connect(shiny_raids, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerRaids.forceShinyRaidsKitakami = checked;
    });

    QCheckBox* balance_bst_area = new QCheckBox("Balace Raid per BST (Useless for now)", kitakamiRaidGroupSettings);
    row0->addWidget(balance_bst_area);
    // Connect for Setting the values
    connect(balance_bst_area, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerRaids.forNoWarning = checked;
    });

    kitakamiRaidSettingsLayout->addLayout(row0);

    setupAllowedPokemon(kitakamiRaidSettingsLayout, randomizer.svRandomizerRaids.kitakamiPokemon);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return kitakamiRaidSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createBlueberryRaid(){
    QVBoxLayout *blueberryRaidSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> blueberryRaidSettings;

    // Creates the Main Button
    QCheckBox* enable_blueberry_raid = new QCheckBox("Randomize All Blueberry's Raid");
    blueberryRaidSettingLayout->addWidget(enable_blueberry_raid);
    // Connect for Setting the values
    connect(enable_blueberry_raid, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerRaids.randomizeBlueberry = checked;
    });

    // Creates Hidden Group based on button
    QGroupBox *blueberryRaidGroupSettings= new QGroupBox("All Blueberry's Raids Settings Section");
    QVBoxLayout *blueberryRaidSettingsLayout = new QVBoxLayout(blueberryRaidGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    blueberryRaidGroupSettings->setVisible(false);
    connect(enable_blueberry_raid, &QCheckBox::toggled, blueberryRaidGroupSettings, &QGroupBox::setVisible);

    blueberryRaidSettingLayout->addWidget(blueberryRaidGroupSettings);

    // Row 0
    QCheckBox* shiny_raids = new QCheckBox("Force Shiny Raids", blueberryRaidGroupSettings);
    row0->addWidget(shiny_raids);
    // Connect for Setting the values
    connect(shiny_raids, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerRaids.forceShinyRaidsBlueberry = checked;
    });

    QCheckBox* balance_bst_area = new QCheckBox("Balace Raid per BST (Useless for now)", blueberryRaidGroupSettings);
    row0->addWidget(balance_bst_area);
    // Connect for Setting the values
    connect(balance_bst_area, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerRaids.forNoWarning = checked;
    });

    blueberryRaidSettingsLayout->addLayout(row0);

    setupAllowedPokemon(blueberryRaidSettingsLayout, randomizer.svRandomizerRaids.blueberryPokemon);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return blueberryRaidSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createTrainerSettings(QString region, QString trainerType, trainerSettings& trainer, bool paldea){
    QVBoxLayout *globalTrainerSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> globalTrainerSettings;

    // Creates the Main Button
    QCheckBox* enable_trainers_global = new QCheckBox(QStringLiteral("Randomize %1 %2's Trainers").arg(trainerType).arg(region));
    globalTrainerSettingLayout->addWidget(enable_trainers_global);
    // Connect for Setting the values
    connect(enable_trainers_global, &QCheckBox::toggled, this, [&trainer](bool checked) mutable{
        trainer.randomize = checked;
    });

    // Creates Hidden Group based on button
    QGroupBox *paldeaRaidGroupSettings= new QGroupBox(QStringLiteral("%1's Trainers Settings Section").arg(region));
    QVBoxLayout *paldeaRaidSettingsLayout = new QVBoxLayout(paldeaRaidGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();
    QHBoxLayout *row1 = new QHBoxLayout();
    QHBoxLayout *row2;
    QHBoxLayout *row3 = new QHBoxLayout();
    QHBoxLayout *row4 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    paldeaRaidGroupSettings->setVisible(false);
    connect(enable_trainers_global, &QCheckBox::toggled, paldeaRaidGroupSettings, &QGroupBox::setVisible);

    globalTrainerSettingLayout->addWidget(paldeaRaidGroupSettings);

    QCheckBox* tera_trainers = new QCheckBox(QStringLiteral("Allow %1 to Tera").arg(trainerType), paldeaRaidGroupSettings);
    row0->addWidget(tera_trainers);
    // Connect for Setting the values
    connect(tera_trainers, &QCheckBox::toggled, this, [&trainer](bool checked) mutable{
        trainer.allowTera = checked;
    });

    QCheckBox* randomize_tera_types = new QCheckBox("Randomize Tera Types", paldeaRaidGroupSettings);
    row0->addWidget(randomize_tera_types);
    // Connect for Setting the values
    connect(randomize_tera_types, &QCheckBox::toggled, this, [&trainer](bool checked) mutable{
        trainer.randomizeTeras = checked;
    });

    paldeaRaidSettingsLayout->addLayout(row0);

    QCheckBox* shiny_trainers = new QCheckBox("Allow Shiny Pokemon", paldeaRaidGroupSettings);
    row1->addWidget(shiny_trainers);
    // Connect for Setting the values
    connect(shiny_trainers, &QCheckBox::toggled, this, [&trainer](bool checked) mutable{
        trainer.allowShinies = checked;
    });

    paldeaRaidSettingsLayout->addLayout(row1);
    QCheckBox* singles_or_doubles;
    QCheckBox* all_doubles;
    if(trainerType != "Raid"){
        row2 = new QHBoxLayout();
        singles_or_doubles = new QCheckBox("Randomnly Choose between Singles/Double Battles", paldeaRaidGroupSettings);
        row2->addWidget(singles_or_doubles);
        // Connect for Setting the values
        connect(singles_or_doubles, &QCheckBox::toggled, this, [&trainer](bool checked) mutable{
            trainer.singlesOrDoubles = checked;
        });

        all_doubles = new QCheckBox("All Double Battles (where possible)", paldeaRaidGroupSettings);
        row2->addWidget(all_doubles);
        // Connect for Setting the values
        connect(all_doubles, &QCheckBox::toggled, this, [&trainer](bool checked) mutable{
            trainer.allDoubles = checked;
        });

        paldeaRaidSettingsLayout->addLayout(row2);
    }

    QCheckBox* force_6 = new QCheckBox("Force 6 Pokemons", paldeaRaidGroupSettings);
    row3->addWidget(force_6);
    // Connect for Setting the values
    connect(force_6, &QCheckBox::toggled, this, [&trainer](bool checked) mutable{
        trainer.force6Pokemon = checked;
    });

    QCheckBox* give_extra = new QCheckBox("Give Extra Pokemon", paldeaRaidGroupSettings);
    row3->addWidget(give_extra);
    // Connect for Setting the values
    connect(give_extra, &QCheckBox::toggled, this, [&trainer](bool checked) mutable{
        trainer.extraPokemon = checked;
    });

    QCheckBox* force_perfect = new QCheckBox("Force Perfect IVs", paldeaRaidGroupSettings);
    row3->addWidget(force_perfect);
    // Connect for Setting the values
    connect(force_perfect, &QCheckBox::toggled, this, [&trainer](bool checked) mutable{
        trainer.forcePerfectIV = checked;
    });

    paldeaRaidSettingsLayout->addLayout(row3);

    QCheckBox* make_ai_smart = new QCheckBox("Make AI Smart", paldeaRaidGroupSettings);
    row4->addWidget(make_ai_smart);
    // Connect for Setting the values
    connect(make_ai_smart, &QCheckBox::toggled, this, [&trainer](bool checked) mutable{
        trainer.makeAISmart = checked;
    });

    paldeaRaidSettingsLayout->addLayout(row4);

    setupAllowedPokemon(paldeaRaidSettingsLayout, trainer.allowedPokemons);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return globalTrainerSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createBossSettings(QString boss, allowedPokemonLimiter limiter, bool paldea){
    QVBoxLayout *bossSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> bossSettings;

    // Creates the Main Button
    QCheckBox* enable_boss = new QCheckBox(QStringLiteral("Randomize %1").arg(boss));
    bossSettingLayout->addWidget(enable_boss);
    // Connect for Setting the values
    connect(enable_boss, &QCheckBox::toggled, this, [this](bool checked) mutable{
        randomizer.svRandomizerBosses.randomize_bosses = checked;
    });

    // Creates Hidden Group based on button
    QGroupBox *bossGroupSettings= new QGroupBox(QStringLiteral("%1's Settings Section").arg(boss));
    QVBoxLayout *bossSettingsLayout = new QVBoxLayout(bossGroupSettings);
    // QHBoxLayout *row0 = new QHBoxLayout();

    // // Connect Randomize Starters to visibility
    // paldeaRaidGroupSettings->setVisible(false);
    // connect(enable_boss, &QCheckBox::toggled, paldeaRaidGroupSettings, &QGroupBox::setVisible);

    // bossSettingLayout->addWidget(paldeaRaidGroupSettings);

    // // Row 0
    // QCheckBox* paldea_all_regions;
    // if(paldea == true && boss == "All"){
    //     paldea_all_regions = new QCheckBox("Use Paldea Settings for all Regions", paldeaRaidGroupSettings);
    //     row0->addWidget(paldea_all_regions);
    //     // Connect for Setting the values
    //     connect(paldea_all_regions, &QCheckBox::toggled, this, [this](bool checked) mutable{

    //     });
    // }

    // QCheckBox* force_shiny = new QCheckBox("Force Shiny", paldeaRaidGroupSettings);
    // row0->addWidget(force_shiny);
    // // Connect for Setting the values
    // connect(force_shiny, &QCheckBox::toggled, this, [this](bool checked) mutable{

    // });

    // paldeaRaidSettingsLayout->addLayout(row0);

    setupAllowedPokemon(bossSettingsLayout, limiter);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return bossSettingLayout;
}

// Helpers
void SVRandomizerWindow::switchTabs(QStackedWidget * stackedWidget, int index) {
    if (index >= 0 && index < stackedWidget->count()) {
        stackedWidget->setCurrentIndex(index);
    } else {
        qWarning() << "Invalid tab index:" << index;
    }
}

QJsonDocument SVRandomizerWindow::exportSettings(QMap<QString, QVariant> map){
    QJsonObject jsonObj;
    for (auto it = map.begin(); it != map.end(); ++it) {
        const QString &key = it.key();
        const QVariant &value = it.value();

        // Convert QVariant to appropriate QJsonValue
        if (value.typeId() == QMetaType::Bool) {
            jsonObj.insert(key, value.toBool());
        } else if (value.typeId() == QMetaType::Int) {
            jsonObj.insert(key, value.toInt());
        } else if (value.typeId() == QMetaType::Double) {
            jsonObj.insert(key, value.toDouble());
        } else if (value.typeId() == QMetaType::QString) {
            jsonObj.insert(key, value.toString());
        } else if (value.typeId() == QMetaType::QVariantList) {
            jsonObj.insert(key, QJsonArray::fromVariantList(value.toList()));
        } else if (value.typeId() == QMetaType::QVariantMap) {
            jsonObj.insert(key, exportSettings(value.toMap()).object()); // Recursive call for nested maps
        } else {
            qDebug() << "Unsupported QVariant type for key:" << key << "with type:" << value.typeName();
        }
    }

    return QJsonDocument(jsonObj);
};

void SVRandomizerWindow::createStarterPokemonSelection(QString starter, QStringList allowedPokemon,
                                                       QStringList allowedPokeballs, QGroupBox* groupToAdd,
                                                       QHBoxLayout* rowToAdd, QVBoxLayout* layoutToAdd, int index){

    QCheckBox* starters_shiny = new QCheckBox("Shiny ", groupToAdd);
    rowToAdd->addWidget(starters_shiny);
    connect(starters_shiny, &QCheckBox::toggled, this, [this, index, starter](bool checked) mutable{
        QMap<QString, QVariant> localSettings = settings["Starters"].toMap();
        QMap<QString, QVariant> startersSettings = localSettings[starter].toMap();

        startersSettings["Shiny"] = checked;
        randomizer.svRandomizerStarters.startersShiny[index] = checked;
        localSettings[starter] = startersSettings;
        settings["Starters"] = localSettings;
    });

    QLabel *starterLabel = new QLabel(starter, groupToAdd);
    starterLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    rowToAdd->addWidget(starterLabel);

    QLineEdit* starters = new QLineEdit(groupToAdd);

    // Sample list of suggestions
    QCompleter *completer = new QCompleter(allowedPokemon, starters);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    starters->setCompleter(completer);
    // Connect to save pokemon
    connect(starters, &QLineEdit::textChanged, this, [this, index, starter](const QString &text) mutable{
        QMap<QString, QVariant> localSettings = settings["Starters"].toMap();
        QMap<QString, QVariant> startersSettings = localSettings[starter].toMap();

        startersSettings["Name"] = text;
        randomizer.svRandomizerStarters.starters[index] = text;
        localSettings[starter] = startersSettings;
        settings["Starters"] = localSettings;
    });

    starters->setFixedWidth(175);
    starters->setPlaceholderText("Leave Blank for Random...");
    rowToAdd->addWidget(starters);

    QLabel *form_s1 = new QLabel("Form ", groupToAdd);
    form_s1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    rowToAdd->addWidget(form_s1);

    QComboBox* starters_form = new QComboBox(groupToAdd);
    starters_form->addItem("---");
    starters_form->setFixedSize(100, 25);

    rowToAdd->addWidget(starters_form);

    // Double connect to change forms based on pokemon selected
    connect(starters, &QLineEdit::textChanged, this, [=, this](const QString &text) mutable {
        starters_form->clear();
        for(int i = 0; i< randomizer.nationalDexPokemonNamesAndForms[text].size(); i++){
            starters_form->addItem(randomizer.nationalDexPokemonNamesAndForms[text][i]);
        }

    });
    connect(starters_form, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=, this]() mutable{
        QMap<QString, QVariant> localSettings = settings["Starters"].toMap();
        QMap<QString, QVariant> startersSettings = localSettings[starter].toMap();

        int setForm = 0;
        for(int i =0; i< randomizer.nationalDexPokemonNamesAndForms[randomizer.svRandomizerStarters.starters[index]].size(); i++){
            if(starters_form->currentText() == randomizer.nationalDexPokemonNamesAndForms[randomizer.svRandomizerStarters.starters[index]][i]){
                setForm = i;
                if(randomizer.svRandomizerStarters.starters[index] == "Pikachu" && setForm == 8){
                    setForm = 9;
                }
            }
        }
        startersSettings["Form"] = setForm;
        randomizer.svRandomizerStarters.startersForms[index] = setForm;
        localSettings[starter] = startersSettings;
        settings["Starters"] = localSettings;
    });

    QLabel *gender_s1 = new QLabel("Gender ", groupToAdd);
    gender_s1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    rowToAdd->addWidget(gender_s1);

    QComboBox* starters_gender = new QComboBox(groupToAdd);
    starters_gender->addItem("---");
    starters_gender->setFixedSize(100, 25);

    rowToAdd->addWidget(starters_gender);

    // Triple connect to adjust for a pokemon's gender varying based on form and the last one is to save it.
    connect(starters, &QLineEdit::textChanged, this, [=, this](const QString &text) mutable{
        starters_gender->clear();
        if(randomizer.genderForms.contains(text)){
            starters_gender->addItem("DEFAULT");
        }else if(randomizer.femaleOnlyPokemon.contains(text)){
            starters_gender->addItem("FEMALE");
        }else if(randomizer.maleOnlyPokemon.contains(text) || randomizer.formsMaleOnly[text].contains(index)){
            starters_gender->addItem("MALE");
        } else if(randomizer.genderlessPokemon.contains(text)){
            starters_gender->addItem("GENDERLESS");
        } else{
            starters_gender->addItem("MALE");
            starters_gender->addItem("FEMALE");
        }
    });
    connect(starters_form, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=, this]() mutable{
        starters_gender->clear();
        if(randomizer.genderForms.contains(randomizer.svRandomizerStarters.starters[index])){
            starters_gender->addItem("DEFAULT");
        }else if(randomizer.femaleOnlyPokemon.contains(randomizer.svRandomizerStarters.starters[index])){
            starters_gender->addItem("FEMALE");
        }else if(randomizer.maleOnlyPokemon.contains(randomizer.svRandomizerStarters.starters[index]) || randomizer.formsMaleOnly[randomizer.svRandomizerStarters.starters[index]].contains(randomizer.svRandomizerStarters.startersForms[index])){
            starters_gender->addItem("MALE");
        } else if(randomizer.genderlessPokemon.contains(randomizer.svRandomizerStarters.starters[index])){
            starters_gender->addItem("GENDERLESS");
        } else{
            starters_gender->addItem("MALE");
            starters_gender->addItem("FEMALE");
        }
    });
    connect(starters_gender, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=, this]() mutable{
        QMap<QString, QVariant> localSettings = settings["Starters"].toMap();
        QMap<QString, QVariant> startersSettings = localSettings[starter].toMap();

        startersSettings["Gender"] = starters_gender->currentText();
        randomizer.svRandomizerStarters.startersGenders[index] = starters_gender->currentText();
        localSettings[starter] = startersSettings;
        settings["Starters"] = localSettings;
    });

    QLabel *pokeball_s1 = new QLabel("Pokeball ", groupToAdd);
    pokeball_s1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    rowToAdd->addWidget(pokeball_s1);

    // Update the combobox to match allowed pokeballs
    QComboBox* starters_pokeball = new QComboBox(groupToAdd);
    for(int i = 0; i<allowedPokeballs.length(); i++){
        starters_pokeball->addItem(allowedPokeballs[i]);
    }
    starters_pokeball->setFixedSize(100, 25);

    // Connect to save the pokeball
    connect(starters_pokeball, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, starters_pokeball, starter, index]() mutable{
        QMap<QString, QVariant> localSettings = settings["Starters"].toMap();
        QMap<QString, QVariant> startersSettings = localSettings[starter].toMap();

        startersSettings["Pokeball"] = starters_pokeball->currentText();
        randomizer.svRandomizerStarters.startersPokeballs[index] = starters_pokeball->currentText(); // connected
        localSettings[starter] = startersSettings;
        settings["Starters"] = localSettings;
    });

    rowToAdd->addWidget(starters_pokeball);

    layoutToAdd->addLayout(rowToAdd);
    rowToAdd->addStretch(1);

    // Connect based on imports
    connect(this, &SVRandomizerWindow::importSettings, this, [=, this]() mutable{
        qDebug()<<"Here";
    });
}

void SVRandomizerWindow::setupAllowedPokemon(QVBoxLayout *outerBox, allowedPokemonLimiter& limiter){
    QGroupBox* limiterGroup = new QGroupBox("Pokemon Limiter", this);
    outerBox->addWidget(limiterGroup);

    QVBoxLayout* limiterSetUp = new QVBoxLayout(limiterGroup);
    QHBoxLayout* generations = new QHBoxLayout();
    QHBoxLayout* legendaries = new QHBoxLayout();
    QHBoxLayout* stages = new QHBoxLayout();
    QHBoxLayout* special = new QHBoxLayout();

    QVector<QCheckBox*> generationslist;

    QLabel *generationsHeader = new QLabel("Allowed Generations", limiterGroup);
    generationsHeader->setStyleSheet("font-weight: bold; padding: 0px 0;");
    generations->addWidget(generationsHeader);

    for(int i =0; i<9; i++){
        generationslist.append(new QCheckBox(QStringLiteral("%1").arg(i+1), limiterGroup));
        generationslist[i]->setChecked(true);
        generationslist[i]->setFixedSize(60, 20);
        generations->addWidget(generationslist[i]);
        // Connects Here
        connect(generationslist[i], &QCheckBox::toggled, this, [&limiter, i](bool checked) mutable{
           // add settings changes
            limiter.gens[i] = checked;
        });
    }
    limiterSetUp->addLayout(generations);

    //-------------------------------------------------------------------------------------------

    QVector<QCheckBox*> legendariesList;

    QLabel *legendsHeader = new QLabel("Allowed Legendaries", limiterGroup);
    legendsHeader->setStyleSheet("font-weight: bold; padding: 0px 0;");
    legendaries->addWidget(legendsHeader);

    for(int i =0; i<9; i++){
        legendariesList.append(new QCheckBox(QStringLiteral("%1").arg(i+1), limiterGroup));
        legendariesList[i]->setChecked(true);
        legendariesList[i]->setFixedSize(60, 20);
        legendaries->addWidget(legendariesList[i]);
        // Connects Here
        connect(legendariesList[i], &QCheckBox::toggled, this, [&limiter, i](bool checked) mutable{
            // add settings changes
            limiter.genLegends[i] = checked;
        });
    }
    limiterSetUp->addLayout(legendaries);

    //-------------------------------------------------------------------------------------------

    QCheckBox *stage_1 = new QCheckBox("Stage 1", limiterGroup);
    stage_1->setChecked(true);
    stages->addWidget(stage_1);
    connect(stage_1, &QCheckBox::toggled, this, [&limiter](bool checked) mutable{
        // add settings changes
        limiter.stage1 = checked;
    });

    QCheckBox *stage_2 = new QCheckBox("Stage 2", limiterGroup);
    stage_2->setChecked(true);
    stages->addWidget(stage_2);
    connect(stage_2, &QCheckBox::toggled, this, [&limiter](bool checked) mutable{
        // add settings changes
        limiter.stage2 = checked;
    });

    QCheckBox *stage_3 = new QCheckBox("Stage 3", limiterGroup);
    stage_3->setChecked(true);
    stages->addWidget(stage_3);
    connect(stage_3, &QCheckBox::toggled, this, [&limiter](bool checked) mutable{
        // add settings changes
        limiter.stage3 = checked;
    });

    QCheckBox *single_stage = new QCheckBox("Single stage", limiterGroup);
    single_stage->setChecked(true);
    stages->addWidget(single_stage);
    connect(single_stage, &QCheckBox::toggled, this, [&limiter](bool checked) mutable{
        // add settings changes
        limiter.singleStage = checked;
    });

    limiterSetUp->addLayout(stages);

    //-------------------------------------------------------------------------------------------

    QCheckBox *paradox = new QCheckBox("Paradox", limiterGroup);
    paradox->setChecked(true);
    special->addWidget(paradox);
    limiterSetUp->addLayout(special);
    connect(paradox, &QCheckBox::toggled, this, [&limiter](bool checked) mutable{
        // add settings changes
        limiter.paradox = checked;
    });

}

// Intializers
void SVRandomizerWindow::initializeSettings(){

    // Allowed Settings (Used for all)
        QMap<QString, QVariant> allowedPokemonSettings;
        QMap<QString, QVariant> generationsSettings;
        QMap<QString, QVariant> legendarySettings;
        for(int i=1; i<=maxGeneration; i++){
            generationsSettings[QStringLiteral("%1").arg(i)] = true;
            legendarySettings[QStringLiteral("%1").arg(i)] = true;
        }
        allowedPokemonSettings["Generations"] = QVariant::fromValue(generationsSettings);
        allowedPokemonSettings["Legends"] = QVariant::fromValue(legendarySettings);
        allowedPokemonSettings["Stage 1"] = true;
        allowedPokemonSettings["Stage 2"] = true;
        allowedPokemonSettings["Stage 3"] = true;
        allowedPokemonSettings["Single Stage"] = true;
        allowedPokemonSettings["Paradox"] = true;

    // Starter Settings
        QMap<QString, QVariant> startersSettings;
        startersSettings["Randomize"] = false;
        startersSettings["Tera Types"] = false;
        startersSettings["All Shiny"] = false;
        startersSettings["One Shiny"] = false;
        for(int i =0; i<starterNames.size(); i++){
            QMap<QString, QVariant> starterSpecificSettings;
            starterSpecificSettings["Shiny"] = false;
            starterSpecificSettings["Name"] = "";
            starterSpecificSettings["Form"] = 0;
            starterSpecificSettings["Gender"] = "MALE";
            starterSpecificSettings["Pokeball"] = "Poke Ball";
            startersSettings[starterNames[i]] = QVariant::fromValue(starterSpecificSettings);
        }
        startersSettings["Allowed Pokemon"] = QVariant::fromValue(allowedPokemonSettings);
        settings["Starters"] = QVariant::fromValue(startersSettings);

    // Gift Settings
        QMap<QString, QVariant> giftSettings;
        giftSettings["Randomize"] = false;
        giftSettings["Tera Types"] = false;
        giftSettings["Allowed Pokemon"] = QVariant::fromValue(allowedPokemonSettings);
        settings["Gifts"] = QVariant::fromValue(giftSettings);

    // Stats Settings
        QMap<QString, QVariant> abilitySettings;
        abilitySettings["Randomize"] = false;
        abilitySettings["Ban Wonder Guard"] = false;
        abilitySettings["Ban Exit Abilities"] = false;
        settings["Abilities"] = QVariant::fromValue(abilitySettings);

    // Types Settings
        QMap<QString, QVariant> typesSettings;
        typesSettings["Randomize"] = false;
        typesSettings["Grant Extra Types"] = false;
        settings["Types"] = QVariant::fromValue(typesSettings);

    // Moveset Settings
        QMap<QString, QVariant> movesSettings;
        movesSettings["Randomize"] = false;
        movesSettings["Same Move Type"] = false;
        settings["Moves"] = QVariant::fromValue(movesSettings);

    // BST Settings
        QMap<QString, QVariant> bstSettings;
        bstSettings["Randomize"] = false;
        bstSettings["Same BST"] = false;
        settings["BST"] = QVariant::fromValue(bstSettings);

    // Evolutions Settings
        QMap<QString, QVariant> evolutionsSettings;
        evolutionsSettings["Randomize"] = false;
        evolutionsSettings["Fix Evolutions"] = false;
        evolutionsSettings["Randomize Every Level"] = false;
        settings["Evolutions"] = QVariant::fromValue(evolutionsSettings);

    // TM Settings
        QMap<QString, QVariant> tmSettings;
        tmSettings["Randomize"] = false;
        tmSettings["Moves Without Animations"] = false;
        settings["TMs"] = QVariant::fromValue(tmSettings);

    // Item Settings
        QMap<QString, QVariant> itemSettings;
        itemSettings["Randomize"] = false;
        itemSettings["Let's Go"] = false;
        itemSettings["Hidden"] = false;
        itemSettings["Drops"] = false;
        itemSettings["Pick Up"] = false;
        settings["Items"] = QVariant::fromValue(itemSettings);

    // Wild Settings
        QStringList keys = {"Paldea", "Kitakami", "Blueberry"};
        for(int i =0; i< keys.size(); i++){
            QMap<QString, QVariant> wildSettings;
            wildSettings["Randomizer"] = false;
            wildSettings["Ogerpon/Terapagos"] = false;
            wildSettings["Balance BST"] = false;
            if(keys[i]=="Paldea"){
                wildSettings["Paldea For All"] = false;
            }
            wildSettings["Allowed Pokemon"] = QVariant::fromValue(allowedPokemonSettings);
            settings[keys[i]+"-Wilds"] = QVariant::fromValue(wildSettings);

            QMap<QString, QVariant> raidSettings;
            raidSettings["Randomize"] = false;
            raidSettings["Shiny Raids"] = false;
            raidSettings["Balance BST"] = false;
            if(keys[i]=="Paldea"){
                raidSettings["Paldea For All"] = false;
            }
            raidSettings["Allowed Pokemon"] = QVariant::fromValue(allowedPokemonSettings);
            settings[keys[i]+"-Raids"] = QVariant::fromValue(raidSettings);
        }
}

// Example for big button (kept for future things)
// QGroupBox *startersGroupSettings= new QGroupBox("Starter Settings Section");
// QPushButton *toggleStartersGroupSettings= new QPushButton("Toggle Starter Settings Section", startersWidget);

// // StyleSheet is a format changer. Lets you change the way something looks - right now only used on buttons but can be used
// // for everything else.
// toggleStartersGroupSettings->setStyleSheet(
//     "QPushButton {"
//     "   background-color: #3498db;"
//     "   color: white;"
//     "   border: none;"
//     "   padding: 8px 16px;"
//     "   border-radius: 10px;"
//     "}"
//     "QPushButton:hover {"
//     "   background-color: #2980b9;"
//     "}"
//     "QPushButton:pressed {"
//     "   background-color: #1c6692;"
//     "}"
// );

// // Connect to Hide the section
// connect(toggleStartersGroupSettings, &QPushButton::clicked, startersGroupSettings, [startersGroupSettings]() {
//     startersGroupSettings->setVisible(!startersGroupSettings->isVisible());
// });
// formLayout->addRow(toggleStartersGroupSettings);
