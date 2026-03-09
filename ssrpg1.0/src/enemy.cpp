#include "enemy.h"
#include <QVector>

int Enemy::s_mapLevel = 0;

Enemy::Enemy(QObject *parent)
    : QObject(parent)
{
    generateRandomEnemy();
}

void Enemy::setMapLevel(int mapLevel)
{
    if (mapLevel < 0) {
        s_mapLevel = 0;
    } else if (mapLevel > 2) {
        s_mapLevel = 2;
    } else {
        s_mapLevel = mapLevel;
    }
}

void Enemy::generateRandomEnemy()
{
    struct EnemyTemplate {
        QString name;
        QString emoji;
        int minHp;
        int maxHp;
        int attack;
        int expReward;
        int goldReward;
    };

    QVector<EnemyTemplate> map0 = {
        {"Slime", ":/resources/mapa1/slime.png", 30, 50, 5, 20, 15},
        {"Red Slime", ":/resources/mapa1/red_slime.png", 40, 60, 8, 25, 20},
        {"Green Slime", ":/resources/mapa1/green_slime.png", 60, 80, 12, 40, 30},
        {"Jelly", ":/resources/mapa1/jelly.png", 80, 100, 15, 50, 40},
        {"King Slime", ":/resources/mapa1/king_slime.png", 90, 110, 14, 45, 35}
    };

    QVector<EnemyTemplate> map1 = {
        {"Spider", ":/resources/mapa2/spider.png", 120, 160, 20, 70, 60},
        {"Bat", ":/resources/mapa2/bat.png", 100, 140, 22, 65, 55},
        {"Black Widow", ":/resources/mapa2/black_widow.png", 180, 240, 28, 95, 80},
        {"Wolf", ":/resources/mapa2/wolf.png", 140, 180, 24, 85, 70},
        {"Arachne", ":/resources/mapa2/arachne.png", 220, 280, 26, 110, 90}
    };

    QVector<EnemyTemplate> map2 = {
        {"Bandit", ":/resources/mapa3/bandit.png", 260, 340, 34, 150, 120},
        {"Berserk", ":/resources/mapa3/berserk.png", 300, 380, 38, 180, 145},
        {"Warrior", ":/resources/mapa3/warrior.png", 380, 500, 45, 240, 200},
        {"Mag", ":/resources/mapa3/mag.png", 320, 420, 40, 195, 160},
        {"King of Bandits", ":/resources/mapa3/king_bandit.png", 360, 460, 42, 220, 180}
    };

    QVector<EnemyTemplate> enemies;
    if (s_mapLevel == 0) {
        enemies = map0;
    } else if (s_mapLevel == 1) {
        enemies = map1;
    } else {
        enemies = map2;
    }

    int randomIndex = QRandomGenerator::global()->bounded(enemies.size());
    EnemyTemplate selected = enemies[randomIndex];

    name = selected.name;
    emoji = selected.emoji;
    maxHp = QRandomGenerator::global()->bounded(selected.minHp, selected.maxHp + 1);
    hp = maxHp;
    attack = selected.attack;
    expReward = selected.expReward;
    goldReward = selected.goldReward;
}

void Enemy::takeDamage(int damage)
{
    hp -= damage;
    if (hp <= 0) {
        hp = 0;
        emit enemyDefeated();
    }
    emit hpChanged();
}
