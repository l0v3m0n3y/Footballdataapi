# Footballdataapi
api for football-data.org football data info
# main
```cpp
#include "Footballdataapi.h"
#include <iostream>

int main() {
   Footballdataapi api;

    auto areas = api.get_areas().then([](json::value result) {
        std::cout << "Search results: " << result.serialize() << std::endl;
    });
    areas.wait();
    
    return 0;
}
```

# Launch (your script)
```
g++ -std=c++11 -o main main.cpp -lcpprest -lssl -lcrypto -lpthread -lboost_system -lboost_chrono -lboost_thread
./main
```
