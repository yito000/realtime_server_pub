// automatically generated, do not modify

package DemoBattle

import (
	flatbuffers "github.com/google/flatbuffers/go"
)
type BattlePlayerCharacter struct {
	_tab flatbuffers.Table
}

func (rcv *BattlePlayerCharacter) Init(buf []byte, i flatbuffers.UOffsetT) {
	rcv._tab.Bytes = buf
	rcv._tab.Pos = i
}

func (rcv *BattlePlayerCharacter) CharacterId() int32 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(4))
	if o != 0 {
		return rcv._tab.GetInt32(o + rcv._tab.Pos)
	}
	return 0
}

func (rcv *BattlePlayerCharacter) Name() []byte {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(6))
	if o != 0 {
		return rcv._tab.ByteVector(o + rcv._tab.Pos)
	}
	return nil
}

func (rcv *BattlePlayerCharacter) BaseHp() int16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(8))
	if o != 0 {
		return rcv._tab.GetInt16(o + rcv._tab.Pos)
	}
	return 100
}

func (rcv *BattlePlayerCharacter) BaseMana() int16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(10))
	if o != 0 {
		return rcv._tab.GetInt16(o + rcv._tab.Pos)
	}
	return 100
}

func (rcv *BattlePlayerCharacter) PhysicalAttack() int16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(12))
	if o != 0 {
		return rcv._tab.GetInt16(o + rcv._tab.Pos)
	}
	return 10
}

func (rcv *BattlePlayerCharacter) MagicalAttack() int16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(14))
	if o != 0 {
		return rcv._tab.GetInt16(o + rcv._tab.Pos)
	}
	return 10
}

func (rcv *BattlePlayerCharacter) PhysicalDefense() int16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(16))
	if o != 0 {
		return rcv._tab.GetInt16(o + rcv._tab.Pos)
	}
	return 10
}

func (rcv *BattlePlayerCharacter) MagicalDefense() int16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(18))
	if o != 0 {
		return rcv._tab.GetInt16(o + rcv._tab.Pos)
	}
	return 10
}

func (rcv *BattlePlayerCharacter) Speed() int16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(20))
	if o != 0 {
		return rcv._tab.GetInt16(o + rcv._tab.Pos)
	}
	return 10
}

func BattlePlayerCharacterStart(builder *flatbuffers.Builder) { builder.StartObject(9) }
func BattlePlayerCharacterAddCharacterId(builder *flatbuffers.Builder, characterId int32) { builder.PrependInt32Slot(0, characterId, 0) }
func BattlePlayerCharacterAddName(builder *flatbuffers.Builder, name flatbuffers.UOffsetT) { builder.PrependUOffsetTSlot(1, flatbuffers.UOffsetT(name), 0) }
func BattlePlayerCharacterAddBaseHp(builder *flatbuffers.Builder, baseHp int16) { builder.PrependInt16Slot(2, baseHp, 100) }
func BattlePlayerCharacterAddBaseMana(builder *flatbuffers.Builder, baseMana int16) { builder.PrependInt16Slot(3, baseMana, 100) }
func BattlePlayerCharacterAddPhysicalAttack(builder *flatbuffers.Builder, physicalAttack int16) { builder.PrependInt16Slot(4, physicalAttack, 10) }
func BattlePlayerCharacterAddMagicalAttack(builder *flatbuffers.Builder, magicalAttack int16) { builder.PrependInt16Slot(5, magicalAttack, 10) }
func BattlePlayerCharacterAddPhysicalDefense(builder *flatbuffers.Builder, physicalDefense int16) { builder.PrependInt16Slot(6, physicalDefense, 10) }
func BattlePlayerCharacterAddMagicalDefense(builder *flatbuffers.Builder, magicalDefense int16) { builder.PrependInt16Slot(7, magicalDefense, 10) }
func BattlePlayerCharacterAddSpeed(builder *flatbuffers.Builder, speed int16) { builder.PrependInt16Slot(8, speed, 10) }
func BattlePlayerCharacterEnd(builder *flatbuffers.Builder) flatbuffers.UOffsetT { return builder.EndObject() }
