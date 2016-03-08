#include <emscripten/emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;

// Callback class (instance of ths class should be called from js)
class Callback {
  public:
    ~Callback () {
      printf("Callback dies! \n");
    };
    void sayHi () {
      printf("Hello, I'm callback! \n");
    };
};
EMSCRIPTEN_BINDINGS(Callback)
{
    class_<Callback>("Callback")
      .function("sayHi", &Callback::sayHi);
}

Callback *cb = new Callback(); // global raw pointer to the callback
//Callback cb = Callback(); // global callback

// App class (entry point)
class App
{
  public:
    App(const val& requestManager) :
      m_requestManager(requestManager)
    {
    }
    ~App()
    {
      printf("App dies! \n");
    }
    void run()
    {
      m_requestManager.call<void>("sendRequest", cb);
    }

  private:
    val m_requestManager;
};
EMSCRIPTEN_BINDINGS(App)
{
    class_<App>("App")
      .constructor<val>()
      .function("run", &App::run);
}