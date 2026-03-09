#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

class Player : public QObject
{
    Q_OBJECT

public:
    explicit Player(QObject *parent = nullptr);

    // Getters
    int getHp() const { return hp; }
    int getMaxHp() const { return maxHp; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    int getLevel() const { return level; }
    int getExp() const { return exp; }
    int getGold() const { return gold; }
    int getExpToNextLevel() const { return expToNextLevel; }

    // Upgrade methods
    void upgradeAttack();
    void upgradeDefense();
    void upgradeMaxHp();

    // Game mechanics
    void takeDamage(int damage);
    void gainExp(int amount);
    void gainGold(int amount);
    void heal();
    bool isAlive() const { return hp > 0; }
    void reset();

signals:
    void statsChanged();
    void leveledUp();
    void playerDied();

private:
    int hp;
    int maxHp;
    int attack;
    int defense;
    int level;
    int exp;
    int gold;
    int expToNextLevel;

    int getUpgradeCost() const;
    void checkLevelUp();
};

#endif // PLAYER_H
