#include <iostream>
#include "graph_tests.h"
#include "vehicle_tests.h"
#include "environment_tests.h"
#include "vehicle_graph_tests.h"
#include "integration_tests.h"

int main() {
    std::cout << "Running Graph tests\n";
    GraphTests::runAllTests();
    std::cout << "Graph tests passed\n\n";

    std::cout << "Running Vehicle tests\n";
    VehicleTests::runAllTests();
    std::cout << "Vehicle tests passed\n\n";

    std::cout << "Running Environment tests\n";
    EnvironmentTests::runAllTests();
    std::cout << "Environment tests passed\n";

   std::cout << "Running Vehicle Graph tests\n";
   VehicleGraphTests::runAllTests();
   std::cout << "Vehicle Graph tests passed\n";

    std::cout << "Running Integration tests\n";
    IntegrationTests::runAllTests();
    std::cout << "Vehicle Integration passed\n";

    return 0;
}