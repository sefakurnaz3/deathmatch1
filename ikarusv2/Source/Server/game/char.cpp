//add where you want
#ifdef ENABLE_IKASHOP_RENEWAL
#include <numeric>
#include <chrono>
#include "ikarus_shop.h"
#include "ikarus_shop_manager.h"
#endif

//search
	m_dwMountVnum = 0;
	m_chHorse = NULL;
	m_chRider = NULL;

//add after
#ifdef ENABLE_IKASHOP_RENEWAL
	m_pkIkarusShop = NULL;
	m_pkIkarusShopSafebox	= NULL;
	m_pkIkarusAuction		= NULL;
	m_pkIkarusAuctionGuest= NULL;
	m_pkIkarusShopGuest = NULL;
	m_bIsLookingIkarusShopOfferList = false;
#endif

//search in function @@@ void CHARACTER::Destroy()
	if (GetSectree())
		GetSectree()->RemoveEntity(this);

	if (m_bMonsterLog)
		CHARACTER_MANAGER::instance().UnregisterForMonsterLog(this);

//add after
#ifdef ENABLE_IKASHOP_RENEWAL
	SetIkarusShop(nullptr);
	SetIkarusShopGuest(nullptr);
	SetIkarusShopSafebox(nullptr);
	SetIkarusAuction(nullptr);
	SetIkarusAuctionGuest(nullptr);
#endif

//search in function @@@ bool CHARACTER::CanWarp() const
	if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen())
		return false;

//add after
#ifdef ENABLE_IKASHOP_RENEWAL
	if (iPulse - GetIkarusShopUseTime() < limit_time)
		return false;

	if (GetIkarusShopGuest() || GetIkarusAuctionGuest())
		return false;
#endif


//if you have acce system(sash) if not skip

//search
BYTE CHARACTER::CanRefineSashMaterials
//or
BYTE CHARACTER::CanRefineAcceMaterials
{
	BYTE bReturn = 0;
	if (!GetDesc())
		return bReturn;

	if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen())
		return bReturn;

//after add
#ifdef ENABLE_IKASHOP_RENEWAL
	if (GetIkarusShopGuest() || GetIkarusAuctionGuest())
		return bReturn;
#endif

//add at the end of file
#ifdef ENABLE_IKASHOP_RENEWAL
void CHARACTER::SetIkarusShopSafebox(SAFEBOX_HANDLE pk)
{
	if(m_pkIkarusShopSafebox && pk == nullptr)
		m_pkIkarusShopSafebox->SetOwner(nullptr);

	else if(m_pkIkarusShopSafebox == nullptr && pk)
		pk->SetOwner(this);

	m_pkIkarusShopSafebox  = pk;
}

void CHARACTER::SetIkarusShopGuest(SHOP_HANDLE pkShop)
{
	if(m_pkIkarusShopGuest)
		m_pkIkarusShopGuest->RemoveGuest(this);
	m_pkIkarusShopGuest = pkShop;
	if(pkShop)
		pkShop->AddGuest(this);
}

void CHARACTER::SetIkarusAuctionGuest(AUCTION_HANDLE pk)
{
	if(m_pkIkarusAuctionGuest)
		m_pkIkarusAuctionGuest->RemoveGuest(this);
	m_pkIkarusAuctionGuest = pk;
	if(pk)
		pk->AddGuest(this);
}

bool CHARACTER::CanTakeInventoryItem(LPITEM item, TItemPos* pos)
{
	int emptyPos = -1;

	if (item->IsDragonSoul())
	{
		pos->cell = emptyPos = GetEmptyDragonSoulInventory(item);
		pos->window_type = DRAGON_SOUL_INVENTORY;
	}
	else
	{
		pos->cell = emptyPos = GetEmptyInventory(item->GetSize());
		pos->window_type = INVENTORY;
	}

	return emptyPos != -1;
}

bool CHARACTER::IkarusShopFloodCheck(ShopActionWeight weight)
{
	using namespace std::chrono;
	const auto now = duration<double>(high_resolution_clock::now().time_since_epoch()).count();
	const auto elapsedTime = now - 60.0;

	std::erase_if(m_IkarusShopActionRegister, [&](auto& pair){ return pair.second < elapsedTime; });
	const auto totalWeight = std::accumulate(m_IkarusShopActionRegister.begin(), m_IkarusShopActionRegister.end(), 0UL,
		[](auto total, auto& pair){ return total + pair.first; });

	if(totalWeight + weight > OFFLINESHOP_MAX_WEIGHT_PER_PLAYER){
		ikashop::GetManager().SendPopupMessage(this, "IKASHOP_SERVER_POPUP_MESSAGE_PLEASE_SLOW_DOWN");
		return false;
	}

	m_IkarusShopActionRegister.emplace_back(std::make_pair(weight, now));
	return true;
}
#endif
