#include <string>

class Image
{
public:
    BITMAP* image;
    int img_width;
    int img_height;
    string image_path;

public:
    Image(int img_height, int img_width,string image_path)
    {
        this->img_width=img_width;
        this->img_height=img_height;
        this->image_path=img_path;
        this->loadImage();
    }
    ~Image()
    {
        destroy_bitmap(this->image);
    }

    void loadImage();
    {

        this->image=create_bitmap(this>img_width,this->img_height);
        this->image=load_bitmap(this->image_path,0);

        if(!image)
        {
            Key[KEY_ESC]=false;
            while(!Key[KEY_ESC])
            {
                textprintf_ex(screen, letra, 20, 20, makecol(glow.fontaux, 0, 0),-1, "error: couldn't find "+image_path+" ESC to exit");
            }

            readkey(); liberaMemoria(); deinit(); exit(1);
        }


    }


};
