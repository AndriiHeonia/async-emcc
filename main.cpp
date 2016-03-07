#include <emscripten/emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;

// Callback class (instance of ths class should be called from js)
class Callback
{
  public:
    ~Callback()
    {
      printf("Callback dies! \n");
    };
    void sayHi()
    {
      printf("Hello, I'm callback! \n");
    };
};
EMSCRIPTEN_BINDINGS(Callback)
{
    class_<Callback>("Callback")
      .function("sayHi", &Callback::sayHi);
}

// App class (entry point)
class App
{
  public:
    App()
    {
      m_cb = new Callback();
    }
    ~App()
    {
      printf("App dies! \n");
      delete m_cb;
    }
    void run(val requestManager)
    {
      EM_ASM_ARGS({
        // TODO: how to not use global sendRequest? I'd like to do something like this:
        // requestManager.sendRequest($0, $1);
        sendRequest($0, $1);
      }, &App::callbackWrapFunc, m_cb);
    }
  private:
    Callback *m_cb;
    static void callbackWrapFunc(Callback *cb)
    {
      cb->sayHi();
    }
};
EMSCRIPTEN_BINDINGS(App)
{
    class_<App>("App")
      .constructor<>()
      .function("run", &App::run);
}