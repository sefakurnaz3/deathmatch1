//search
    HEADER_CG_FISHING                           = 82,
    HEADER_CG_GIVE_ITEM                         = 83,

//add after
#ifdef ENABLE_IKASHOP_RENEWAL
	HEADER_CG_NEW_OFFLINESHOP		= 84,
#endif

//search
	HEADER_GC_MAIN_CHARACTER3_BGM				= 137,
	HEADER_GC_MAIN_CHARACTER4_BGM_VOL			= 138,

//add after
#ifdef ENABLE_IKASHOP_RENEWAL
	HEADER_GC_NEW_OFFLINESHOP					= 139,
#endif

//add at the end of file
#ifdef ENABLE_IKASHOP_RENEWAL
typedef struct
{
	BYTE header;
#ifdef ENABLE_LARGE_DYNAMIC_PACKETS
	int size;
#else
	WORD size;
#endif
	BYTE subheader;
} TPacketGCNewOfflineshop;

typedef struct 
{
	BYTE header;
	WORD size;
	BYTE subheader;
} TPacketCGNewOfflineShop;

namespace ikashop
{
	typedef struct SPlayerItem
	{
		DWORD id;
		BYTE window;
		WORD pos;
		DWORD count;
		DWORD vnum;
		long alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
		TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
		DWORD owner;
	} TPlayerItem;

