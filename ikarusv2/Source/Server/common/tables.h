//search
	HEADER_GD_REQUEST_CHANNELSTATUS	= 140,

//after add 
#ifdef ENABLE_IKASHOP_RENEWAL
	HEADER_GD_NEW_OFFLINESHOP		= 153,
#endif

//search
	HEADER_DG_RESPOND_CHANNELSTATUS	= 181,

//after add
#ifdef ENABLE_IKASHOP_RENEWAL
	HEADER_DG_NEW_OFFLINESHOP		= 182,
#endif

//add at the end of file
#ifdef ENABLE_IKASHOP_RENEWAL
//common
struct TPacketGDNewIkarusShop 
{
	BYTE bSubHeader;
};

struct TPacketDGNewIkarusShop 
{
	BYTE bSubHeader;
};


namespace ikashop
{
	//patch 08-03-2020
	enum class ExpirationType {
		EXPIRE_NONE,
		EXPIRE_REAL_TIME,
		EXPIRE_REAL_TIME_FIRST_USE,
	};

	struct TPriceInfo
	{
		long long yang = 0;
#ifdef ENABLE_CHEQUE_SYSTEM
		int cheque = 0;
#endif
		bool operator < (const TPriceInfo& rItem) const
		{
			return GetTotalYangAmount() < rItem.GetTotalYangAmount();
		}

		long long GetTotalYangAmount() const{
			long long total = yang;
#ifdef ENABLE_CHEQUE_SYSTEM
			total += (long long) YANG_PER_CHEQUE * (long long) cheque;
#endif
			return total;
		}
	};

	struct TShopPlayerItem : public TPlayerItem
	{
		ExpirationType expiration;
	};

	struct TShopItem : public TShopPlayerItem
	{
		TPriceInfo price;
	};

#ifdef ENABLE_IKASHOP_ENTITIES
	struct TShopSpawn
	{
		int x;
		int y;
		int map;
	};
#endif

	struct TOfferInfo 
	{
		DWORD offerid;
		DWORD ownerid;
		DWORD itemid;
		DWORD buyerid;
		TPriceInfo price;
		bool notified;
		bool accepted;
		char buyername[CHARACTER_NAME_MAX_LEN+1];
		int64_t datetime;
	};

	//AUCTION
	struct TAuctionInfo : public TShopItem {
		char ownername[CHARACTER_NAME_MAX_LEN + 1];
		DWORD duration;
#ifdef ENABLE_IKASHOP_ENTITIES
	#ifdef EXTEND_IKASHOP_ULTIMATE
		TShopSpawn spawn;
	#endif
#endif
	};


	struct TAuctionOfferInfo {
		TPriceInfo price;
		DWORD ownerid;
		DWORD buyerid;
		char buyername[CHARACTER_NAME_MAX_LEN + 1];
		bool seenflag;
		int64_t datetime;
	};

	struct TValutesInfo
	{
		long long yang = 0;

		void operator +=(const TValutesInfo& r){
			yang += r.yang;
		}

		void operator -=(const TValutesInfo& r){
			yang -= r.yang;
		}
	};

	struct TShopInfo 
	{
		DWORD ownerid;
		DWORD duration;
		char name[OFFLINE_SHOP_NAME_MAX_LEN];
		DWORD count;
#ifdef ENABLE_IKASHOP_ENTITIES
		TShopSpawn spawn;
#endif
#ifdef EXTEND_IKASHOP_ULTIMATE
		int decoration;
		int decoration_time;
		int lock_index;
#endif
	};

#ifdef EXTEND_IKASHOP_PRO
	enum class ENotificationType : uint8_t
	{
		SELLER_SOLD_ITEM,
		SELLER_NEW_OFFER_ON_ITEM,
		SELLER_SHOP_EXPIRED,
		SELLER_ITEM_EXPIRED,
		SELLER_AUCTION_NEW_OFFER,
		SELLER_AUCTION_EXPIRED,
		SELLER_AUCTION_SOLD,
		SELLER_OFFER_CANCELLED,
		BUYER_OFFER_DENIED,
		BUYER_OFFER_ACCEPTED,
		BUYER_OFFER_EXPIRED,
		BUYER_AUCTION_BUY,
		BUYER_AUCTION_RAISE,
		BUYER_AUCTION_LOST,
#ifdef EXTEND_IKASHOP_ULTIMATE
		SELLER_DECORATION_EXPIRED,
#endif
	};

