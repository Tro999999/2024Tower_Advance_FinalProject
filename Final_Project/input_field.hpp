#ifndef TEXTINPUT_HPP
#define TEXTINPUT_HPP

#include <string>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_color.h>
#include <memory>
#include "IObject.hpp"
#include "Point.hpp"

namespace Engine {
    class TextInput : public IObject {
    private:
        std::string playerName;
        std::string Text;
        std::shared_ptr<ALLEGRO_FONT> font;
        ALLEGRO_COLOR Color;
        ALLEGRO_COLOR BackgroundColor;
        std::unique_ptr<TextInput> inputField;
        int Width;
        int Height;
    public:
        TextInput(const std::string& font, int fontSize, float x, float y, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, unsigned char br, unsigned char bg, unsigned char bb, unsigned char ba, float anchorX = 0.0f, float anchorY = 0.0f);
        void Draw() const override;
        void SetText(const std::string& text);
        std::string GetText() ;
        void OnKeyPress(int keycode);
    };
}

#endif // TEXTINPUT_HPP