#include "stdafx.h"
#include "char.h"
#include "utils.h"
#include "log.h"
#include "db.h"
#include "config.h"
#include "desc.h"
#include "desc_manager.h"
#include "buffer_manager.h"
#include "packet.h"
#include "desc_client.h"
#include "desc_p2p.h"
#include "char_manager.h"
#include "questmanager.h"
#include "start_position.h"
#include "sectree_manager.h"
#include "deathmatch.h"

// #define ENABLE_HOST_CONTROL // ip kontrolü

const int DEATHMATCH_MAP_INDEX = 356;
const int DEATHMATCH_MEMBER_LIMIT = 500;
const int DEATHMATCH_KILL_LIMIT = 500;
const int DEATHMATCH_BATTLE_EVENT = 15; //5 dakika
const int DEATHMATCH_CHECK_PLAYER = 300; //5 dakika
const int DEATHMATCH_DESTROY_EVENT = 180; //3 dakika

/*** Event Funcs ***/
EVENTINFO(deathmatch_info)
{
	DWORD map_index;

	deathmatch_info()
		: map_index(0)
	{
	}
};

EVENTFUNC(deathmatch_battle_event)
{
	deathmatch_info* info = dynamic_cast<deathmatch_info*>(event->info);

	if (info == nullptr)
	{
		sys_err("deathmatch_battle_event> <Factor> NULL pointer");
		return 0;
	}

	CDeathMatchPvP::instance().BattleEvent();
	return 0;
}

EVENTFUNC(deathmatch_check_event)
{
	deathmatch_info* info = dynamic_cast<deathmatch_info*>(event->info);

	if (info == nullptr)
	{
		sys_err("deathmatch_check_event> <Factor> NULL pointer");
		return 0;
	}

	// CDeathMatchPvP::instance().RemoveInactivePlayers();
	return PASSES_PER_SEC(DEATHMATCH_CHECK_PLAYER); // 5 dakika
}

EVENTFUNC(deathmatch_destroy_event)
{
	deathmatch_info* info = dynamic_cast<deathmatch_info*>(event->info);

	if (info == nullptr)
	{
		sys_err("deathmatch_destroy_event> <Factor> NULL pointer");
		return 0;
	}

	CDeathMatchPvP::instance().DelEvent();
	CDeathMatchPvP::instance().SendHome();
	CDeathMatchPvP::instance().ResetEvent();
	return 0;
}
/*** End Of Event Funcs ***/

void CDeathMatchPvP::OnLogin(LPCHARACTER ch)
{
	if (!ch || !ch->IsPC())
	{
		return;
	}

	if (ch->GetMapIndex() == DEATHMATCH_MAP_INDEX && !ch->IsGM())
	{
		switch (GetStatus())
		{
		case DEATHMATCH_CLOSED:
			ch->GoHome();
			ch->SetPKMode(PK_MODE_PROTECT);
			break;

		case DEATHMATCH_OPENED:
			ch->SetPKMode(PK_MODE_PROTECT);
			break;

		case DEATHMATCH_BATTLE:
			ch->SetPKMode(PK_MODE_DEATHMATCH);
			break;

		default:
			break;
		}
	}
}

/*** Unclassed Functions ***/
bool SortPlayers(const CDeathMatchPvP::PlayerData& i, const CDeathMatchPvP::PlayerData& j)
{
	if (i.iKill == j.iKill)
	{
		if (i.iDamage == j.iDamage)
		{
			return (i.iDead < j.iDead); // Daha az ölüme sahip olan öne geçer
		}
		return (i.iDamage > j.iDamage); // Daha fazla damage yapan öne geçer
	}
	return (i.iKill > j.iKill); // Daha fazla kill yapan öne geçer
}
/*** End Of Unclassed Functions ***/

/*** Team Data Begin ***/
int CDeathMatchPvP::SPlayerData::GetAccumulatedJoinerCount()
{
	return set_pidJoiner.size();
}

int CDeathMatchPvP::SPlayerData::GetCurJointerCount()
{
	return iMemberCount;
}

void CDeathMatchPvP::SPlayerData::AppendMember(LPCHARACTER ch)
{
	set_pidJoiner.insert(ch->GetPlayerID());
	++iMemberCount;
}

void CDeathMatchPvP::SPlayerData::RemoveMember(LPCHARACTER ch)
{
	set_pidJoiner.erase(ch->GetPlayerID());
	--iMemberCount;
}