	enum class ExpirationType
	{
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
		long long GetTotalYangAmount() const {
			return yang
#ifdef ENABLE_CHEQUE_SYSTEM
				+ static_cast<long long>(cheque) * YANG_PER_CHEQUE
#endif
				;
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

	struct TOfferInfo
	{
		DWORD offerid;
		DWORD ownerid;
		DWORD itemid;
		DWORD buyerid;
		TPriceInfo price;
		bool notified;
		bool accepted;
		char buyername[CHARACTER_NAME_MAX_LEN + 1];
		int64_t datetime;
	};

#ifdef ENABLE_IKASHOP_ENTITIES
	struct TShopSpawn
	{
		int x;
		int y;
		int map;
	};
#endif

	//AUCTION
	struct TAuctionInfo : public TShopItem
	{
		char  ownername[CHARACTER_NAME_MAX_LEN + 1];
		DWORD duration;
#ifdef ENABLE_IKASHOP_ENTITIES
#ifdef EXTEND_IKASHOP_ULTIMATE
		TShopSpawn spawn;
#endif
#endif
	};

	struct TAuctionOfferInfo
	{
		TPriceInfo	price;
		DWORD ownerid;
		DWORD buyerid;
		char buyername[CHARACTER_NAME_MAX_LEN + 1];
		bool seenflag;
		int64_t datetime;
	};

	struct TValutesInfo
	{
		long long yang = 0;

		void operator +=(const TValutesInfo& r)
		{
			yang += r.yang;
		}

		void operator -=(const TValutesInfo& r)
		{
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

	struct TFilterInfo
	{
		BYTE type;
		BYTE subtype;
		char name[CItemData::ITEM_NAME_MAX_LEN];
		TPriceInfo pricestart;
		TPriceInfo pricend;
		int levelstart;
		int levelend;
		TPlayerItemAttribute attrs[ITEM_ATTRIBUTE_SLOT_NORM_NUM];
		int sashGrade;
		int alchemyGrade;
	};

	struct TShopItemInfo
	{
		TItemPos pos;
		int destpos;
		TPriceInfo price;
	};

#ifdef EXTEND_IKASHOP_ULTIMATE
	struct TShopItemView : public ikashop::TShopItem
	{
		long long priceavg;
	};
#endif

#ifdef EXTEND_IKASHOP_ULTIMATE
	struct TSearchResultInfo : public ikashop::TShopItemView
#else
	struct TSearchResultInfo : public ikashop::TShopItem
#endif
	{
		bool auction;
		char seller_name[OFFLINE_SHOP_NAME_MAX_LEN];
		int duration;
	};

	struct TAuctionListElement
	{
		TAuctionInfo auction;
		TPriceInfo actual_best;
		DWORD offercount;
#ifdef EXTEND_IKASHOP_ULTIMATE
		long long priceavg;
#endif
	};

	struct TOfferListElement : public TOfferInfo
	{
		TShopItem item;
		char shopname[OFFLINE_SHOP_NAME_MAX_LEN];
		bool incoming;
#ifdef EXTEND_IKASHOP_ULTIMATE
		long long priceavg;
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

	enum eSubHeaderGC
	{
		SUBHEADER_GC_SHOP_OPEN,
		SUBHEADER_GC_SHOP_OPEN_OWNER,
		SUBHEADER_GC_SHOP_OPEN_OWNER_NO_SHOP,
		SUBHEADER_GC_SHOP_EXPIRED_GUESTING,
		SUBHEADER_GC_SHOP_REMOVE_ITEM_OWNER,
		SUBHEADER_GC_SHOP_REMOVE_ITEM_GUEST,
		SUBHEADER_GC_SHOP_EDIT_ITEM_OWNER,
		SUBHEADER_GC_SHOP_EDIT_ITEM_GUEST,
		SUBHEADER_GC_OFFER_LIST,
		SUBHEADER_GC_SHOP_FILTER_RESULT,
		SUBHEADER_GC_SHOP_SAFEBOX_REFRESH,
		SUBHEADER_GC_SHOP_SAFEBOX_REMOVE_ITEM,
		SUBHEADER_GC_SHOP_SAFEBOX_ADD_ITEM,
		SUBHEADER_GC_AUCTION_LIST,
		SUBHEADER_GC_OPEN_MY_AUCTION,
		SUBHEADER_GC_OPEN_MY_AUCTION_NO_AUCTION,
		SUBHEADER_GC_OPEN_AUCTION,
		SUBHEDAER_GC_POPUP_MESSAGE,

#ifdef ENABLE_IKASHOP_ENTITIES
		SUBHEADER_GC_INSERT_SHOP_ENTITY,
		SUBHEADER_GC_REMOVE_SHOP_ENTITY,
#endif
		SUBHEADER_GC_BOARD_COUNTERS,
#ifdef EXTEND_IKASHOP_PRO
		SUBHEADER_GC_NOTIFICATION_LIST,
#endif
#ifdef EXTEND_IKASHOP_ULTIMATE
		SUBHEADER_GC_PRICE_AVERAGE_RESPONSE,
#endif

		SUBHEADER_GC_SEARCH_RESULT_DELETE,
	};

	struct TSubPacketGCShopOpen
	{
		TShopInfo shop;
	};

	struct TSubPacketGCShopOpenOwner
	{
		TShopInfo shop;
	};

	struct TSubPacketGCShopExpiredGuesting
	{
		DWORD ownerid;
	};

	struct TSubPacketGCShopRemoveItem
	{
		DWORD itemid;
	};

	struct TSubPacketGCShopEditItem
	{
		DWORD itemid;
		TPriceInfo price;
	};

	struct TSubPacketGCShopFilterResult
	{
		DWORD count;
	};

	struct TSubPacketGCShopOfferList
	{
		DWORD offercount;
	};

	struct TSubPacketGCShopSafeboxRefresh
	{
		TValutesInfo valute;
		DWORD itemcount;
	};

	struct TSubPacketGCShopSafeboxRemoveItem
	{
		DWORD itemid;
	};

	struct TSubPacketGCShoppingSafeboxAddItem
	{
		TShopPlayerItem item;
	};

	struct TSubPacketGCAuctionList
	{
		DWORD count;
	};

	struct TSubPacketGCAuctionOpen
	{
		TAuctionInfo auction;
		DWORD offercount;
#ifdef EXTEND_IKASHOP_ULTIMATE
		long long priceavg;
#endif
	};

	struct TSubPacketGCPopupMessage
	{
		static constexpr inline auto MESSAGE_LEN = 80;
		char localeString[MESSAGE_LEN];
	};

#ifdef ENABLE_IKASHOP_ENTITIES
	struct TSubPacketGCInsertShopEntity
	{
		DWORD vid;
		char name[OFFLINE_SHOP_NAME_MAX_LEN];
		int type;
		int x, y, z;
	};

	struct TSubPacketGCRemoveShopEntity
	{
		DWORD vid;
	};
#endif

	struct TSubPacketGCBoardCounters
	{
		int safebox;
		int offers;
		int auction;
#ifdef EXTEND_IKASHOP_PRO
		int notification;
#endif
	};

#ifdef EXTEND_IKASHOP_PRO
	struct TSubPacketGCNotificationList
	{
		DWORD count;
	};
#endif

	enum eSubHeaderCG
	{
		SUBHEADER_CG_SHOP_CREATE_NEW,
		SUBHEADER_CG_SHOP_FORCE_CLOSE,
		SUBHEADER_CG_SHOP_REQUEST_SHOPLIST,
		SUBHEADER_CG_SHOP_OPEN,
		SUBHEADER_CG_SHOP_OPEN_OWNER,
		SUBHEADER_CG_SHOP_BUY_ITEM,
		SUBHEADER_CG_SHOP_ADD_ITEM,
		SUBHEADER_CG_SHOP_REMOVE_ITEM,
		SUBHEADER_CG_SHOP_EDIT_ITEM,
		SUBHEADER_CG_SHOP_FILTER_REQUEST,
		SUBHEADER_CG_SHOP_SEARCH_FILL_REQUEST,
		SUBHEADER_CG_SHOP_OFFER_CREATE,
		SUBHEADER_CG_SHOP_OFFER_ACCEPT,
		SUBHEADER_CG_SHOP_OFFER_CANCEL,
		SUBHEADER_CG_SHOP_REQUEST_OFFER_LIST,
		SUBHEADER_CG_SHOP_SAFEBOX_OPEN,
		SUBHEADER_CG_SHOP_SAFEBOX_GET_ITEM,
		SUBHEADER_CG_SHOP_SAFEBOX_GET_VALUTES,
		SUBHEADER_CG_SHOP_SAFEBOX_CLOSE,
		SUBHEADER_CG_AUCTION_LIST_REQUEST,
		SUBHEADER_CG_AUCTION_OPEN_REQUEST,
		SUBHEADER_CG_MY_AUCTION_OPEN_REQUEST,
		SUBHEADER_CG_MY_AUCTION_CLOSE,
		SUBHEADER_CG_MY_AUCTION_CANCEL,
		SUBHEADER_CG_CREATE_AUCTION,
		SUBHEADER_CG_AUCTION_ADD_OFFER,
		SUBHEADER_CG_EXIT_FROM_AUCTION,
		SUBHEADER_CG_CLOSE_MY_SHOP_BOARD,
		SUBHEADER_CG_CLOSE_OFFER_LIST,
		SUBHEADER_CG_CLOSE_SHOP_GUEST,
#ifdef ENABLE_IKASHOP_ENTITIES
		SUBHEADER_CG_CLICK_ENTITY,
#ifdef EXTEND_IKASHOP_PRO
		SUBHEADER_CG_MOVE_SHOP_ENTITY,
#endif
#endif

#ifdef EXTEND_IKASHOP_PRO
		SUBHEADER_CG_NOTIFICATION_LIST_REQUEST,
		SUBHEADER_CG_NOTIFICATION_LIST_CLOSE,
#endif
#ifdef EXTEND_IKASHOP_ULTIMATE
		SUBHEADER_CG_PRICE_AVERAGE_REQUEST,
		SUBHEADER_CG_SHOP_DECORATION_USE,
		SUBHEADER_CG_SHOP_MOVE_ITEM,
#endif
	};

	struct TSubPacketCGShopOpen
	{
		DWORD ownerid;
	};

	struct TSubPacketCGAddItem
	{
		TItemPos pos;
		TPriceInfo price;
		int destpos;
	};

	struct TSubPacketCGRemoveItem
	{
		DWORD itemid;
	};

	struct TSubPacketCGEditItem
	{
		DWORD itemid;
		TPriceInfo price;
	};

	struct TSubPacketCGFilterRequest
	{
		TFilterInfo filter;
	};

	struct TSubPacketCGOfferCreate
	{
		TOfferInfo offer;
	};

	struct TSubPacketCGOfferAccept
	{
		DWORD offerid;
	};

	struct TSubPacketCGOfferCancel
	{
		DWORD offerid;
		DWORD ownerid;
	};

	struct TSubPacketCGShopSafeboxGetItem
	{
		DWORD itemid;
	};

	struct TSubPacketCGShopBuyItem
	{
		DWORD ownerid;
		DWORD itemid;
		bool searching;
		long long seenprice;
	};

	struct TSubPacketCGAuctionOpenRequest
	{
		DWORD ownerid;
	};

	struct TSubPacketCGAuctionCreate
	{
		TItemPos pos;
		TPriceInfo startprice;
	};

	struct TSubPacketCGAuctionAddOffer
	{
		DWORD ownerid;
		TPriceInfo price;
	};

	struct TSubPacketCGAuctionExitFrom
	{
		DWORD ownerid;
	};

#ifdef ENABLE_IKASHOP_ENTITIES
	struct TSubPacketCGShopClickEntity
	{
		DWORD vid;
#ifdef EXTEND_IKASHOP_ULTIMATE
		bool isAuction = false;
#endif
	};
#endif

#ifdef EXTEND_IKASHOP_ULTIMATE
	struct TSubPacketCGPriceAverageRequest
	{
		DWORD vnum;
		DWORD count;
	};

	struct TSubPacketCGShopMoveItem 
	{
		int srcPos;
		int destPos;
	};
#endif
}
#endif

//search
typedef struct packet_header_dynamic_size
{
	BYTE		header;
	WORD		size;
} TDynamicSizePacketHeader;

//add after
#ifdef ENABLE_LARGE_DYNAMIC_PACKETS
typedef struct packet_header_large_dynamic_size
{
	BYTE		header;
	int			size;
} TLargeDynamicSizePacketHeader;
#endif