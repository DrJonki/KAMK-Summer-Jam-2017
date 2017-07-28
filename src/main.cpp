#include <iostream>
#include <Jam/Instance.hpp>

int main() {
  jam::Instance instance;

  while (instance.window.isOpen()) {
    instance.draw();
    instance.handleEvents();
  }

  return 0;
}