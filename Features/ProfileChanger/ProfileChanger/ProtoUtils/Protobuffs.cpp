#ifdef RELEASE
#include "../../../Menu/Menu.h"
#include "../ProfileChanger/MemAlloc.h"
#include "../../../SDK/SteamAPI.h"
#include "Protobuffs.h"
#include "../ProfileChanger/Messages.h"
#include "ProtoWriter.h"
#include "../ProfileChanger/valve_parser.h"

#include "../../../Utils/Interfaces.h"

#include <array>

#include "../Protobuf/base_gcmessages.pb.h"
#include "../Protobuf/cstrike15_gcmessages.pb.h"
#include "../Protobuf/econ_gcmessages.pb.h"
#include "../Protobuf/engine_gcmessages.pb.h"
#include "../Protobuf/gcsystemmsgs.pb.h"
#include "../Protobuf/gcsdk_gcmessages.pb.h"
#include "../Protobuf/netmessages.pb.h"
#include "../Protobuf/steammessages.pb.h"
#include "../protobuf/ProtoField.h"
#include "ProtoWriter.h"

#include <../ProfileChanger/Messages.h>
#include "../../../Utils/Interfaces.h"
#include "../../../SDK/IVEngineClient.h"

#define CAST(cast, address, add) reinterpret_cast<cast>((uint32_t)address + (uint32_t)add)

#define _gc2ch MatchmakingGC2ClientHellos
#define _pci PlayerCommendationInfos
#define _pri PlayerRankingInfos
static std::string ProfileChangerr(void* pubDest, uint32_t* pcubMsgSize)
{
	ProtoWriter msg((void*)((DWORD)pubDest + 8), *pcubMsgSize - 8, 19);
	auto _commendation = msg.has(_gc2ch::commendation) ? msg.get(_gc2ch::commendation).String() : std::string("");
	ProtoWriter commendation(_commendation, 4);
	commendation.replace(Field(_pci::cmd_friendly, TYPE_UINT32, (int64_t)g_Menu.Config.RankChanger.cmd_friendly));
	commendation.replace(Field(_pci::cmd_teaching, TYPE_UINT32, (int64_t)g_Menu.Config.RankChanger.cmd_teaching));
	commendation.replace(Field(_pci::cmd_leader, TYPE_UINT32, (int64_t)g_Menu.Config.RankChanger.cmd_leader));
	msg.replace(Field(_gc2ch::commendation, TYPE_STRING, commendation.serialize()));
	auto _ranking = msg.has(_gc2ch::ranking) ? msg.get(_gc2ch::ranking).String() : std::string("");
	ProtoWriter ranking(_ranking, 6);
	ranking.replace(Field(_pri::rank_id, TYPE_UINT32, (int64_t)g_Menu.Config.RankChanger.rank_id));
	ranking.replace(Field(_pri::wins, TYPE_UINT32, (int64_t)g_Menu.Config.RankChanger.wins));
	msg.replace(Field(_gc2ch::ranking, TYPE_STRING, ranking.serialize()));
	msg.replace(Field(_gc2ch::player_level, TYPE_INT32, (int64_t)g_Menu.Config.RankChanger.player_level));
	return msg.serialize();
}



void Protobuffs::WritePacket(std::string packet, void* thisPtr, void* oldEBP, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize)
{
	auto g_MemAlloc = g_pMemAlloc;
	if ((uint32_t)packet.size() <= cubDest - 8)
	{
		memcpy((void*)((DWORD)pubDest + 8), (void*)packet.data(), packet.size());
		*pcubMsgSize = packet.size() + 8;
	}
	else if (g_MemAlloc)
	{
		auto memPtr = *CAST(void**, thisPtr, 0x14);
		auto memPtrSize = *CAST(uint32_t*, thisPtr, 0x18);
		auto newSize = (memPtrSize - cubDest) + packet.size() + 8;

		auto memory = g_MemAlloc->Realloc(memPtr, newSize + 4);

		*CAST(void**, thisPtr, 0x14) = memory;
		*CAST(uint32_t*, thisPtr, 0x18) = newSize;
		*CAST(void**, oldEBP, -0x14) = memory;

		memcpy(CAST(void*, memory, 24), (void*)packet.data(), packet.size());

		*pcubMsgSize = packet.size() + 8;
	}
}

void Protobuffs::ReceiveMessage(void* thisPtr, void* oldEBP, uint32_t messageType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize)
{
	if (messageType == k_EMsgGCCStrike15_v2_MatchmakingGC2ClientHello && (g_Menu.Config.RankChanger.rank_id > 0))
	{
		auto packet = ProfileChangerr(pubDest, pcubMsgSize);
		WritePacket(packet, thisPtr, oldEBP, pubDest, cubDest, pcubMsgSize);
	}
}

bool Protobuffs::PreSendMessage(uint32_t& unMsgType, void* pubData, uint32_t& cubData)
{
	return true;
}

bool Protobuffs::SendClientHello()
{
	ProtoWriter msg(7);
	msg.add(Field(3, TYPE_UINT32, (int64_t)1));
	auto packet = msg.serialize();

	void* ptr = malloc(packet.size() + 8);

	if (!ptr)
		return false;

	((uint32_t*)ptr)[0] = k_EMsgGCClientHello | ((DWORD)1 << 31);
	((uint32_t*)ptr)[1] = 0;

	memcpy((void*)((DWORD)ptr + 8), (void*)packet.data(), packet.size());
	bool result = g_SteamGameCoordinator->GCSendMessage(k_EMsgGCClientHello | ((DWORD)1 << 31), ptr, packet.size() + 8) == k_EGCResultOK;
	free(ptr);

	return result;
}


bool Protobuffs::SendMatchmakingClient2GCHello()
{
	ProtoWriter msg(0);
	auto packet = msg.serialize();
	void* ptr = malloc(packet.size() + 8);

	if (!ptr)
		return false;

	((uint32_t*)ptr)[0] = k_EMsgGCCStrike15_v2_MatchmakingClient2GCHello | ((DWORD)1 << 31);
	((uint32_t*)ptr)[1] = 0;

	memcpy((void*)((DWORD)ptr + 8), (void*)packet.data(), packet.size());
	bool result = g_SteamGameCoordinator->GCSendMessage(k_EMsgGCCStrike15_v2_MatchmakingClient2GCHello | ((DWORD)1 << 31), ptr, packet.size() + 8) == k_EGCResultOK;
	free(ptr);

	return result;
}
#endif // DEBUG