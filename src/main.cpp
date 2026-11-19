#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

// First UI scaffold: a clean entry point for the Prismatic Menu.
// Feature controls will be implemented incrementally and wired to real settings.
class PrismaticMenuPopup : public FLAlertLayer {
public:
    static PrismaticMenuPopup* create() {
        auto ret = new PrismaticMenuPopup();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init() {
        if (!FLAlertLayer::init(90)) return false;
        this->setTitle("Prismatic Menu");

        auto label = CCLabelBMFont::create(
            "Welcome to Prismatic Menu\\nUI framework is starting up.",
            "bigFont.fnt"
        );
        label->setScale(0.45f);
        label->setPosition(m_mainLayer->getContentSize() / 2);
        m_mainLayer->addChild(label);

        this->setTouchEnabled(true);
        return true;
    }
};

class $modify(PrismaticMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto menu = this->getChildByID("main-menu");
        if (!menu) return true;

        auto button = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png"),
            this,
            menu_selector(PrismaticMenuLayer::openPrismatic)
        );
        button->setID("prismatic-menu-button");
        menu->addChild(button);
        menu->updateLayout();
        return true;
    }

    void openPrismatic(CCObject*) {
        PrismaticMenuPopup::create()->show();
    }
};
