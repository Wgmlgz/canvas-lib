import wasm from 'pkg';
const module = await wasm();
const { CanvasApp } = module;
// export class CanvasLib {
//   constructor(public instance: any)  {
//   }
//   init(selector: string) {
//     this.instance.init(selector)
//   }
//   render() {
//     this.instance.EventLoopCallback()
//   }
// }

export const canvasInit = async (selector: string) => {
  const app = new CanvasApp(selector);
  // console.log(CanvasApp)
  // console.log(instance)
  // const lib = new CanvasLib(instance);
  // lib.init(selector)
  return app;
};
