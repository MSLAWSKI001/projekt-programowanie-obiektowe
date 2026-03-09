import { EnemyTemplate, Enemy, Player } from './types';

export const ENEMY_TEMPLATES: EnemyTemplate[] = [
  { name: 'Szczur', minHp: 20, maxHp: 30, expReward: 10, goldReward: 5, attack: 2 },
  { name: 'Szlam', minHp: 30, maxHp: 50, expReward: 15, goldReward: 8, attack: 4 },
  { name: 'Goblin', minHp: 50, maxHp: 80, expReward: 25, goldReward: 15, attack: 8 },
  { name: 'Wilk', minHp: 80, maxHp: 120, expReward: 40, goldReward: 25, attack: 15 },
  { name: 'Ork', minHp: 150, maxHp: 250, expReward: 80, goldReward: 50, attack: 25 },
  { name: 'Smok', minHp: 500, maxHp: 1000, expReward: 300, goldReward: 200, attack: 60 },
];

export const generateEnemy = (playerLevel: number): Enemy => {
  // Simple logic to pick enemy based on player level
  // Allow higher level enemies as player levels up
  const maxIndex = Math.min(ENEMY_TEMPLATES.length - 1, Math.floor(playerLevel / 2) + 1);
  const template = ENEMY_TEMPLATES[Math.floor(Math.random() * (maxIndex + 1))];
  
  // Scale stats a bit by player level for replayability (optional, keeping it simple for now)
  const hp = Math.floor(Math.random() * (template.maxHp - template.minHp + 1)) + template.minHp;
  
  return {
    name: template.name,
    hp: hp,
    maxHp: hp,
    expReward: template.expReward,
    goldReward: template.goldReward,
    attack: template.attack,
  };
};

export const INITIAL_PLAYER: Player = {
  level: 1,
  exp: 0,
  maxExp: 50,
  gold: 0,
  hp: 100,
  maxHp: 100,
  attack: 10,
  defense: 2,
};

export const calculateUpgradeCost = (currentValue: number, baseCost: number, scaling: number) => {
  return Math.floor(baseCost * Math.pow(scaling, currentValue));
};
