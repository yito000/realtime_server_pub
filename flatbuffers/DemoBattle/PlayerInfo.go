// automatically generated, do not modify

package DemoBattle

import (
	flatbuffers "github.com/google/flatbuffers/go"
)
type PlayerInfo struct {
	_tab flatbuffers.Table
}

func GetRootAsPlayerInfo(buf []byte, offset flatbuffers.UOffsetT) *PlayerInfo {
	n := flatbuffers.GetUOffsetT(buf[offset:])
	x := &PlayerInfo{}
	x.Init(buf, n + offset)
	return x
}

func (rcv *PlayerInfo) Init(buf []byte, i flatbuffers.UOffsetT) {
	rcv._tab.Bytes = buf
	rcv._tab.Pos = i
}

func (rcv *PlayerInfo) PlayerId() int32 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(4))
	if o != 0 {
		return rcv._tab.GetInt32(o + rcv._tab.Pos)
	}
	return 0
}

func (rcv *PlayerInfo) Name() string {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(6))
	if o != 0 {
		return rcv._tab.String(o + rcv._tab.Pos)
	}
	return ""
}

func (rcv *PlayerInfo) Characters(obj *PlayerCharacterInfo, j int) bool {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(8))
	if o != 0 {
		x := rcv._tab.Vector(o)
		x += flatbuffers.UOffsetT(j) * 4
		x = rcv._tab.Indirect(x)
	if obj == nil {
		obj = new(PlayerCharacterInfo)
	}
		obj.Init(rcv._tab.Bytes, x)
		return true
	}
	return false
}

func (rcv *PlayerInfo) CharactersLength() int {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(8))
	if o != 0 {
		return rcv._tab.VectorLen(o)
	}
	return 0
}

func PlayerInfoStart(builder *flatbuffers.Builder) { builder.StartObject(3) }
func PlayerInfoAddPlayerId(builder *flatbuffers.Builder, playerId int32) { builder.PrependInt32Slot(0, playerId, 0) }
func PlayerInfoAddName(builder *flatbuffers.Builder, name flatbuffers.UOffsetT) { builder.PrependUOffsetTSlot(1, flatbuffers.UOffsetT(name), 0) }
func PlayerInfoAddCharacters(builder *flatbuffers.Builder, characters flatbuffers.UOffsetT) { builder.PrependUOffsetTSlot(2, flatbuffers.UOffsetT(characters), 0) }
func PlayerInfoStartCharactersVector(builder *flatbuffers.Builder, numElems int) flatbuffers.UOffsetT { return builder.StartVector(4, numElems, 4)
}
func PlayerInfoEnd(builder *flatbuffers.Builder) flatbuffers.UOffsetT { return builder.EndObject() }