void CDeathMatchPvP::SPlayerData::AppendDamage(int damage)
{
	iDamage += damage;
}

void CDeathMatchPvP::SPlayerData::AppendKill(int kill)
{
	iKill += kill;
}

void CDeathMatchPvP::SPlayerData::AppendDead(int dead)
{
	iDead += dead;
}

void CDeathMatchPvP::SPlayerData::Initialize()
{
	dwID = 0;
	strName.clear();
	iMemberCount = 0;
	iDamage = 0;
	iKill = 0;
	iDead = 0;
	set_pidJoiner.clear();
}
/*** Team Data End ***/

/*** Public Team Data Functions ***/
CDeathMatchPvP::PlayerData* CDeathMatchPvP::GetTable(DWORD dwID)
{
	PlayerDataMap::iterator itor = m_PlayerDataMap.find(dwID);

	if (itor == m_PlayerDataMap.end())
		return nullptr;

	return itor->second;
}

DWORD CDeathMatchPvP::GetPlayerDamage(DWORD dwID)
{
	CDeathMatchPvP::PlayerData* c_pTable = GetTable(dwID);
	if (!c_pTable)
		return 0;

	return c_pTable->iDamage;
}

DWORD CDeathMatchPvP::GetPlayerKill(DWORD dwID)
{
	CDeathMatchPvP::PlayerData* c_pTable = GetTable(dwID);
	if (!c_pTable)
		return 0;

	return c_pTable->iKill;
}

DWORD CDeathMatchPvP::GetPlayerDead(DWORD dwID)
{
	CDeathMatchPvP::PlayerData* c_pTable = GetTable(dwID);
	if (!c_pTable)
		return 0;

	return c_pTable->iDead;
}

DWORD CDeathMatchPvP::GetPlayerMemberCount(DWORD dwID)
{
	CDeathMatchPvP::PlayerData* c_pTable = GetTable(dwID);
	if (!c_pTable)
		return 0;

	return c_pTable->iMemberCount;
}
/*** End Of Public Team Data Functions ***/

/*** Public Append/Remove ***/
void CDeathMatchPvP::IncMember(LPCHARACTER ch)
{
	if (!ch || !ch->IsPC())
	{
		return;
	}

	PlayerData* pCheck = GetTable(ch->GetPlayerID());
	if (!pCheck)
	{
		AppendPlayer(ch);
	}

	PlayerData* pPlayer = GetTable(ch->GetPlayerID());

	if (pPlayer)
	{
		pPlayer->AppendMember(ch);
		// sys_log(0, "CDeathMatchPvP:: DeathMatch Append Member: %s %d", ch->GetName(), ch->GetPlayerID());
	}

	m_set_pkChr.insert(ch);
}

void CDeathMatchPvP::DecMember(LPCHARACTER ch)
{
	if (!ch || !ch->IsPC())
	{
		return;
	}

	PlayerData* pPlayer = GetTable(ch->GetPlayerID());

	if (pPlayer)
	{
		pPlayer->RemoveMember(ch);
		// sys_log(0, "CDeathMatchPvP:: DeathMatch Remove Member: %s %d", ch->GetName(), ch->GetPlayerID());
	}

	m_set_pkChr.erase(ch);
}

void CDeathMatchPvP::AppendPlayer(LPCHARACTER ch)
{
	if (!ch || !ch->IsPC())
	{
		return;
	}

	auto [iter, inserted] = m_PlayerDataMap.insert({ ch->GetPlayerID(), new PlayerData() });
	if (!inserted)
		return;

	PlayerData* pPlayerData = iter->second;
	pPlayerData->dwID = ch->GetPlayerID();
	pPlayerData->strName = ch->GetName();
	pPlayerData->iMemberCount = 0;
	pPlayerData->iKill = 0;
	pPlayerData->iDead = 0;
	pPlayerData->iDamage = 0;
}

