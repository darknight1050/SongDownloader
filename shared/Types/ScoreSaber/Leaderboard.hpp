#pragma once

#include "../../_config.h"
#include "../TypeMacros.hpp"
#include "Difficulty.hpp"

// Forward declaration of Leaderbaord and the GetCoverImageAsync function.
namespace ScoreSaber {
    class Leaderboard;
    namespace API {
        SONGDOWNLOADER_EXPORT void GetCoverImageAsync(const ScoreSaber::Leaderboard& ldb, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate);
    }

    DECLARE_JSON_STRUCT(Leaderboard) {
        ERROR_CHECK
        GETTER_VALUE(int, Id, "id");
        GETTER_VALUE(std::string, SongHash, "songHash");
        GETTER_VALUE(std::string, SongName, "songName");
        GETTER_VALUE(std::string, SongSubName, "songSubName");
        GETTER_VALUE(std::string, SongAuthorName, "songAuthorName");
        GETTER_VALUE(std::string, LevelAuthorName, "levelAuthorName");
        GETTER_VALUE(Difficulty_t, Difficulty, "difficulty");
        GETTER_VALUE(int, MaxScore, "maxScore");
        GETTER_VALUE_OPTIONAL(int, MaxScoreEx, "maxScoreEx");
        GETTER_VALUE(std::string, CreatedDate, "createdDate");
        GETTER_VALUE_OPTIONAL(std::string, RankedDate, "rankedDate");
        GETTER_VALUE_OPTIONAL(std::string, QualifiedDate, "qualifiedDate");
        GETTER_VALUE_OPTIONAL(std::string, LovedDate, "lovedDate");
        GETTER_VALUE(bool, Ranked, "ranked");
        GETTER_VALUE(bool, Qualified, "qualified");
        GETTER_VALUE(bool, Loved, "loved");
        private:
            using MaxPPTypes = TypeOptions<float, int>;
            NAMED_VALUE(MaxPPTypes, MaxPP, "maxPP");
            using StarsTypes = TypeOptions<float, int>;
            NAMED_VALUE(StarsTypes, Stars, "stars");
        public:
            float GetMaxPP() const {
                if (auto value = MaxPP.GetValue<float>())
                    return *value;
                return *MaxPP.GetValue<int>();
            }
            float GetStars() const {
                if(auto flt = Stars.GetValue<float>())
                    return *flt;
                return *Stars.GetValue<int>();
            }
        GETTER_VALUE(int, Plays, "plays");
        GETTER_VALUE(int, DailyPlays, "dailyPlays");
        GETTER_VALUE(bool, PositiveModifiers, "positiveModifiers");
        //GETTER_VALUE_OPTIONAL(PlayerScore, playerScore, Int, "int");
        GETTER_VALUE(std::string, CoverImage, "coverImage");
        GETTER_VALUE_OPTIONAL(std::vector<Difficulty_t>, Difficulties, "difficulties");
        inline void GetCoverImageAsync(std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate = nullptr) {
            ScoreSaber::API::GetCoverImageAsync(*this, finished, progressUpdate);
        }
    };
}
