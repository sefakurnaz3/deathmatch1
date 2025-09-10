//search
class CBuffOnAttributes;
class CPetSystem;

//add after
#ifdef ENABLE_IKASHOP_RENEWAL
namespace ikashop
{
	class CShop;
	class CShopSafebox;
	class CAuction;
}
#endif

//search
};

ESex GET_SEX(LPCHARACTER ch);

//add before @@@@ };
#ifdef ENABLE_IKASHOP_RENEWAL
	private:
		using ShopActionWeight = uint16_t;
		using ShopActionTimestamp = double;
		using ShopActionData = std::pair<ShopActionWeight, ShopActionTimestamp>;
		using ShopActionRegister = std::deque<ShopActionData>;
	public:
		using SHOP_HANDLE = std::shared_ptr<ikashop::CShop>;
		using SAFEBOX_HANDLE = std::shared_ptr<ikashop::CShopSafebox>;
		using AUCTION_HANDLE = std::shared_ptr<ikashop::CAuction>;
		SHOP_HANDLE GetIkarusShop() {return m_pkIkarusShop;}
		void SetIkarusShop(SHOP_HANDLE pkShop) {m_pkIkarusShop = pkShop;}
		SHOP_HANDLE	GetIkarusShopGuest() const {return m_pkIkarusShopGuest;}
		void SetIkarusShopGuest(SHOP_HANDLE pkShop);
		SAFEBOX_HANDLE GetIkarusShopSafebox() {return m_pkIkarusShopSafebox;}
		void SetIkarusShopSafebox(SAFEBOX_HANDLE pk);
		void SetIkarusAuction(AUCTION_HANDLE pk) {m_pkIkarusAuction = pk;}
		void SetIkarusAuctionGuest(AUCTION_HANDLE pk);
		AUCTION_HANDLE GetIkarusAuction() {return m_pkIkarusAuction;}
		AUCTION_HANDLE GetIkarusAuctionGuest() const {return m_pkIkarusAuctionGuest;}
		int GetIkarusShopUseTime() const { return m_iIkarusShopUseTime; }
		void SetIkarusShopUseTime() { m_iIkarusShopUseTime = thecore_pulse(); }
		void SetLookingIkarusShopOfferList(bool state) { m_bIsLookingIkarusShopOfferList = state; }
		bool IsLookingIkarusShopOfferList() const { return m_bIsLookingIkarusShopOfferList; }
		void SetLookingShopOwner(bool state) { m_bIsLookingShopOwner  = state;}
		bool IsLookingShopOwner() const { return m_bIsLookingShopOwner; }
		void SetLookingAuctionOwner(bool state) { m_bIsLookingAuctionOwner = state;}
		bool IsLookingAuctionOwner() const { return m_bIsLookingAuctionOwner; }
#ifdef EXTEND_IKASHOP_PRO
		void SetLookingNotificationList(bool state) { m_bIsLookingNotificationList = state; }
		bool IsLookingNotificationList() const { return m_bIsLookingNotificationList; }
#endif
		bool CanTakeInventoryItem(LPITEM item, TItemPos* pos);
		bool IkarusShopFloodCheck(ShopActionWeight weight);

	private:
		ShopActionRegister m_IkarusShopActionRegister;
		SHOP_HANDLE	m_pkIkarusShop;
		SHOP_HANDLE	m_pkIkarusShopGuest;
		SAFEBOX_HANDLE m_pkIkarusShopSafebox;
		AUCTION_HANDLE m_pkIkarusAuction;
		AUCTION_HANDLE m_pkIkarusAuctionGuest;

		int m_iIkarusShopUseTime = 0;
		bool m_bIsLookingIkarusShopOfferList = false;
		bool m_bIsLookingShopOwner = false;
		bool m_bIsLookingAuctionOwner = false;
#ifdef EXTEND_IKASHOP_PRO
		bool m_bIsLookingNotificationList = false;
#endif
#endif