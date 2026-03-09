#include "mainwindow.h"
#include <QApplication>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QRandomGenerator>
#include <QFont>
#include <QFrame>
#include <QGridLayout>
#include <QSizePolicy>

// ==================== COLORS (matching preview) ====================
static const QString COL_BG      = "#111827"; // gray-900
static const QString COL_SIDEBAR  = "#1f2937"; // gray-800
static const QString COL_CONTENT  = "#374151"; // gray-700 (fight area)
static const QString COL_CARD     = "#374151"; // gray-700
static const QString COL_DARK     = "#111827"; // gray-900
static const QString COL_BORDER   = "#374151"; // gray-700
static const QString COL_TEXT     = "#f3f4f6"; // gray-100
static const QString COL_TEXTDIM  = "#9ca3af"; // gray-400
static const QString COL_BLUE     = "#2563eb"; // blue-600
static const QString COL_GREEN    = "#22c55e"; // green-500
static const QString COL_RED      = "#dc2626"; // red-600
static const QString COL_YELLOW   = "#facc15"; // yellow-400

// ==================== CONSTRUCTOR ====================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , player(new Player(this))
    , currentEnemy(nullptr)
    , enemyAttackTimer(new QTimer(this))
    , currentMapLevel(0)
    , mapKills(3, 0)
{
    Enemy::setMapLevel(0);
    setupUI();
    setupConnections();
    spawnNewEnemy();
    updateBottomBar();
    updateStatsPage();
    updateMapPage();

    connect(enemyAttackTimer, &QTimer::timeout, this, &MainWindow::onEnemyAttack);
    enemyAttackTimer->start(1500);
}

MainWindow::~MainWindow() {}

// ==================== SETUP UI ====================
void MainWindow::setupUI()
{
    setWindowTitle("Clicker RPG");
    setMinimumSize(1000, 700);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    central->setStyleSheet(QString("background-color: %1;").arg(COL_BG));

    QHBoxLayout *mainH = new QHBoxLayout(central);
    mainH->setContentsMargins(0, 0, 0, 0);
    mainH->setSpacing(0);

    // ===== SIDEBAR =====
    QWidget *sidebar = new QWidget;
    sidebar->setFixedWidth(256);
    sidebar->setStyleSheet(QString("background-color: %1; border-right: 1px solid %2;").arg(COL_SIDEBAR, COL_BORDER));

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(24, 24, 24, 24);
    sidebarLayout->setSpacing(0);
    setupSidebar(sidebarLayout);
    mainH->addWidget(sidebar);

    // ===== RIGHT SIDE (content + bottom bar) =====
    QWidget *rightSide = new QWidget;
    QVBoxLayout *rightLayout = new QVBoxLayout(rightSide);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);

    contentStack = new QStackedWidget;
    setupFightPage();
    setupStatsPage();
    setupMapPage();
    rightLayout->addWidget(contentStack, 1);

    setupBottomBar(rightLayout);
    mainH->addWidget(rightSide, 1);

    setActiveView(0);
}

// ==================== SIDEBAR ====================
void MainWindow::setupSidebar(QVBoxLayout *layout)
{
    // Title
    QLabel *title = new QLabel("Clicker RPG");
    title->setFont(QFont("Arial", 20, QFont::Bold));
    title->setStyleSheet("color: #818cf8; background: transparent;"); // purple-ish
    layout->addWidget(title);
    layout->addSpacing(32);

    // Nav buttons
    QString btnBase =
        "QPushButton { text-align: left; padding: 12px 16px; border-radius: 8px; "
        "font-size: 15px; font-weight: bold; border: none; } ";

    btnFight = new QPushButton("  ⚔  Walka");
    btnStats = new QPushButton("  📊  Statystyki");
    btnMap   = new QPushButton("  🗺  Mapa");

    for (auto *btn : {btnFight, btnStats, btnMap}) {
        btn->setFont(QFont("Arial", 14, QFont::Bold));
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(btnBase +
            QString("QPushButton { background-color: %1; color: %2; } "
                    "QPushButton:hover { background-color: #4b5563; }")
            .arg(COL_CARD, COL_TEXTDIM));
        layout->addWidget(btn);
        layout->addSpacing(8);
    }

    layout->addStretch();

    // Player info box at bottom of sidebar
    QWidget *infoBox = new QWidget;
    infoBox->setStyleSheet(QString(
        "background-color: rgba(17,24,39,0.5); border: 1px solid %1; border-radius: 8px;"
    ).arg(COL_BORDER));

    QVBoxLayout *infoLayout = new QVBoxLayout(infoBox);
    infoLayout->setContentsMargins(16, 12, 16, 12);
    infoLayout->setSpacing(4);

    lblSidebarLevel = new QLabel("Poziom 1");
    lblSidebarLevel->setStyleSheet("color: #9ca3af; font-size: 13px; background: transparent;");

    lblSidebarAttack = new QLabel("⚔  10 Atak");
    lblSidebarAttack->setStyleSheet("color: white; font-size: 16px; font-weight: bold; background: transparent;");

    lblSidebarDefense = new QLabel("🛡  5 Obrona");
    lblSidebarDefense->setStyleSheet("color: white; font-size: 16px; font-weight: bold; background: transparent;");

    infoLayout->addWidget(lblSidebarLevel);
    infoLayout->addWidget(lblSidebarAttack);
    infoLayout->addWidget(lblSidebarDefense);

    layout->addWidget(infoBox);
}

