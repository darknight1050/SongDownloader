# SongDownloader

## Credits

* [Sc2ad](https://github.com/Sc2ad) and [jakibaki](https://github.com/jakibaki) - [beatsaber-hook](https://github.com/sc2ad/beatsaber-hook)
* [raftario](https://github.com/raftario) - [vscode-bsqm](https://github.com/raftario/vscode-bsqm) and [this template](https://github.com/raftario/bmbf-mod-template)
* [lolPants](https://github.com/lolPants) - [BeatSaverSharp](https://github.com/lolPants/BeatSaverSharp)
* [Kylemc1413](https://github.com/Kylemc1413) - [BeatSaverDownloader](https://github.com/Kylemc1413/BeatSaverDownloader/)


ScoreSaber browsing uses [the v2 maps API](https://scoresaber.com/api/docs#tag/maps/GET/api/v2/maps).
`GetList`, `GetListAsync`, and `SearchAsync` now return `ScoreSaber::Maps` instead of
`ScoreSaber::Leaderboards`; consumers must rebuild against the updated headers.
Read entries with `GetData()`, hashes with `Map::GetHash()`, covers with
`Map::GetCoverUrl()`, and pagination with `GetMetadata().GetTotalPages()`.
The legacy `unique` parameter is ignored because results are already grouped by map.