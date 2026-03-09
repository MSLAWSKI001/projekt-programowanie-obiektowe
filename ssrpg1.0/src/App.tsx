import React, { useState, useEffect, useCallback } from 'react';
import { Sword, BarChart2, Map as MapIcon, Heart, Coins, Shield, ChevronRight } from 'lucide-react';

const ENEMIES = {
  1: [
    { name: 'Szczur', maxHp: 20, damage: 2, exp: 10, gold: 5, emoji: '🐀' },
    { name: 'Szlam', maxHp: 30, damage: 3, exp: 15, gold: 8, emoji: '🦠' },
    { name: 'Goblin', maxHp: 40, damage: 5, exp: 20, gold: 12, emoji: '👺' },
  ],
  2: [
    { name: 'Szkielet', maxHp: 100, damage: 15, exp: 50, gold: 30, emoji: '💀' },
    { name: 'Nietoperz', maxHp: 80, damage: 12, exp: 40, gold: 25, emoji: '🦇' },
    { name: 'Troll', maxHp: 150, damage: 20, exp: 80, gold: 50, emoji: '🧌' },
  ],
  3: [
    { name: 'Ognisty Golem', maxHp: 300, damage: 40, exp: 150, gold: 100, emoji: '🗿' },
    { name: 'Demon', maxHp: 250, damage: 50, exp: 180, gold: 120, emoji: '👿' },
    { name: 'Smok', maxHp: 500, damage: 80, exp: 300, gold: 250, emoji: '🐉' },
  ]
};

type View = 'fight' | 'stats' | 'map';