void CDeathMatchPvP::OnDamage(LPCHARACTER attacker, LPCHARACTER victim, int damage)
{
	if (!attacker || !victim || !attacker->IsPC() || !victim->IsPC())
	{
		return;
	}

#ifdef ENABLE_HOST_CONTROL
	int attacker_ip = inet_addr(attacker->GetDesc()->GetHostName());
	int victim_ip = inet_addr(victim->GetDesc()->GetHostName());
#endif

	PlayerData* pPlayer = GetTable(attacker->GetPlayerID());

	if (pPlayer)
	{
#ifdef ENABLE_HOST_CONTROL
		if (attacker_ip != victim_ip)
		{
#endif
			pPlayer->AppendDamage(damage);
			// sys_log(0, "CDeathMatchPvP:: OnDamage: PlayerID=%d, Damage=%d", attacker->GetPlayerID(), damage);
#ifdef ENABLE_HOST_CONTROL
		}
		else
		{
			// attacker->ChatPacket(CHAT_TYPE_INFO, "Ayný IP'den puan kazanamazsýnýz.");
		}
#endif
	}

	SendScorePacket();
}

void CDeathMatchPvP::OnKill(LPCHARACTER killer, LPCHARACTER victim)
{
	if (!killer || !victim || !killer->IsPC() || !victim->IsPC())
	{
		return;
	}

	PlayerData* pKiller = GetTable(killer->GetPlayerID());
	PlayerData* pVictim = GetTable(victim->GetPlayerID());

	if (!pKiller)
	{
		AppendPlayer(killer);
		pKiller = GetTable(killer->GetPlayerID());
	}

	if (!pVictim)
	{
		AppendPlayer(victim);
		pVictim = GetTable(victim->GetPlayerID());
	}

#ifdef ENABLE_HOST_CONTROL
	int killer_ip = inet_addr(killer->GetDesc()->GetHostName());
	int victim_ip = inet_addr(victim->GetDesc()->GetHostName());
#endif

	if (pKiller)
	{
#ifdef ENABLE_HOST_CONTROL
		if (killer_ip != victim_ip)
		{
#endif
			pKiller->AppendKill(1);
			if (pKiller->iKill >= GetKillCountLimit())
			{
				SetStatus(DEATHMATCH_CLOSED, m_iDeathMatchMapIndex);
			}
			// sys_log(0, "CDeathMatchPvP:: OnKill: KillerID=%d, VictimID=%d, KillCount=%d", killer->GetPlayerID(), victim->GetPlayerID(), pKiller->iKill);
#ifdef ENABLE_HOST_CONTROL
		}
		else
		{
			// killer->ChatPacket(CHAT_TYPE_INFO, "Ayný IP'den puan kazanamazsýnýz.");
		}
#endif
	}

	if (pVictim)
	{
		pVictim->AppendDead(1);
		// sys_log(0, "CDeathMatchPvP:: OnKill: VictimID=%d, DeadCount=%d", victim->GetPlayerID(), pVictim->iDead);
	}

	SendScorePacket();
}
/*** End Of Public Append Remove ***/

/*** Callable Funcs ***/
std::vector<DWORD> CDeathMatchPvP::GetWinnerPlayer()
{
	std::vector<CDeathMatchPvP::PlayerData> playerDataList;

	// Mevcut oyuncu verilerini topla
	for (PlayerDataMap::iterator itor = m_PlayerDataMap.begin(); itor != m_PlayerDataMap.end(); ++itor)
	{
		// Ölü, öldürme ve hasar deðerleri sýfýr olan oyuncular eklenmeyecek
		if (itor->second->iKill != 0 || itor->second->iDamage != 0 || itor->second->iDead != 0)
		{
			playerDataList.push_back(*itor->second);
		}
	}

	// Eðer oyuncu listesi boþsa, log mesajý yazdýr ve boþ bir vektör döndür
	if (playerDataList.empty()) {
		sys_log(0, "GetWinnerPlayer: No player data available.");
		return std::vector<DWORD>();
	}

	// Oyuncularý sýralama
	std::stable_sort(playerDataList.begin(), playerDataList.end(), SortPlayers);

	// Ýlk üç oyuncunun ID'lerini al
	std::vector<DWORD> topPlayers;
	for (size_t i = 0; i < playerDataList.size() && i < 3; ++i)
	{
		topPlayers.push_back(playerDataList[i].dwID);
	}

	return topPlayers;
}
/*** End Of Callable Funcs ***/

