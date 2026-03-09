#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QString>
#include <QRandomGenerator>

class Enemy : public QObject
{
    Q_OBJECT

public:
    explicit Enemy(QObject *parent = nullptr);

    QString getName() const { return name; }
    int getHp() const { return hp; }
    int getMaxHp() const { return maxHp; }
    int getAttack() const { return attack; }
    int getExpReward() const { return expReward; }
    int getGoldReward() const { return goldReward; }
    QString getEmoji() const { return emoji; }

    void takeDamage(int damage);
    bool isAlive() const { return hp > 0; }

    static void setMapLevel(int mapLevel);

signals:
    void enemyDefeated();
    void hpChanged();

private:
    QString name;
    QString emoji;
    int hp;
    int maxHp;
    int attack;
    int expReward;
    int goldReward;

    static int s_mapLevel;
    void generateRandomEnemy();
};

#endif // ENEMY_H
