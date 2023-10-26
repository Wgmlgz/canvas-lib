#include "main.hpp"

#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/html5.h>

#include <memory>
#include <string>

#include "NativeAppBase.hpp"
#include "Timer.hpp"

EM_BOOL EventResizeCallback(int32_t EventType, const EmscriptenUiEvent* Event,
                            void* pUserData) {
  CanvasApp* canvasApp = static_cast<CanvasApp*>(pUserData);
  if (canvasApp->g_pTheApp->IsReady())
    canvasApp->g_pTheApp->WindowResize(Event->documentBodyClientWidth,
                                       Event->documentBodyClientHeight);
  return true;
}

EM_BOOL EventMouseCallback(int32_t EventType, const EmscriptenMouseEvent* Event,
                           void* pUserData) {
  CanvasApp* canvasApp = static_cast<CanvasApp*>(pUserData);
  printf("a\n");

  canvasApp->g_pTheApp->OnMouseEvent(EventType, Event);
  return true;
}

EM_BOOL EventWheelCallback(int32_t EventType, const EmscriptenWheelEvent* Event,
                           void* pUserData) {
  CanvasApp* canvasApp = static_cast<CanvasApp*>(pUserData);

  canvasApp->g_pTheApp->OnWheelEvent(EventType, Event);
  return true;
}

EM_BOOL EventKeyCallback(int32_t EventType,
                         const EmscriptenKeyboardEvent* Event,
                         void* pUserData) {
  CanvasApp* canvasApp = static_cast<CanvasApp*>(pUserData);

  printf("%s\n", Event->code);

  canvasApp->g_pTheApp->OnKeyEvent(EventType, Event);
  return true;
}
void CanvasApp::EventLoopCallback() {
  auto CurrTime = g_Timer.GetElapsedTime();
  auto ElapsedTime = CurrTime - g_PrevTime;
  g_PrevTime = CurrTime;

  if (g_pTheApp->IsReady()) {
    g_pTheApp->Update(CurrTime, ElapsedTime);
    g_pTheApp->Render();
  }
}

CanvasApp::CanvasApp(std::string StrCanvasId) {
  printf("1\n");
  g_pTheApp.reset(Diligent::CreateApplication());

  int32_t CanvasWidth = 0;
  int32_t CanvasHeight = 0;

  const char* CanvasID = StrCanvasId.c_str();
  printf("2\n");

  emscripten_get_canvas_element_size(CanvasID, &CanvasWidth, &CanvasHeight);
  emscripten_set_mousedown_callback(CanvasID, this, true, EventMouseCallback);
  emscripten_set_mouseup_callback(CanvasID, reinterpret_cast<void*>(this), true,
                                  EventMouseCallback);
  emscripten_set_mousemove_callback(CanvasID, this, true, EventMouseCallback);
  emscripten_set_wheel_callback(CanvasID, this, true, EventWheelCallback);
  emscripten_set_keydown_callback(CanvasID, this, true, EventKeyCallback);
  emscripten_set_keyup_callback(CanvasID, this, true, EventKeyCallback);
  emscripten_set_keypress_callback(CanvasID, this, true, EventKeyCallback);
  emscripten_set_resize_callback(CanvasID, this, true, EventResizeCallback);

  printf("3\n");

  g_pTheApp->OnWindowCreated(CanvasID, CanvasWidth, CanvasHeight);

  printf("4\n");
  // emscripten_set_main_loop(EventLoopCallback, 0, true);

  printf("5\n");

  // g_pTheApp.reset();
}

using namespace emscripten;
EMSCRIPTEN_BINDINGS(my_module) {
  class_<CanvasApp>("CanvasApp")
      .constructor<std::string>()
      .function("EventLoopCallback", &CanvasApp::EventLoopCallback);
  // function("init", &init);
  // function("EventLoopCallback", &EventLoopCallback);
}