/*** Status And Map ***/
void CDeathMatchPvP::SetStatus(int iStatus, int iMap)
{
	// Set map index and activation status
	m_iDeathMatchMapIndex = iMap;
	m_iActivateStatus = iStatus;

	switch (iStatus)
	{
	case DEATHMATCH_OPENED:
	{
		CleanKing();
		StartEvent();
		sys_log(0, "CDeathMatchPvP:: DeathMatch has started.");
		break;
	}

	case DEATHMATCH_BATTLE:
	{
		for (auto ch : m_set_pkChr)
		{
			if (ch)
			{
				ch->SetPKMode(PK_MODE_DEATHMATCH);
				sys_log(0, "Player %s (ID: %d) PK mode set to DEATHMATCH.", ch->GetName(), ch->GetPlayerID());
			}
		}

		sys_log(0, "CDeathMatchPvP:: DeathMatch has battle.");
		break;
	}

	case DEATHMATCH_CLOSED:
	{
		Reward();
		EndEvent();
		sys_log(0, "CDeathMatchPvP:: DeathMatch has closed.");
		break;
	}

	default:
	{
		sys_log(0, "CDeathMatchPvP:: Unknown status: %d", iStatus);
		break;
	}
	}
}

int CDeathMatchPvP::GetStatus()
{
	return m_iActivateStatus;
}

bool CDeathMatchPvP::IsDeathMatchMap(int mapindex)
{
	return m_iDeathMatchMapIndex == mapindex ? true : false;
}

bool CDeathMatchPvP::IsDeathMatchActivate()
{
	return m_iActivateStatus == DEATHMATCH_OPENED ? true : false;
}

bool CDeathMatchPvP::IsDeathMatchBattle()
{
	return m_iActivateStatus == DEATHMATCH_BATTLE ? true : false;
}
/*** End Of Status And Map ***/

/*** Member Limit Count ***/
void CDeathMatchPvP::SetMemberLimitCount(int memberlimit)
{
	m_iMemberLimitCount = memberlimit;
}

int CDeathMatchPvP::GetMemberLimitCount()
{
	return m_iMemberLimitCount;
}
/*** End Of Member Limit Count ***/

/*** Kill Limit Count ***/
void CDeathMatchPvP::SetKillCountLimit(int limit)
{
	m_iLimit = limit;
}

int CDeathMatchPvP::GetKillCountLimit()
{
	return m_iLimit;
}
/*** End Of Kill Limit Count ***/

/*** Packets Begin ***/
namespace
{
	struct FPacket
	{
		FPacket(const void* p, int size) : m_pvData(p), m_iSize(size)
		{
		}

		void operator () (LPCHARACTER ch)
		{
			ch->GetDesc()->Packet(m_pvData, m_iSize);
		}

		const void* m_pvData;
		int m_iSize;
	};

	struct FNotice
	{
		FNotice(const char* psz) : m_psz(psz)
		{
		}

		void operator() (LPCHARACTER ch)
		{
			ch->ChatPacket(CHAT_TYPE_NOTICE, "%s", m_psz);
		}

		const char* m_psz;
	};

#ifdef ENABLE_WAR_KILL_NOTICE
	struct FSendDeathMatchKillNotice
	{
		char buf[512];

		FSendDeathMatchKillNotice(const char* c_szKillerName, const char* c_szVictimName, DWORD dwKillerRace, DWORD dwVictimRace) {
			snprintf(buf, sizeof(buf), "GW_Kill_Update %s %u %s %u", c_szKillerName, dwKillerRace, c_szVictimName, dwVictimRace);
		}

		void operator() (LPCHARACTER ch) {
			ch->ChatPacket(CHAT_TYPE_COMMAND, buf);
		}
	};
#endif

	struct FGoToVillage
	{
		void operator() (LPCHARACTER ch)
		{
			ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()));
		}
	};
};

void CDeathMatchPvP::Notice(const char* psz)
{
	FNotice f(psz);
	std::for_each(m_set_pkChr.begin(), m_set_pkChr.end(), f);
}

#ifdef ENABLE_WAR_KILL_NOTICE
void CDeathMatchPvP::SendKillNotice(const char* c_szKillerName, const char* c_szVictimName, DWORD dwKillerRace, DWORD dwVictimRace)
{
	if (!IsDeathMatchBattle())
		return;

	FSendDeathMatchKillNotice f(c_szKillerName, c_szVictimName, dwKillerRace, dwVictimRace);
	std::for_each(m_set_pkChr.begin(), m_set_pkChr.end(), f);
}
#endif

void CDeathMatchPvP::Packet(const void* p, int size)
{
	FPacket f(p, size);
	std::for_each(m_set_pkChr.begin(), m_set_pkChr.end(), f);
}

void CDeathMatchPvP::SendHome()
{
	FGoToVillage f;
	std::for_each(m_set_pkChr.begin(), m_set_pkChr.end(), f);
}

