#include "menuChoice.h"

void MenuChoice::handleInput(Input input) {
    if (input == Input::kEnter) {
        chosen = true;
    }
    int size = (int) options.size();
    if (input == Input::kUp && currentPosition > 0) {
        currentPosition = ((int) currentPosition - 1) % size;
    }
    if (input == Input::kDown && currentPosition < options.size() - 1) {
        currentPosition = (currentPosition + 1) % size;
    }
}

void MenuChoice::reset() {
    chosen = false;
    currentPosition = 0;
}

MenuChoice::MenuChoice(std::vector<MenuConstants::Option> opts, bool pScore) : options(std::move(opts)), printScore(pScore) {
}

bool MenuChoice::isChosen() const {
    return chosen;
}

MenuConstants::Option MenuChoice::getChosenOption() {
    if (currentPosition >= options.size() ||
        MenuConstants::optionMap.find(options[currentPosition]) == MenuConstants::optionMap.end()) {
        return MenuConstants::Option::QUIT;
    }
    return options[currentPosition];
}

const std::vector<MenuConstants::Option>& MenuChoice::getOptions() {
    return options;
}

bool MenuChoice::isPrintScore() const {
    return printScore;
}