// ==================== FIGHT PAGE ====================
void MainWindow::setupFightPage()
{
    fightPage = new QWidget;
    fightPage->setStyleSheet(
        "background-image: url(:/resources/forest.jpg);"
        "background-repeat: no-repeat;"
        "background-position: center;"
        "background-size: cover;"
        );
    QVBoxLayout *outerLayout = new QVBoxLayout(fightPage);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    // The entire fight area is ONE big clickable button
    btnAttackArea = new QPushButton(fightPage);
    btnAttackArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    btnAttackArea->setCursor(Qt::CrossCursor);
    btnAttackArea->setStyleSheet(QString(
        "QPushButton { background-color: %1; border: none; } "
        "QPushButton:hover { background-color: #4b5563; } "
        "QPushButton:pressed { background-color: #374151; }"
    ).arg(COL_CONTENT));

    // We overlay labels on top of the button using a layout inside the button
    QVBoxLayout *fightLayout = new QVBoxLayout(btnAttackArea);
    fightLayout->setContentsMargins(0, 60, 0, 40);
    fightLayout->setSpacing(0);

    // Enemy name
    lblEnemyName = new QLabel("Szczur");
    lblEnemyName->setFont(QFont("Arial", 32, QFont::ExtraBold));
    lblEnemyName->setAlignment(Qt::AlignCenter);
    lblEnemyName->setStyleSheet("color: white; background: transparent;");
    lblEnemyName->setAttribute(Qt::WA_TransparentForMouseEvents);
    fightLayout->addWidget(lblEnemyName);
    fightLayout->addSpacing(16);

    // Enemy HP bar
    QWidget *hpBarContainer = new QWidget;
    hpBarContainer->setFixedWidth(500);
    hpBarContainer->setStyleSheet("background: transparent;");
    hpBarContainer->setAttribute(Qt::WA_TransparentForMouseEvents);
    QVBoxLayout *hpBarLayout = new QVBoxLayout(hpBarContainer);
    hpBarLayout->setContentsMargins(0, 0, 0, 0);

    barEnemyHp = new QProgressBar;
    barEnemyHp->setFixedHeight(32);
    barEnemyHp->setTextVisible(false);
    barEnemyHp->setAttribute(Qt::WA_TransparentForMouseEvents);
    barEnemyHp->setStyleSheet(
        "QProgressBar { background-color: #111827; border-radius: 16px; border: 2px solid #1f2937; } "
        "QProgressBar::chunk { background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #dc2626, stop:1 #f87171); border-radius: 14px; }");
    hpBarLayout->addWidget(barEnemyHp);

    lblEnemyHpText = new QLabel("20 / 20 HP");
    lblEnemyHpText->setAlignment(Qt::AlignCenter);
    lblEnemyHpText->setFont(QFont("Arial", 12, QFont::Bold));
    lblEnemyHpText->setStyleSheet("color: white; background: transparent; margin-top: -30px;");
    lblEnemyHpText->setAttribute(Qt::WA_TransparentForMouseEvents);
    hpBarLayout->addWidget(lblEnemyHpText);

    // Center the HP bar
    QHBoxLayout *hpCenter = new QHBoxLayout;
    hpCenter->addStretch();
    hpCenter->addWidget(hpBarContainer);
    hpCenter->addStretch();
    QWidget *hpCenterWidget = new QWidget;
    hpCenterWidget->setLayout(hpCenter);
    hpCenterWidget->setStyleSheet("background: transparent;");
    hpCenterWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
    fightLayout->addWidget(hpCenterWidget);

    fightLayout->addSpacing(40);

    // Enemy emoji
    lblEnemyEmoji = new QLabel("🐀");
    lblEnemyEmoji->setFont(QFont("Arial", 140));
    lblEnemyEmoji->setAlignment(Qt::AlignCenter);
    lblEnemyEmoji->setStyleSheet("background: transparent;");
    lblEnemyEmoji->setAttribute(Qt::WA_TransparentForMouseEvents);
    fightLayout->addWidget(lblEnemyEmoji);

    fightLayout->addStretch();

    // Bottom hint text
    QLabel *hint = new QLabel("KLIKNIJ DOWOLNE MIEJSCE ABY ZAATAKOWAĆ");
    hint->setFont(QFont("Arial", 13, QFont::Bold));
    hint->setAlignment(Qt::AlignCenter);
    hint->setStyleSheet("color: #6b7280; background: transparent; letter-spacing: 3px;");
    hint->setAttribute(Qt::WA_TransparentForMouseEvents);
    fightLayout->addWidget(hint);

    outerLayout->addWidget(btnAttackArea);
    contentStack->addWidget(fightPage);
}