void CDeathMatchPvP::SendScorePacket()
{
	if (!IsDeathMatchBattle())
		return;

	std::vector<CDeathMatchPvP::PlayerData> sendLists;
	for (PlayerDataMap::iterator itor = m_PlayerDataMap.begin(); itor != m_PlayerDataMap.end(); ++itor)
		sendLists.push_back(*itor->second);

	std::stable_sort(sendLists.begin(), sendLists.end(), SortPlayers);

	std::vector<CDeathMatchPvP::PlayerData>::iterator ch;
	int list = 0;
	for (ch = sendLists.begin(); ch != sendLists.end(); ++ch)
	{
		DWORD dwID = ch->dwID;

		TPacketGCDeathMatch p;
		p.header = HEADER_GC_DEATHMATCH;
		strlcpy(p.szName, ch->strName.c_str(), sizeof(p.szName));
		p.dwKill = GetPlayerKill(dwID);
		p.dwDead = GetPlayerDead(dwID);
		p.dwDamage = GetPlayerDamage(dwID);
		p.dwLimit = GetMemberLimitCount();
		p.dwKillLimit = GetKillCountLimit();
		p.iList = list++;

		Packet(&p, sizeof(p));
	}
}
/*** End Of Packets Begin ***/

/*void CDeathMatchPvP::RemoveInactivePlayers()
{
	for (auto it = m_PlayerDataMap.begin(); it != m_PlayerDataMap.end();)
	{
		PlayerData* pData = it->second;

		if (pData->iDamage == 0 && pData->iKill == 0 && pData->iDead == 0)
		{
			// Oyuncuyu eve gönder
			for (auto ch : m_set_pkChr)
			{
				if (ch && ch->GetPlayerID() == pData->dwID)
				{
					ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()));
					ch->SetPKMode(PK_MODE_PROTECT);
					break;
				}
			}

			delete pData;
			it = m_PlayerDataMap.erase(it);
		}
		else
		{
			++it;
		}
	}
}
*/
void CDeathMatchPvP::StartEvent()
{
	BroadcastNotice("DeathMatch girisleri acildi ! Girisler 5 dakika sonra kapatilacak   !");

	for (auto ch : m_set_pkChr)
	{
		if (ch)
		{
			ch->SetPKMode(PK_MODE_PROTECT);
			sys_log(0, "Player %s (ID: %d) PK mode set to PROTECT.", ch->GetName(), ch->GetPlayerID());
		}
	}

	deathmatch_info* info = AllocEventInfo<deathmatch_info>();
	info->map_index = m_iDeathMatchMapIndex;
	battleEvent = event_create(deathmatch_battle_event, info, PASSES_PER_SEC(DEATHMATCH_BATTLE_EVENT));
}

void CDeathMatchPvP::BattleEvent()
{
	SetStatus(DEATHMATCH_BATTLE, m_iDeathMatchMapIndex);

	BroadcastNotice("DeathMatch girisleri kapatildi ! Savaþ baþlasýn!");

	deathmatch_info* info = AllocEventInfo<deathmatch_info>();
	info->map_index = m_iDeathMatchMapIndex;
	checkEvent = event_create(deathmatch_check_event, info, PASSES_PER_SEC(DEATHMATCH_CHECK_PLAYER));
}

void CDeathMatchPvP::EndEvent()
{
	for (auto ch : m_set_pkChr)
	{
		if (ch)
		{
			ch->SetPKMode(PK_MODE_PROTECT);
			sys_log(0, "Player %s (ID: %d) PK mode set to PROTECT.", ch->GetName(), ch->GetPlayerID());
		}
	}

	deathmatch_info* info = AllocEventInfo<deathmatch_info>();
	info->map_index = m_iDeathMatchMapIndex;
	destroyEvent = event_create(deathmatch_destroy_event, info, PASSES_PER_SEC(DEATHMATCH_DESTROY_EVENT));
}

void CDeathMatchPvP::CleanKing()
{
	auto pMsg = DBManager::Instance().DirectQuery("SELECT first, second, third, id FROM player.player");
	if (pMsg && pMsg->Get()->uiNumRows != 0)
	{
		MYSQL_ROW row;
		while ((row = mysql_fetch_row(pMsg->Get()->pSQLResult)) != nullptr)
		{
			int first = 0;
			int second = 0;
			int third = 0;
			int dwPlayerID = 0;

			str_to_number(first, row[0]);
			str_to_number(second, row[1]);
			str_to_number(third, row[2]);
			str_to_number(dwPlayerID, row[3]);

			if (first != 0 || second != 0 || third != 0)
			{
				auto ch = CHARACTER_MANAGER::instance().FindByPID(dwPlayerID);
				if (ch)
				{
					ch->RequestKingPlayer(false, false, false, false);
				}
			}
		}
	}
}

