// automatically generated by the FlatBuffers compiler, do not modify

#ifndef FLATBUFFERS_GENERATED_BATTLEENTRY_DEMOBATTLE_H_
#define FLATBUFFERS_GENERATED_BATTLEENTRY_DEMOBATTLE_H_

#include "flatbuffers/flatbuffers.h"


namespace DemoBattle {

struct BattleEntry;

struct BattleEntry FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  const flatbuffers::String *battle_key() const { return GetPointer<const flatbuffers::String *>(4); }
  int32_t player_id() const { return GetField<int32_t>(6, 0); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, 4 /* battle_key */) &&
           verifier.Verify(battle_key()) &&
           VerifyField<int32_t>(verifier, 6 /* player_id */) &&
           verifier.EndTable();
  }
};

struct BattleEntryBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_battle_key(flatbuffers::Offset<flatbuffers::String> battle_key) { fbb_.AddOffset(4, battle_key); }
  void add_player_id(int32_t player_id) { fbb_.AddElement<int32_t>(6, player_id, 0); }
  BattleEntryBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  BattleEntryBuilder &operator=(const BattleEntryBuilder &);
  flatbuffers::Offset<BattleEntry> Finish() {
    auto o = flatbuffers::Offset<BattleEntry>(fbb_.EndTable(start_, 2));
    return o;
  }
};

inline flatbuffers::Offset<BattleEntry> CreateBattleEntry(flatbuffers::FlatBufferBuilder &_fbb,
   flatbuffers::Offset<flatbuffers::String> battle_key = 0,
   int32_t player_id = 0) {
  BattleEntryBuilder builder_(_fbb);
  builder_.add_player_id(player_id);
  builder_.add_battle_key(battle_key);
  return builder_.Finish();
}

inline const DemoBattle::BattleEntry *GetBattleEntry(const void *buf) { return flatbuffers::GetRoot<DemoBattle::BattleEntry>(buf); }

inline bool VerifyBattleEntryBuffer(flatbuffers::Verifier &verifier) { return verifier.VerifyBuffer<DemoBattle::BattleEntry>(); }

inline void FinishBattleEntryBuffer(flatbuffers::FlatBufferBuilder &fbb, flatbuffers::Offset<DemoBattle::BattleEntry> root) { fbb.Finish(root); }

}  // namespace DemoBattle

#endif  // FLATBUFFERS_GENERATED_BATTLEENTRY_DEMOBATTLE_H_