// ==================== STATS PAGE ====================
void MainWindow::setupStatsPage()
{
    statsPage = new QWidget;
    statsPage->setStyleSheet(QString("background-color: %1;").arg(COL_SIDEBAR));

    QVBoxLayout *layout = new QVBoxLayout(statsPage);
    layout->setContentsMargins(48, 48, 48, 48);
    layout->setSpacing(0);

    // Title
    QLabel *title = new QLabel("📊  Statystyki i Ulepszenia");
    title->setFont(QFont("Arial", 24, QFont::Bold));
    title->setStyleSheet(QString("color: %1; padding-bottom: 16px; border-bottom: 1px solid %2; background: transparent;")
                         .arg(COL_TEXT, COL_BORDER));
    layout->addWidget(title);
    layout->addSpacing(32);

    // 3-card grid
    QHBoxLayout *grid = new QHBoxLayout;
    grid->setSpacing(24);

    // Helper lambda to create upgrade card
    auto makeCard = [&](const QString &icon, const QString &titleText, const QString &iconColor,
                        QLabel *&valueLabel, QPushButton *&btn) -> QWidget* {
        QWidget *card = new QWidget;
        card->setStyleSheet(QString(
            "background-color: %1; border: 1px solid #4b5563; border-radius: 12px;"
        ).arg(COL_CARD));

        QVBoxLayout *cl = new QVBoxLayout(card);
        cl->setContentsMargins(24, 24, 24, 24);
        cl->setSpacing(8);

        QLabel *header = new QLabel(icon + "  " + titleText);
        header->setFont(QFont("Arial", 18, QFont::Bold));
        header->setStyleSheet(QString("color: %1; background: transparent;").arg(iconColor));
        cl->addWidget(header);
        cl->addSpacing(8);

        QLabel *curLabel = new QLabel("Obecnie:");
        curLabel->setStyleSheet("color: #d1d5db; font-size: 14px; background: transparent;");
        cl->addWidget(curLabel);

        valueLabel = new QLabel("10");
        valueLabel->setFont(QFont("Arial", 22, QFont::Bold));
        valueLabel->setStyleSheet("color: white; background: transparent;");
        cl->addWidget(valueLabel);

        cl->addStretch();

        btn = new QPushButton("Ulepsz");
        btn->setFont(QFont("Arial", 13, QFont::Bold));
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(QString(
            "QPushButton { background-color: %1; color: white; padding: 12px; border-radius: 8px; border: none; } "
            "QPushButton:hover { background-color: #3b82f6; } "
            "QPushButton:disabled { background-color: #374151; color: #6b7280; }"
        ).arg(COL_BLUE));
        cl->addWidget(btn);

        return card;
    };

    QWidget *atkCard = makeCard("⚔", "Siła Ataku", "#f87171", lblAtkValue, btnUpgAtk);
    QWidget *defCard = makeCard("🛡", "Obrona", "#60a5fa", lblDefValue, btnUpgDef);
    QWidget *hpCard  = makeCard("❤", "Maksymalne HP", "#4ade80", lblHpValue, btnUpgHp);

    grid->addWidget(atkCard);
    grid->addWidget(defCard);
    grid->addWidget(hpCard);

    layout->addLayout(grid);
    layout->addStretch();

    contentStack->addWidget(statsPage);
}

