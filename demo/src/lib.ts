import wasm from 'pkg'

export class CanvasLib {
  constructor(public instance: any)  {
  }
  init(selector: string) {
    this.instance.init(selector)
  }
  render() {
    this.instance.EventLoopCallback()
  }
}


export const canvasInit = async (selector: string) => {
  const instance = await wasm()
  const lib = new CanvasLib(instance);
  lib.init(selector)
  return lib
}