	struct TNotificationInfo
	{
		DWORD id;
		ENotificationType type;
		DWORD what;
		char who[OFFLINE_SHOP_NAME_MAX_LEN];
		char format[OFFLINESHOP_NOTIFICATION_FORMAT_LEN];
		int64_t datetime;
		bool seenflag;
	};
#endif

#ifdef EXTEND_IKASHOP_ULTIMATE
	struct TSaleHistory 
	{
		DWORD account;
		DWORD vnum;
		DWORD count;
		long long price;
		int64_t datetime;
	};
#endif

	// ### GAME TO DB ###

	enum eNewIkarusShopSubHeaderGD
	{
		SUBHEADER_GD_BUY_ITEM = 0,
		SUBHEADER_GD_BUY_LOCK_ITEM,
		SUBHEADER_GD_EDIT_ITEM,
		SUBHEADER_GD_REMOVE_ITEM,
		SUBHEADER_GD_ADD_ITEM,

		SUBHEADER_GD_SHOP_FORCE_CLOSE,
		SUBHEADER_GD_SHOP_CREATE_NEW,
		SUBHEADER_GD_SHOP_CHANGE_NAME,


		SUBHEADER_GD_OFFER_CREATE,
		SUBHEADER_GD_OFFER_NOTIFIED,
		SUBHEADER_GD_OFFER_ACCEPT,
		SUBHEADER_GD_OFFER_CANCEL,

		SUBHEADER_GD_SAFEBOX_GET_ITEM,
		SUBHEADER_GD_SAFEBOX_GET_VALUTES,
		SUBHEADER_GD_SAFEBOX_ADD_ITEM,
		SUBHEADER_GD_SAFEBOX_GET_TIME_FEEDBACK,

		//AUCTION
		SUBHEADER_GD_AUCTION_CREATE,
		SUBHEADER_GD_AUCTION_ADD_OFFER,
		SUBHEADER_GD_AUCTION_CANCEL,
		SUBHEADER_GD_AUCTION_OFFER_SEEN,

#ifdef EXTEND_IKASHOP_PRO
		SUBHEADER_GD_NOTIFICATION_SEEN,
		SUBHEADER_GD_SHOP_RESTORE_DURATION,
#ifdef ENABLE_IKASHOP_ENTITIES
		SUBHEADER_GD_MOVE_SHOP_ENTITY,
#endif
#endif
#ifdef EXTEND_IKASHOP_ULTIMATE
		SUBHEADER_GD_SHOP_DECORATION_USE,
		SUBHEADER_GD_SHOP_MOVE_ITEM,
		SUBHEADER_GD_SHOP_UNLOCK_CELL,
#endif
	};

	struct TSubPacketGDBuyItem {
		DWORD ownerid;
		DWORD itemid;
		DWORD guestid;
		bool success;
	};

	struct TSubPacketGDLockBuyItem {
		DWORD ownerid;
		DWORD itemid;
		DWORD guestid;
#ifdef EXTEND_IKASHOP_ULTIMATE
		DWORD accountid;
		long long normalizedPrice;
#endif
		long long seenprice;
	};

	struct TSubPacketGDEditItem {
		DWORD ownerid;
		DWORD itemid;
		TPriceInfo price;
	};

	struct TSubPacketGDRemoveItem {
		DWORD ownerid;
		DWORD itemid;
	};

	struct TSubPacketGDAddItem {
		DWORD ownerid;
		TShopItem sitem;
	};

	struct TSubPacketGDShopForceClose {
		DWORD ownerid;
		bool gm;
	};

	struct TSubPacketGDShopCreateNew {
		TShopInfo shop;
	};

	struct TSubPacketGDShopChangeName {
		DWORD ownerid;
		char name[OFFLINE_SHOP_NAME_MAX_LEN];
	};

