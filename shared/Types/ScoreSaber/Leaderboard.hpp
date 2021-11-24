#pragma once
#include "../TypeMacros.hpp"

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
    GETTER_VALUE(int, Difficulty);
    GETTER_VALUE(std::string, DifficultyRaw);
    GETTER_VALUE(int, MaxScore);
    GETTER_VALUE(std::string, CreatedDate);
    GETTER_VALUE(int, RankedDate);
    GETTER_VALUE(int, QualifiedDate);
    GETTER_VALUE(int, LovedDate);
    GETTER_VALUE(int, Ranked);
    GETTER_VALUE(int, Qualified);
    GETTER_VALUE(int, Loved);
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
