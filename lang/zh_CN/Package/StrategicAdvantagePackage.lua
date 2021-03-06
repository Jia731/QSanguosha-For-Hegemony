-- translation for StrategicAdvantage Package

return {
	["strategic_advantage"] = "势备篇",

	["Blade"] = "青龙偃月刀",
	[":Blade"] = "装备牌·武器\n\n攻击范围：3\n技能：锁定技，每当你使用【杀】指定一个目标后，其不能明置武将直到此【杀】结算完毕。",

	["Halberd"] = "方天画戟",
	[":Halberd"] = "装备牌·武器\n\n攻击范围：4\n技能：君主技，限定技，锁定技，主将技，副将技，阵法技，出牌阶段限一次，Fs了不起。",
	["#HalberdUse"] = "%from 发动了【<font color=\"yellow\"><b>方天画戟</b></font>】效果",

	["Breastplate"] = "护心镜",
	[":Breastplate"] = "装备牌·防具\n\n技能：每当你受到XXXXXXXXX的伤害时，你可以XXXXXXXXXX，防止此伤害。",

	["IronArmor"] = "明光铠",
	[":IronArmor"] = "装备牌·防具\n\n技能：每当你成为【火攻】、火【杀】或【火XXXX】的目标时，你取消自己。",
	["#IronArmor"] = "%from 的装备技能【%arg】被触发",

	["WoodenOx"] = "木牛流马",
	[":WoodenOx"] = "装备牌·宝物\n\n技能：\n" ..
					"1. 出牌阶段限一次，你可以将一张手牌置于【木牛流马】下，若如此做，你可以将【木牛流马】移动至一名其他角色的装备区。\n" ..
					"2. 你可以将【木牛流马】下的牌视为手牌使用或打出。\n" ..
					"◆每当你失去装备区的【木牛流马】后，若【木牛流马】未移动至装备区，其下的牌置入弃牌堆，否则这些牌仍置于【木牛流马】下。\n◆【木牛流马】下的牌为移出游戏。",
	["@wooden_ox-move"] = "你可以将【木牛流马】移动至一名其他角色的装备区",
	["wooden_ox"] = "木牛流马",
	["#WoodenOx"] = "%from 使用/打出了 %arg 张 %arg2 牌",

	["JadeSeal"] = "玉玺",
	[":JadeSeal"] = "装备牌·宝物\n\n技能：君主技，限定技，锁定技，主将技，副将技，阵法技，出牌阶段限一次，你可以砍了啦啦。",

	["drowning"] = "水淹七军",
	[":drowning"] = "锦囊牌\n\n使用时机：出牌阶段。\n使用目标：装备区有牌的一名其他角色。\n作用效果：目标角色选择一项：1.弃置装备区里的所有牌；2.受到你造成的1点伤害。",
	["drowning:throw"] = "弃置装备区里的所有牌",
	["drowning:damage"] = "受到其造成的1点伤害",

	["alliance_feast"] = "联军盛宴",
	[":alliance_feast"] = "锦囊牌\n\n使用时机：出牌阶段。\n使用目标：XXXXXXXXXX。\n作用效果：你XXXXXXXXX。然后XXXXXXXXX各选择一项：XXXXXXX；或XXXXXXXX，XXXXXXX。",
}