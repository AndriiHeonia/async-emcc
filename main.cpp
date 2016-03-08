#include <emscripten/emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;

// Callback class (instance of ths class should be called from js asynchronously)
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
    App(val ptrToSendRequest)
    {
      m_cb = new Callback();
      m_ptrToSendRequest = ptrToSendRequest.as<int>();
    }
    ~App()
    {
      printf("App dies! \n");
      delete m_cb;
    }
    void run()
    {
      EM_ASM_ARGS({
        Module.dynCall_vii($0, $1, $2); // sendRequest($1, $2);
        Runtime.removeFunction($0);
      }, m_ptrToSendRequest, &App::callbackWrapFunc, m_cb);
    }
  private:
    Callback *m_cb;
    static void callbackWrapFunc(Callback *cb)
    {
      cb->sayHi();
    }

    int m_ptrToSendRequest;
};
EMSCRIPTEN_BINDINGS(App)
{
    class_<App>("App")
      .constructor<val>()
      .function("run", &App::run);
}