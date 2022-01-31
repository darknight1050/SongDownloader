#pragma once
#include "../TypeMacros.hpp"
#include "Difficulty.hpp"

// Forward declaration of Leaderbaord and the GetCoverImageAsync function.
namespace ScoreSaber {
    class Leaderboard;
    namespace API {
        void GetCoverImageAsync(const ScoreSaber::Leaderboard& ldb, std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate);
    }
}

DECLARE_JSON_CLASS(ScoreSaber, Leaderboard, 
    GETTER_VALUE(int, Id);
    GETTER_VALUE(std::string, SongHash);
    GETTER_VALUE(std::string, SongName);
    GETTER_VALUE(std::string, SongSubName);
    GETTER_VALUE(std::string, SongAuthorName);
    GETTER_VALUE(std::string, LevelAuthorName);
    GETTER_VALUE(Difficulty_t, Difficulty);
    GETTER_VALUE(int, MaxScore);
    GETTER_VALUE(std::string, CreatedDate);
    GETTER_VALUE_OPTIONAL(int, RankedDate);
    GETTER_VALUE_OPTIONAL(int, QualifiedDate);
    GETTER_VALUE_OPTIONAL(int, LovedDate);
    GETTER_VALUE(bool, Ranked);
    GETTER_VALUE(bool, Qualified);
    GETTER_VALUE(bool, Loved);
    GETTER_VALUE(int, MaxPP);
    GETTER_VALUE(float, Stars);
    GETTER_VALUE(int, Plays);
    GETTER_VALUE(int, DailyPlays);
    GETTER_VALUE(bool, PositiveModifiers);
    //DESERIALIZE_CLASS_OPTIONAL(PlayerScore, playerScore, Int);
    GETTER_VALUE(std::string, CoverImage);
    //DESERIALIZE_VECTOR_OPTIONAL(Difficulties, difficulties, );
    inline void GetCoverImageAsync(std::function<void(std::vector<uint8_t>)> finished, std::function<void(float)> progressUpdate = nullptr) {
        ScoreSaber::API::GetCoverImageAsync(*this, finished, progressUpdate);
    }
)
