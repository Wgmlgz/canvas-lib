import './style.css';
import typescriptLogo from './typescript.svg';
import viteLogo from '/vite.svg';
import { setupCounter } from './counter.ts';
import { canvasInit } from './lib.ts';

// setTimeout(async () => {
//   // console.log(module)
//   module.init('#canvas2')
//   module.init('#canvas1')

//   requestAnimationFrame(() => {
//     module.EventLoopCallback()
//   })
// })
document.querySelector<HTMLDivElement>('#app')!.innerHTML = `
<canvas width="1000" height="1000" tabindex="1000" class="emscripten" id="canvas1" oncontextmenu="event.preventDefault()"></canvas>
  <div>
    <a href="https://vitejs.dev" target="_blank">
      <img src="${viteLogo}" class="logo" alt="Vite logo" />
    </a>
    <a href="https://www.typescriptlang.org/" target="_blank">
      <img src="${typescriptLogo}" class="logo vanilla" alt="TypeScript logo" />
    </a>
    <h1>Vite + TypeScript</h1>
    <div class="card">
      <button id="counter" type="button"></button>
    </div>
    <p class="read-the-docs">
      Click on the Vite and TypeScript logos to learn more
    </p>
  </div>
`;
const c1 = await canvasInit('#canvas1');

const loop = () =>
  requestAnimationFrame(() => {
    c1.EventLoopCallback();
    loop();
  });
loop();
setupCounter(document.querySelector<HTMLButtonElement>('#counter')!);
