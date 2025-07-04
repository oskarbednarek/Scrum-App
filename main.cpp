#include "ScrumApp.h"

int main(int, char**) {
    ScrumApp app;

    if (!app.Initialize()) {
        return -1;
    }

    app.Run();

    app.Shutdown();

    return 0;
}
