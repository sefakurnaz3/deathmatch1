SET SESSION sql_mode = "";

INSERT INTO `item_proto` (`vnum`, `name`, `locale_name`, `type`, `subtype`, `weight`, `size`, `antiflag`, `flag`, `wearflag`, `immuneflag`, `gold`, `shop_buy_price`, `refined_vnum`, `refine_set`, `refine_set2`, `magic_pct`, `limittype0`, `limitvalue0`, `limittype1`, `limitvalue1`, `applytype0`, `applyvalue0`, `applytype1`, `applyvalue1`, `applytype2`, `applyvalue2`, `value0`, `value1`, `value2`, `value3`, `value4`, `value5`, `socket0`, `socket1`, `socket2`, `socket3`, `socket4`, `socket5`, `specular`, `socket_pct`, `addon_type`) VALUES (51998, 0x507269766174652053686F70204B6579, 0x507269766174652053686F70204B6579, 3, 10, 0, 1, 90496, 4, 0, '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, 0, 0, 0);
INSERT INTO `item_proto` (`vnum`, `name`, `locale_name`, `type`, `subtype`, `weight`, `size`, `antiflag`, `flag`, `wearflag`, `immuneflag`, `gold`, `shop_buy_price`, `refined_vnum`, `refine_set`, `refine_set2`, `magic_pct`, `limittype0`, `limitvalue0`, `limittype1`, `limitvalue1`, `applytype0`, `applyvalue0`, `applytype1`, `applyvalue1`, `applytype2`, `applyvalue2`, `value0`, `value1`, `value2`, `value3`, `value4`, `value5`, `socket0`, `socket1`, `socket2`, `socket3`, `socket4`, `socket5`, `specular`, `socket_pct`, `addon_type`) VALUES (51999, 0x5365617263682053686F70205072656D69756D, 0x5365617263682053686F70205072656D69756D, 3, 10, 0, 1, 123264, 0, 0, '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, 0, 0, 0);
INSERT INTO `item_proto` (`vnum`, `name`, `locale_name`, `type`, `subtype`, `weight`, `size`, `antiflag`, `flag`, `wearflag`, `immuneflag`, `gold`, `shop_buy_price`, `refined_vnum`, `refine_set`, `refine_set2`, `magic_pct`, `limittype0`, `limitvalue0`, `limittype1`, `limitvalue1`, `applytype0`, `applyvalue0`, `applytype1`, `applyvalue1`, `applytype2`, `applyvalue2`, `value0`, `value1`, `value2`, `value3`, `value4`, `value5`, `socket0`, `socket1`, `socket2`, `socket3`, `socket4`, `socket5`, `specular`, `socket_pct`, `addon_type`) VALUES (52000, 0x53686F70204465636F726174696F6E, 0x53686F70204465636F726174696F6E, 3, 10, 0, 1, 123264, 0, 0, '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, 0, 0, 0);

ALTER TABLE `item` MODIFY COLUMN `window` ENUM('INVENTORY','EQUIPMENT','SAFEBOX','MALL','DRAGON_SOUL_INVENTORY','BELT_INVENTORY','GROUND', 'IKASHOP_OFFLINESHOP', 'IKASHOP_SAFEBOX', 'IKASHOP_AUCTION') NOT NULL;
ALTER TABLE `item` ADD COLUMN `ikashop_data` TEXT(250);



SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for ikashop_auction_offer
-- ----------------------------
DROP TABLE IF EXISTS `ikashop_auction_offer`;
CREATE TABLE `ikashop_auction_offer` (
  `seller` int unsigned NOT NULL,
  `buyer` int unsigned NOT NULL,
  `buyer_name` varchar(30) NOT NULL,
  `price` bigint unsigned NOT NULL,
  `datetime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `seen` tinyint unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`seller`,`buyer`,`price`)
);

-- ----------------------------
-- Table structure for ikashop_notification
-- ----------------------------
DROP TABLE IF EXISTS `ikashop_notification`;
CREATE TABLE `ikashop_notification` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `owner` int unsigned NOT NULL,
  `type` int unsigned NOT NULL,
  `who` varchar(50) NOT NULL,
  `what` int unsigned NOT NULL,
  `format` varchar(50) NOT NULL,
  `seen` tinyint NOT NULL DEFAULT '0',
  `datetime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
);

-- ----------------------------
-- Table structure for ikashop_offlineshop
-- ----------------------------
DROP TABLE IF EXISTS `ikashop_offlineshop`;
CREATE TABLE `ikashop_offlineshop` (
  `owner` int unsigned NOT NULL,
  `duration` int unsigned NOT NULL DEFAULT '0',
  `map` int unsigned NOT NULL DEFAULT '0',
  `x` int unsigned NOT NULL DEFAULT '0',
  `y` int unsigned NOT NULL DEFAULT '0',
  `decoration` int unsigned NOT NULL DEFAULT '0',
  `decoration_time` int unsigned NOT NULL DEFAULT '0',
  `lock_index` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`owner`)
);

-- ----------------------------
-- Table structure for ikashop_private_offer
-- ----------------------------
DROP TABLE IF EXISTS `ikashop_private_offer`;
CREATE TABLE `ikashop_private_offer` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `seller` int unsigned NOT NULL,
  `buyer` int unsigned NOT NULL,
  `item` int unsigned NOT NULL,
  `price` bigint unsigned NOT NULL,
  `notified` tinyint unsigned NOT NULL,
  `accepted` tinyint unsigned NOT NULL,
  `buyer_name` varchar(30) NOT NULL,
  `datetime` datetime DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
);

-- ----------------------------
-- Table structure for ikashop_safebox
-- ----------------------------
DROP TABLE IF EXISTS `ikashop_safebox`;
CREATE TABLE `ikashop_safebox` (
  `owner` int unsigned NOT NULL,
  `gold` bigint unsigned NOT NULL,
  `cheque` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`owner`)
);

-- ----------------------------
-- Table structure for ikashop_sale
-- ----------------------------
DROP TABLE IF EXISTS `ikashop_sale`;
CREATE TABLE `ikashop_sale` (
  `id` int unsigned NOT NULL,
  `vnum` int unsigned NOT NULL,
  `count` int unsigned NOT NULL,
  `price` bigint unsigned NOT NULL,
  `datetime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP
);
SET FOREIGN_KEY_CHECKS=1;