// ==================== MAP PAGE ====================
void MainWindow::setupMapPage()
{
    mapPage = new QWidget;
    mapPage->setStyleSheet(QString("background-color: %1;").arg(COL_SIDEBAR));

    QVBoxLayout *layout = new QVBoxLayout(mapPage);
    layout->setContentsMargins(48, 48, 48, 48);
    layout->setSpacing(0);

    QLabel *title = new QLabel("🗺  Wybór Mapy");
    title->setFont(QFont("Arial", 24, QFont::Bold));
    title->setStyleSheet(QString("color: %1; padding-bottom: 16px; border-bottom: 1px solid %2; background: transparent;")
                         .arg(COL_TEXT, COL_BORDER));
    layout->addWidget(title);
    layout->addSpacing(24);

    // Helper to create map row
    auto makeMapRow = [&](const QString &emoji, const QString &mapName, const QString &level,
                          QLabel *&killsLabel, QPushButton *&selectBtn) -> QWidget* {
        QWidget *row = new QWidget;
        row->setMinimumHeight(80);
        row->setStyleSheet(QString(
            "background-color: %1; border: 2px solid %2; border-radius: 12px;"
        ).arg(COL_SIDEBAR, COL_BORDER));

        QHBoxLayout *rl = new QHBoxLayout(row);
        rl->setContentsMargins(24, 16, 24, 16);

        // Left side - map info
        QVBoxLayout *infoLayout = new QVBoxLayout;

        QLabel *name = new QLabel(emoji + "  " + mapName + " (" + level + ")");
        name->setFont(QFont("Arial", 18, QFont::Bold));
        name->setStyleSheet("color: white; background: transparent;");
        infoLayout->addWidget(name);

        killsLabel = new QLabel("Pokonani przeciwnicy: 0");
        killsLabel->setStyleSheet("color: #9ca3af; font-size: 13px; background: transparent;");
        infoLayout->addWidget(killsLabel);

        rl->addLayout(infoLayout, 1);

        // Right side - button
        selectBtn = new QPushButton("Wybierz  ▶");
        selectBtn->setFont(QFont("Arial", 13, QFont::Bold));
        selectBtn->setCursor(Qt::PointingHandCursor);
        selectBtn->setFixedWidth(160);
        selectBtn->setFixedHeight(44);
        selectBtn->setStyleSheet(QString(
            "QPushButton { background-color: #4b5563; color: white; border-radius: 8px; border: none; } "
            "QPushButton:hover { background-color: #6b7280; } "
            "QPushButton:disabled { background-color: %1; color: #6b7280; border: 1px solid %2; }"
        ).arg(COL_SIDEBAR, COL_BORDER));
        rl->addWidget(selectBtn);

        return row;
    };

    QWidget *map1Row = makeMapRow("🌲", "Mroczny Las", "Poziom 1", lblMap1Kills, btnMap1);
    QWidget *map2Row = makeMapRow("⛰", "Zaginione Jaskinie", "Poziom 2", lblMap2Kills, btnMap2);
    QWidget *map3Row = makeMapRow("🌋", "Przeklęty Wulkan", "Poziom 3", lblMap3Kills, btnMap3);

    layout->addWidget(map1Row);
    layout->addSpacing(16);
    layout->addWidget(map2Row);
    layout->addSpacing(16);
    layout->addWidget(map3Row);
    layout->addStretch();

    contentStack->addWidget(mapPage);
}

