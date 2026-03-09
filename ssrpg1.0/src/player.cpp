#include "player.h"
#include <QtGlobal>

Player::Player(QObject *parent)
    : QObject(parent), hp(100), maxHp(100), attack(10), defense(5),
      level(1), exp(0), gold(0), expToNextLevel(100) {}

void Player::upgradeAttack()
{
    int cost = getUpgradeCost();
    if (gold >= cost) {
        gold -= cost;
        attack += 5;
        emit statsChanged();
    }
}

void Player::upgradeDefense()
{
    int cost = getUpgradeCost();
    if (gold >= cost) {
        gold -= cost;
        defense += 3;
        emit statsChanged();
    }
}

void Player::upgradeMaxHp()
{
    int cost = getUpgradeCost();
    if (gold >= cost) {
        gold -= cost;
        maxHp += 20;
        hp = maxHp;
        emit statsChanged();
    }
}

void Player::heal()
{
    hp = maxHp;
    emit statsChanged();
}

void Player::takeDamage(int damage)
{
    hp -= qMax(1, damage);
    if (hp <= 0) {
        hp = 0;
        emit playerDied();
    }
    emit statsChanged();
}

void Player::gainExp(int amount)
{
    exp += amount;
    checkLevelUp();
    emit statsChanged();
}

void Player::gainGold(int amount)
{
    gold += amount;
    emit statsChanged();
}

void Player::reset()
{
    hp = maxHp;
    emit statsChanged();
}

int Player::getUpgradeCost() const
{
    return 50 + (level * 25);
}

void Player::checkLevelUp()
{
    while (exp >= expToNextLevel) {
        exp -= expToNextLevel;
        level++;
        attack += 2;
        defense += 1;
        maxHp += 10;
        hp = maxHp;
        expToNextLevel = static_cast<int>(expToNextLevel * 1.5);
        emit leveledUp();
    }
}
