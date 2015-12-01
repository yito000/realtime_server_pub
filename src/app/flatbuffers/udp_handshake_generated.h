// automatically generated by the FlatBuffers compiler, do not modify

#ifndef FLATBUFFERS_GENERATED_UDPHANDSHAKE_DEMOBATTLE_H_
#define FLATBUFFERS_GENERATED_UDPHANDSHAKE_DEMOBATTLE_H_

#include "flatbuffers/flatbuffers.h"


namespace DemoBattle {

struct UdpHandshake;

struct UdpHandshake FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  const flatbuffers::String *battle_key() const { return GetPointer<const flatbuffers::String *>(4); }
  int32_t player_id() const { return GetField<int32_t>(6, 0); }
  const flatbuffers::String *access_token() const { return GetPointer<const flatbuffers::String *>(8); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, 4 /* battle_key */) &&
           verifier.Verify(battle_key()) &&
           VerifyField<int32_t>(verifier, 6 /* player_id */) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, 8 /* access_token */) &&
           verifier.Verify(access_token()) &&
           verifier.EndTable();
  }
};

struct UdpHandshakeBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_battle_key(flatbuffers::Offset<flatbuffers::String> battle_key) { fbb_.AddOffset(4, battle_key); }
  void add_player_id(int32_t player_id) { fbb_.AddElement<int32_t>(6, player_id, 0); }
  void add_access_token(flatbuffers::Offset<flatbuffers::String> access_token) { fbb_.AddOffset(8, access_token); }
  UdpHandshakeBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  UdpHandshakeBuilder &operator=(const UdpHandshakeBuilder &);
  flatbuffers::Offset<UdpHandshake> Finish() {
    auto o = flatbuffers::Offset<UdpHandshake>(fbb_.EndTable(start_, 3));
    return o;
  }
};

inline flatbuffers::Offset<UdpHandshake> CreateUdpHandshake(flatbuffers::FlatBufferBuilder &_fbb,
   flatbuffers::Offset<flatbuffers::String> battle_key = 0,
   int32_t player_id = 0,
   flatbuffers::Offset<flatbuffers::String> access_token = 0) {
  UdpHandshakeBuilder builder_(_fbb);
  builder_.add_access_token(access_token);
  builder_.add_player_id(player_id);
  builder_.add_battle_key(battle_key);
  return builder_.Finish();
}

inline const DemoBattle::UdpHandshake *GetUdpHandshake(const void *buf) { return flatbuffers::GetRoot<DemoBattle::UdpHandshake>(buf); }

inline bool VerifyUdpHandshakeBuffer(flatbuffers::Verifier &verifier) { return verifier.VerifyBuffer<DemoBattle::UdpHandshake>(); }

inline void FinishUdpHandshakeBuffer(flatbuffers::FlatBufferBuilder &fbb, flatbuffers::Offset<DemoBattle::UdpHandshake> root) { fbb.Finish(root); }

}  // namespace DemoBattle

#endif  // FLATBUFFERS_GENERATED_UDPHANDSHAKE_DEMOBATTLE_H_