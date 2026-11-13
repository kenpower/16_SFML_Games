#include <SFML/Graphics.hpp>

int main()
{
    sf::RectangleShape shape({1.f, 1.f});
    return shape.getSize().x > 0.f ? 0 : 1;
}