// ==================== BOTTOM BAR ====================
void MainWindow::setupBottomBar(QVBoxLayout *outerLayout)
{
    QWidget *bar = new QWidget;
    bar->setFixedHeight(112);
    bar->setStyleSheet(QString(
        "background-color: %1; border-top: 2px solid %2;"
    ).arg(COL_SIDEBAR, COL_BORDER));

    QHBoxLayout *barLayout = new QHBoxLayout(bar);
    barLayout->setContentsMargins(32, 16, 32, 16);
    barLayout->setSpacing(0);

    // ===== LEFT: Gold + EXP =====
    QWidget *leftWidget = new QWidget;
    leftWidget->setStyleSheet("background: transparent;");
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(8);

    lblGold = new QLabel("💰  0 Złota");
    lblGold->setFont(QFont("Arial", 20, QFont::ExtraBold));
    lblGold->setStyleSheet("color: #facc15; background: transparent;");
    leftLayout->addWidget(lblGold);

    // EXP row
    QHBoxLayout *expRow = new QHBoxLayout;
    expRow->setSpacing(12);

    lblLevel = new QLabel("Lvl 1");
    lblLevel->setFont(QFont("Arial", 14, QFont::Bold));
    lblLevel->setStyleSheet("color: #93c5fd; background-color: rgba(30,58,138,0.5); padding: 4px 12px; border-radius: 6px;");
    expRow->addWidget(lblLevel);

    barExp = new QProgressBar;
    barExp->setFixedHeight(28);
    barExp->setStyleSheet(
        "QProgressBar { background-color: #111827; border-radius: 14px; border: 1px solid #4b5563; } "
        "QProgressBar::chunk { background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #2563eb, stop:1 #60a5fa); border-radius: 13px; }");
    barExp->setFormat("EXP: %v / %m");
    barExp->setAlignment(Qt::AlignCenter);
    expRow->addWidget(barExp, 1);

    leftLayout->addLayout(expRow);

    barLayout->addWidget(leftWidget, 2);
    barLayout->addSpacing(80);

    // ===== RIGHT: HP =====
    QWidget *rightWidget = new QWidget;
    rightWidget->setStyleSheet("background: transparent;");
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(8);

    lblHpRight = new QLabel("100 / 100 HP  ❤");
    lblHpRight->setFont(QFont("Arial", 20, QFont::ExtraBold));
    lblHpRight->setAlignment(Qt::AlignRight);
    lblHpRight->setStyleSheet("color: #f87171; background: transparent;");
    rightLayout->addWidget(lblHpRight);

    barHp = new QProgressBar;
    barHp->setFixedHeight(28);
    barHp->setTextVisible(false);
    barHp->setStyleSheet(
        "QProgressBar { background-color: #111827; border-radius: 14px; border: 1px solid #4b5563; } "
        "QProgressBar::chunk { background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #dc2626, stop:1 #ef4444); border-radius: 13px; }");
    rightLayout->addWidget(barHp);

    barLayout->addWidget(rightWidget, 2);

    outerLayout->addWidget(bar);
}

// ==================== CONNECTIONS ====================
void MainWindow::setupConnections()
{
    connect(btnFight, &QPushButton::clicked, this, &MainWindow::onFightClicked);
    connect(btnStats, &QPushButton::clicked, this, &MainWindow::onStatsClicked);
    connect(btnMap,   &QPushButton::clicked, this, &MainWindow::onMapClicked);
    connect(btnAttackArea, &QPushButton::clicked, this, &MainWindow::onEnemyClicked);
    connect(btnUpgAtk, &QPushButton::clicked, this, &MainWindow::onUpgradeAttack);
    connect(btnUpgDef, &QPushButton::clicked, this, &MainWindow::onUpgradeDefense);
    connect(btnUpgHp,  &QPushButton::clicked, this, &MainWindow::onUpgradeMaxHp);
    connect(btnMap1, &QPushButton::clicked, this, &MainWindow::onSelectMap1);
    connect(btnMap2, &QPushButton::clicked, this, &MainWindow::onSelectMap2);
    connect(btnMap3, &QPushButton::clicked, this, &MainWindow::onSelectMap3);
    connect(player, &Player::statsChanged, this, &MainWindow::onPlayerStatsChanged);
    connect(player, &Player::leveledUp, this, &MainWindow::onPlayerLeveledUp);
}

