#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Cloud {
    public :
        optional<Sprite> Sprite;
        Texture Texture;
        Cloud(string file, Vector2f coord, float speed);

        void display(RenderWindow &window);
        void move();

    private :
        string file;
        Vector2f coord;
        float speed;
    

};