	struct TSubPacketGDOfferCreate {
		DWORD ownerid;
		DWORD itemid;
		DWORD vnum;
		DWORD count;
		TOfferInfo offer;
	};

	struct TSubPacketGDOfferCancel {
		DWORD offerid;
		DWORD ownerid;
		DWORD requester;
	};

	struct TSubPacketGDOfferNotified {
		DWORD ownerid;
		DWORD offerid;
	};

	struct TSubPacketGDOfferAccept {
		DWORD ownerid;
		DWORD offerid;
	};

	struct TSubPacketGDSafeboxGetItem {
		DWORD ownerid;
		DWORD itemid;
	};

	struct TSubPacketGDSafeboxAddItem {
		DWORD ownerid;
		DWORD itemid;
		TShopPlayerItem item;
	};

	struct TSubPacketGDSafeboxGetItemFeedback {
		DWORD ownerid;
		DWORD itemid;
		bool result;
	};

	struct TSubPacketGDSafeboxGetValutes {
		DWORD ownerid;
		TValutesInfo valute;
	};


	//AUCTION
	struct TSubPacketGDAuctionCreate {
		TAuctionInfo auction;
	};

	struct TSubPacketGDAuctionAddOffer {
		TAuctionOfferInfo offer;
	};

	struct TSubPacketGDAuctionOfferSeen {
		DWORD ownerid;
		DWORD buyerid;
		long long price;
	};

#ifdef EXTEND_IKASHOP_PRO
	struct TSubPacketGDNotificationSeen
	{
		DWORD owner;
		DWORD id;
	};
#ifdef ENABLE_IKASHOP_ENTITIES
	struct TSubPacketGDMoveShopEntity
	{
		DWORD owner;
		TShopSpawn spawn;
	};
#endif
#endif


#ifdef EXTEND_IKASHOP_ULTIMATE
	struct TSubPacketGDShopDecorationUse 
	{
		DWORD owner;
		int index;
	};

	struct TSubPacketGDShopMoveItem
	{
		DWORD owner;
		DWORD itemid;
		int destCell;
	};
#endif

	// ### DB TO GAME

	enum eSubHeaderDGNewIkarusShop
	{
		SUBHEADER_DG_BUY_ITEM,
		SUBHEADER_DG_LOCKED_BUY_ITEM,
		SUBHEADER_DG_EDIT_ITEM,
		SUBHEADER_DG_REMOVE_ITEM,
		SUBHEADER_DG_ADD_ITEM,

		SUBHEADER_DG_SHOP_FORCE_CLOSE,
		SUBHEADER_DG_SHOP_CREATE_NEW,
		SUBHEADER_DG_SHOP_CHANGE_NAME,
		SUBHEADER_DG_SHOP_EXPIRED,


		SUBHEADER_DG_OFFER_CREATE,
		SUBHEADER_DG_OFFER_NOTIFIED,
		SUBHEADER_DG_OFFER_ACCEPT,
		SUBHEADER_DG_OFFER_CANCEL,

		SUBHEADER_DG_LOAD_TABLES,

		SUBHEADER_DG_SAFEBOX_ADD_ITEM,
		SUBHEADER_DG_SAFEBOX_ADD_VALUTES,
		SUBHEADER_DG_SAFEBOX_LOAD,
		SUBHEADER_DG_SAFEBOX_EXPIRED_ITEM,
		SUBHEADER_DG_SAFEBOX_GET_ITEM_CONFIRM,

		//AUCTION
		SUBHEADER_DG_AUCTION_CREATE,
		SUBHEADER_DG_AUCTION_ADD_OFFER,
		SUBHEADER_DG_AUCTION_EXPIRED,
		SUBHEADER_DG_AUCTION_OFFER_SEEN,

#ifdef EXTEND_IKASHOP_PRO
		SUBHEADER_DG_NOTIFICATION_LOAD,
		SUBHEADER_DG_NOTIFICATION_FORWARD,
		SUBHEADER_DG_SHOP_RESTORE_DURATION,
#ifdef ENABLE_IKASHOP_ENTITIES
		SUBHEADER_DG_MOVE_SHOP_ENTITY,
#endif
#endif
#ifdef EXTEND_IKASHOP_ULTIMATE
		SUBHEADER_DG_SALE_HISTORY_REGISTER,
		SUBHEADER_DG_SHOP_DECORATION_SET,
		SUBHEADER_DG_SHOP_MOVE_ITEM,
		SUBHEADER_DG_SHOP_UNLOCK_CELL,
#endif
	};

