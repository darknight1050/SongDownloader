#!/usr/bin/env python3
"""Exercise the production cover methods and scheduled callbacks with Unity fakes.

Run with Python 3 and a host C++20 compiler (optionally selected by CXX).
This tests ownership/dispatch, not Unity's deferred destruction or Android hooks.
"""
from pathlib import Path
import json
import os
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
source = (root / "src/CustomTypes/SearchEntry.cpp").read_text()


def balanced_body(start):
    opening = source.index("{", start)
    depth = 1
    end = opening + 1
    while depth:
        depth += (source[end] == "{") - (source[end] == "}")
        end += 1
    return source[start:end]


methods = "\n".join(
    balanced_body(source.index("void SearchEntry::" + name + "("))
    for name in ("ClearCover", "ApplyCoverImage", "Disable")
)
callback_prefix = "[this, owner, request, currentSearchIndex, bytes = std::move(bytes)]"
callbacks = []
cursor = 0
while (cursor := source.find(callback_prefix, cursor)) >= 0:
    callbacks.append(balanced_body(cursor))
    cursor += len(callback_prefix)
assert len(callbacks) == 2, "Expected both BeatSaver and ScoreSaber callbacks"

program = r'''
#include <cassert>
#include <cstdint>
#include <functional>
#include <iostream>
#include <utility>
#include <vector>
struct Texture { bool alive = true; };
struct Sprite {
    bool alive = true; Texture* texture;
    Texture* get_texture() { assert(alive); return texture; }
};
struct GameObject {
    bool alive = true, active = true;
    void SetActive(bool value) { assert(alive); active = value; }
};
namespace HMUI {
struct ImageView {
    bool alive = true; Sprite* image = nullptr; bool enabled = false;
    void set_sprite(Sprite* value) { assert(alive); image = value; }
    void set_enabled(bool value) { assert(alive); enabled = value; }
};
}
template<class T> struct UnityW {
    T* value = nullptr;
    UnityW() = default;
    UnityW(T* p) : value(p) {}
    T* ptr() { return value; }
    T* operator->() { return value; }
    operator bool() const { return value && value->alive; }
    UnityW& operator=(T* p) { value = p; return *this; }
};
template<class T> using ArrayW = std::vector<T>;
static int textures = 0, sprites = 0;
static std::vector<Sprite*> allocations;
struct Object {
    static void Destroy(Texture* p) { assert(p && p->alive); p->alive = false; --textures; }
    static void Destroy(Sprite* p) { assert(p && p->alive); p->alive = false; --sprites; }
};
Sprite* ArrayToSprite(ArrayW<uint8_t> const& bytes) {
    assert(!bytes.empty());
    if (bytes[0] == 0) return nullptr; // Failed image decoding.
    ++textures; ++sprites;
    auto p = new Sprite{true, new Texture};
    allocations.push_back(p);
    return p;
}
struct DownloadSongsSearchViewController { static inline int searchIndex = 0; };
struct SearchEntry {
    GameObject* gameObject;
    HMUI::ImageView* coverImageView;
    UnityW<Sprite> ownedCover;
    uint64_t coverRequest = 0;
    void ClearCover();
    void ApplyCoverImage(std::vector<uint8_t> const&);
    void Disable();
    std::function<void()> PendingBeatSaver();
    std::function<void()> PendingScoreSaber();
};
'''
program += methods
for name, callback in zip(("PendingBeatSaver", "PendingScoreSaber"), callbacks):
    program += f'''
std::function<void()> SearchEntry::{name}() {{
    UnityW<GameObject> owner = gameObject;
    const auto request = coverRequest;
    const auto currentSearchIndex = DownloadSongsSearchViewController::searchIndex;
    std::vector<uint8_t> bytes{{1, 2, 3}};
    return {callback};
}}
'''
program += r'''
int main() {
    Texture sharedTexture;
    Sprite shared{true, &sharedTexture};
    GameObject owner;
    HMUI::ImageView view{true, &shared};
    SearchEntry row{&owner, &view};
    row.ClearCover();
    assert(shared.alive && sharedTexture.alive && textures == 0 && sprites == 0);
    for (int i = 0; i < 1000; ++i) {
        row.ApplyCoverImage({1, 2, 3});
        assert(sprites == 1 && textures == 1 && view.image == row.ownedCover.ptr());
    }
    row.ClearCover(); row.ClearCover();
    assert(textures == 0 && sprites == 0 && !view.image);
    row.ApplyCoverImage({});
    row.ApplyCoverImage({0});
    assert(textures == 0 && sprites == 0);
    row.ApplyCoverImage({1});
    view.alive = false;
    row.ClearCover();
    assert(textures == 0 && sprites == 0 && shared.alive && sharedTexture.alive);
    view.alive = true;
    view.image = nullptr;
    int callbackCases = 0;
    for (auto pending : {&SearchEntry::PendingBeatSaver, &SearchEntry::PendingScoreSaber}) {
        auto obsolete = (row.*pending)();
        row.ClearCover(); // A different result reused this row.
        auto current = (row.*pending)();
        obsolete();
        assert(textures == 0 && sprites == 0);
        current();
        assert(textures == 1 && sprites == 1 && view.image == row.ownedCover.ptr());
        auto expected = view.image;
        obsolete(); // An old response also cannot replace the current image.
        assert(view.image == expected && textures == 1 && sprites == 1);
        row.ClearCover();
        auto olderSearch = (row.*pending)();
        ++DownloadSongsSearchViewController::searchIndex;
        olderSearch();
        assert(textures == 0 && sprites == 0);
        auto disabled = (row.*pending)();
        row.ApplyCoverImage({1});
        row.Disable();
        disabled();
        assert(!owner.active && textures == 0 && sprites == 0);
        owner.active = true;
        auto destroyedOwner = new SearchEntry{&owner, &view};
        auto afterDestruction = (destroyedOwner->*pending)();
        owner.alive = false;
        delete destroyedOwner;
        afterDestruction(); // The Unity owner guard precedes any row access.
        assert(textures == 0 && sprites == 0);
        owner.alive = true;
        callbackCases += 6;
    }
    for (auto p : allocations) { delete p->texture; delete p; }
    std::cout << "{\"passed\":true,\"replacements\":1000,\"callback_cases\":"
              << callbackCases << ",\"remaining_owned_sprites\":" << sprites
              << ",\"remaining_owned_textures\":" << textures << "}\n";
}
'''
with tempfile.TemporaryDirectory(prefix="songdownloader-cover-test-") as directory:
    cpp = Path(directory) / "test.cpp"
    exe = Path(directory) / "test"
    cpp.write_text(program)
    subprocess.run(
        [os.environ.get("CXX", "c++"), "-std=c++20", "-O1", str(cpp), "-o", str(exe)],
        check=True,
    )
    result = json.loads(subprocess.check_output([str(exe)], text=True))
    result["scope"] = "Actual cover methods and both scheduled callback bodies; mocked Unity objects. Android hook dispatch, Unity deferred destruction and network integration are not tested."
    print(json.dumps(result, indent=2))
