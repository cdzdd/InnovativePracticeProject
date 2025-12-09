import { Game } from './game/Game.js';

window.onload = () => {
  const canvas = document.getElementById('gameCanvas');
  const ctx = canvas.getContext('2d');
  const game = new Game(canvas, ctx);
  game.start();
};
