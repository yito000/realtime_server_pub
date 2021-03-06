// automatically generated by the FlatBuffers compiler, do not modify

#ifndef FLATBUFFERS_GENERATED_NOTIFYDEBUFFACTION_DEMOBATTLE_H_
#define FLATBUFFERS_GENERATED_NOTIFYDEBUFFACTION_DEMOBATTLE_H_

#include "flatbuffers/flatbuffers.h"


namespace DemoBattle {

struct DebuffDetail;
struct NotifyDebuffAction;

MANUALLY_ALIGNED_STRUCT(4) DebuffDetail FLATBUFFERS_FINAL_CLASS {
 private:
  int32_t character_id_;
  int32_t target_player_id_;
  int32_t target_character_id_;
  int32_t action_id_;
  int32_t debuff_id_;

 public:
  DebuffDetail(int32_t character_id, int32_t target_player_id, int32_t target_character_id, int32_t action_id, int32_t debuff_id)
    : character_id_(flatbuffers::EndianScalar(character_id)), target_player_id_(flatbuffers::EndianScalar(target_player_id)), target_character_id_(flatbuffers::EndianScalar(target_character_id)), action_id_(flatbuffers::EndianScalar(action_id)), debuff_id_(flatbuffers::EndianScalar(debuff_id)) { }

  int32_t character_id() const { return flatbuffers::EndianScalar(character_id_); }
  int32_t target_player_id() const { return flatbuffers::EndianScalar(target_player_id_); }
  int32_t target_character_id() const { return flatbuffers::EndianScalar(target_character_id_); }
  int32_t action_id() const { return flatbuffers::EndianScalar(action_id_); }
  int32_t debuff_id() const { return flatbuffers::EndianScalar(debuff_id_); }
};
STRUCT_END(DebuffDetail, 20);

struct NotifyDebuffAction FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  int32_t seq_id() const { return GetField<int32_t>(4, 0); }
  int32_t player_id() const { return GetField<int32_t>(6, 0); }
  const flatbuffers::Vector<const DebuffDetail *> *actions() const { return GetPointer<const flatbuffers::Vector<const DebuffDetail *> *>(8); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, 4 /* seq_id */) &&
           VerifyField<int32_t>(verifier, 6 /* player_id */) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, 8 /* actions */) &&
           verifier.Verify(actions()) &&
           verifier.EndTable();
  }
};

struct NotifyDebuffActionBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_seq_id(int32_t seq_id) { fbb_.AddElement<int32_t>(4, seq_id, 0); }
  void add_player_id(int32_t player_id) { fbb_.AddElement<int32_t>(6, player_id, 0); }
  void add_actions(flatbuffers::Offset<flatbuffers::Vector<const DebuffDetail *>> actions) { fbb_.AddOffset(8, actions); }
  NotifyDebuffActionBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  NotifyDebuffActionBuilder &operator=(const NotifyDebuffActionBuilder &);
  flatbuffers::Offset<NotifyDebuffAction> Finish() {
    auto o = flatbuffers::Offset<NotifyDebuffAction>(fbb_.EndTable(start_, 3));
    return o;
  }
};

inline flatbuffers::Offset<NotifyDebuffAction> CreateNotifyDebuffAction(flatbuffers::FlatBufferBuilder &_fbb,
   int32_t seq_id = 0,
   int32_t player_id = 0,
   flatbuffers::Offset<flatbuffers::Vector<const DebuffDetail *>> actions = 0) {
  NotifyDebuffActionBuilder builder_(_fbb);
  builder_.add_actions(actions);
  builder_.add_player_id(player_id);
  builder_.add_seq_id(seq_id);
  return builder_.Finish();
}

inline const NotifyDebuffAction *GetNotifyDebuffAction(const void *buf) { return flatbuffers::GetRoot<NotifyDebuffAction>(buf); }

inline bool VerifyNotifyDebuffActionBuffer(flatbuffers::Verifier &verifier) { return verifier.VerifyBuffer<NotifyDebuffAction>(); }

inline void FinishNotifyDebuffActionBuffer(flatbuffers::FlatBufferBuilder &fbb, flatbuffers::Offset<NotifyDebuffAction> root) { fbb.Finish(root); }

}  // namespace DemoBattle

#endif  // FLATBUFFERS_GENERATED_NOTIFYDEBUFFACTION_DEMOBATTLE_H_
