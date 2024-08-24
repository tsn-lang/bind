#include <bind/bind.h>
using namespace bind;

int main(int argc, const char** argv) {
    Registry::Create();

    // test

    Registry::Destroy();
    return 0;
}