// ==================== VIEW SWITCHING ====================
void MainWindow::setActiveView(int index)
{
    contentStack->setCurrentIndex(index);

    QString active =
        QString("QPushButton { background-color: %1; color: white; text-align: left; "
                "padding: 12px 16px; border-radius: 8px; font-size: 15px; font-weight: bold; border: none; }")
        .arg(COL_BLUE);

    QString inactive =
        QString("QPushButton { background-color: %1; color: %2; text-align: left; "
                "padding: 12px 16px; border-radius: 8px; font-size: 15px; font-weight: bold; border: none; } "
                "QPushButton:hover { background-color: #4b5563; }")
        .arg(COL_CARD, COL_TEXTDIM);

    btnFight->setStyleSheet(index == 0 ? active : inactive);
    btnStats->setStyleSheet(index == 1 ? active : inactive);
    btnMap->setStyleSheet(index == 2 ? active : inactive);
}

void MainWindow::onFightClicked() { setActiveView(0); }
void MainWindow::onStatsClicked() { setActiveView(1); updateStatsPage(); }
void MainWindow::onMapClicked()   { setActiveView(2); updateMapPage(); }

// ==================== ENEMY ====================
void MainWindow::spawnNewEnemy()
{
    if (currentEnemy) currentEnemy->deleteLater();
    currentEnemy = new Enemy(this);
    connect(currentEnemy, &Enemy::enemyDefeated, this, &MainWindow::onEnemyDefeated);
    connect(currentEnemy, &Enemy::hpChanged, this, &MainWindow::updateEnemyDisplay);
    updateEnemyDisplay();
}

