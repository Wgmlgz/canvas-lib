#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/html5.h>

#include <memory>
#include <string>

#include "NativeAppBase.hpp"
#include "Timer.hpp"

class CanvasApp {
 public:
  std::unique_ptr<Diligent::NativeAppBase> g_pTheApp = nullptr;
  Diligent::Timer g_Timer = {};
  double g_PrevTime = 0.0;

  void EventLoopCallback();
  CanvasApp(std::string StrCanvasId = "#canvas");
};
