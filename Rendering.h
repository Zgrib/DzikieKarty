#ifndef RENDERING_H
#define RENDERING_H

#endif // RENDERING_H
///argument konstruktora stanowi kolejnosc renderowania, im mniejsze z, tym "niżej" jest sprite
class CustomDrawable:public sf::Sprite{
public:
    ///wartosc z to kolejnosc rysowania obiektow


    CustomDrawable(int _z = 0){
        z=_z;
    }
    const int &getZ(){
        return z;
    }

private:
        int z;

};
