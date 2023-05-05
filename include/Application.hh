#ifndef APPLICATION_H
#define APPLICATION_H

namespace Kate {
    class Application {
    public:
        /**
         * Initialize and setup the context for this application
         * */
        virtual auto startUp()  -> void = 0;
        /**
         * Run this application
         * */
        virtual auto run()      -> void = 0;
        /**
         * Shutdown this application freeing any resources it may own
         * */
        virtual auto shutDown() -> void = 0;
    private:

    };
}


#endif // APPLICATION_H
