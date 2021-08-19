#include <iostream>
//#include <tree.h>
#include <node.h>

using namespace fefu;

int main() {
    auto *node = new Node<int, int>();
    std::cout << node->get_ref_count() << std::endl;
    node++;

    std::cout << node->get_ref_count();

    return 0;
}
