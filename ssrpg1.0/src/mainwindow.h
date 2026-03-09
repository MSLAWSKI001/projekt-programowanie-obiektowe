#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QTimer>
#include <QVector>
#include "player.h"
#include "enemy.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onFightClicked();
    void onStatsClicked();
    void onMapClicked();
    void onEnemyClicked();
    void onEnemyDefeated();
    void onPlayerStatsChanged();
    void onPlayerLeveledUp();
    void onEnemyAttack();
    void onUpgradeAttack();
    void onUpgradeDefense();
    void onUpgradeMaxHp();
    void onSelectMap1();
    void onSelectMap2();
    void onSelectMap3();

private:
    void setupUI();
    void setupSidebar(QVBoxLayout *sidebarLayout);
    void setupFightPage();
    void setupStatsPage();
    void setupMapPage();
    void setupBottomBar(QVBoxLayout *outerLayout);
    void setupConnections();
    void spawnNewEnemy();
    void updateEnemyDisplay();
    void updateBottomBar();
    void updateStatsPage();
    void updateMapPage();
    void setActiveView(int index);
    void showFloatingDamage(int damage, const QPoint &pos);

    // Sidebar
    QPushButton *btnFight;
    QPushButton *btnStats;
    QPushButton *btnMap;
    QLabel *lblSidebarLevel;
    QLabel *lblSidebarAttack;
    QLabel *lblSidebarDefense;

    // Stack
    QStackedWidget *contentStack;

    // Fight page
    QWidget *fightPage;
    QLabel *lblEnemyName;
    QProgressBar *barEnemyHp;
    QLabel *lblEnemyHpText;
    QPushButton *btnAttackArea;
    QLabel *lblEnemyEmoji;

    // Stats page
    QWidget *statsPage;
    QLabel *lblAtkValue;
    QLabel *lblDefValue;
    QLabel *lblHpValue;
    QPushButton *btnUpgAtk;
    QPushButton *btnUpgDef;
    QPushButton *btnUpgHp;

    // Map page
    QWidget *mapPage;
    QLabel *lblMap1Kills;
    QLabel *lblMap2Kills;
    QLabel *lblMap3Kills;
    QPushButton *btnMap1;
    QPushButton *btnMap2;
    QPushButton *btnMap3;

    // Bottom bar
    QLabel *lblGold;
    QLabel *lblLevel;
    QProgressBar *barExp;
    QLabel *lblHpRight;
    QProgressBar *barHp;

    // Game objects
    Player *player;
    Enemy *currentEnemy;
    QTimer *enemyAttackTimer;
    int currentMapLevel;
    QVector<int> mapKills;
};

#endif // MAINWINDOW_H