void MainWindow::updateEnemyDisplay()
{
    if (!currentEnemy) return;

    lblEnemyName->setText(currentEnemy->getName());
    barEnemyHp->setMaximum(currentEnemy->getMaxHp());
    barEnemyHp->setValue(currentEnemy->getHp());
    lblEnemyHpText->setText(QString("%1 / %2 HP")
                                .arg(currentEnemy->getHp())
                                .arg(currentEnemy->getMaxHp()));

    // ZAWSZE traktujemy to jako ścieżkę do obrazka
    QPixmap pix(currentEnemy->getEmoji());
    QPixmap scaled = pix.scaled(260, 260, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    lblEnemyEmoji->setPixmap(scaled);
    lblEnemyEmoji->setText("");
}

void MainWindow::onEnemyClicked()
{
    if (!currentEnemy || !player->isAlive()) return;

    int damage = player->getAttack() + QRandomGenerator::global()->bounded(-2, 3);
    damage = qMax(1, damage);
    currentEnemy->takeDamage(damage);

    // Floating damage
    QPoint center = btnAttackArea->mapTo(this, btnAttackArea->rect().center());
    int rx = QRandomGenerator::global()->bounded(-60, 60);
    int ry = QRandomGenerator::global()->bounded(-40, 40);
    showFloatingDamage(damage, QPoint(center.x() + rx, center.y() + ry - 40));
}

void MainWindow::onEnemyDefeated()
{
    if (!currentEnemy) return;
    player->gainExp(currentEnemy->getExpReward());
    player->gainGold(currentEnemy->getGoldReward());
    mapKills[currentMapLevel]++;
    player->heal();
    spawnNewEnemy();
}

void MainWindow::showFloatingDamage(int damage, const QPoint &pos)
{
    QLabel *lbl = new QLabel(QString("-%1").arg(damage), this);
    lbl->setFont(QFont("Arial", 32, QFont::ExtraBold));
    lbl->setStyleSheet("color: #f87171; background: transparent;");
    lbl->move(pos);
    lbl->show();
    lbl->raise();

    QPropertyAnimation *anim = new QPropertyAnimation(lbl, "pos", this);
    anim->setDuration(800);
    anim->setStartValue(pos);
    anim->setEndValue(QPoint(pos.x(), pos.y() - 100));

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(lbl);
    lbl->setGraphicsEffect(effect);
    QPropertyAnimation *fade = new QPropertyAnimation(effect, "opacity", this);
    fade->setDuration(800);
    fade->setStartValue(1.0);
    fade->setEndValue(0.0);

    connect(anim, &QPropertyAnimation::finished, lbl, &QLabel::deleteLater);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    fade->start(QAbstractAnimation::DeleteWhenStopped);
}

// ==================== ENEMY ATTACK ====================
void MainWindow::onEnemyAttack()
{
    if (!currentEnemy || !currentEnemy->isAlive() || !player->isAlive()) return;
    int dmg = qMax(1, currentEnemy->getAttack() - player->getDefense());
    player->takeDamage(dmg);

    if (!player->isAlive()) {
        // Auto-revive
        player->heal();
    }
}

// ==================== UPDATE UI ====================
void MainWindow::updateBottomBar()
{
    lblGold->setText(QString("💰  %1 Złota").arg(player->getGold()));
    lblLevel->setText(QString("Lvl %1").arg(player->getLevel()));

    barExp->setMaximum(player->getExpToNextLevel());
    barExp->setValue(player->getExp());

    lblHpRight->setText(QString("%1 / %2 HP  ❤").arg(player->getHp()).arg(player->getMaxHp()));
    barHp->setMaximum(player->getMaxHp());
    barHp->setValue(player->getHp());

    // Sidebar
    lblSidebarLevel->setText(QString("Poziom %1").arg(player->getLevel()));
    lblSidebarAttack->setText(QString("⚔  %1 Atak").arg(player->getAttack()));
    lblSidebarDefense->setText(QString("🛡  %1 Obrona").arg(player->getDefense()));
}

void MainWindow::updateStatsPage()
{
    lblAtkValue->setText(QString::number(player->getAttack()));
    lblDefValue->setText(QString::number(player->getDefense()));
    lblHpValue->setText(QString::number(player->getMaxHp()));

    int cost = 50 + (player->getLevel() * 25);
    btnUpgAtk->setText(QString("Ulepsz (+5)  💰 %1").arg(cost));
    btnUpgDef->setText(QString("Ulepsz (+3)  💰 %1").arg(cost));
    btnUpgHp->setText(QString("Ulepsz (+20)  💰 %1").arg(cost));

    bool canAfford = player->getGold() >= cost;
    btnUpgAtk->setEnabled(canAfford);
    btnUpgDef->setEnabled(canAfford);
    btnUpgHp->setEnabled(canAfford);
}

void MainWindow::updateMapPage()
{
    bool map2Open = mapKills[0] >= 50;
    bool map3Open = mapKills[1] >= 50;

    // Map 1
    lblMap1Kills->setText(QString("Pokonani przeciwnicy: %1").arg(mapKills[0]));
    if (currentMapLevel == 0) {
        btnMap1->setText("Aktywna  ✓");
        btnMap1->setStyleSheet(
            "QPushButton { background-color: #22c55e; color: white; border-radius: 8px; border: none; "
            "padding: 12px; font-size: 13px; font-weight: bold; }");
    } else {
        btnMap1->setText("Wybierz  ▶");
        btnMap1->setStyleSheet(
            "QPushButton { background-color: #4b5563; color: white; border-radius: 8px; border: none; "
            "padding: 12px; font-size: 13px; font-weight: bold; } QPushButton:hover { background-color: #6b7280; }");
    }
    btnMap1->setEnabled(true);

    // Map 2
    if (map2Open) {
        lblMap2Kills->setText(QString("Pokonani przeciwnicy: %1").arg(mapKills[1]));
        btnMap2->setEnabled(true);
        if (currentMapLevel == 1) {
            btnMap2->setText("Aktywna  ✓");
            btnMap2->setStyleSheet(
                "QPushButton { background-color: #22c55e; color: white; border-radius: 8px; border: none; "
                "padding: 12px; font-size: 13px; font-weight: bold; }");
        } else {
            btnMap2->setText("Wybierz  ▶");
            btnMap2->setStyleSheet(
                "QPushButton { background-color: #4b5563; color: white; border-radius: 8px; border: none; "
                "padding: 12px; font-size: 13px; font-weight: bold; } QPushButton:hover { background-color: #6b7280; }");
        }
    } else {
        lblMap2Kills->setText(QString("Wymaga 50 pokonanych na Mapie 1 (Masz: %1)").arg(mapKills[0]));
        btnMap2->setText("Zablokowana");
        btnMap2->setEnabled(false);
        btnMap2->setStyleSheet(
            "QPushButton { background-color: #1f2937; color: #6b7280; border-radius: 8px; "
            "border: 1px solid #374151; padding: 12px; font-size: 13px; font-weight: bold; }");
    }

    // Map 3
    if (map3Open) {
        lblMap3Kills->setText(QString("Pokonani przeciwnicy: %1").arg(mapKills[2]));
        btnMap3->setEnabled(true);
        if (currentMapLevel == 2) {
            btnMap3->setText("Aktywna  ✓");
            btnMap3->setStyleSheet(
                "QPushButton { background-color: #22c55e; color: white; border-radius: 8px; border: none; "
                "padding: 12px; font-size: 13px; font-weight: bold; }");
        } else {
            btnMap3->setText("Wybierz  ▶");
            btnMap3->setStyleSheet(
                "QPushButton { background-color: #4b5563; color: white; border-radius: 8px; border: none; "
                "padding: 12px; font-size: 13px; font-weight: bold; } QPushButton:hover { background-color: #6b7280; }");
        }
    } else {
        lblMap3Kills->setText(QString("Wymaga 50 pokonanych na Mapie 2 (Masz: %1)").arg(mapKills[1]));
        btnMap3->setText("Zablokowana");
        btnMap3->setEnabled(false);
        btnMap3->setStyleSheet(
            "QPushButton { background-color: #1f2937; color: #6b7280; border-radius: 8px; "
            "border: 1px solid #374151; padding: 12px; font-size: 13px; font-weight: bold; }");
    }

    // Highlight active map row border
    QWidget *map1Row = btnMap1->parentWidget();
    QWidget *map2Row = btnMap2->parentWidget();
    QWidget *map3Row = btnMap3->parentWidget();

    QString activeRow = QString("background-color: %1; border: 2px solid %2; border-radius: 12px;").arg(COL_CARD, COL_GREEN);
    QString normalRow = QString("background-color: %1; border: 2px solid %2; border-radius: 12px;").arg(COL_SIDEBAR, COL_BORDER);
    QString lockedRow = QString("background-color: #111827; border: 2px solid #1f2937; border-radius: 12px; opacity: 0.6;");

    map1Row->setStyleSheet(currentMapLevel == 0 ? activeRow : normalRow);
    map2Row->setStyleSheet(currentMapLevel == 1 ? activeRow : (map2Open ? normalRow : lockedRow));
    map3Row->setStyleSheet(currentMapLevel == 2 ? activeRow : (map3Open ? normalRow : lockedRow));
}

// ==================== PLAYER EVENTS ====================
void MainWindow::onPlayerStatsChanged()
{
    updateBottomBar();
    updateStatsPage();
}

void MainWindow::onPlayerLeveledUp()
{
    QLabel *lbl = new QLabel(QString("⭐  LEVEL UP!  Poziom %1  ⭐").arg(player->getLevel()), this);
    lbl->setFont(QFont("Arial", 22, QFont::ExtraBold));
    lbl->setStyleSheet("color: #facc15; background-color: rgba(0,0,0,200); padding: 20px 40px; border-radius: 12px;");
    lbl->adjustSize();
    lbl->move(width() / 2 - lbl->width() / 2, height() / 2 - lbl->height() / 2);
    lbl->show();
    lbl->raise();
    QTimer::singleShot(1500, lbl, &QLabel::deleteLater);
}

// ==================== UPGRADES ====================
void MainWindow::onUpgradeAttack()  { player->upgradeAttack(); }
void MainWindow::onUpgradeDefense() { player->upgradeDefense(); }
void MainWindow::onUpgradeMaxHp()   { player->upgradeMaxHp(); }

// ==================== MAP SELECTION ====================
void MainWindow::onSelectMap1()
{
    currentMapLevel = 0;
    Enemy::setMapLevel(0);
    spawnNewEnemy();
    updateMapPage();
    setActiveView(0);
}

void MainWindow::onSelectMap2()
{
    if (mapKills[0] < 50) return;
    currentMapLevel = 1;
    Enemy::setMapLevel(1);
    spawnNewEnemy();
    updateMapPage();
    setActiveView(0);
}

void MainWindow::onSelectMap3()
{
    if (mapKills[1] < 50) return;
    currentMapLevel = 2;
    Enemy::setMapLevel(2);
    spawnNewEnemy();
    updateMapPage();
    setActiveView(0);
}
