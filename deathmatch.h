class CDeathMatchPvP : public singleton<CDeathMatchPvP>
{
public:
	typedef struct SPlayerData
	{
		DWORD	dwID;
		int		iMemberCount;
		std::string strName;
		DWORD		iKill;
		DWORD		iDead;
		DWORD		iDamage;
		std::set<DWORD> set_pidJoiner;

		void Initialize();

		int GetAccumulatedJoinerCount();
		int GetCurJointerCount();

		void AppendMember(LPCHARACTER ch);
		void RemoveMember(LPCHARACTER ch);
		void AppendDamage(int damage);
		void AppendKill(int kill);
		void AppendDead(int dead);
	} PlayerData;

	enum EDeathMatchStatus
	{
		DEATHMATCH_CLOSED = 0,
		DEATHMATCH_OPENED = 1,
		DEATHMATCH_BATTLE = 2,
	};

	typedef std::map<DWORD, PlayerData*> PlayerDataMap;
public:
	CDeathMatchPvP(void);
	virtual ~CDeathMatchPvP(void);

	PlayerData* GetTable(DWORD dwID);

	DWORD GetPlayerDamage(DWORD dwID);
	DWORD GetPlayerKill(DWORD dwID);
	DWORD GetPlayerDead(DWORD dwID);
	DWORD GetPlayerMemberCount(DWORD dwID);

	void OnLogin(LPCHARACTER ch);
	void IncMember(LPCHARACTER ch);
	void DecMember(LPCHARACTER ch);

	void AppendPlayer(LPCHARACTER ch);
	void OnDamage(LPCHARACTER attacker, LPCHARACTER victim, int damage);
	void OnKill(LPCHARACTER killer, LPCHARACTER victim);

	void SendScorePacket();
	// void RemoveInactivePlayers();
	void StartEvent();
	void BattleEvent();
	void EndEvent();
	void CleanKing();
	void Reward();

	void SetStatus(int iStatus, int iMap);
	int GetStatus();
	bool IsDeathMatchMap(int mapindex);
	bool IsDeathMatchActivate();
	bool IsDeathMatchBattle();

	void SetMemberLimitCount(int memberlimit);
	int GetMemberLimitCount();
	void SetKillCountLimit(int limit);
	int GetKillCountLimit();

	int GetTotalMemberCount() { return m_set_pkChr.size(); }
	std::vector<DWORD> GetWinnerPlayer();

	void Packet(const void* pv, int size);
	void Notice(const char* psz);
	void SendKillNotice(const char* c_szKillerName, const char* c_szVictimName, DWORD dwKillerRace, DWORD dwVictimRace);
	void SendHome();

	void ClearPlayerData();
	void DelEvent();
	void ResetEvent();
	void Destroy();

protected:
	PlayerDataMap	m_PlayerDataMap;
	int m_iDeathMatchMapIndex;
	int m_iMemberLimitCount;
	int m_iLimit;
	int m_iActivateStatus;
	LPEVENT battleEvent;
	LPEVENT checkEvent;
	LPEVENT destroyEvent;
private:
	CHARACTER_SET m_set_pkChr;
};