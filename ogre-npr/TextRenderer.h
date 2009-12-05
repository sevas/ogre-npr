// Taken from http://www.ogre3d.org/wiki/index.php/Simple_Text_Output

#ifndef TextRenderer_h__
#define TextRenderer_h__

#include <Ogre.h>
#include <OgreSingleton.h>

class TextRenderer : public Ogre::Singleton<TextRenderer>
{
private:

    Ogre::OverlayManager*    _overlayMgr;
    Ogre::Overlay*           _overlay;
    Ogre::OverlayContainer*  _panel;

public:

    TextRenderer();
    ~TextRenderer();

    void addTextBox(
        const std::string& ID,
        const std::string& text,
        Ogre::Real x, Ogre::Real y,
        Ogre::Real width, Ogre::Real height,
        const Ogre::ColourValue& color = Ogre::ColourValue(1.0, 1.0, 1.0, 1.0));

    void removeTextBox(const std::string& ID);

    void setText(const std::string& ID, const std::string& Text);

    void setPosition(Ogre::Real _left, Ogre::Real _top)
    {
        _panel->setPosition(_left, _top);
    }
};
#endif // TextRenderer_h__