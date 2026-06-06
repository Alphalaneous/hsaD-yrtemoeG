#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace code_helper {

    inline void parseObjectDecomp(int tabID, std::string_view decomp) {
        StringBuffer<> buffer;

        auto split = utils::string::split(decomp, "\n");
        for (const auto& line : split) {
            auto trimmed = utils::string::trim(line);
            if (!trimmed.starts_with("pCVar9")) continue;

            auto split2 = utils::string::split(line, "pCVar9 = (CCObject *)getCreateBtn(this,");
            auto part1 = split2[1].substr(0, split2[1].size() - 2);

            auto split3 = utils::string::split(part1, ",");

            int id;

            if (split3[0].starts_with("0x")) {
                id = numFromString<int>(split3[0].substr(2), 16).unwrapOr(-1);
            }
            else {
                id = numFromString<int>(split3[0]).unwrapOr(-1);
            }

            auto btnType = numFromString<int>(split3[1]).unwrapOr(-1);

            buffer.append("objects->addObject(getCreateBtn({}, {}));\n", id, btnType);
        }

        (void) utils::file::writeString(Mod::get()->getSaveDir() / fmt::format("objects-generated-{}.txt", tabID), buffer.str());
    
    }

    inline void hexCaseToDecimal(std::string_view code) {
        StringBuffer<> buffer;

        auto split = utils::string::split(code, "\n");
        for (const auto& line : split) {
            auto trimmed = utils::string::trim(line);
            if (trimmed.starts_with("case")) {
                auto spaceSplit = utils::string::split(trimmed, " ");
                if (spaceSplit.size() >= 2) {
                    auto num = spaceSplit[1].substr(0, spaceSplit[1].size() - 1);
                    if (num.starts_with("0x")) {
                        int id = numFromString<int>(num.substr(2), 16).unwrapOr(-1);

                        buffer.append("        case {}:\n", id);
                        continue;
                    }
                }
            }
            buffer.append("{}\n", line);
        }

        (void) utils::file::writeString(Mod::get()->getSaveDir() / fmt::format("hexCaseToDecimal.txt"), buffer.str());
    }
};