export interface Player {
  level: number;
  exp: number;
  maxExp: number;
  gold: number;
  hp: number;
  maxHp: number;
  attack: number;
  defense: number;
}

export interface EnemyTemplate {
  name: string;
  minHp: number;
  maxHp: number;
  expReward: number;
  goldReward: number;
  attack: number;
}

export interface Enemy {
  name: string;
  hp: number;
  maxHp: number;
  expReward: number;
  goldReward: number;
  attack: number;
}

export type Tab = 'combat' | 'stats';
