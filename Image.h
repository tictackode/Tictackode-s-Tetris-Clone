
/*   not working class, but it is a good idea
#include <iostream>
#include <string>
#include <allegro.h>
using namespace std;

class Image
{

public:
    BITMAP* image;
    int img_width;
    int img_height;
    std::string image_path;

public:
    Image(int img_height, int img_width,string image_path)
    {
        this->img_width=img_width;
        this->img_height=img_height;
        this->image_path=image_path;
        this->loadImage();
    }
    ~Image()
    {
        destroy_bitmap(this->image);
    }

    void Image::loadImage()
    {

        this->image=create_bitmap(this>img_width,this->img_height);
        this->image=load_bitmap(this->image_path,0);

        if(!image)
        {
            key[KEY_ESC]=false;
            while(!key[KEY_ESC])
            {
                textprintf_ex(screen, letra, 20, 20, makecol(255, 0, 0),-1, "error: couldn't find "+image_path+" ESC to exit");
            }

            readkey(); liberaMemoria(); deinit(); exit(1);
        }


    }


};
*/