export default function App() {
  const [view, setView] = useState<View>('fight');
  const [currentMap, setCurrentMap] = useState<1 | 2 | 3>(1);
  const [killsMap1, setKillsMap1] = useState(0);
  const [killsMap2, setKillsMap2] = useState(0);
  const [killsMap3, setKillsMap3] = useState(0);

  const [player, setPlayer] = useState({
    level: 1,
    exp: 0,
    maxExp: 100,
    gold: 0,
    hp: 100,
    maxHp: 100,
    attack: 10,
    defense: 5,
  });

  const [enemy, setEnemy] = useState(() => {
    const pool = ENEMIES[1];
    const e = pool[Math.floor(Math.random() * pool.length)];
    return { ...e, hp: e.maxHp };
  });

  const [damageNumbers, setDamageNumbers] = useState<{id: number, val: number, x: number, y: number}[]>([]);

  const upgradeCosts = {
    attack: Math.floor(10 * Math.pow(1.5, player.attack - 10)),
    defense: Math.floor(10 * Math.pow(1.5, player.defense - 5)),
    maxHp: Math.floor(15 * Math.pow(1.2, (player.maxHp - 100) / 10)),
  };

  const spawnNewEnemy = useCallback((mapIndex: 1 | 2 | 3) => {
    const pool = ENEMIES[mapIndex];
    const e = pool[Math.floor(Math.random() * pool.length)];
    setEnemy({ ...e, hp: e.maxHp });
  }, []);

  const handleDefeatEnemy = useCallback(() => {
    setPlayer(p => {
      let newExp = p.exp + enemy.exp;
      let newLevel = p.level;
      let newMaxExp = p.maxExp;
      let newMaxHp = p.maxHp;
      let newAttack = p.attack;
      let newDefense = p.defense;

      if (newExp >= newMaxExp) {
        newExp -= newMaxExp;
        newLevel++;
        newMaxExp = Math.floor(newMaxExp * 1.5);
        newMaxHp += 20;
        newAttack += 2;
        newDefense += 1;
      }

      return {
        ...p,
        exp: newExp,
        level: newLevel,
        maxExp: newMaxExp,
        gold: p.gold + enemy.gold,
        hp: newMaxHp, // Heal to max on kill
        maxHp: newMaxHp,
        attack: newAttack,
        defense: newDefense
      };
    });

    if (currentMap === 1) setKillsMap1(k => k + 1);
    else if (currentMap === 2) setKillsMap2(k => k + 1);
    else if (currentMap === 3) setKillsMap3(k => k + 1);

    spawnNewEnemy(currentMap);
  }, [enemy.exp, enemy.gold, currentMap, spawnNewEnemy]);

  const handleEnemyAttack = useCallback(() => {
    setPlayer(p => {
      const dmg = Math.max(1, enemy.damage - p.defense);
      const newHp = Math.max(0, p.hp - dmg);
      if (newHp === 0) {
        return { ...p, hp: p.maxHp };
      }
      return { ...p, hp: newHp };
    });
  }, [enemy.damage]);

  useEffect(() => {
    if (view !== 'fight') return;
    const timer = setInterval(() => {
      handleEnemyAttack();
    }, 1500);
    return () => clearInterval(timer);
  }, [view, handleEnemyAttack]);

  const attackEnemy = (e: React.MouseEvent<HTMLDivElement>) => {
    if (view !== 'fight') return;

    const rect = e.currentTarget.getBoundingClientRect();
    const x = e.clientX - rect.left;
    const y = e.clientY - rect.top;

    setDamageNumbers(prev => [...prev, { id: Math.random(), val: player.attack, x, y }]);
    setTimeout(() => {
      setDamageNumbers(prev => prev.slice(1));
    }, 1000);

    setEnemy(prev => {
      const newHp = Math.max(0, prev.hp - player.attack);
      if (newHp === 0) {
        setTimeout(handleDefeatEnemy, 0);
        return prev;
      }
      return { ...prev, hp: newHp };
    });
  };

  const buyUpgrade = (type: 'attack' | 'defense' | 'maxHp') => {
    setPlayer(p => {
      const cost = upgradeCosts[type];
      if (p.gold >= cost) {
        const updates: any = { gold: p.gold - cost };
        if (type === 'attack') updates.attack = p.attack + 1;
        if (type === 'defense') updates.defense = p.defense + 1;
        if (type === 'maxHp') {
          updates.maxHp = p.maxHp + 10;
          updates.hp = p.hp + 10;
        }
        return { ...p, ...updates };
      }
      return p;
    });
  };

  const changeMap = (map: 1 | 2 | 3) => {
    setCurrentMap(map);
    spawnNewEnemy(map);
    setView('fight');
  };

  return (
    <div className="flex h-screen bg-gray-900 text-gray-100 font-sans overflow-hidden selection:bg-transparent">
      {/* Sidebar */}
      <div className="w-64 bg-gray-800 p-6 flex flex-col border-r border-gray-700 shadow-xl z-20">
        <h1 className="text-2xl font-bold mb-8 text-transparent bg-clip-text bg-gradient-to-r from-blue-400 to-purple-500">
          Clicker RPG
        </h1>
        
        <div className="flex flex-col gap-3 flex-1">
          <button 
            onClick={() => setView('fight')}
            className={`flex items-center gap-3 px-4 py-3 rounded-lg font-bold transition-all ${view === 'fight' ? 'bg-blue-600 text-white shadow-lg shadow-blue-500/30' : 'bg-gray-700 text-gray-300 hover:bg-gray-600'}`}
          >
            <Sword size={20} /> Walka
          </button>
          
          <button 
            onClick={() => setView('stats')}
            className={`flex items-center gap-3 px-4 py-3 rounded-lg font-bold transition-all ${view === 'stats' ? 'bg-blue-600 text-white shadow-lg shadow-blue-500/30' : 'bg-gray-700 text-gray-300 hover:bg-gray-600'}`}
          >
            <BarChart2 size={20} /> Statystyki
          </button>

          <button 
            onClick={() => setView('map')}
            className={`flex items-center gap-3 px-4 py-3 rounded-lg font-bold transition-all ${view === 'map' ? 'bg-blue-600 text-white shadow-lg shadow-blue-500/30' : 'bg-gray-700 text-gray-300 hover:bg-gray-600'}`}
          >
            <MapIcon size={20} /> Mapa
          </button>
        </div>

        <div className="mt-auto bg-gray-900/50 p-4 rounded-lg border border-gray-700">
          <div className="text-sm text-gray-400 mb-1">Poziom {player.level}</div>
          <div className="text-lg font-bold text-white flex items-center gap-2">
            <Sword size={16} className="text-red-400"/> {player.attack} Atak
          </div>
          <div className="text-lg font-bold text-white flex items-center gap-2">
            <Shield size={16} className="text-blue-400"/> {player.defense} Obrona
          </div>
        </div>
      </div>

      {/* Main Content Area */}
      <div className="flex-1 flex flex-col relative bg-gray-900">
        
        {/* Dynamic View Content */}
        <div className="flex-1 flex flex-col relative">
          
          {view === 'fight' && (
            <div 
              onClick={attackEnemy} 
              className="flex-1 w-full bg-gray-700 hover:bg-gray-600 transition-colors cursor-crosshair relative flex flex-col items-center pt-16 shadow-[inset_0_0_100px_rgba(0,0,0,0.5)]"
            >
              {/* Enemy Info at Top of Clickable Area */}
              <div className="text-4xl font-extrabold mb-4 text-white tracking-wide drop-shadow-lg">
                {enemy.name}
              </div>
              
              <div className="w-[500px] max-w-[90%] bg-gray-900 h-8 rounded-full overflow-hidden border-2 border-gray-800 relative shadow-inner">
                <div 
                  className="bg-gradient-to-r from-red-600 to-red-400 h-full transition-all duration-150" 
                  style={{ width: `${(enemy.hp / enemy.maxHp) * 100}%` }}
                ></div>
                <div className="absolute inset-0 flex items-center justify-center font-bold text-sm text-white drop-shadow-md">
                  {enemy.hp} / {enemy.maxHp} HP
                </div>
              </div>

              {/* Enemy Visual */}
              <div className="text-[180px] mt-24 select-none pointer-events-none animate-[bounce_3s_ease-in-out_infinite] drop-shadow-2xl">
                {enemy.emoji}
              </div>

              {/* Attack Instruction */}
              <div className="absolute bottom-10 text-gray-400 text-lg font-bold pointer-events-none opacity-50 tracking-widest uppercase">
                Kliknij dowolne miejsce aby zaatakować
              </div>

              {/* Damage Numbers */}
              {damageNumbers.map(d => (
                <div 
                  key={d.id} 
                  className="absolute text-red-400 font-extrabold text-4xl pointer-events-none drop-shadow-lg animate-[ping_1s_ease-out_forwards]" 
                  style={{ left: d.x - 20, top: d.y - 40 }}
                >
                  -{d.val}
                </div>
              ))}
            </div>
          )}

          {view === 'stats' && (
            <div className="flex-1 p-12 overflow-y-auto bg-gray-800">
              <h2 className="text-3xl font-bold mb-8 border-b border-gray-700 pb-4 flex items-center gap-3">
                <BarChart2 size={32} className="text-blue-400"/> Statystyki i Ulepszenia
              </h2>
              
              <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
                {/* Attack Upgrade */}
                <div className="bg-gray-700 p-6 rounded-xl border border-gray-600 flex flex-col">
                  <div className="flex items-center gap-3 mb-4 text-red-400">
                    <Sword size={32} />
                    <h3 className="text-2xl font-bold">Siła Ataku</h3>
                  </div>
                  <p className="text-gray-300 mb-6 flex-1">Obecnie: <span className="text-white font-bold text-xl">{player.attack}</span></p>
                  <button 
                    onClick={() => buyUpgrade('attack')}
                    disabled={player.gold < upgradeCosts.attack}
                    className="w-full py-3 rounded-lg font-bold flex items-center justify-center gap-2 transition-all disabled:opacity-50 disabled:cursor-not-allowed bg-blue-600 hover:bg-blue-500 text-white"
                  >
                    Ulepsz (+1) <Coins size={18}/> {upgradeCosts.attack}
                  </button>
                </div>

                {/* Defense Upgrade */}
                <div className="bg-gray-700 p-6 rounded-xl border border-gray-600 flex flex-col">
                  <div className="flex items-center gap-3 mb-4 text-blue-400">
                    <Shield size={32} />
                    <h3 className="text-2xl font-bold">Obrona</h3>
                  </div>
                  <p className="text-gray-300 mb-6 flex-1">Obecnie: <span className="text-white font-bold text-xl">{player.defense}</span></p>
                  <button 
                    onClick={() => buyUpgrade('defense')}
                    disabled={player.gold < upgradeCosts.defense}
                    className="w-full py-3 rounded-lg font-bold flex items-center justify-center gap-2 transition-all disabled:opacity-50 disabled:cursor-not-allowed bg-blue-600 hover:bg-blue-500 text-white"
                  >
                    Ulepsz (+1) <Coins size={18}/> {upgradeCosts.defense}
                  </button>
                </div>

                {/* Max HP Upgrade */}
                <div className="bg-gray-700 p-6 rounded-xl border border-gray-600 flex flex-col">
                  <div className="flex items-center gap-3 mb-4 text-green-400">
                    <Heart size={32} />
                    <h3 className="text-2xl font-bold">Maksymalne HP</h3>
                  </div>
                  <p className="text-gray-300 mb-6 flex-1">Obecnie: <span className="text-white font-bold text-xl">{player.maxHp}</span></p>
                  <button 
                    onClick={() => buyUpgrade('maxHp')}
                    disabled={player.gold < upgradeCosts.maxHp}
                    className="w-full py-3 rounded-lg font-bold flex items-center justify-center gap-2 transition-all disabled:opacity-50 disabled:cursor-not-allowed bg-blue-600 hover:bg-blue-500 text-white"
                  >
                    Ulepsz (+10) <Coins size={18}/> {upgradeCosts.maxHp}
                  </button>
                </div>
              </div>
            </div>
          )}

          {view === 'map' && (
            <div className="flex-1 p-12 overflow-y-auto bg-gray-800">
              <h2 className="text-3xl font-bold mb-8 border-b border-gray-700 pb-4 flex items-center gap-3">
                <MapIcon size={32} className="text-green-400"/> Wybór Mapy
              </h2>
              
              <div className="flex flex-col gap-4 max-w-4xl mx-auto">
                {/* Map 1 */}
                <div className={`p-6 rounded-xl border-2 transition-all flex justify-between items-center ${currentMap === 1 ? 'bg-gray-700 border-green-500 shadow-[0_0_15px_rgba(34,197,94,0.3)]' : 'bg-gray-800 border-gray-700 hover:bg-gray-750'}`}>
                  <div>
                    <h3 className="text-2xl font-bold mb-1 flex items-center gap-2">🌲 Mroczny Las (Poziom 1)</h3>
                    <p className="text-gray-400">Pokonani przeciwnicy: <span className="text-white font-bold">{killsMap1}</span></p>
                  </div>
                  <button 
                    onClick={() => changeMap(1)} 
                    className={`px-8 py-3 rounded-lg font-bold transition-all flex items-center gap-2 ${currentMap === 1 ? 'bg-green-600 text-white' : 'bg-gray-600 hover:bg-gray-500 text-white'}`}
                  >
                    {currentMap === 1 ? 'Aktywna' : 'Wybierz'} <ChevronRight size={20}/>
                  </button>
                </div>

                {/* Map 2 */}
                <div className={`p-6 rounded-xl border-2 transition-all flex justify-between items-center ${currentMap === 2 ? 'bg-gray-700 border-green-500 shadow-[0_0_15px_rgba(34,197,94,0.3)]' : (killsMap1 >= 50 ? 'bg-gray-800 border-gray-700 hover:bg-gray-750' : 'bg-gray-900 border-gray-800 opacity-60')}`}>
                  <div>
                    <h3 className="text-2xl font-bold mb-1 flex items-center gap-2">⛰️ Zaginione Jaskinie (Poziom 2)</h3>
                    <p className="text-gray-400">
                      {killsMap1 >= 50 
                        ? `Pokonani przeciwnicy: ${killsMap2}` 
                        : `Wymaga 50 pokonanych na Mapie 1 (Masz: ${killsMap1})`}
                    </p>
                  </div>
                  {killsMap1 >= 50 ? (
                    <button 
                      onClick={() => changeMap(2)} 
                      className={`px-8 py-3 rounded-lg font-bold transition-all flex items-center gap-2 ${currentMap === 2 ? 'bg-green-600 text-white' : 'bg-gray-600 hover:bg-gray-500 text-white'}`}
                    >
                      {currentMap === 2 ? 'Aktywna' : 'Wybierz'} <ChevronRight size={20}/>
                    </button>
                  ) : (
                    <div className="px-8 py-3 bg-gray-800 rounded-lg font-bold text-gray-500 flex items-center gap-2 border border-gray-700">
                      Zablokowana
                    </div>
                  )}
                </div>

                {/* Map 3 */}
                <div className={`p-6 rounded-xl border-2 transition-all flex justify-between items-center ${currentMap === 3 ? 'bg-gray-700 border-green-500 shadow-[0_0_15px_rgba(34,197,94,0.3)]' : (killsMap2 >= 50 ? 'bg-gray-800 border-gray-700 hover:bg-gray-750' : 'bg-gray-900 border-gray-800 opacity-60')}`}>
                  <div>
                    <h3 className="text-2xl font-bold mb-1 flex items-center gap-2">🌋 Przeklęty Wulkan (Poziom 3)</h3>
                    <p className="text-gray-400">
                      {killsMap2 >= 50 
                        ? `Pokonani przeciwnicy: ${killsMap3}` 
                        : `Wymaga 50 pokonanych na Mapie 2 (Masz: ${killsMap2})`}
                    </p>
                  </div>
                  {killsMap2 >= 50 ? (
                    <button 
                      onClick={() => changeMap(3)} 
                      className={`px-8 py-3 rounded-lg font-bold transition-all flex items-center gap-2 ${currentMap === 3 ? 'bg-green-600 text-white' : 'bg-gray-600 hover:bg-gray-500 text-white'}`}
                    >
                      {currentMap === 3 ? 'Aktywna' : 'Wybierz'} <ChevronRight size={20}/>
                    </button>
                  ) : (
                    <div className="px-8 py-3 bg-gray-800 rounded-lg font-bold text-gray-500 flex items-center gap-2 border border-gray-700">
                      Zablokowana
                    </div>
                  )}
                </div>
              </div>
            </div>
          )}

        </div>

        {/* Bottom Status Bar */}
        <div className="h-28 bg-gray-800 border-t-2 border-gray-700 p-4 px-8 flex justify-between items-center z-20 shadow-[0_-5px_15px_rgba(0,0,0,0.3)]">
          
          {/* Left: Gold & EXP */}
          <div className="flex flex-col gap-3 w-2/5 min-w-[300px]">
            <div className="text-yellow-400 font-extrabold flex items-center gap-2 text-2xl drop-shadow-md">
              <Coins size={28} /> {player.gold} Złota
            </div>
            
            <div className="flex items-center gap-3 w-full">
              <span className="text-blue-300 font-bold text-lg whitespace-nowrap bg-blue-900/50 px-3 py-1 rounded-md">
                Lvl {player.level}
              </span>
              <div className="flex-1 bg-gray-900 h-7 rounded-full overflow-hidden relative border border-gray-600 shadow-inner">
                <div 
                  className="bg-gradient-to-r from-blue-600 to-blue-400 h-full transition-all duration-300 ease-out" 
                  style={{ width: `${(player.exp / player.maxExp) * 100}%` }}
                ></div>
                <div className="absolute inset-0 flex items-center justify-center text-xs font-bold text-white drop-shadow-md tracking-wider">
                  EXP: {player.exp} / {player.maxExp}
                </div>
              </div>
            </div>
          </div>

          {/* Right: HP */}
          <div className="flex flex-col gap-3 w-2/5 min-w-[300px] justify-end">
            <div className="text-red-400 font-extrabold flex items-center justify-end gap-2 text-2xl drop-shadow-md">
              {player.hp} / {player.maxHp} HP <Heart size={28} className="fill-current" />
            </div>
            
            <div className="w-full bg-gray-900 h-7 rounded-full overflow-hidden relative border border-gray-600 shadow-inner ml-auto">
              <div 
                className="bg-gradient-to-r from-red-600 to-red-500 h-full transition-all duration-300 ease-out" 
                style={{ width: `${(player.hp / player.maxHp) * 100}%` }}
              ></div>
            </div>
          </div>

        </div>

      </div>
    </div>
  );
}