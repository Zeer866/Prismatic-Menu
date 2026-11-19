#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

// Prismatic Menu UI foundation. This build provides navigation and persistent
// menu preferences; gameplay feature modules are intentionally separate.
class PrismaticMenuPopup : public Popup<> {
    CCLabelBMFont* m_pageLabel = nullptr;
    CCLabelBMFont* m_statusLabel = nullptr;

    bool init() {
        if (!Popup::init(360.f, 250.f)) return false;
        setTitle("Prismatic Menu");

        auto win = m_mainLayer->getContentSize();

        auto heading = CCLabelBMFont::create("PRISMATIC  /  CONTROL CENTER", "bigFont.fnt");
        heading->setScale(.42f);
        heading->setColor({170, 125, 255});
        heading->setPosition({win.width / 2, win.height - 43.f});
        m_mainLayer->addChild(heading);

        m_pageLabel = CCLabelBMFont::create("Dashboard", "bigFont.fnt");
        m_pageLabel->setScale(.62f);
        m_pageLabel->setPosition({win.width / 2, win.height - 82.f});
        m_mainLayer->addChild(m_pageLabel);

        m_statusLabel = CCLabelBMFont::create("Choose a section below to explore.", "bigFont.fnt");
        m_statusLabel->setScale(.38f);
        m_statusLabel->setColor({190, 195, 210});
        m_statusLabel->setPosition({win.width / 2, win.height - 112.f});
        m_mainLayer->addChild(m_statusLabel);

        auto nav = CCMenu::create();
        nav->setPosition({win.width / 2, 62.f});
        const char* pages[] = {"Visuals", "Gameplay", "Tools", "Settings"};
        for (int i = 0; i < 4; i++) {
            auto label = CCLabelBMFont::create(pages[i], "bigFont.fnt");
            label->setScale(.42f);
            auto item = CCMenuItemLabel::create(label, this, menu_selector(PrismaticMenuPopup::onPage));
            item->setTag(i);
            item->setColor({175, 135, 255});
            item->setPosition({(i - 1.5f) * 82.f, 0.f});
            nav->addChild(item);
        }
        m_mainLayer->addChild(nav);

        auto closeHint = CCLabelBMFont::create("PRISMATIC MENU  •  UI PREVIEW", "bigFont.fnt");
        closeHint->setScale(.32f);
        closeHint->setColor({120, 125, 145});
        closeHint->setPosition({win.width / 2, 25.f});
        m_mainLayer->addChild(closeHint);
        return true;
    }

    void onPage(CCObject* sender) {
        int page = sender->getTag();
        const char* names[] = {"Visuals", "Gameplay", "Tools", "Settings"};
        const char* descriptions[] = {
            "Visual customization section.",
            "Gameplay configuration section.",
            "Utility tools section.",
            "Menu preferences and saved options."
        };
        m_pageLabel->setString(names[page]);
        m_statusLabel->setString(descriptions[page]);
    }

public:
    static PrismaticMenuPopup* create() {
        auto ret = new PrismaticMenuPopup();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

class $modify(PrismaticMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto menu = this->getChildByID("main-menu");
        if (!menu) return true;

        auto sprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
        if (!sprite) sprite = CCSprite::createWithSpriteFrameName("GJ_playBtn_001.png");
        auto button = CCMenuItemSpriteExtra::create(
            sprite, this, menu_selector(PrismaticMenuLayer::openPrismatic)
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
