// automatically generated, do not modify

package DemoBattle

import (
	flatbuffers "github.com/google/flatbuffers/go"
)
type TargetCharacterInfo struct {
	_tab flatbuffers.Table
}

func (rcv *TargetCharacterInfo) Init(buf []byte, i flatbuffers.UOffsetT) {
	rcv._tab.Bytes = buf
	rcv._tab.Pos = i
}

func (rcv *TargetCharacterInfo) CharacterId() int32 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(4))
	if o != 0 {
		return rcv._tab.GetInt32(o + rcv._tab.Pos)
	}
	return 0
}

func (rcv *TargetCharacterInfo) Name() []byte {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(6))
	if o != 0 {
		return rcv._tab.ByteVector(o + rcv._tab.Pos)
	}
	return nil
}

func (rcv *TargetCharacterInfo) BaseHp() int16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(8))
	if o != 0 {
		return rcv._tab.GetInt16(o + rcv._tab.Pos)
	}
	return 100
}

func (rcv *TargetCharacterInfo) BaseMana() int16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(10))
	if o != 0 {
		return rcv._tab.GetInt16(o + rcv._tab.Pos)
	}
	return 100
}

func (rcv *TargetCharacterInfo) PhysicalAttack() int16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(12))
	if o != 0 {
		return rcv._tab.GetInt16(o + rcv._tab.Pos)
	}
	return 10
}

func (rcv *TargetCharacterInfo) MagicalAttack() int16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(14))
	if o != 0 {
		return rcv._tab.GetInt16(o + rcv._tab.Pos)
	}
	return 10
}

func (rcv *TargetCharacterInfo) PhysicalDefense() int16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(16))
	if o != 0 {
		return rcv._tab.GetInt16(o + rcv._tab.Pos)
	}
	return 10
}

func (rcv *TargetCharacterInfo) MagicalDefense() int16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(18))
	if o != 0 {
		return rcv._tab.GetInt16(o + rcv._tab.Pos)
	}
	return 10
}

func (rcv *TargetCharacterInfo) Speed() int16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(20))
	if o != 0 {
		return rcv._tab.GetInt16(o + rcv._tab.Pos)
	}
	return 10
}

func TargetCharacterInfoStart(builder *flatbuffers.Builder) { builder.StartObject(9) }
func TargetCharacterInfoAddCharacterId(builder *flatbuffers.Builder, characterId int32) { builder.PrependInt32Slot(0, characterId, 0) }
func TargetCharacterInfoAddName(builder *flatbuffers.Builder, name flatbuffers.UOffsetT) { builder.PrependUOffsetTSlot(1, flatbuffers.UOffsetT(name), 0) }
func TargetCharacterInfoAddBaseHp(builder *flatbuffers.Builder, baseHp int16) { builder.PrependInt16Slot(2, baseHp, 100) }
func TargetCharacterInfoAddBaseMana(builder *flatbuffers.Builder, baseMana int16) { builder.PrependInt16Slot(3, baseMana, 100) }
func TargetCharacterInfoAddPhysicalAttack(builder *flatbuffers.Builder, physicalAttack int16) { builder.PrependInt16Slot(4, physicalAttack, 10) }
func TargetCharacterInfoAddMagicalAttack(builder *flatbuffers.Builder, magicalAttack int16) { builder.PrependInt16Slot(5, magicalAttack, 10) }
func TargetCharacterInfoAddPhysicalDefense(builder *flatbuffers.Builder, physicalDefense int16) { builder.PrependInt16Slot(6, physicalDefense, 10) }
func TargetCharacterInfoAddMagicalDefense(builder *flatbuffers.Builder, magicalDefense int16) { builder.PrependInt16Slot(7, magicalDefense, 10) }
func TargetCharacterInfoAddSpeed(builder *flatbuffers.Builder, speed int16) { builder.PrependInt16Slot(8, speed, 10) }
func TargetCharacterInfoEnd(builder *flatbuffers.Builder) flatbuffers.UOffsetT { return builder.EndObject() }
