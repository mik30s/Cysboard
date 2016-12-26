#include <spdlog/spdlog.h>
#include <thread>
#include "cysboard.h"
#include "default_theme.cpp"

int uimain(std::function<int()> run)
{
    auto logger = spdlog::stdout_logger_mt("logger");

    // loaded default resources
    sciter::archive::instance().open(aux::elements_of(resources)); // bind resources[] (defined in
                    // "resources.cpp") with the archive

    CysBoard* cysboard = new CysBoard();

    // load deafulat as theme
    cysboard->load(WSTR("this://app/default.htm"));
    cysboard->expand();

    // start update thread
    std::thread updateThread([cysboard](){std::cout << "starting update thread"; cysboard->update();});

    return run();
}