	struct TSubPacketDGBuyItem {
		DWORD ownerid;
		DWORD itemid;
		DWORD buyerid;
	};

	struct TSubPacketDGLockedBuyItem {
		DWORD ownerid;
		DWORD itemid;
		DWORD buyerid;
	};

	struct TSubPacketDGEditItem {
		DWORD ownerid;
		DWORD itemid;
		TPriceInfo price;
	};

	struct TSubPacketDGRemoveItem {
		DWORD ownerid;
		DWORD itemid;
		bool requester = false;
	};

	struct TSubPacketDGAddItem {
		DWORD ownerid;
		DWORD itemid;
		TShopItem item;
	};

	struct TSubPacketDGShopForceClose {
		DWORD ownerid;
	};

	struct TSubPacketDGShopCreateNew {
		TShopInfo shop;
	};

	struct TSubPacketDGShopChangeName {
		DWORD ownerid;
		char name[OFFLINE_SHOP_NAME_MAX_LEN];
	};

	struct TSubPacketDGOfferCreate {
		DWORD ownerid;
		DWORD itemid;
		TOfferInfo offer;
	};

	struct TSubPacketDGOfferCancel {
		DWORD offerid;
		DWORD ownerid;
		bool removing;
	};

	struct TSubPacketDGOfferNotified {
		DWORD ownerid;
		DWORD offerid;
	};

	struct TSubPacketDGOfferAccept {
		DWORD ownerid;
		DWORD offerid;
	};

	struct TSubPacketDGLoadTables {
		DWORD shopcount;
		DWORD offercount;
		DWORD auctioncount;
		DWORD auctionoffercount;
	};

	struct TSubPacketDGShopExpired {
		DWORD ownerid;
	};

	struct TSubPacketDGSafeboxAddItem {
		DWORD ownerid;
		DWORD itemid;
		TShopPlayerItem item;
	};

	struct TSubPacketDGSafeboxAddValutes {
		DWORD ownerid;
		TValutesInfo valute;
	};

	struct TSubPacketDGSafeboxLoad {
		DWORD ownerid;
		TValutesInfo valute;
		DWORD itemcount;
	};

	struct TSubPacketDGSafeboxExpiredItem {
		DWORD ownerid;
		DWORD itemid;
	};

	struct TSubPacketDGSafeboxGetItemConfirm {
		DWORD ownerid;
		DWORD itemid;
	};

	//AUCTION
	struct TSubPacketDGAuctionCreate {
		TAuctionInfo auction;
	};

	struct TSubPacketDGAuctionAddOffer {
		TAuctionOfferInfo offer;
	};

	struct TSubPacketDGAuctionExpired {
		DWORD ownerid;
	};

	struct TSubPacketDGAuctionOfferSeen {
		DWORD ownerid;
		DWORD buyerid;
		long long price;
	};

#ifdef EXTEND_IKASHOP_PRO
	struct TSubPacketDGNotificationLoad 
	{
		DWORD ownerid;
		DWORD count;
	};

	struct TSubPacketDGNotificationForward : public TNotificationInfo
	{
		DWORD ownerid;
	};
#ifdef ENABLE_IKASHOP_ENTITIES
	struct TSubPacketDGMoveShopEntity
	{
		DWORD owner;
		TShopSpawn spawn;
	};
#endif
#endif


#ifdef EXTEND_IKASHOP_ULTIMATE
	struct TSubPacketDGShopDecorationSet
	{
		DWORD owner;
		int index;
		int time;
	};

	struct TSubPacketDGShopMoveItem
	{
		DWORD owner;
		DWORD itemid;
		int destCell;
	};

	struct TSubPacketDGShopUnlockCell 
	{
		DWORD owner;
		int lockIndex;
	};
#endif
}
#endif
