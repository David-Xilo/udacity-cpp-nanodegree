#ifndef SDL2TEST_MENUCHOICE_H
#define SDL2TEST_MENUCHOICE_H

#include <string>
#include <vector>
#include <map>
#include "gameEntity.h"
#include "controller.h"

namespace MenuConstants {

    enum class Option {
        PLAY, QUIT, REPLAY
    };

    const std::string selector{">"};
    const std::string preMenuTitle{"Would you like to play?"};
    const std::string postMenuTitle{"Play again?"};
    const std::string playOption{"Play"};
    const std::string replayOption{"Replay"};
    const std::string quitOption{"Quit"};
    const std::string scoreText{"Score: "};
    const std::string sizeText{"Size: "};

    const std::vector<Option> preMenuOptions{Option::PLAY, Option::QUIT};
    const std::vector<Option> postMenuOptions{Option::REPLAY, Option::QUIT};

    const std::map<Option, std::string> optionMap{{Option::PLAY, playOption},
                                                  {Option::REPLAY, replayOption},
                                                  {Option::QUIT, quitOption}};
}

class MenuChoice : public InputEntity {
public:
    explicit MenuChoice(std::vector<MenuConstants::Option> opts, bool pScore);

    void handleInput(Input input) override;

    void reset();

    bool isChosen() const;

    bool isPrintScore() const;

    MenuConstants::Option getChosenOption();

    const std::vector<MenuConstants::Option>& getOptions();

private:
    std::vector<MenuConstants::Option> options;

    int currentPosition{0};

    bool chosen{false};

    bool printScore{false};
};

#endif //SDL2TEST_MENUCHOICE_H