void CDeathMatchPvP::Reward()
{
	std::vector<DWORD> topPlayers = GetWinnerPlayer();

	/*
		// Kazanan oyuncu sayýsý 3'ten azsa, ödül verilmez
		if (topPlayers.size() < 3)
		{
			return;
		}
	*/
	// Ýlk üç oyuncunun ID'lerini al
	DWORD firstPlaceID = topPlayers[0];
	DWORD secondPlaceID = topPlayers[1];
	DWORD thirdPlaceID = topPlayers[2];

	// Ýlk oyuncuya ödül ver
	LPCHARACTER firstPlaceCh = CHARACTER_MANAGER::instance().FindByPID(firstPlaceID);
	if (firstPlaceCh)
	{
		sys_log(0, "Found First Place Player: %s", firstPlaceCh->GetName());
		char szNotice[513];
		snprintf(szNotice, sizeof(szNotice), LC_TEXT("Winner: %s"), firstPlaceCh->GetName());
		BroadcastNotice(szNotice);
		firstPlaceCh->RequestKingPlayer(false, true, false, false);
	}

	// Ýkinci oyuncuya ödül ver
	LPCHARACTER secondPlaceCh = CHARACTER_MANAGER::instance().FindByPID(secondPlaceID);
	if (secondPlaceCh)
	{
		sys_log(0, "Found Second Place Player: %s", secondPlaceCh->GetName());
		char szNotice[513];
		snprintf(szNotice, sizeof(szNotice), LC_TEXT("Second Place: %s"), secondPlaceCh->GetName());
		BroadcastNotice(szNotice);
		secondPlaceCh->RequestKingPlayer(false, false, true, false);
	}

	// Üçüncü oyuncuya ödül ver
	LPCHARACTER thirdPlaceCh = CHARACTER_MANAGER::instance().FindByPID(thirdPlaceID);
	if (thirdPlaceCh)
	{
		sys_log(0, "Found Third Place Player: %s", thirdPlaceCh->GetName());
		char szNotice[513];
		snprintf(szNotice, sizeof(szNotice), LC_TEXT("Third Place: %s"), thirdPlaceCh->GetName());
		BroadcastNotice(szNotice);
		thirdPlaceCh->RequestKingPlayer(false, false, false, true);
	}
}

void CDeathMatchPvP::ClearPlayerData()
{
	for (PlayerDataMap::iterator itor = m_PlayerDataMap.begin(); itor != m_PlayerDataMap.end(); ++itor)
	{
		delete itor->second;
	}
	m_PlayerDataMap.clear();
}

void CDeathMatchPvP::DelEvent()
{
	if (battleEvent)
		event_cancel(&battleEvent);
	if (checkEvent)
		event_cancel(&checkEvent);
	if (destroyEvent)
		event_cancel(&destroyEvent);
	battleEvent = nullptr;
	checkEvent = nullptr;
	destroyEvent = nullptr;
}

void CDeathMatchPvP::ResetEvent()
{
	ClearPlayerData();
	m_iDeathMatchMapIndex = 0;
	m_iActivateStatus = 0;
	m_iMemberLimitCount = DEATHMATCH_MEMBER_LIMIT;
	m_iLimit = DEATHMATCH_KILL_LIMIT;
	CDeathMatchPvP::SendHome();
	DelEvent();
	m_set_pkChr.clear();
}

void CDeathMatchPvP::Destroy()
{
	ClearPlayerData();
	m_iDeathMatchMapIndex = 0;
	m_iActivateStatus = 0;
	m_iMemberLimitCount = DEATHMATCH_MEMBER_LIMIT;
	m_iLimit = DEATHMATCH_KILL_LIMIT;
	CDeathMatchPvP::SendHome();
	DelEvent();
	m_set_pkChr.clear();
}

CDeathMatchPvP::CDeathMatchPvP()
{
	ResetEvent();
}

CDeathMatchPvP::~CDeathMatchPvP()
{
	Destroy();
}