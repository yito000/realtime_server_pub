// automatically generated, do not modify

package DemoBattle

import (
	flatbuffers "github.com/google/flatbuffers/go"
)
type MatchingInfo struct {
	_tab flatbuffers.Table
}

func GetRootAsMatchingInfo(buf []byte, offset flatbuffers.UOffsetT) *MatchingInfo {
	n := flatbuffers.GetUOffsetT(buf[offset:])
	x := &MatchingInfo{}
	x.Init(buf, n + offset)
	return x
}

func (rcv *MatchingInfo) Init(buf []byte, i flatbuffers.UOffsetT) {
	rcv._tab.Bytes = buf
	rcv._tab.Pos = i
}

func (rcv *MatchingInfo) BattleKey() []byte {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(4))
	if o != 0 {
		return rcv._tab.ByteVector(o + rcv._tab.Pos)
	}
	return nil
}

func (rcv *MatchingInfo) Host() []byte {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(6))
	if o != 0 {
		return rcv._tab.ByteVector(o + rcv._tab.Pos)
	}
	return nil
}

func (rcv *MatchingInfo) WsPath() []byte {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(8))
	if o != 0 {
		return rcv._tab.ByteVector(o + rcv._tab.Pos)
	}
	return nil
}

func (rcv *MatchingInfo) WsPort() uint16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(10))
	if o != 0 {
		return rcv._tab.GetUint16(o + rcv._tab.Pos)
	}
	return 0
}

func (rcv *MatchingInfo) UdpHost() []byte {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(12))
	if o != 0 {
		return rcv._tab.ByteVector(o + rcv._tab.Pos)
	}
	return nil
}

func (rcv *MatchingInfo) UdpPorts(j int) uint16 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(14))
	if o != 0 {
		a := rcv._tab.Vector(o)
		return rcv._tab.GetUint16(a + flatbuffers.UOffsetT(j * 2))
	}
	return 0
}

func (rcv *MatchingInfo) UdpPortsLength() int {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(14))
	if o != 0 {
		return rcv._tab.VectorLen(o)
	}
	return 0
}

func (rcv *MatchingInfo) PlayerId() int32 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(16))
	if o != 0 {
		return rcv._tab.GetInt32(o + rcv._tab.Pos)
	}
	return 0
}

func (rcv *MatchingInfo) TargetId() int32 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(18))
	if o != 0 {
		return rcv._tab.GetInt32(o + rcv._tab.Pos)
	}
	return 0
}

func (rcv *MatchingInfo) TargetName() []byte {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(20))
	if o != 0 {
		return rcv._tab.ByteVector(o + rcv._tab.Pos)
	}
	return nil
}

func (rcv *MatchingInfo) TargetCharacters(obj *TargetCharacterInfo, j int) bool {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(22))
	if o != 0 {
		x := rcv._tab.Vector(o)
		x += flatbuffers.UOffsetT(j) * 4
		x = rcv._tab.Indirect(x)
	if obj == nil {
		obj = new(TargetCharacterInfo)
	}
		obj.Init(rcv._tab.Bytes, x)
		return true
	}
	return false
}

func (rcv *MatchingInfo) TargetCharactersLength() int {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(22))
	if o != 0 {
		return rcv._tab.VectorLen(o)
	}
	return 0
}

func MatchingInfoStart(builder *flatbuffers.Builder) { builder.StartObject(10) }
func MatchingInfoAddBattleKey(builder *flatbuffers.Builder, battleKey flatbuffers.UOffsetT) { builder.PrependUOffsetTSlot(0, flatbuffers.UOffsetT(battleKey), 0) }
func MatchingInfoAddHost(builder *flatbuffers.Builder, host flatbuffers.UOffsetT) { builder.PrependUOffsetTSlot(1, flatbuffers.UOffsetT(host), 0) }
func MatchingInfoAddWsPath(builder *flatbuffers.Builder, wsPath flatbuffers.UOffsetT) { builder.PrependUOffsetTSlot(2, flatbuffers.UOffsetT(wsPath), 0) }
func MatchingInfoAddWsPort(builder *flatbuffers.Builder, wsPort uint16) { builder.PrependUint16Slot(3, wsPort, 0) }
func MatchingInfoAddUdpHost(builder *flatbuffers.Builder, udpHost flatbuffers.UOffsetT) { builder.PrependUOffsetTSlot(4, flatbuffers.UOffsetT(udpHost), 0) }
func MatchingInfoAddUdpPorts(builder *flatbuffers.Builder, udpPorts flatbuffers.UOffsetT) { builder.PrependUOffsetTSlot(5, flatbuffers.UOffsetT(udpPorts), 0) }
func MatchingInfoStartUdpPortsVector(builder *flatbuffers.Builder, numElems int) flatbuffers.UOffsetT { return builder.StartVector(2, numElems, 2)
}
func MatchingInfoAddPlayerId(builder *flatbuffers.Builder, playerId int32) { builder.PrependInt32Slot(6, playerId, 0) }
func MatchingInfoAddTargetId(builder *flatbuffers.Builder, targetId int32) { builder.PrependInt32Slot(7, targetId, 0) }
func MatchingInfoAddTargetName(builder *flatbuffers.Builder, targetName flatbuffers.UOffsetT) { builder.PrependUOffsetTSlot(8, flatbuffers.UOffsetT(targetName), 0) }
func MatchingInfoAddTargetCharacters(builder *flatbuffers.Builder, targetCharacters flatbuffers.UOffsetT) { builder.PrependUOffsetTSlot(9, flatbuffers.UOffsetT(targetCharacters), 0) }
func MatchingInfoStartTargetCharactersVector(builder *flatbuffers.Builder, numElems int) flatbuffers.UOffsetT { return builder.StartVector(4, numElems, 4)
}
func MatchingInfoEnd(builder *flatbuffers.Builder) flatbuffers.UOffsetT { return builder.EndObject() }
