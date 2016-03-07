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

//Callback *cb = new Callback(); // global raw pointer to the callback
Callback cb = Callback(); // global callback

// App class (entry point)
class App {
  public:
    void run() {
      val::global("window").call<void>("sendRequest", cb);      
    }
};
EMSCRIPTEN_BINDINGS(App)
{
    class_<App>("App")
      .constructor<>()
      .function("run", &App::run);
}