#include <emscripten/bind.h>


#include <emscripten.h>
#include <emscripten/html5.h>

#include <memory>
#include <string>

#include "NativeAppBase.hpp"
#include "Timer.hpp"

std::unique_ptr<Diligent::NativeAppBase> g_pTheApp  = nullptr;
Diligent::Timer                          g_Timer    = {};
double                                   g_PrevTime = 0.0;

void EventLoopCallback()
{
    auto CurrTime    = g_Timer.GetElapsedTime();
    auto ElapsedTime = CurrTime - g_PrevTime;
    g_PrevTime       = CurrTime;

    if (g_pTheApp->IsReady())
    {
        g_pTheApp->Update(CurrTime, ElapsedTime);
        g_pTheApp->Render();
    }
}

EM_BOOL EventResizeCallback(int32_t EventType, const EmscriptenUiEvent* Event, void* pUserData)
{
    if (g_pTheApp->IsReady())
        g_pTheApp->WindowResize(Event->documentBodyClientWidth, Event->documentBodyClientHeight);
    return true;
}

EM_BOOL EventMouseCallback(int32_t EventType, const EmscriptenMouseEvent* Event, void* pUserData)
{
    g_pTheApp->OnMouseEvent(EventType, Event);
    return true;
}

EM_BOOL EventWheelCallback(int32_t EventType, const EmscriptenWheelEvent* Event, void* pUserData)
{
    g_pTheApp->OnWheelEvent(EventType, Event);
    return true;
}

EM_BOOL EventKeyCallback(int32_t EventType, const EmscriptenKeyboardEvent* Event, void* pUserData)
{
    g_pTheApp->OnKeyEvent(EventType, Event);
    return true;
}


int init(std::string StrCanvasId = "#canvas")
{
    printf("1\n");
    g_pTheApp.reset(Diligent::CreateApplication());

    int32_t     CanvasWidth  = 0;
    int32_t     CanvasHeight = 0;
    
    const char* CanvasID     = StrCanvasId.c_str();
    printf("2\n");

    emscripten_get_canvas_element_size(CanvasID, &CanvasWidth, &CanvasHeight);
    emscripten_set_mousedown_callback(CanvasID, nullptr, true, EventMouseCallback);
    emscripten_set_mouseup_callback(CanvasID, nullptr, true, EventMouseCallback);
    emscripten_set_mousemove_callback(CanvasID, nullptr, true, EventMouseCallback);
    emscripten_set_wheel_callback(CanvasID, nullptr, true, EventWheelCallback);
    emscripten_set_keydown_callback(CanvasID, nullptr, true, EventKeyCallback);
    emscripten_set_keyup_callback(CanvasID, nullptr, true, EventKeyCallback);
    emscripten_set_keypress_callback(CanvasID, nullptr, true, EventKeyCallback);
    emscripten_set_resize_callback(CanvasID, nullptr, true, EventResizeCallback);

    printf("3\n");

    g_pTheApp->OnWindowCreated(CanvasID, CanvasWidth, CanvasHeight);

    printf("4\n");

    // emscripten_set_main_loop(EventLoopCallback, 0, true);

    printf("5\n");

    // g_pTheApp.reset();

    return 0;
}


using namespace emscripten;
EMSCRIPTEN_BINDINGS(my_module) {
    function("init", &init);
    function("EventLoopCallback", &EventLoopCallback);
}