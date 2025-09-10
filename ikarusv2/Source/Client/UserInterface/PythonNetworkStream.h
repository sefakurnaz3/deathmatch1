//search
		void __BettingGuildWar_Initialize();
		void __BettingGuildWar_SetObserverCount(UINT uObserverCount);
		void __BettingGuildWar_SetBettingMoney(UINT uBettingMoney);


//add after
#ifdef ENABLE_IKASHOP_RENEWAL
	public:
		bool RecvOfflineshopPacket();
		bool RecvOfflineshopShopOpen();
		bool RecvOfflineshopShopOpenOwner();
		bool RecvOfflineshopShopOpenOwnerNoShop();
		bool RecvOfflineshopShopExpiredGuesting();
		bool RecvOfflineshopShopRemoveItem(bool owner);
		bool RecvOfflineshopShopGuestEditItem();
		bool RecvOfflineshopShopOwnerEditItem();
		bool RecvOfflineshopShopFilterResult();
		bool RecvOfflineshopOfferList();
		bool RecvOfflineshopShopSafeboxRefresh();
		bool RecvOfflineshopShopSafeboxRemoveItem();
		bool RecvOfflineshopShopSafeboxAddItem();
		bool RecvOfflineshopAuctionList();
		bool RecvOfflineshopOpenMyAuction();
		bool RecvOfflineshopOpenMyAuctionNoAuction();
		bool RecvOfflineshopOpenAuction();
		bool RecvOfflineshopPopupMessage();
		bool RecvOfflineshopBoardCounters();
#ifdef EXTEND_IKASHOP_PRO
		bool RecvOfflineshopNotificationList();
#endif
#ifdef EXTEND_IKASHOP_ULTIMATE
		bool RecvOfflineshopPriceAverageResponse();
#endif
#ifdef ENABLE_IKASHOP_ENTITIES
		bool RecvOfflineshopInsertEntity();
		bool RecvOfflineshopRemoveEntity();
		void SendIkarusShopOnClickShopEntity(DWORD dwPickedShopVID);
#ifdef EXTEND_IKASHOP_PRO
		void SendIkarusShopMoveShopEntity();
#endif
		bool RecvOfflineshopSearchResultDelete();
#endif
		void SendIkarusShopCreateShop();
		void SendIkarusShopForceCloseShop();
		void SendIkarusShopRequestShopList();
		void SendIkarusShopOpenShop(DWORD dwOwnerID);
		void SendIkarusShopOpenShopOwner();
		void SendIkarusShopBuyItem(DWORD dwOwnerID, DWORD dwItemID, bool isSearch, long long TotalPriceSeen);
		void SendIkarusShopAddItem(ikashop::TShopItemInfo& itemInfo);
		void SendIkarusShopRemoveItem(DWORD dwItemID);
		void SendOfflineShopEditItem(DWORD dwItemID, const ikashop::TPriceInfo& price);
		void SendIkarusShopFilterRequest(const ikashop::TFilterInfo& filter);
		void SendIkarusShopSearchFillRequest();
		void SendIkarusShopOfferCreate(const ikashop::TOfferInfo& offer);
		void SendIkarusShopOfferAccept(DWORD dwOfferID);
		void SendIkarusShopOfferCancel(DWORD dwOfferID, DWORD dwOwnerID);
		void SendIkarusShopOfferListRequest();
		void SendIkarusShopSafeboxOpen();
		void SendIkarusShopSafeboxGetItem(DWORD dwItemID);
		void SendIkarusShopSafeboxGetValutes();
		void SendIkarusShopSafeboxClose();
		void SendIkarusShopAuctionListRequest();
		void SendIkarusShopAuctionOpen(DWORD dwOwnerID);
		void SendIkarusShopAuctionAddOffer(DWORD dwOwnerID, const ikashop::TPriceInfo& price);
		void SendIkarusShopAuctionExitFrom(DWORD dwOwnerID);
		void SendIkarusShopAuctionCreate(const TItemPos& pos, const ikashop::TPriceInfo& price);
		void SendIkarusShopMyAuctionOpen();
		void SendIkarusShopMyAuctionClose();
		void SendIkarusShopMyAuctionCancel();
		void SendIkarusMyShopCloseBoard();
		void SendIkarusOffersListClose();
		void SendIkarusCloseShopGuestBoard();
#ifdef EXTEND_IKASHOP_PRO
		void SendIkarusNotificationListRequest();
		void SendIkarusNotificationListClose();
#endif
#ifdef EXTEND_IKASHOP_ULTIMATE
		void SendPriceAverageRequest(DWORD vnum, DWORD count);
		void SendShopDecorationUsage(int index);
		void SendShopMoveItem(int srcPos, int destPos);
#endif
#endif