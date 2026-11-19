#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

namespace {
    constexpr int PAGE_DASHBOARD = 0;
    constexpr int PAGE_VISUALS = 1;
    constexpr int PAGE_PERFORMANCE = 2;
    constexpr int PAGE_INTERFACE = 3;

    bool savedBool(char const* key, bool fallback) {
        return Mod::get()->getSavedValue<bool>(key, fallback);
    }
    void saveBool(char const* key, bool value) {
        Mod::get()->setSavedValue<bool>(key, value);
    }
}

class PrismaticMenuPopup : public Popup<> {
    CCLabelBMFont* m_pageTitle = nullptr;
    CCLabelBMFont* m_description = nullptr;
    CCMenu* m_options = nullptr;
    int m_page = PAGE_DASHBOARD;

    ccColor3B accent() const {
        return savedBool("warm-accent", false) ? cc3b(80, 205, 255) : cc3b(180, 125, 255);
    }

    void rebuildOptions() {
        m_options->removeAllChildrenWithCleanup(true);
        float y = 28.f;
        float gap = savedBool("compact-layout", false) ? 31.f : 42.f;

        auto add = [&](char const* title, char const* key, char const* help, bool fallback) {
            bool enabled = savedBool(key, fallback);
            auto label = CCLabelBMFont::create(
                fmt::format("{}  [{}]", title, enabled ? "ON" : "OFF").c_str(),
                "bigFont.fnt"
            );
            if (!label) return;
            label->setScale(.43f);
            label->setAnchorPoint({0.f, .5f});
            label->setColor(enabled ? accent() : cc3b(145, 150, 165));
            auto item = CCMenuItemLabel::create(label, this, menu_selector(PrismaticMenuPopup::onOption));
            static char const* keys[] = {"fps-counter", "compact-layout", "show-descriptions", "warm-accent"};
            int optionTag = 0;
            for (int i = 0; i < 4; ++i) if (std::string(keys[i]) == key) optionTag = i;
            item->setTag(optionTag);
            item->setPosition({-125.f, y});
            m_options->addChild(item);
            y -= gap;
        };

        switch (m_page) {
            case PAGE_DASHBOARD:
                add("FPS counter", "fps-counter", "Show Geometry Dash performance statistics.", false);
                add("Compact layout", "compact-layout", "Use tighter spacing between menu options.", false);
                add("Show descriptions", "show-descriptions", "Show helpful text below the page title.", true);
                break;
            case PAGE_VISUALS:
                add("Cyan prism accent", "warm-accent", "Switch the menu accent between violet and cyan.", false);
                add("Show descriptions", "show-descriptions", "Show helpful text below the page title.", true);
                break;
            case PAGE_PERFORMANCE:
                add("FPS counter", "fps-counter", "Show Geometry Dash performance statistics.", false);
                add("Compact layout", "compact-layout", "Use tighter spacing between menu options.", false);
                break;
            case PAGE_INTERFACE:
                add("Compact layout", "compact-layout", "Use tighter spacing between menu options.", false);
                add("Show descriptions", "show-descriptions", "Show helpful text below the page title.", true);
                break;
        }

        m_options->setPosition({m_mainLayer->getContentSize().width / 2, 92.f});
        m_options->setContentSize({260.f, 125.f});
    }

    void setPage(int page) {
        m_page = page;
        char const* names[] = {"Dashboard", "Visuals", "Performance", "Interface"};
        char const* descriptions[] = {
            "Your Geometry Dash control center",
            "Personalize the Prismatic appearance",
            "Small tools for your game display",
            "Adjust how this menu is presented"
        };
        m_pageTitle->setString(names[page]);
        m_pageTitle->setColor(accent());
        m_description->setString(savedBool("show-descriptions", true) ? descriptions[page] : "");
        rebuildOptions();
    }

    void onOption(CCObject* sender) {
        auto item = typeinfo_cast<CCMenuItemLabel*>(sender);
        if (!item) return;
        static char const* keys[] = {"fps-counter", "compact-layout", "show-descriptions", "warm-accent"};
        int tag = item->getTag();
        if (tag < 0 || tag > 3) return;
        auto key = keys[tag];
        bool current = savedBool(key, tag == 2);
        bool next = !current;
        saveBool(key, next);

        if (tag == 0) {
            CCDirector::sharedDirector()->setDisplayStats(next);
        }
        if (tag == 3) {
            m_pageTitle->setColor(accent());
        }
        if (tag == 2) {
            setPage(m_page);
            return;
        }
        rebuildOptions();
    }

    void onNavigate(CCObject* sender) {
        setPage(sender->getTag());
    }

    bool init() {
        if (!Popup::init(340.f, 260.f)) return false;
        setTitle("PRISMATIC MENU");

        auto size = m_mainLayer->getContentSize();

        auto brand = CCLabelBMFont::create("PRISMATIC  /  CONTROL CENTER", "bigFont.fnt");
        brand->setScale(.38f);
        brand->setColor(cc3b(165, 170, 190));
        brand->setPosition({size.width / 2, size.height - 39.f});
        m_mainLayer->addChild(brand);

        m_pageTitle = CCLabelBMFont::create("Dashboard", "bigFont.fnt");
        m_pageTitle->setScale(.62f);
        m_pageTitle->setPosition({size.width / 2, size.height - 72.f});
        m_mainLayer->addChild(m_pageTitle);

        m_description = CCLabelBMFont::create("", "bigFont.fnt");
        m_description->setScale(.34f);
        m_description->setColor(cc3b(175, 180, 195));
        m_description->setPosition({size.width / 2, size.height - 96.f});
        m_mainLayer->addChild(m_description);

        m_options = CCMenu::create();
        m_options->setPosition({size.width / 2, 92.f});
        m_mainLayer->addChild(m_options);

        auto nav = CCMenu::create();
        nav->setPosition({size.width / 2, 28.f});
        char const* pages[] = {"Home", "Visuals", "FPS", "UI"};
        for (int i = 0; i < 4; ++i) {
            auto label = CCLabelBMFont::create(pages[i], "bigFont.fnt");
            label->setScale(.38f);
            auto button = CCMenuItemLabel::create(label, this, menu_selector(PrismaticMenuPopup::onNavigate));
            button->setTag(i);
            button->setColor(cc3b(180, 140, 255));
            button->setPosition({(i - 1.5f) * 72.f, 0.f});
            nav->addChild(button);
        }
        m_mainLayer->addChild(nav);

        auto footer = CCLabelBMFont::create("Z E E R   •   GEODE 2.208", "bigFont.fnt");
        footer->setScale(.30f);
        footer->setColor(cc3b(115, 120, 140));
        footer->setPosition({size.width / 2, 12.f});
        m_mainLayer->addChild(footer);

        CCDirector::sharedDirector()->setDisplayStats(savedBool("fps-counter", false));
        setPage(PAGE_DASHBOARD);
        return true;
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
        if (!menu) menu = this->getChildByID("bottom-menu");
        if (!menu) return true;

        auto sprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
        if (!sprite) sprite = CCSprite::createWithSpriteFrameName("GJ_playBtn_001.png");
        if (!sprite) return true;

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
