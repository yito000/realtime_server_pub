// automatically generated, do not modify

package DemoBattle

import (
	flatbuffers "github.com/google/flatbuffers/go"
)
type BattleInfo struct {
	_tab flatbuffers.Table
}

func GetRootAsBattleInfo(buf []byte, offset flatbuffers.UOffsetT) *BattleInfo {
	n := flatbuffers.GetUOffsetT(buf[offset:])
	x := &BattleInfo{}
	x.Init(buf, n + offset)
	return x
}

func (rcv *BattleInfo) Init(buf []byte, i flatbuffers.UOffsetT) {
	rcv._tab.Bytes = buf
	rcv._tab.Pos = i
}

func (rcv *BattleInfo) Host() []byte {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(4))
	if o != 0 {
		return rcv._tab.ByteVector(o + rcv._tab.Pos)
	}
	return nil
}

func (rcv *BattleInfo) Player1Id() int32 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(6))
	if o != 0 {
		return rcv._tab.GetInt32(o + rcv._tab.Pos)
	}
	return 0
}

func (rcv *BattleInfo) Player1Name() []byte {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(8))
	if o != 0 {
		return rcv._tab.ByteVector(o + rcv._tab.Pos)
	}
	return nil
}

func (rcv *BattleInfo) Player1Characters(obj *BattlePlayerCharacter, j int) bool {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(10))
	if o != 0 {
		x := rcv._tab.Vector(o)
		x += flatbuffers.UOffsetT(j) * 4
		x = rcv._tab.Indirect(x)
	if obj == nil {
		obj = new(BattlePlayerCharacter)
	}
		obj.Init(rcv._tab.Bytes, x)
		return true
	}
	return false
}

func (rcv *BattleInfo) Player1CharactersLength() int {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(10))
	if o != 0 {
		return rcv._tab.VectorLen(o)
	}
	return 0
}

func (rcv *BattleInfo) Player2Id() int32 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(12))
	if o != 0 {
		return rcv._tab.GetInt32(o + rcv._tab.Pos)
	}
	return 0
}

func (rcv *BattleInfo) Player2Name() []byte {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(14))
	if o != 0 {
		return rcv._tab.ByteVector(o + rcv._tab.Pos)
	}
	return nil
}

func (rcv *BattleInfo) Player2Characters(obj *BattlePlayerCharacter, j int) bool {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(16))
	if o != 0 {
		x := rcv._tab.Vector(o)
		x += flatbuffers.UOffsetT(j) * 4
		x = rcv._tab.Indirect(x)
	if obj == nil {
		obj = new(BattlePlayerCharacter)
	}
		obj.Init(rcv._tab.Bytes, x)
		return true
	}
	return false
}

func (rcv *BattleInfo) Player2CharactersLength() int {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(16))
	if o != 0 {
		return rcv._tab.VectorLen(o)
	}
	return 0
}

func BattleInfoStart(builder *flatbuffers.Builder) { builder.StartObject(7) }
func BattleInfoAddHost(builder *flatbuffers.Builder, host flatbuffers.UOffsetT) { builder.PrependUOffsetTSlot(0, flatbuffers.UOffsetT(host), 0) }
func BattleInfoAddPlayer1Id(builder *flatbuffers.Builder, player1Id int32) { builder.PrependInt32Slot(1, player1Id, 0) }
func BattleInfoAddPlayer1Name(builder *flatbuffers.Builder, player1Name flatbuffers.UOffsetT) { builder.PrependUOffsetTSlot(2, flatbuffers.UOffsetT(player1Name), 0) }
func BattleInfoAddPlayer1Characters(builder *flatbuffers.Builder, player1Characters flatbuffers.UOffsetT) { builder.PrependUOffsetTSlot(3, flatbuffers.UOffsetT(player1Characters), 0) }
func BattleInfoStartPlayer1CharactersVector(builder *flatbuffers.Builder, numElems int) flatbuffers.UOffsetT { return builder.StartVector(4, numElems, 4)
}
func BattleInfoAddPlayer2Id(builder *flatbuffers.Builder, player2Id int32) { builder.PrependInt32Slot(4, player2Id, 0) }
func BattleInfoAddPlayer2Name(builder *flatbuffers.Builder, player2Name flatbuffers.UOffsetT) { builder.PrependUOffsetTSlot(5, flatbuffers.UOffsetT(player2Name), 0) }
func BattleInfoAddPlayer2Characters(builder *flatbuffers.Builder, player2Characters flatbuffers.UOffsetT) { builder.PrependUOffsetTSlot(6, flatbuffers.UOffsetT(player2Characters), 0) }
func BattleInfoStartPlayer2CharactersVector(builder *flatbuffers.Builder, numElems int) flatbuffers.UOffsetT { return builder.StartVector(4, numElems, 4)
}
func BattleInfoEnd(builder *flatbuffers.Builder) flatbuffers.UOffsetT { return builder.EndObject() }
