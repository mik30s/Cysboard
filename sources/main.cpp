#include <spdlog/spdlog.h>
#include <thread>
#include "cysboard.h"
#include "default_theme.cpp"
#ifdef __linux
    #include <pwd.h>
    #include <sys/types.h>
    #include <poll.h>
    #include <sys/inotify.h>
#endif

#define MISC_BUF_LEN 512

int uimain(std::function<int()> run)
{
    char *miscBuf = new char[MISC_BUF_LEN];

    // create logger for debugging stuff
    auto logger = spdlog::stdout_logger_mt("logger");
    logger->set_level(spdlog::level::debug);


    // loaded default resources
    sciter::archive::instance().open(aux::elements_of(resources));

    // initialize window
    CysBoard* cysboard = new CysBoard();

    // load default as theme
    const char* homeDirectory;
    if((homeDirectory = getenv("HOME")) == nullptr){
        homeDirectory = getpwuid(getuid())->pw_dir;
    }
    std::string path(homeDirectory);
    path += "/.config/cysboard/";

    // monitor directory for changes
    int inotifyfd = inotify_init();
    int watchd = inotify_add_watch(inotifyfd, path.c_str(), IN_MODIFY);

    // if cant load theme then load default theme instead
    bool isOpen = cysboard->load(aux::utf2w((path + "main.html").c_str()));
    if(!isOpen) {
        cysboard->load(WSTR("this://app/default.htm"));
    }

    if(!cysboard->configure()){
        return 0;
    }
    cysboard->expand();

    std::thread updateThread([&]() {
        // check for theme directory changes and reload file
        while(true) {
            struct pollfd pfd = {inotifyfd, POLLIN, 0 };
            if(poll(&pfd, 1, 50) > 0) {
                bzero(miscBuf, MISC_BUF_LEN);
                int len = read(inotifyfd, miscBuf, 50);
                if(len > 0) {
                    cysboard->load(aux::utf2w((path + "main.html").c_str()));
                    cysboard->configure();
                };
            }
            cysboard->update();
        }
    });

    return run();
}
