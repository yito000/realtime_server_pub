// automatically generated, do not modify

package DemoBattle

import (
	flatbuffers "github.com/google/flatbuffers/go"
)
type PlayerCharacterInfo struct {
	_tab flatbuffers.Table
}

func (rcv *PlayerCharacterInfo) Init(buf []byte, i flatbuffers.UOffsetT) {
	rcv._tab.Bytes = buf
	rcv._tab.Pos = i
}

func (rcv *PlayerCharacterInfo) CharacterId() int32 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(4))
	if o != 0 {
		return rcv._tab.GetInt32(o + rcv._tab.Pos)
	}
	return 0
}

func (rcv *PlayerCharacterInfo) Name() string {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(6))
	if o != 0 {
		return rcv._tab.String(o + rcv._tab.Pos)
	}
	return ""
}

func (rcv *PlayerCharacterInfo) BaseHp() int16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(8))
	if o != 0 {
		return rcv._tab.GetInt16(o + rcv._tab.Pos)
	}
	return 100
}

func (rcv *PlayerCharacterInfo) BaseMana() int16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(10))
	if o != 0 {
		return rcv._tab.GetInt16(o + rcv._tab.Pos)
	}
	return 100
}

func (rcv *PlayerCharacterInfo) PhysicalAttack() int16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(12))
	if o != 0 {
		return rcv._tab.GetInt16(o + rcv._tab.Pos)
	}
	return 10
}

func (rcv *PlayerCharacterInfo) MagicalAttack() int16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(14))
	if o != 0 {
		return rcv._tab.GetInt16(o + rcv._tab.Pos)
	}
	return 10
}

func (rcv *PlayerCharacterInfo) PhysicalDefense() int16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(16))
	if o != 0 {
		return rcv._tab.GetInt16(o + rcv._tab.Pos)
	}
	return 10
}

func (rcv *PlayerCharacterInfo) MagicalDefense() int16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(18))
	if o != 0 {
		return rcv._tab.GetInt16(o + rcv._tab.Pos)
	}
	return 10
}

func (rcv *PlayerCharacterInfo) Speed() int16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(20))
	if o != 0 {
		return rcv._tab.GetInt16(o + rcv._tab.Pos)
	}
	return 10
}

func PlayerCharacterInfoStart(builder *flatbuffers.Builder) { builder.StartObject(9) }
func PlayerCharacterInfoAddCharacterId(builder *flatbuffers.Builder, characterId int32) { builder.PrependInt32Slot(0, characterId, 0) }
func PlayerCharacterInfoAddName(builder *flatbuffers.Builder, name flatbuffers.UOffsetT) { builder.PrependUOffsetTSlot(1, flatbuffers.UOffsetT(name), 0) }
func PlayerCharacterInfoAddBaseHp(builder *flatbuffers.Builder, baseHp int16) { builder.PrependInt16Slot(2, baseHp, 100) }
func PlayerCharacterInfoAddBaseMana(builder *flatbuffers.Builder, baseMana int16) { builder.PrependInt16Slot(3, baseMana, 100) }
func PlayerCharacterInfoAddPhysicalAttack(builder *flatbuffers.Builder, physicalAttack int16) { builder.PrependInt16Slot(4, physicalAttack, 10) }
func PlayerCharacterInfoAddMagicalAttack(builder *flatbuffers.Builder, magicalAttack int16) { builder.PrependInt16Slot(5, magicalAttack, 10) }
func PlayerCharacterInfoAddPhysicalDefense(builder *flatbuffers.Builder, physicalDefense int16) { builder.PrependInt16Slot(6, physicalDefense, 10) }
func PlayerCharacterInfoAddMagicalDefense(builder *flatbuffers.Builder, magicalDefense int16) { builder.PrependInt16Slot(7, magicalDefense, 10) }
func PlayerCharacterInfoAddSpeed(builder *flatbuffers.Builder, speed int16) { builder.PrependInt16Slot(8, speed, 10) }
func PlayerCharacterInfoEnd(builder *flatbuffers.Builder) flatbuffers.UOffsetT { return builder.EndObject() }
